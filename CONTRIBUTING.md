# Contributing to DSInternals

## How can I contribute?

Any contributions to this project are warmly welcome. These are the most important areas:
- Bug reports
- Feature requests
- Testing the module against different Active Directory configurations
- Documentation
    * PowerShell help
    * XML documentation comments in the source code
- Code
    * Bug fixes
    * New features
    * Test cases
    * Code cleanup (StyleCop, FXCop, String to Resource extraction,...)

## Submitting Bug Reports

When submitting a [new bug report](https://github.com/MichaelGrafnetter/DSInternals/issues), please do not forget to mention the version of the target domain controller. The following information would also be very useful:

```powershell
# The entire command that caused the error
Get-History

# Error message
$Error[0].Exception.Message

# The entire exception stack trace
$Error[0].Exception.StackTrace

# OS on which the command was running
(Get-WmiObject -Class Win32_OperatingSystem).Caption

# PowerShell version
$PSVersion

# Anything else that might help identifying and fixing the issue
```

## Building from Source Code

### Development Environment
If you want to build the module from source code yourself, you need to install these programs first:
- [Microsoft Visual Studio Community](https://www.visualstudio.com/en-us/products/visual-studio-community-vs.aspx) 2015 or 2017 with these features installed:
   * Common Tools for Visual C++
   * Windows SDK (you have to retarget the [DSInternals.Replication.Interop](Src/DSInternals.Replication.Interop/DSInternals.Replication.Interop.vcxproj) project to the version you have)
   * PowerShell Tools for Visual Studio (optional)
   * Git for Windows (optional)
   * GitHub Extension for Visual Studio (optional)
- [Windows Management Framework 5](https://www.microsoft.com/en-us/download/details.aspx?id=50395).

### Build Steps
1. Install the prerequisities.
2. Checkout or download the source codes.
3. Run the [Scripts\Make.ps1](Scripts/Make.ps1) script from PowerShell.
4. The resulting module will appear in the *Build\bin\Release* folder.

### Debugging

1. Open the [Src\DSInternals.sln](Src/DSInternals.sln) file in Visual Studio.
2. Put any cmdlets you wish to debug into the [Src\DSInternals.PowerShell\Run-Cmdlets.ps1](Src/DSInternals.PowerShell/Run-Cmdlets.ps1) script.
3. Set the [DSInternals.PowerShell](Src/DSInternals.PowerShell/DSInternals.PowerShell.csproj) project as StartUp Project.
4. Switch to the _Debug_ configuration.
5. Start debugging (F5).

### Continuous Integration

| Release Build + Smoke Tests  |  Debug Build + Unit Tests |
--- | ---
| [![Build Status](https://dev.azure.com/DSInternals/DSInternals%20CI/_apis/build/status/MichaelGrafnetter.DSInternals?branchName=master&jobName=Release)](https://dev.azure.com/DSInternals/DSInternals%20CI/_build/latest?definitionId=2?branchName=master) | [![Build Status](https://dev.azure.com/DSInternals/DSInternals%20CI/_apis/build/status/MichaelGrafnetter.DSInternals?branchName=master&jobName=Test)](https://dev.azure.com/DSInternals/DSInternals%20CI/_build/latest?definitionId=2?branchName=master) |

Automatic build configuration is in the [azure-pipelines.yml](azure-pipelines.yml) file.
