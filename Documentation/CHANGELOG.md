# Changelog
All notable changes to this project will be documented in this file. The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [Unreleased]
### Added
- Implemented a slightly more secure handling of [GMSA passwords](../Src/DSInternals.Common/Data/Principals/ManagedPassword.cs).

### Fixed
- A more explanatory exception is now thrown when opening databases that originate from different OS versions.

## [3.2.1] - 2019-01-04
### Fixed
- The implementation of database re-encryption now behaves more closely to Windows Server 2016.

## [3.2] - 2019-01-03
### Added
- [Module] Added the `Get-LsaBackupKey` cmdlet for DPAPI domain backup key retrieval through LSARPC.
- [Framework] Added support for DPAPI domain backup key retrieval from LSA Policy.

### Changed
- [Module] The `Set-ADDBBootKey` cmdlet now works with Windows Server 2000-2019 databases.
- [Module] The `New-ADDBRestoreFromMediaScript` cmdlet now uses shutdown.exe instead of Restart-Computer.
- [Framework] Updated package references.

### Fixed
- [Framework] Fixed `DSInternals.Replication.Interop` assembly versioning.

## [3.1] - 2018-12-29
### Added
- [Module] Added the `New-ADDBRestoreFromMediaScript` cmdlet to aid with file-level DC recovery process.
- [Module] Added the `Get-LSAPolicyInformation` and `Set-LSAPolicyInformation` cmdlets that can be used to retrieve and change domain-related LSA Policies.
- [Module] Extended the information returned by the `Get-ADDBDomainController` cmdlet.
- [Module] Added MAML documentation for `Get-Help`.
- [Framework] Added support for LSA Policy retrieval and modification.

### Changed
- [Framework] Implemented distinguished name (DN) caching in the database access layer.

### Fixed
- [Module] Path to the DSInternals.psd1 file now does not need to be specified when loading the module from a non-default location.

## [3.0] - 2018-09-29
### Added
- [Module] Added the `Set-ADDBAccountPassword` and `Set-ADDBAccountPasswordHash` cmdlets for offline password modification.
- [Module] The `Test-PasswordQuality` cmdlet now supports NTLM hash list from haveibeenpwned.com.
- [Module] Added the `Get-ADKeyCredential` cmdlet for linked credential generation (AKA Windows Hello for Business).
- [Module] The `Get-ADDBAccount`, `Get-ADReplAccount` and `Get-ADSIAccount` cmdlets now display linked credentials.
- [Module] Databases from Windows Server 2016 can now be read on non-DCs.
- [Module] Added the `ConvertTo-KerberosKey` cmdlet for key generation.
- [Module] The `Save-DPAPIBlob` now generates scripts for mimikatz.
- [Module] The `Save-DPAPIBlob` cmdlet now accepts pipeline input from both Get-ADDBBackupKey and ADDBAccount cmdlets.
- [Module] Added Views `JohnNTHistory`, `HashcatNTHistory` and `NTHashHistory`.
- [Module] The `Get-ADDBDomainController` now displays domain and forest functional levels.
- [Module] The `Set-ADDBDomainController` can now be used to modify backup expiration.
- [Module] The `Get-ADDBAccount` cmdlet now reports progress when retrieving multiple accounts.
- [Framework] Added support for offline password changes.
- [Framework] Added support for kerberos key derivation.
- [Framework] Added support for WDigest hash calculation.

### Fixed
- [Framework] Minor bug fixes.

### Removed
- [Module] Removed the `ConvertTo-NTHashDictionary` cmdlet as its functionality had been integrated into the `Test-PasswordQuality` cmdlet.

## [2.23] - 2018-07-07
### CHanged
- [Module] The `Test-PasswordQuality` now supports accounts that require smart card authentication.

### Fixed
- [Module] Fixed a bug in in the processing of the `-SkipDuplicatePasswordTest` switch of the `Test-PasswordQuality` cmdlet.

## [2.22] - 2017-04-29
### Added
- [Framework] Added the `Enable-ADDBAccount` and `Disable-ADDBAccount` cmdlets.
- [Module] Added the ability to enable or disable accounts in offline databases.

## [2.21.2] - 2017-04-19
### Fixed
- [Framework] Fixed a bug in roamed credentials processing.
- [Module] Fixed a bug in hexadecimal parameter parsing. 

## [2.21.1] - 2017-04-14
### Fixed
- Fixed a bug in linked value replication.

