﻿namespace DSInternals.PowerShell.Commands
{
    using System;
    using System.Management.Automation;
    using DSInternals.Common;
    using DSInternals.Common.Cryptography;
    using DSInternals.DataStore;

    [Cmdlet(VerbsCommon.Set, "ADDBBootKey")]
    public class SetADDBBootKeyCommand : ADDBCommandBase
    {
        [Parameter(
            Mandatory = true,
            HelpMessage = "TODO"
        )]
        [ValidateNotNullOrEmpty]
        [ValidateHexString(BootKeyRetriever.BootKeyLength)]
        [Alias("OldKey", "Old", "OldSysKey")]
        public string OldBootKey
        {
            get;
            set;
        }

        [Parameter(
            Mandatory = false,
            HelpMessage = "TODO"
        )]
        [ValidateNotNullOrEmpty]
        [ValidateHexString(BootKeyRetriever.BootKeyLength)]
        [Alias("NewKey", "New", "NewSysKey")]
        public string NewBootKey
        {
            get;
            set;
        }


        protected override void BeginProcessing()
        {
            base.BeginProcessing();
            byte[] oldBinaryBootKey = this.OldBootKey.HexToBinary();
            byte[] newBinaryBootKey = this.NewBootKey.HexToBinary();
            using(var directoryAgent = new DirectoryAgent(this.DirectoryContext))
            {
                directoryAgent.ChangeBootKey(oldBinaryBootKey, newBinaryBootKey);
            }
            // TODO: Verbosity
            // TODO: Exception handling
        }

        protected override bool ReadOnly
        {
            get
            {
                // We need to modify the PEK List attribute.
                return false;
            }
        }
    }
}