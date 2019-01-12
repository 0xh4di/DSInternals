﻿<#
.SYNOPSIS
Refreshes MD documentation files and builds MAML files.

.AUTHOR
Przemysław Kłys (@PrzemyslawKlys)
#>
#Requires -Version 5 -Module platyPS

# Set directory paths
$rootDir = Split-Path $PSScriptRoot -Parent
$locale = 'en-US'
$dsInternalsModulePath = Join-Path $rootDir 'Build\bin\Release\DSInternals'
$mdHelpPath = Join-Path $rootDir 'Documentation\PowerShell'
$modulePagePath = Join-Path $mdHelpPath 'Readme.md'
$xmlHelpPath = Join-Path $rootDir "Src\DSInternals.PowerShell\$locale"

# Import dependencies
Import-Module -Name platyPS
Import-Module -Name $dsInternalsModulePath

<#
Note: This code has been used to create the initial version of the help files:
New-MarkdownHelp -Module DSInternals -AlphabeticParamsOrder -Locale $locale -WithModulePage -HelpVersion 1.0 -OutputFolder $mdHelpPath
New-MarkdownAboutHelp -AboutName DSInternals -OutputFolder $mdHelpPath
#>

# Update MD files
Update-MarkdownHelpModule -Path $mdHelpPath -ModulePagePath $modulePagePath -RefreshModulePage -AlphabeticParamsOrder -UpdateInputOutput

# Generate the MAML file
New-ExternalHelp -Path $mdHelpPath -OutputPath $xmlHelpPath -Force -ShowProgress