## [2.21] - 2017-03-25
- [Module] The replication cmdlets now use Kerberos authentication by default. 
- [Module] Added support for roamed credentials.
- [Module] Cmdlets now accept hashes in both byte array and hexadecimal string forms.
- [Framework] Added support for linked value retrieval.
- [Framework] Updated referenced packages.
- [Framework] Added the SamEnumerateDomainsInSamServer call.

## [2.20] - 2016-11-15
- Added the `Get-ADPasswordPolicy` cmdlet.

## [2.19] - 2016-10-21
- Added support for the ServicePrincipalName attribute.

## [2.18] - 2016-10-02
- [Module] Added the `Get-ADDBKdsRootKey` cmdlet to aid DPAPI-NG decryption, e.g. SID-protected PFX files.
- [Module] The `Get-ADReplAccount` cmdlet now correctly reports the access denied error.
- [Module] Fixed a bug in progress reporting of the `Get-ADReplAccount` cmdlet.
- [Framework] Added support for KDS Root Key retrieval.
- [Framework] Replication errors are now reported using more suitable exception types.

## 2.17 - 2016-09-16
- [Module] The `Get-ADReplAccount -All` command now reports replication progress.
- [Framework] Added the ability to retrieve the replication cursor.
- [Framework] The `ReplicationCookie` class is now immutable and replication progress is reported using a delegate.
- [Framework] Win32 exceptions are now translated to more specific .NET exceptions by the `Validator` class.

## [2.16.1] - 2016-08-08
- [Module] Added the `-ShowPlainTextPasswords` parameter to the `Test-PasswordQuality` cmdlet.
  Cracked and cleartext passwords now do not get displayed by default.

## [2.16] - 2016-08-07
- [Module] Added the `Test-PasswordQuality` and `ConvertTo-NTHashSet` cmdlets.
- [Module] Added support for the the UserAccountControl attribute of user accounts.
- [Framework] Added the ability to replicate user accounts by specifying their UPN.
- [Framework] Added the ability to calculate a NT hash from both String and SecureString.
- [Framework] Added the `HashEqualityComparer`, which allows the hashes to be stored 
  in the built-in generic collections.

## [2.15] - 2016-06-18
- Removed dependency on ADSI.
- Added support for the PAM optional feature. 
- Added the `PWDump` custom view.
- Added the `HashNT` custom view.
- Added the `HashLM` custom view.

## [2.14] - 2016-04-30
- Added support for Windows Server 2016 ntds.dit encryption.
- Added support for replication with renamed domains.
- Added support for reading security descriptors (ACLs) from both ntds.dit files and DRS-R.
- Added support for the AdminCount attribute.
- Updated the forked ManagedEsent source codes to version 1.9.3.3.

## [2.13.1] - 2016-02-25
- Fixed a bug regarding incorrect OS version detection.

## [2.13] - 2016-02-21
- Fixed a rare bug which caused the database cmdlets to hang while loading indices.
- Meaningful error messages are now displayed when a dirty or downlevel ntds.dit file is encountered.
- The `DSInternals.Replication` library now supports incremental replication (not exposed through PowerShell).

## [2.12] - 2016-02-07
- Commandlets for ntds.dit manipulation now work on Windows 7 / Windows Server 2008 R2.
- The module now requires .NET Framework 4.5.1 instead of 4.5.
- Both Visual Studio 2013 and 2015 are now supported platforms.

## 2.11.1 - 2016-02-03
- Added support for Windows Server 2003 R2.
- The replication now works on x86, again.
- Fixed a bug in temporary index loading.

## [2.10] - 2016-01-14
- Added support for the NTLM-Strong-NTOWF package in Supplemental Credentials (new in Windows Server 2016 TP4)
- Added support for initial databases
- Added partial support for ADAM/LDS databases
- The `Get-ADDBSchemaAttribute` now shows attribute OIDs
- Fixed a bug in Exchange schema loading

## 2.9 - 2015-12-27
- The `Get-BootKey` cmdlet now supports online boot key retrieval
- The PBKDF2.NET library has been replaced by CryptSharp
- The `Get-ADDBDomainController` cmdlet now extracts some more data from the DB
- The project has been open-sourced

## 2.8 - 2015-10-20
- Added the `ConvertFrom-ADManagedPasswordBlob` cmdlet
- Added the `Get-ADDBBackupKey` cmdlet
- Added the `Get-ADReplBackupKey` cmdlet
- Added the `Save-DPAPIBlob` cmdlet
- Added the `HashcatLM` view

## 2.7 - 2015-09-30
- Added the `about_DSInternals` help page (work in progress)
- Fixed a bug in the `Set-ADDBPrimaryGroup` cmdlet

