---
external help file: DSInternals.PowerShell.dll-Help.xml
Module Name: DSInternals
online version: https://github.com/MichaelGrafnetter/DSInternals/blob/master/Documentation/PowerShell/ConvertFrom-ADManagedPasswordBlob.md
schema: 2.0.0
---

# ConvertFrom-ADManagedPasswordBlob

## SYNOPSIS
Decodes the value of the msDS-ManagedPassword attribute of a Group Managed Service Account.

## SYNTAX

```
ConvertFrom-ADManagedPasswordBlob [-Blob] <Byte[]> [<CommonParameters>]
```

## DESCRIPTION
{{Fill in the Description}}

## EXAMPLES

### Example 1
```powershell
PS C:\> {{ Add example code here }}
```

{{ Add example description here }}

## PARAMETERS

### -Blob
{{Fill Blob Description}}

```yaml
Type: Byte[]
Parameter Sets: (All)
Aliases: msDS-ManagedPassword, ManagedPassword, ManagedPasswordBlob

Required: True
Position: 0
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### CommonParameters
This cmdlet supports the common parameters: -Debug, -ErrorAction, -ErrorVariable, -InformationAction, -InformationVariable, -OutVariable, -OutBuffer, -PipelineVariable, -Verbose, -WarningAction, and -WarningVariable. For more information, see about_CommonParameters (http://go.microsoft.com/fwlink/?LinkID=113216).

## INPUTS

### None
## OUTPUTS

### DSInternals.Common.Data.ManagedPassword
## NOTES

## RELATED LINKS
