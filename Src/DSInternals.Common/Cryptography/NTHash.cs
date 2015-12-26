﻿using DSInternals.Common;
using DSInternals.Common.Interop;
using System.Security;

namespace DSInternals.Common.Cryptography
{
    // See http://msdn.microsoft.com/en-us/library/system.security.cryptography.hashalgorithm%28v=vs.110%29.aspx
    public static class NTHash
    {
        /// <summary>
        /// The size, in bytes, of the computed hash code.
        /// </summary>
        public const int HashSize = NativeMethods.NTHashNumBytes;
        public const int MaxInputLength = NativeMethods.NTPasswordMaxChars;

        public static byte[] ComputeHash(SecureString password)
        {
            Validator.AssertNotNull(password, "password");
            byte[] hash;
            using(SafeUnicodeSecureStringPointer passwordPtr = new SafeUnicodeSecureStringPointer(password))
            {
                NtStatus result = NativeMethods.RtlCalculateNtOwfPassword(passwordPtr, out hash);
                Validator.AssertSuccess(result);
            }
            return hash;
        }
    }
}
