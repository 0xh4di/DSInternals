#include "stdafx.h"
#include "DrsConnection.h"
#include "RpcTypeConverter.h"

using namespace DSInternals::Common;
using namespace DSInternals::Common::Exceptions;
using namespace DSInternals::Common::Interop;
using namespace DSInternals::Replication::Model;

using namespace System;
using namespace System::ComponentModel;
using namespace System::Reflection;
using namespace System::Security::Principal;
using namespace System::Runtime::InteropServices;
using namespace Microsoft::Win32::SafeHandles;
using namespace msclr::interop;

namespace DSInternals
{
	namespace Replication
	{
		namespace Interop
		{
			DrsConnection::DrsConnection(IntPtr rpcHandle, Guid clientDsa)
				: SafeHandleZeroOrMinusOneIsInvalid(true)
			{
				this->_clientDsa = clientDsa;
				this->_serverReplEpoch = DrsConnection::defaultReplEpoch;

				// Register the RetrieveSessionKey as RCP security callback. Mind the delegate lifecycle.
				this->_securityCallback = gcnew SecurityCallback(this, &DrsConnection::RetrieveSessionKey);
				RPC_STATUS status = RpcBindingSetOption(rpcHandle.ToPointer(), RPC_C_OPT_SECURITY_CALLBACK, (ULONG_PTR)Marshal::GetFunctionPointerForDelegate(this->_securityCallback).ToPointer());

				this->Bind(rpcHandle);
				if (this->_serverReplEpoch != DrsConnection::defaultReplEpoch)
				{
					// The domain must have been renamed, so we need to rebind with the proper dwReplEpoch.
					this->ReleaseHandle();
					this->Bind(rpcHandle);
				}
			}

			DrsConnection::DrsConnection(IntPtr preexistingDrssHandle, bool ownsHandle)
				: SafeHandleZeroOrMinusOneIsInvalid(ownsHandle)
			{
				this->SetHandle(preexistingDrssHandle);
			}

			DrsConnection::DrsConnection()
				: SafeHandleZeroOrMinusOneIsInvalid(true)
			{
			}

			void DrsConnection::Bind(IntPtr rpcHandle)
			{
				// Init binding parameters
				UUID clientDsaUuid = RpcTypeConverter::ToUUID(this->_clientDsa);
				auto clientInfo = this->CreateClientInfo();
				DRS_EXTENSIONS *genericServerInfo = nullptr;
				DRS_HANDLE drsHandle = nullptr;

				// Bind
				ULONG result = IDL_DRSBind_NoSEH(rpcHandle.ToPointer(), &clientDsaUuid, (DRS_EXTENSIONS*)clientInfo.get(), &genericServerInfo, &drsHandle);
				Validator::AssertSuccess((Win32ErrorCode)result);
				
				// Prevent memory leak by storing the genericServerInfo in midl_ptr
				auto genericServerInfoSafePtr = midl_ptr<DRS_EXTENSIONS>(genericServerInfo);

				// Store the DRS handle
				this->SetHandle((IntPtr)drsHandle);

				// Parse the server info
				DRS_EXTENSIONS_INT serverInfo = DRS_EXTENSIONS_INT(genericServerInfo);
				this->_serverSiteObjectGuid = RpcTypeConverter::ToGuid(serverInfo.siteObjGuid);
				this->_serverReplEpoch = serverInfo.dwReplEpoch;
				this->_serverCapabilities = serverInfo.dwFlags;
			}

			array<byte>^ DrsConnection::SessionKey::get()
			{
				return this->_sessionKey;
			}

			Guid DrsConnection::ServerSiteGuid::get()
			{
				return this->_serverSiteObjectGuid;
			}

			midl_ptr<DRS_EXTENSIONS_INT> DrsConnection::CreateClientInfo()
			{
				auto clientInfo = make_midl_ptr<DRS_EXTENSIONS_INT>();
				clientInfo->dwFlags = DRS_EXT::ALL_EXT;
				clientInfo->dwFlagsExt = DRS_EXT2::DRS_EXT_LH_BETA2 | DRS_EXT2::DRS_EXT_RECYCLE_BIN | DRS_EXT2::DRS_EXT_PAM;
				clientInfo->dwExtCaps = DRS_EXT2::DRS_EXT_LH_BETA2 | DRS_EXT2::DRS_EXT_RECYCLE_BIN | DRS_EXT2::DRS_EXT_PAM;
				clientInfo->dwReplEpoch = this->_serverReplEpoch;
				return clientInfo;
			}

