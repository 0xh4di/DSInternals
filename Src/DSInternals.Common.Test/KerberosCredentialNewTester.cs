﻿namespace DSInternals.Common.Test
{
    using DSInternals.Common.Data;
    using Microsoft.VisualStudio.TestTools.UnitTesting;

    [TestClass]
    public class KerberosCredentialNewTester
    {
        [TestMethod]
        public void KerberosCredentialNew_Vector1()
        {
            byte[] blob = "0400000003000000030003001c001c00080100000010000000000000000000000010000012000000200000002401000000000000000000000010000011000000100000004401000000000000000000000010000003000000080000005401000000000000000000000010000012000000200000005c01000000000000000000000010000011000000100000007c01000000000000000000000010000003000000080000008c0100000000000000000000001000001200000020000000940100000000000000000000001000001100000010000000b40100000000000000000000001000000300000008000000c4010000000000000000000000000000000000000000000000000000410044004100540055004d002e0043004f004d0074006500730074000f4eb676104ad1fcac9a8fb9f2a19d6477c52ca584b4be6894851d469bf1049ca477bb826e98d86465ff4915b859c6cd453820da83b6d64a0f4eb676104ad1fcac9a8fb9f2a19d6477c52ca584b4be6894851d469bf1049ca477bb826e98d86465ff4915b859c6cd453820da83b6d64a0f4eb676104ad1fcac9a8fb9f2a19d6477c52ca584b4be6894851d469bf1049ca477bb826e98d86465ff4915b859c6cd453820da83b6d64a".HexToBinary();
            var credential = new KerberosCredentialNew(blob);

            // Serialize the structure
            byte[] newBlob = credential.ToByteArray();

            // Try to parse it again
            var newCredential = new KerberosCredentialNew(newBlob);

            // Check that we have the same key material
            Assert.AreEqual(credential.DefaultSalt, newCredential.DefaultSalt);
            Assert.AreEqual(credential.OlderCredentials[0].ToString(), newCredential.OlderCredentials[0].ToString());

            // Check binary equality
            Assert.AreEqual(blob.ToHex(), newBlob.ToHex());
        }

        [TestMethod]
        public void KerberosCredentialNew_Vector2()
        {
            byte[] blob = "0400000003000000000000002000200078000000001000000000000000000000001000001200000020000000980000000000000000000000001000001100000010000000b80000000000000000000000001000000300000008000000c8000000000000000000000000000000000000000000000000000000410044004100540055004d002e0043004f004d00750073006500720030003200622fca1d12de1df65e840afd5f3b9844b18a72a82aed6e3e58c39257753fdfa830666974cb729ef6bd44f8eab3296e5c13f8fd37d557a401".HexToBinary();
            var credential = new KerberosCredentialNew(blob);

            // Serialize the structure
            byte[] newBlob = credential.ToByteArray();

            // Try to parse it again
            var newCredential = new KerberosCredentialNew(newBlob);

            // Check that we have the same key material
            Assert.AreEqual(credential.DefaultSalt, newCredential.DefaultSalt);
            Assert.AreEqual(credential.Credentials[2].ToString(), newCredential.Credentials[2].ToString());

            // Check binary equality
            Assert.AreEqual(blob.ToHex(), newBlob.ToHex());
        }

        [TestMethod]
        public void KerberosCredentialNew_Vector3()
        {
            byte[] blob = "04000000030000000300000020002000c0000000001000000000000000000000001000001200000020000000e0000000000000000000000000100000110000001000000000010000000000000000000000100000030000000800000010010000000000000000000000100000120000002000000018010000000000000000000000100000110000001000000038010000000000000000000000100000030000000800000048010000000000000000000000000000000000000000000000000000410044004100540055004d002e0043004f004d007500730065007200300033007dbf2b822719450fc53051434bd83caad3a4624b2af1e89dac2f56e3d81d6f97c3be2bcd9ae33fed8b83884cd028e25962e967e60bd675ea7dbf2b822719450fc53051434bd83caad3a4624b2af1e89dac2f56e3d81d6f97c3be2bcd9ae33fed8b83884cd028e25962e967e60bd675ea".HexToBinary();
            var credential = new KerberosCredentialNew(blob);

            // Serialize the structure
            byte[] newBlob = credential.ToByteArray();

            // Try to parse it again
            var newCredential = new KerberosCredentialNew(newBlob);

            // Check that we have the same key material
            Assert.AreEqual(credential.DefaultSalt, newCredential.DefaultSalt);
            Assert.AreEqual(credential.Credentials[1].ToString(), newCredential.Credentials[1].ToString());

            // Check binary equality
            Assert.AreEqual(blob.ToHex(), newBlob.ToHex());
        }

        [TestMethod]
        public void KerberosCredentialNew_Vector4()
        {
            byte[] blob = "0400000003000000030003002000200008010000001000000000000000000000001000001200000020000000280100000000000000000000001000001100000010000000480100000000000000000000001000000300000008000000580100000000000000000000001000001200000020000000600100000000000000000000001000001100000010000000800100000000000000000000001000000300000008000000900100000000000000000000001000001200000020000000980100000000000000000000001000001100000010000000b80100000000000000000000001000000300000008000000c8010000000000000000000000000000000000000000000000000000410044004100540055004d002e0043004f004d00750073006500720030003400b22f604f44cfc2e2ca421f6ec3a91dd3af73019b301485d46b75f1ac02fbd356e8013160c038ba5f67b69ff7963e1ab0c86b313bbc89b5f1ded853490b1fde39406b73b615a0c0ead24401d62b79f9b77d1bee0c30ff975d14e0b8825a02d2941c3c0e6a5560f97401730ed90764515eb22f604f44cfc2e2ca421f6ec3a91dd3af73019b301485d46b75f1ac02fbd356e8013160c038ba5f67b69ff7963e1ab0c86b313bbc89b5f1".HexToBinary();
            var credential = new KerberosCredentialNew(blob);

            // Serialize the structure
            byte[] newBlob = credential.ToByteArray();

            // Try to parse it again
            var newCredential = new KerberosCredentialNew(newBlob);

            // Check that we have the same key material
            Assert.AreEqual(credential.DefaultSalt, newCredential.DefaultSalt);
            Assert.AreEqual(credential.OlderCredentials[0].ToString(), newCredential.OlderCredentials[0].ToString());

            // Check binary equality
            Assert.AreEqual(blob.ToHex(), newBlob.ToHex());
        }

        [TestMethod]
        public void KerberosCredentialNew_Vector5()
        {
            byte[] blob = "0400000003000000030003004000400008010000001000000000000000000000001000001200000020000000480100000000000000000000001000001100000010000000680100000000000000000000001000000300000008000000780100000000000000000000001000001200000020000000800100000000000000000000001000001100000010000000a00100000000000000000000001000000300000008000000b00100000000000000000000001000001200000020000000b80100000000000000000000001000001100000010000000d80100000000000000000000001000000300000008000000e8010000000000000000000000000000000000000000000000000000410044004100540055004d002e0043004f004d0068006f00730074006c006f006e002d006400630031002e00610064006100740075006d002e0063006f006d0083d5cf5352e9035ee6e9fbf782f60bc398f523d2c0a0de8ad86bba73192d6d510045f6350d94badb832fd631b3f3f98c7091ce8545613d31e9acde11592501ea389d5ddaa582ea365b5b86a361c3b5329362230ebf6dd00e986e33f385e866acc436cb98c126900da4cd57ea0b3d404aa35646d961ba8e94e52ae02f6f1815a50f9aa0a4baec7b185af8feb33884ce3398cf5c8f7d0e67a3fd4f97038a74184eda10a72af87c1f01".HexToBinary();
            var credential = new KerberosCredentialNew(blob);

            // Serialize the structure
            byte[] newBlob = credential.ToByteArray();

            // Try to parse it again
            var newCredential = new KerberosCredentialNew(newBlob);

            // Check that we have the same key material
            Assert.AreEqual(credential.DefaultSalt, newCredential.DefaultSalt);
            Assert.AreEqual(credential.OlderCredentials[0].ToString(), newCredential.OlderCredentials[0].ToString());

            // Check binary equality
            Assert.AreEqual(blob.ToHex(), newBlob.ToHex());
        }
    }
}