## 2.6 - 2015-09-21
- Implemented CRC checks in the `Get-ADReplAccount` cmdlet
- The `Get-ADReplAccount` cmdlet now displays meaningful error messages on 64-bit systems
- The `-Server` parameter of the `Get-ADReplAccount` is now compulsory instead of localhost being default
- The `Get-ADReplAccount` and `Set-SamAccountPasswordHash` cmdlets now display a warning in case they are supplied with a DNS domain name instead of a NetBIOS one.
- Fixed a bug in SupplementalCredentials parsing

## 2.5 - 2015-09-14
- Both x86 and x64 platforms are now supported.
- A few parameters have been changed and new aliases added.
- Fixed a bug in the `Add-ADDBSidHistory` cmdlet.

## 2.4 - 2015-09-05
- Fixed a bug regarding distinguished name parsing in the `Get-ADDBAccount` cmdlet
- Removed a big memory leak in the `Get-ADReplAccount` cmdlet
- Added the `Get-ADReplicationAccount` alias for `Get-ADReplAccount`
- Updated AutoMapper to the latest version
- Switched to the official build of Microsoft's Managed Esent libraries
- The module has been published in PowerShell Gallery.

## 2.3
- Parameter `-SystemHiveFilePath` of the `Get-BootKey` cmdlet is now positional
- Added the Readme.txt file with system requirements
- Fixed a bug in distinguished name parsing that caused the `Get-ADReplAccount` cmdlet to fail under some circumstances

## 2.2
### Changed
- Added a few parameter validations
### Fixed
- Fixed a bug in SupplementalCredentials parsing

## 2.1
- The `Get-ADReplAccount` cmdlet can now retrieve all accounts from AD or just a sigle one
- Added Microsoft Visual C++ 2013 Runtime libraries to the distribution
- The module is now 64-bit only
- Minor bug fixes

## 2.0 - 2015-07-14
- Added the `Get-ADDBAccount` cmdlet
- Added the `Get-BootKey` cmdlet
- Added the `Get-ADReplAccount` cmdlet
- Added the `Remove-ADDBObject` cmdlet
- Added the `Format-Hex` cmdlet
- Merged the `DSInternals.Cryptography` assembly into `DSInternals.Common`
- Minor bug fixes

## 1.6
### Added
- Added the `Set-ADDBDomainController` cmdlet
- Added the `Get-ADDBSchemaAttribute` cmdlet

## 1.5
### Added
- Added the `Get-ADDBDomainController` cmdlet

## 1.4 - 2015-05-31
### Added
- Added the `Set-ADDBPrimaryGroup` cmdlet

### Fixed
- The `Add-ADDBSidHistory` cmdlet now supports relative file paths

## 1.3.1
### Fixed
- Fixed a bug in the Microsoft.Isam.Esent.Interop library,
  that prevented the `Add-ADDBSidHistory` cmdlet to run on Windows Server 2008 R2

## 1.3 - 2015-05-24
### Added
- Added the `Add-ADDBSidHistory` cmdlet

## 1.2
### Added
- Added the `ConvertTo-GPPrefPassword` cmdlet

## 1.1
### Added
- Added the `ConvertTo-OrgIdHash` cmdlet
- Added the `ConvertFrom-GPPrefPassword` cmdlet

## 1.0 - 2015-01-20
Initial release!

[Unreleased]: https://github.com/MichaelGrafnetter/DSInternals/compare/v3.2.1...HEAD
[3.2.1]: https://github.com/MichaelGrafnetter/DSInternals/compare/v3.2...v3.2.1
[3.2]: https://github.com/MichaelGrafnetter/DSInternals/compare/v3.1...v3.2
[3.1]: https://github.com/MichaelGrafnetter/DSInternals/compare/v3.0...v3.1
[3.0]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.23...v3.0
[2.23]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.22...v2.23
[2.22]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.21.2...v2.22
[2.21.2]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.21.1...v2.21.2
[2.21.1]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.21...v2.21.1
[2.21]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.20...v2.21
[2.20]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.19...v2.20
[2.19]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.18...v2.19
[2.18]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.16.1...v2.18
[2.16.1]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.16...v2.16.1
[2.16]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.15...v2.16
[2.15]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.14...v2.15
[2.14]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.13.1...v2.14
[2.13.1]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.13...v2.13.1
[2.13]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.12...v2.13
[2.12]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.11...v2.12
[2.11]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.10...v2.11
[2.10]: https://github.com/MichaelGrafnetter/DSInternals/compare/v2.9...v2.10