			/// <summary>
			/// Gets the replication cursor information for the specified partition.
			/// </summary>
			/// <param name="namingContext">The distinguished name of the partition for which to retrieve the replication cursor information.</param>
			array<ReplicationCursor^>^ DrsConnection::GetReplicationCursors(String^ namingContext)
			{
				// Validate connection
				// TODO: Extract connection validation as a proteted method
				if (this->IsInvalid)
				{
					// TODO: Exception type
					throw gcnew Exception("Not connected");
				}
				Validator::AssertNotNullOrWhiteSpace(namingContext, "namingContext");

				// Prepare the parameters
				DRS_HANDLE handle = this->handle.ToPointer();
				const DWORD inVersion = 1;
				DWORD outVersion = 0;
				auto request = CreateReplicationCursorsRequest(namingContext);
				
				DRS_MSG_GETREPLINFO_REPLY reply = { nullptr };

				// Retrieve info from DC
				auto result = IDL_DRSGetReplInfo_NoSEH(handle, inVersion, (DRS_MSG_GETREPLINFO_REQ*)request.get(), &outVersion, &reply);
				
				// Validate the return code
				Validator::AssertSuccess((Win32ErrorCode)result);

				// TODO: Check the returned outVersion.

				// Prevent memory leak by storing the cursors in midl_ptr
				auto cursors = midl_ptr<DS_REPL_CURSORS>(reply.pCursors);

				// Process the results
				auto managedCursors = RpcTypeConverter::ToReplicationCursors(move(cursors));
				return managedCursors;
			}

			midl_ptr<DRS_MSG_GETCHGREQ_V10> DrsConnection::CreateGenericReplicateRequest(midl_ptr<DSNAME> &&dsName, array<ATTRTYP>^ partialAttributeSet, ULONG maxBytes, ULONG maxObjects)
			{
				// TODO: Add support for Windows Server 2003
				auto request = make_midl_ptr<DRS_MSG_GETCHGREQ_V10>();
				// Inset client ID:
				request->uuidDsaObjDest = RpcTypeConverter::ToUUID(this->_clientDsa);
				// Insert DSNAME
				request->pNC = dsName.release(); // Note: Request deleter will also delete DSNAME.
				// Insert PAS:
				auto nativePas = CreateNativePas(partialAttributeSet);
				request->pPartialAttrSetEx = nativePas.release(); // Note: Request deleter will also delete PAS.
				// Insert response size limits:
				request->cMaxBytes = maxBytes;
				request->cMaxObjects = maxObjects;
				// Set correct flags:
				// TODO: + DRS_OPTIONS::PER_SYNC ?
				request->ulFlags = DRS_OPTIONS::DRS_INIT_SYNC |
					DRS_OPTIONS::DRS_WRIT_REP |
					DRS_OPTIONS::DRS_NEVER_SYNCED;
				return request;
			}

			midl_ptr<DRS_MSG_GETREPLINFO_REQ_V1> DrsConnection::CreateReplicationCursorsRequest(String^ namingContext)
			{
				auto request = make_midl_ptr<DRS_MSG_GETREPLINFO_REQ_V1>();
				request->InfoType = DS_REPL_INFO_TYPE::DS_REPL_INFO_CURSORS_FOR_NC;
				request->pszObjectDN = RpcTypeConverter::ToNativeString(namingContext).release();
				return request;
			}

			midl_ptr<DRS_MSG_GETCHGREQ_V10> DrsConnection::CreateReplicateAllRequest(ReplicationCookie^ cookie, array<ATTRTYP>^ partialAttributeSet, ULONG maxBytes, ULONG maxObjects)
			{
				auto ncToReplicate = RpcTypeConverter::ToDsName(cookie->NamingContext);
				auto request = CreateGenericReplicateRequest(move(ncToReplicate), partialAttributeSet, maxBytes, maxObjects);
				// Insert replication state from cookie:
				request->usnvecFrom.usnHighObjUpdate = cookie->HighObjUpdate;
				request->usnvecFrom.usnHighPropUpdate = cookie->HighPropUpdate;
				request->usnvecFrom.usnReserved = cookie->Reserved;
				request->uuidInvocIdSrc = RpcTypeConverter::ToUUID(cookie->InvocationId);
				request->ulFlags |= DRS_OPTIONS::DRS_GET_NC_SIZE;
				return request;
			}

