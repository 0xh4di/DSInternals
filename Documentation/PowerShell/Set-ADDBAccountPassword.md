---
external help file: DSInternals.PowerShell.dll-Help.xml
Module Name: DSInternals
online version: https://github.com/MichaelGrafnetter/DSInternals/blob/master/Documentation/PowerShell/Set-ADDBAccountPassword.md
schema: 2.0.0
---

# Set-ADDBAccountPassword

## SYNOPSIS
Sets the password for a user, computer, or service account stored in a ntds.dit file.

## SYNTAX

### ByName
```
Set-ADDBAccountPassword -NewPassword <SecureString> -BootKey <Byte[]> [-SkipMetaUpdate]
 [-SamAccountName] <String> -DBPath <String> [-LogPath <String>] [<CommonParameters>]
```

### BySID
```
Set-ADDBAccountPassword -NewPassword <SecureString> -BootKey <Byte[]> [-SkipMetaUpdate]
 -ObjectSid <SecurityIdentifier> -DBPath <String> [-LogPath <String>] [<CommonParameters>]
```

### ByDN
```
Set-ADDBAccountPassword -NewPassword <SecureString> -BootKey <Byte[]> [-SkipMetaUpdate]
 -DistinguishedName <String> -DBPath <String> [-LogPath <String>] [<CommonParameters>]
```

### ByGuid
```
Set-ADDBAccountPassword -NewPassword <SecureString> -BootKey <Byte[]> [-SkipMetaUpdate] -ObjectGuid <Guid>
 -DBPath <String> [-LogPath <String>] [<CommonParameters>]
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

### -BootKey
{{Fill BootKey Description}}

```yaml
Type: Byte[]
Parameter Sets: (All)
Aliases: Key, SysKey, SystemKey

Required: True
Position: Named
Default value: None
Accept pipeline input: False
Accept wildcard characters: False
```

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

### -NewPassword
Specifies a new password value. This value is stored as an encrypted string.

```yaml
Type: SecureString
Parameter Sets: (All)
Aliases: Password, Pwd, Pass, AccountPassword, p

Required: True
Position: Named
Default value: None
Accept pipeline input: True (ByPropertyName)
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

### System.Security.SecureString
### System.String
### System.Security.Principal.SecurityIdentifier
### System.Guid
## OUTPUTS

### None
## NOTES

## RELATED LINKS

[Set-ADDBAccountPasswordHash](Set-ADDBAccountPasswordHash.md)
[Set-SamAccountPasswordHash](Set-SamAccountPasswordHash.md)
[Get-BootKey](Get-BootKey.md)
[Get-ADDBAccount](Get-ADDBAccount.md)