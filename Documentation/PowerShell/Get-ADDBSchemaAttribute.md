---
external help file: DSInternals.PowerShell.dll-Help.xml
Module Name: DSInternals
online version: https://github.com/MichaelGrafnetter/DSInternals/blob/master/Documentation/PowerShell/Get-ADDBSchemaAttribute.md
schema: 2.0.0
---

# Get-ADDBSchemaAttribute

## SYNOPSIS
Reads AD schema from a ntds.dit file, including datatable column names.

## SYNTAX

```
Get-ADDBSchemaAttribute [[-Name] <String[]>] -DBPath <String> [-LogPath <String>] [<CommonParameters>]
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

### -DBPath
TODO

```yaml
Type: String
Parameter Sets: (All)
Aliases: Database, DatabasePath, DatabaseFilePath, DBFilePath

Required: True
Position: Named
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -LogPath
TODO

```yaml
Type: String
Parameter Sets: (All)
Aliases: Log, TransactionLogPath

Required: False
Position: Named
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### -Name
TODO

```yaml
Type: String[]
Parameter Sets: (All)
Aliases: LdapDisplayName,AttributeName,AttrName,Attr

Required: False
Position: 0
Default value: None
Accept pipeline input: True (ByValue)
Accept wildcard characters: False
```

### CommonParameters
This cmdlet supports the common parameters: -Debug, -ErrorAction, -ErrorVariable, -InformationAction, -InformationVariable, -OutVariable, -OutBuffer, -PipelineVariable, -Verbose, -WarningAction, and -WarningVariable. For more information, see about_CommonParameters (http://go.microsoft.com/fwlink/?LinkID=113216).

## INPUTS

### System.String[]
## OUTPUTS

### DSInternals.PowerShell.SchemaAttribute
## NOTES

## RELATED LINKS