			midl_ptr<DRS_MSG_GETCHGREQ_V10> DrsConnection::CreateReplicateSingleRequest(Guid objectGuid, array<ATTRTYP>^ partialAttributeSet)
			{
				auto objectToReplicate = RpcTypeConverter::ToDsName(objectGuid);
				auto request = CreateGenericReplicateRequest(move(objectToReplicate), partialAttributeSet, defaultMaxBytes, defaultMaxObjects);
				request->ulExtendedOp = EXOP_REQ::EXOP_REPL_OBJ;
				// Guid of an existing DC must be set for the replication to work
				request->uuidDsaObjDest = RpcTypeConverter::ToUUID(this->_serverSiteObjectGuid);
				return request;
			}

			midl_ptr<DRS_MSG_GETCHGREQ_V10> DrsConnection::CreateReplicateSingleRequest(String^ distinguishedName, array<ATTRTYP>^ partialAttributeSet)
			{
				auto objectToReplicate = RpcTypeConverter::ToDsName(distinguishedName);
				auto request = CreateGenericReplicateRequest(move(objectToReplicate), partialAttributeSet, defaultMaxBytes, defaultMaxObjects);
				request->ulExtendedOp = EXOP_REQ::EXOP_REPL_OBJ;
				// Guid of an existing object must be set for the replication to work
				request->uuidDsaObjDest = RpcTypeConverter::ToUUID(this->_serverSiteObjectGuid);
				return request;
			}

			ReplicationResult^ DrsConnection::ReplicateAllObjects(ReplicationCookie^ cookie)
			{
				return this->ReplicateAllObjects(cookie, nullptr, DrsConnection::defaultMaxBytes, DrsConnection::defaultMaxObjects);
			}

			ReplicationResult^ DrsConnection::ReplicateAllObjects(ReplicationCookie^ cookie, ULONG maxBytes, ULONG maxObjects)
			{
				return this->ReplicateAllObjects(cookie, nullptr, maxBytes, maxObjects);
			}

			ReplicationResult^ DrsConnection::ReplicateAllObjects(ReplicationCookie^ cookie, array<ATTRTYP>^ partialAttributeSet, ULONG maxBytes, ULONG maxObjects)
			{
				// Validate parameters
				Validator::AssertNotNull(cookie, "cookie");

				auto request = CreateReplicateAllRequest(cookie, partialAttributeSet, maxBytes, maxObjects);
				auto reply = GetNCChanges(move(request));
				auto objects = ReadObjects(reply->pObjects, reply->cNumObjects, reply->rgValues, reply->cNumValues);
				USN_VECTOR usnTo = reply->usnvecTo;
				Guid invocationId = RpcTypeConverter::ToGuid(reply->uuidInvocIdSrc);
				auto newCookie = gcnew ReplicationCookie(cookie->NamingContext, invocationId, usnTo.usnHighObjUpdate, usnTo.usnHighPropUpdate, usnTo.usnReserved);
				bool hasMoreData = reply->fMoreData != 0;
				return gcnew ReplicationResult(objects, hasMoreData, newCookie, reply->cNumNcSizeObjects);
			}

			ReplicaObject^ DrsConnection::ReplicateSingleObject(String^ distinguishedName)
			{
				return this->ReplicateSingleObject(distinguishedName, nullptr);
			}

