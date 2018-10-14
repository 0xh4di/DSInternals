---
external help file: DSInternals.PowerShell.dll-Help.xml
Module Name: DSInternals
online version: https://github.com/MichaelGrafnetter/DSInternals/blob/master/Documentation/PowerShell/Enable-ADDBAccount.md
schema: 2.0.0
---

# Enable-ADDBAccount

## SYNOPSIS
Enables an Active Directory account in an offline ntds.dit file.

## SYNTAX

### ByName
```
Enable-ADDBAccount [-SkipMetaUpdate] [-SamAccountName] <String> -DBPath <String> [-LogPath <String>]
 [<CommonParameters>]
```

### BySID
```
Enable-ADDBAccount [-SkipMetaUpdate] -ObjectSid <SecurityIdentifier> -DBPath <String> [-LogPath <String>]
 [<CommonParameters>]
```

### ByDN
```
Enable-ADDBAccount [-SkipMetaUpdate] -DistinguishedName <String> -DBPath <String> [-LogPath <String>]
 [<CommonParameters>]
```

### ByGuid
```
Enable-ADDBAccount [-SkipMetaUpdate] -ObjectGuid <Guid> -DBPath <String> [-LogPath <String>]
 [<CommonParameters>]
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

### -DistinguishedName
TODO

```yaml
Type: String
Parameter Sets: ByDN
Aliases: dn

Required: True
Position: Named
Default value: None
Accept pipeline input: True (ByPropertyName)
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

### -ObjectGuid
TODO

```yaml
Type: Guid
Parameter Sets: ByGuid
Aliases: Guid

Required: True
Position: Named
Default value: None
Accept pipeline input: True (ByPropertyName)
Accept wildcard characters: False
```

### -ObjectSid
TODO

```yaml
Type: SecurityIdentifier
Parameter Sets: BySID
Aliases: Sid

Required: True
Position: Named
Default value: None
Accept pipeline input: True (ByPropertyName)
Accept wildcard characters: False
```

### -SamAccountName
TODO

```yaml
Type: String
Parameter Sets: ByName
Aliases: Login, sam

Required: True
Position: 0
Default value: None
Accept pipeline input: True (ByPropertyName)
Accept wildcard characters: False
```

### -SkipMetaUpdate
TODO

```yaml
Type: SwitchParameter
Parameter Sets: (All)
Aliases: SkipMeta, NoMetaUpdate, NoMeta, SkipObjMeta, NoObjMeta, SkipMetaDataUpdate, NoMetaDataUpdate

Required: False
Position: Named
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

### CommonParameters
This cmdlet supports the common parameters: -Debug, -ErrorAction, -ErrorVariable, -InformationAction, -InformationVariable, -OutVariable, -OutBuffer, -PipelineVariable, -Verbose, -WarningAction, and -WarningVariable. For more information, see about_CommonParameters (http://go.microsoft.com/fwlink/?LinkID=113216).

## INPUTS

### System.String
### System.Security.Principal.SecurityIdentifier
### System.Guid
## OUTPUTS

### None
## NOTES

## RELATED LINKS

[Disable-ADDBAccount](Disable-ADDBAccount.md)
[Get-ADDBAccount](Get-ADDBAccount.md)
