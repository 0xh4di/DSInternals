﻿namespace DSInternals.PowerShell.Commands
{
    using System;
    using System.Management.Automation;
    using System.Security.Cryptography.X509Certificates;
    using DSInternals.Common;
    using DSInternals.Common.Data;

    [Cmdlet(VerbsCommon.Get, "ADKeyCredential", DefaultParameterSetName = ParamSetFromCertificate)]
    [OutputType(new Type[] { typeof(KeyCredential) })]
    public class GetADKeyCredentialCommand : PSCmdlet
    {
        #region Parameters
        private const string ParamSetFromCertificate = "FromCertificate";
        private const string ParamSetFromBinary = "FromBinary";

        [Parameter(
            Mandatory = true,
            Position = 0,
            ParameterSetName = ParamSetFromBinary
        )]
        [AcceptHexString]
        public byte[] Input
        {
            get;
            set;
        }

        [Parameter(
            Mandatory = true,
            Position = 0,
            ParameterSetName = ParamSetFromCertificate
        )]
        public X509Certificate2 Certificate
        {
            get;
            set;
        }

        [Parameter(
            Mandatory = true,
            Position = 1,
            ParameterSetName = ParamSetFromCertificate
        )]
        public Guid  DeviceId
        {
            get;
            set;
        }
        #endregion Parameters

        #region Cmdlet Overrides

        protected override void ProcessRecord()
        {
            KeyCredential keyCredential;
            switch(this.ParameterSetName)
            {
                case ParamSetFromBinary:
                    keyCredential = new KeyCredential(this.Input);
                    break;
                case ParamSetFromCertificate:
                default:
                    byte[] publicKey = this.Certificate.ExportPublicKeyBlob();
                    keyCredential = new KeyCredential(publicKey, this.DeviceId);
                    break;
            }
            this.WriteObject(keyCredential);
        }

        #endregion Cmdlet Overrides
    }
}