			ReplicaObject^ DrsConnection::ReplicateSingleObject(String^ distinguishedName, array<ATTRTYP>^ partialAttributeSet)
			{
				try
				{
					auto request = CreateReplicateSingleRequest(distinguishedName, partialAttributeSet);
					auto reply = GetNCChanges(move(request));
					auto objects = ReadObjects(reply->pObjects, reply->cNumObjects, reply->rgValues, reply->cNumValues);
					// TODO: Assert objects.Count == 1; It is guaranteed that it is > 0
					return objects[0];
				}
				catch (DirectoryObjectNotFoundException^)
				{
					// ReplicateSingleObject also exits with this error when access is denied, so we need to differentiate between these situations.
					bool objectExists = this->TestObjectExistence(distinguishedName);
					if (objectExists)
					{
						// Force the validator to throw the DRA access denied exception.
						Validator::AssertSuccess(Win32ErrorCode::DS_DRA_ACCESS_DENIED);
					}

					// Rethrow the original exception otherwise, as the object really does not exists.
					throw;
				}
			}

			ReplicaObject^ DrsConnection::ReplicateSingleObject(Guid objectGuid)
			{
				return this->ReplicateSingleObject(objectGuid, nullptr);
			}

			ReplicaObject^ DrsConnection::ReplicateSingleObject(Guid objectGuid, array<ATTRTYP>^ partialAttributeSet)
			{
				try
				{
					auto request = CreateReplicateSingleRequest(objectGuid, partialAttributeSet);
					auto reply = GetNCChanges(move(request));
					auto objects = ReadObjects(reply->pObjects, reply->cNumObjects, reply->rgValues, reply->cNumValues);
					// TODO: Assert objects.Count == 1; It is guaranteed that it is > 0
					return objects[0];
				}
				catch (DirectoryObjectNotFoundException^)
				{
					// ReplicateSingleObject also exits with this error when access is denied, so we need to differentiate between these situations.
					bool objectExists = this->TestObjectExistence(objectGuid);
					if (objectExists)
					{
						// Force the validator to throw the DRA access denied exception.
						Validator::AssertSuccess(Win32ErrorCode::DS_DRA_ACCESS_DENIED);
					}

					// Rethrow the original exception otherwise, as the object really does not exists.
					throw;
				}
			}

			midl_ptr<DRS_MSG_GETCHGREPLY_V9> DrsConnection::GetNCChanges(midl_ptr<DRS_MSG_GETCHGREQ_V10> &&request)
			{
				// Validate connection
				if (this->IsInvalid)
				{
					// TODO: Exception type
					throw gcnew Exception("Not connected");
				}
				DRS_HANDLE handle = this->handle.ToPointer();
				const DWORD inVersion = this->MaxSupportedReplicationRequestVersion;
				DWORD outVersion = 0;
				auto reply = make_midl_ptr<DRS_MSG_GETCHGREPLY_V9>();
				// Send message:
				auto result = (Win32ErrorCode) IDL_DRSGetNCChanges_NoSEH(handle, inVersion, (DRS_MSG_GETCHGREQ*)request.get(), &outVersion, (DRS_MSG_GETCHGREPLY*)reply.get());

				// Validate result
				Validator::AssertSuccess(result);

				// TODO: Test extended error code:
				DWORD extendedError = reply->dwDRSError;

				// Check the returned structure version
				if (outVersion == 6 && reply->cNumValues > 0)
				{
					// We will now convert the REPLVALINF_V1 array into REPLVALINF_V3 array so that the caller does not have to differentiate between them.
					// This convenience comes at the price of a minor performance loss
					auto valuesV1 = ((DRS_MSG_GETCHGREPLY_V6*)reply.get())->rgValues;
					auto valuesV3 = make_midl_ptr<REPLVALINF_V3>(reply->cNumValues);
					
					for (DWORD i = 0; i < reply->cNumValues; i++)
					{
						memcpy(&(valuesV3.get()[i]), &valuesV1[i], sizeof(REPLVALINF_V1));
					}

					// Assign the new value and delete the old one. Only shallow free must be done.
					reply->rgValues = valuesV3.release();
					midl_user_free(valuesV1);
				}

				return reply;
			}

			midl_ptr<DRS_MSG_CRACKREPLY_V1> DrsConnection::CrackNames(midl_ptr<DRS_MSG_CRACKREQ_V1> &&request)
			{
				// Validate connection
				if (this->IsInvalid)
				{
					// TODO: Exception type
					throw gcnew Exception("Not connected");
				}
				const DWORD inVersion = 1;
				// TODO: Check the returned version
				DWORD outVersion = 0;
				midl_ptr<DRS_MSG_CRACKREPLY_V1> reply = make_midl_ptr<DRS_MSG_CRACKREPLY_V1>();
				DRS_HANDLE handle = this->handle.ToPointer();
				auto result = IDL_DRSCrackNames_NoSEH(handle, inVersion, (DRS_MSG_CRACKREQ*)request.get(), &outVersion, (DRS_MSG_CRACKREPLY*)reply.get());
				Validator::AssertSuccess((Win32ErrorCode)result);
				return reply;
			}

			String^ DrsConnection::ResolveDistinguishedName(NTAccount^ accountName)
			{
				auto stringAccountName = accountName->Value;
				auto dn = this->TryResolveName(stringAccountName, DS_NAME_FORMAT::DS_NT4_ACCOUNT_NAME, DS_NAME_FORMAT::DS_FQDN_1779_NAME);
				if (dn == nullptr)
				{
					throw gcnew DirectoryObjectNotFoundException(stringAccountName, nullptr);
				}
				return dn;
			}

			String^ DrsConnection::ResolveDistinguishedName(SecurityIdentifier^ objectSid)
			{
				auto stringSid = objectSid->ToString();
				auto dn = this->TryResolveName(stringSid, DS_NAME_FORMAT::DS_SID_OR_SID_HISTORY_NAME, DS_NAME_FORMAT::DS_FQDN_1779_NAME);
				if (dn == nullptr)
				{
					throw gcnew DirectoryObjectNotFoundException(stringSid, nullptr);
				}
				return dn;
			}

			Guid DrsConnection::ResolveGuid(NTAccount^ accountName)
			{
				auto stringAccountName = accountName->Value;
				auto stringGuid = this->TryResolveName(stringAccountName, DS_NAME_FORMAT::DS_NT4_ACCOUNT_NAME, DS_NAME_FORMAT::DS_UNIQUE_ID_NAME);
				if (stringGuid == nullptr)
				{
					throw gcnew DirectoryObjectNotFoundException(stringAccountName, nullptr);
				}
				return Guid::Parse(stringGuid);
			}

			Guid DrsConnection::ResolveGuid(SecurityIdentifier^ objectSid)
			{
				auto stringSid = objectSid->ToString();
				auto stringGuid = this->TryResolveName(stringSid, DS_NAME_FORMAT::DS_SID_OR_SID_HISTORY_NAME, DS_NAME_FORMAT::DS_UNIQUE_ID_NAME);
				if (stringGuid == nullptr)
				{
					throw gcnew DirectoryObjectNotFoundException(stringSid, nullptr);
				}
				return Guid::Parse(stringGuid);
			}

			Guid DrsConnection::ResolveGuid(String^ userPrincipalName)
			{
				auto stringGuid = this->TryResolveName(userPrincipalName, DS_NAME_FORMAT::DS_USER_PRINCIPAL_NAME, DS_NAME_FORMAT::DS_UNIQUE_ID_NAME);
				if (stringGuid == nullptr)
				{
					throw gcnew DirectoryObjectNotFoundException(userPrincipalName, nullptr);
				}
				return Guid::Parse(stringGuid);
			}

			String^ DrsConnection::TryResolveName(String^ name, DS_NAME_FORMAT formatOffered, DS_NAME_FORMAT formatDesired)
			{
				// We only want to resolve 1 name at a time:
				const size_t numItems = 1;
				
				// Prepare the request
				auto request = make_midl_ptr<DRS_MSG_CRACKREQ_V1>(numItems);
				request->formatOffered = formatOffered;
				request->formatDesired = formatDesired;
				request->rpNames[0] = RpcTypeConverter::ToNativeString(name).release();
				
				// Perform RPC call
				auto reply = this->CrackNames(move(request));
				
				// Process the response
				auto item = reply->pResult->rItems[0];
				if (item.status == DS_NAME_ERROR::DS_NAME_NO_ERROR)
				{
					auto name = marshal_as<String^>(item.pName);
					return name;
				}
				else
				{
					// No name translation has been found for some reason.
					return nullptr;
				}
			}

			bool DrsConnection::TestObjectExistence(String^ distinguishedName)
			{
				auto resolvedName = this->TryResolveName(distinguishedName, DS_NAME_FORMAT::DS_FQDN_1779_NAME, DS_NAME_FORMAT::DS_UNIQUE_ID_NAME);
				// Return true if and only if the object exists
				return resolvedName != nullptr;
			}

			bool DrsConnection::TestObjectExistence(Guid objectGuid)
			{
				auto stringGuid = objectGuid.ToString("B");
				auto resolvedName = this->TryResolveName(stringGuid, DS_NAME_FORMAT::DS_UNIQUE_ID_NAME, DS_NAME_FORMAT::DS_FQDN_1779_NAME);
				// Return true if and only if the object exists
				return resolvedName != nullptr;
			}

			bool DrsConnection::ReleaseHandle()
			{
				DRS_HANDLE ptr = this->handle.ToPointer();
				ULONG result = IDL_DRSUnbind_NoSEH(&ptr);
				// Do not validate result, as we do not want any exceptions in ReleaseHandle
				this->handle = (IntPtr)ptr;
				// Return true if the pointer has been nulled by the UnBind operation
				return ptr == nullptr;
			}
			
			midl_ptr<PARTIAL_ATTR_VECTOR_V1_EXT> DrsConnection::CreateNativePas(array<ATTRTYP>^ partialAttributeSet)
			{
				// TODO: Move to type RpcTypeConverter?
				if (partialAttributeSet == nullptr)
				{
					return nullptr;
				}
				auto attrCount = partialAttributeSet->Length;
				if (attrCount < 1)
				{
					// Must request at least one attribute
					return nullptr;
				}
				// Initialize native PAS (maybe just attrCount-1 items, but safety first)
				auto nativePas = make_midl_ptr<PARTIAL_ATTR_VECTOR_V1_EXT>(attrCount);
				// Copy array of attribute ids.
				ATTRTYP* nativePasAttIds = (ATTRTYP*)&nativePas->rgPartialAttr;
				for (int i = 0; i < attrCount; i++)
				{
					nativePasAttIds[i] = partialAttributeSet[i];
				}
				return nativePas;
			}

			array<byte>^ DrsConnection::ReadValue(const ATTRVAL &value)
			{
				// Allocate managed array
				auto managedValue = gcnew array<byte>(value.valLen);
				// Pin it so the GC does not touch it
				pin_ptr<byte> managedValuePin = &managedValue[0];
				// Copy data from native to managed memory
				memcpy(managedValuePin, value.pVal, value.valLen);
				return managedValue;
			}

			array<array<byte>^>^ DrsConnection::ReadValues(const ATTRVALBLOCK &values)
			{
				auto valCount = values.valCount;
				auto valArray = gcnew array<array<byte>^>(valCount);
				for (ULONG i = 0; i < valCount; i++)
				{
					auto value = values.pAVal[i];
					auto managedValue = ReadValue(value);
					valArray[i] = managedValue;
				}
				return valArray;
			}

			ReplicaAttribute^ DrsConnection::ReadAttribute(const ATTR &attribute)
			{
				auto values = ReadValues(attribute.AttrVal);
				auto managedAttribute = gcnew ReplicaAttribute(attribute.attrTyp, values);
				return managedAttribute;
			}

			ReplicaAttribute^ DrsConnection::ReadAttribute(const REPLVALINF_V3 &attribute)
			{
				auto value = ReadValue(attribute.Aval);
				auto managedAttribute = gcnew ReplicaAttribute(attribute.attrTyp, value);
				return managedAttribute;
			}

			ReplicaAttributeCollection^ DrsConnection::ReadAttributes(const ATTRBLOCK &attributes)
			{
				auto attributeCount = attributes.attrCount;
				auto managedAttributes = gcnew ReplicaAttributeCollection(attributeCount);

				for (size_t i = 0; i < attributeCount; i++)
				{
					auto attribute = attributes.pAttr[i];
					auto managedAttribute = ReadAttribute(attribute);
					if (managedAttribute->Values->Length > 0)
					{
						managedAttributes->Add(managedAttribute);
					}
				}
				return managedAttributes;
			}
			ReplicaObject^ DrsConnection::ReadObject(const ENTINF &object)
			{
				auto attributes = ReadAttributes(object.AttrBlock);
				auto guid = ReadGuid(object.pName->Guid);
				auto sid = ReadSid(object.pName);
				auto dn = ReadName(object.pName);
				return gcnew ReplicaObject(dn, guid, sid, attributes);
			}
			ReplicaObjectCollection^ DrsConnection::ReadObjects(const REPLENTINFLIST *objects, int objectCount, const REPLVALINF_V3 *linkedValues, int valueCount)
			{
				// Read linked values first
				// TODO: Handle the case when linked attributes of an object are split between reveral responses.
				auto linkedValueCollection = gcnew ReplicatedLinkedValueCollection();
				for (int i = 0; i < valueCount; i++)
				{
					auto linkedValue = linkedValues[i];
					if (linkedValue.fIsPresent)
					{
						auto objectId = ReadGuid(linkedValue.pObject->Guid);
						auto attribute = ReadAttribute(linkedValue);
						linkedValueCollection->Add(objectId, attribute);
					}
				}

				// Now read the replicated objects
				auto managedObjects = gcnew ReplicaObjectCollection(objectCount);
				auto currentObject = objects;
				while (currentObject != nullptr)
				{
					auto managedObject = ReadObject(currentObject->Entinf);
					managedObject->LoadLinkedValues(linkedValueCollection);
					managedObjects->Add(managedObject);
					currentObject = currentObject->pNextEntInf;
				}
				return managedObjects;
			}

			Guid DrsConnection::ReadGuid(const GUID &guid)
			{
				// TODO: Move to RpcTypeConverter?
				return *reinterpret_cast<Guid *>(const_cast<GUID *>(&guid));
			}

			String^ DrsConnection::ReadName(const DSNAME* dsName)
			{
				// TODO: Move to RpcTypeConverter?
				if (dsName == nullptr || dsName->NameLen <= 0)
				{
					return nullptr;
				}

				wchar_t* nativeName = (wchar_t*)&dsName->StringName;
				return marshal_as<String^>(nativeName);
			}

			SecurityIdentifier^ DrsConnection::ReadSid(const DSNAME* dsName)
			{
				// TODO: Move to type RpcTypeConverter?
				if (dsName == nullptr || dsName->SidLen <= 0)
				{
					return nullptr;
				}

				return gcnew SecurityIdentifier(IntPtr((void*)&dsName->Sid));
			}

			//! This method is called each time a RPC session key is negotiated.
			void DrsConnection::RetrieveSessionKey(void* rpcContext)
			{
				// Retrieve RPC Security Context
				PSecHandle securityContext = nullptr;
				RPC_STATUS rpcStatus = I_RpcBindingInqSecurityContext(rpcContext, (void**)&securityContext);
				if (rpcStatus != RPC_S_OK)
				{
					// We could not acquire the security context, so do not continue with session key retrieval
					return;
				}
				// Retrieve the Session Key information from Security Context
				SecPkgContext_SessionKey nativeKey = {};
				SECURITY_STATUS secStatus = QueryContextAttributes(securityContext, SECPKG_ATTR_SESSION_KEY, &nativeKey);
				// Extract the actual key if the authentication schema uses one
				if (secStatus == SEC_E_OK && nativeKey.SessionKey != nullptr)
				{
					array<byte>^ managedKey = gcnew array<byte>(nativeKey.SessionKeyLength);
					// Pin it so the GC does not touch it
					pin_ptr<byte> pinnedManagedKey = &managedKey[0];
					// Copy data from native to managed memory
					memcpy(pinnedManagedKey, nativeKey.SessionKey, nativeKey.SessionKeyLength);
					// Do not forget to free the unmanaged memory
					secStatus = FreeContextBuffer(nativeKey.SessionKey);
					this->_sessionKey = managedKey;
				}
			}

			DWORD DrsConnection::MaxSupportedReplicationRequestVersion::get()
			{
				DWORD version = 5;
				
				if (this->_serverCapabilities & DRS_EXT::DRS_EXT_GETCHGREQ_V8)
				{
					version = 8;
				}

				if (this->_serverCapabilities & DRS_EXT::DRS_EXT_GETCHGREQ_V10)
				{
					version = 10;
				}
				
				return version;
			}
		}
	}
}