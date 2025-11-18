# Lesson 5: DLL Dependencies

## 📚 Learning Objectives

By the end of this lesson, you will understand:
- What DLL dependencies are
- The dependency chain concept
- How to manage dependencies in .NET
- Tools for analyzing dependencies
- Common dependency problems and solutions
- NuGet package management
- The Global Assembly Cache (GAC)

## 🎯 What Are DLL Dependencies?

A **dependency** is when one DLL needs another DLL to function.

### Simple Example

```
Your Application depends on:
  YourApp.exe
    └── requires → MathLib.dll

MathLib.dll depends on:
  MathLib.dll
    └── requires → UtilityLib.dll

Result: YourApp.exe → MathLib.dll → UtilityLib.dll
```

**Dependency Chain:** YourApp needs MathLib needs UtilityLib

## 📊 Visualizing Dependencies

### Example 1: Simple Chain

```
┌──────────────┐
│ YourApp.exe  │
└──────┬───────┘
       │ needs
       ▼
┌──────────────┐
│ Graphics.dll │
└──────┬───────┘
       │ needs
       ▼
┌──────────────┐
│ System.dll   │
└──────────────┘
```

### Example 2: Multiple Dependencies

```
                ┌──────────────┐
                │ YourApp.exe  │
                └──────┬───────┘
                       │
         ┌─────────────┼─────────────┐
         │             │             │
         ▼             ▼             ▼
   ┌─────────┐   ┌─────────┐   ┌─────────┐
   │  UI.dll │   │ BL.dll  │   │ DA.dll  │
   └────┬────┘   └────┬────┘   └────┬────┘
        │             │             │
        └─────────────┼─────────────┘
                      │
                      ▼
                ┌──────────────┐
                │ Utils.dll    │
                └──────────────┘
```

### Example 3: Complex Dependencies (Real WPF 3D App)

```
┌─────────────────────────────────────────┐
│        Your3DApp.exe                    │
└──────────────┬──────────────────────────┘
               │
    ┌──────────┼──────────┬──────────┐
    │          │          │          │
    ▼          ▼          ▼          ▼
┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐
│PresentationFramework.dll│ │Rendering│ │Geometry│
│(WPF)   │ │(yours) │ │(yours) │ │        │
└────┬───┘ └────┬───┘ └────┬───┘ └────┬───┘
     │          │          │          │
     └──────────┼──────────┼──────────┘
                │          │
      ┌─────────┼──────────┘
      │         │
      ▼         ▼
  ┌─────────┐ ┌─────────┐
  │PresentationCore.dll │
  │WindowsBase.dll      │
  └──────┬──────────────┘
         │
         ▼
    ┌─────────┐
    │System.dll│
    │mscorlib.dll│
    └─────────┘
```

## 🔍 How .NET Resolves Dependencies

### The Assembly Loading Process

When you run a .NET application:

```
Step 1: Load YourApp.exe
  └→ .NET reads assembly manifest

Step 2: Check manifest for dependencies
  Manifest says:
    - System.dll (version 4.0.0.0)
    - YourLibrary.dll (version 1.0.0.0)

Step 3: Locate assemblies
  Search order:
    1. Application directory
    2. Private bin path
    3. GAC (Global Assembly Cache)

Step 4: Load dependencies
  └→ Load System.dll from GAC
  └→ Load YourLibrary.dll from app directory

Step 5: Check THEIR dependencies
  YourLibrary.dll needs:
    - Newtonsoft.Json.dll (version 12.0.0.0)
  └→ Load Newtonsoft.Json.dll

Step 6: Recursively load all dependencies
  Continue until all DLLs loaded
```

### Assembly Probing Paths

.NET searches for DLLs in this order:

```
1. Application Base Directory
   C:\MyApp\YourApp.exe
   C:\MyApp\YourLibrary.dll ← Look here first!

2. Private Bin Path (if configured)
   C:\MyApp\bin\YourLibrary.dll

3. Global Assembly Cache (GAC)
   C:\Windows\Microsoft.NET\assembly\GAC_MSIL\
   └→ For shared, strong-named assemblies

4. Codebase (if specified in config)
   <codebase href="http://server/path/YourLibrary.dll" />
```

### Example: app.config for Assembly Binding

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <runtime>
    <!-- Specify where to look for assemblies -->
    <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1">

      <!-- Redirect to a different version -->
      <dependentAssembly>
        <assemblyIdentity name="Newtonsoft.Json"
                          publicKeyToken="30ad4fe6b2a6aeed"
                          culture="neutral" />
        <bindingRedirect oldVersion="0.0.0.0-13.0.0.0"
                         newVersion="13.0.1.0" />
      </dependentAssembly>

      <!-- Specify a probing path -->
      <probing privatePath="bin;lib;plugins" />

    </assemblyBinding>
  </runtime>
</configuration>
```

## 🔧 Managing Dependencies in Visual Studio

### Adding References

#### Method 1: Add Reference Dialog

```
In Visual Studio:
1. Right-click project → Add → Reference
2. Choose:
   - Assemblies: .NET Framework assemblies
   - Projects: Other projects in solution
   - Browse: External DLLs
   - COM: COM components

3. Click OK

Result in .csproj:
<ItemGroup>
  <Reference Include="YourLibrary">
    <HintPath>..\Libraries\YourLibrary.dll</HintPath>
  </Reference>
</ItemGroup>
```

#### Method 2: NuGet Package Manager

```
In Visual Studio:
1. Right-click project → Manage NuGet Packages
2. Search for package (e.g., "Newtonsoft.Json")
3. Click Install

Result: Automatic dependency management!

Creates packages.config:
<?xml version="1.0" encoding="utf-8"?>
<packages>
  <package id="Newtonsoft.Json"
           version="13.0.1"
           targetFramework="net472" />
</packages>
```

### Copy Local Setting

```csharp
// In .csproj file
<Reference Include="YourLibrary">
  <HintPath>..\Libraries\YourLibrary.dll</HintPath>
  <Private>True</Private>  ← Copy to output directory
</Reference>

// True (default):
Build output:
  bin\Debug\YourApp.exe
  bin\Debug\YourLibrary.dll  ← Copied!

// False:
Build output:
  bin\Debug\YourApp.exe
  (YourLibrary.dll NOT copied, must be in GAC or elsewhere)
```

## 📦 NuGet Package Management

### What is NuGet?

**NuGet** is .NET's package manager - like npm for JavaScript, pip for Python.

### NuGet Package Structure

```
Package: Newtonsoft.Json.13.0.1.nupkg

Contents:
├── lib/
│   ├── net45/Newtonsoft.Json.dll
│   ├── netstandard2.0/Newtonsoft.Json.dll
│   └── net6.0/Newtonsoft.Json.dll
├── Newtonsoft.Json.nuspec (metadata)
└── [Content, Tools, Build files]

When installed:
  - Correct version copied to your project
  - Dependencies automatically installed
  - package.config/project.assets.json updated
```

### Installing NuGet Packages

#### GUI Method:
```
Tools → NuGet Package Manager → Manage NuGet Packages for Solution
Search → Install → Done!
```

#### Command Line (Package Manager Console):
```powershell
Install-Package Newtonsoft.Json
Install-Package System.Numerics.Vectors -Version 4.5.0
Update-Package Newtonsoft.Json
Uninstall-Package Newtonsoft.Json
```

#### .NET CLI:
```bash
dotnet add package Newtonsoft.Json
dotnet add package System.Numerics.Vectors --version 4.5.0
dotnet remove package Newtonsoft.Json
```

### Transitive Dependencies

NuGet automatically handles dependency chains:

```
You install: PackageA

PackageA depends on:
  - PackageB (version 1.0)
  - PackageC (version 2.0)

PackageB depends on:
  - PackageD (version 3.0)

NuGet automatically installs:
  ✅ PackageA
  ✅ PackageB
  ✅ PackageC
  ✅ PackageD  ← Transitive dependency!
```

## 🏛️ Global Assembly Cache (GAC)

### What is the GAC?

The **Global Assembly Cache** is a machine-wide store for shared assemblies.

**Location:**
```
.NET Framework (4.x and earlier):
C:\Windows\Microsoft.NET\assembly\

.NET Core/.NET 5+:
No GAC! Uses NuGet instead.
```

### GAC Benefits

```
✅ Shared by all applications on the machine
✅ Avoids duplication
✅ Side-by-side versioning
✅ Strong naming enforced
```

### Installing to GAC

**Requirements:**
1. Assembly must be **strong-named**
2. Need administrator privileges

**Using gacutil.exe:**
```batch
# Install
gacutil /i YourLibrary.dll

# Uninstall
gacutil /u YourLibrary

# List all
gacutil /l
```

### Strong Naming

```csharp
// Generate key pair
sn -k MyKey.snk

// In AssemblyInfo.cs
[assembly: AssemblyKeyFile("MyKey.snk")]

// Or in .csproj
<PropertyGroup>
  <SignAssembly>true</SignAssembly>
  <AssemblyOriginatorKeyFile>MyKey.snk</AssemblyOriginatorKeyFile>
</PropertyGroup>

// Result: Assembly gets a unique identity
Assembly Name: YourLibrary
Version: 1.0.0.0
Culture: neutral
PublicKeyToken: b77a5c561934e089 ← From strong name!
```

## 🔍 Tools for Analyzing Dependencies

### Tool 1: Visual Studio Solution Explorer

```
Solution Explorer:
  References
    ├── System
    ├── System.Core
    ├── System.Xaml
    ├── WindowsBase
    ├── PresentationCore
    ├── PresentationFramework
    └── YourLibrary ⚠️ (warning if missing)
```

### Tool 2: ILSpy / dnSpy

**Free decompiler tools:**
```
Features:
- View assembly references
- See what types are used from each DLL
- Decompile IL to C#
- Analyze dependency tree

Usage:
1. Download ILSpy
2. Open your EXE or DLL
3. View References node
4. See all dependencies!
```

### Tool 3: Dependency Walker (depends.exe)

**For native DLLs:**
```
Download: dependencywalker.com

Features:
- Shows all native DLL dependencies
- Detects missing DLLs
- Shows delay-load DLLs
- Function exports/imports

Great for debugging P/Invoke issues!
```

### Tool 4: dotnet CLI

```bash
# List project dependencies
dotnet list package

# Show dependency tree
dotnet list package --include-transitive

# Check for outdated packages
dotnet list package --outdated

# Check for vulnerable packages
dotnet list package --vulnerable
```

### Tool 5: NuGet Package Explorer

```
Features:
- Open .nupkg files
- View package metadata
- See all dependencies
- Examine package contents

Download: github.com/NuGetPackageExplorer/NuGetPackageExplorer
```

## ⚠️ Common Dependency Problems

### Problem 1: Missing DLL

**Error:**
```
Could not load file or assembly 'YourLibrary' or one of its dependencies.
The system cannot find the file specified.
```

**Solutions:**
```
1. Check DLL is in application directory
2. Set "Copy Local" = True for reference
3. Check if DLL should be in GAC
4. Verify correct architecture (x86 vs x64)
```

### Problem 2: Version Mismatch

**Error:**
```
Could not load file or assembly 'Newtonsoft.Json, Version=12.0.0.0' ...
The located assembly's manifest definition does not match the assembly reference.
```

**Solution: Binding Redirect**
```xml
<configuration>
  <runtime>
    <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1">
      <dependentAssembly>
        <assemblyIdentity name="Newtonsoft.Json"
                          publicKeyToken="30ad4fe6b2a6aeed" />
        <bindingRedirect oldVersion="0.0.0.0-13.0.0.0"
                         newVersion="13.0.1.0" />
      </dependentAssembly>
    </assemblyBinding>
  </runtime>
</configuration>
```

### Problem 3: Circular Dependencies

**Scenario:**
```
Assembly A references Assembly B
Assembly B references Assembly A
```

**Error:**
```
Circular dependency detected
```

**Solution:**
```
Refactor code:
1. Extract common code to Assembly C
2. Both A and B reference C
3. A and B no longer reference each other

Before:
A ←→ B

After:
A → C ← B
```

### Problem 4: Diamond Dependency

**Scenario:**
```
     App
    /   \
   A     B
    \   /
     C

A needs C version 1.0
B needs C version 2.0
```

**Solutions:**
```
1. Binding redirect (if compatible)
2. Use newer version (if backward compatible)
3. Separate into different AppDomains
4. Refactor to avoid conflict
```

### Problem 5: Platform Mismatch

**Error:**
```
An attempt was made to load a program with an incorrect format.
```

**Cause:**
```
App is 64-bit, DLL is 32-bit (or vice versa)
```

**Solution:**
```
1. Match architectures:
   Project Properties → Build → Platform target
   - Any CPU
   - x86
   - x64

2. Or use "Prefer 32-bit" option

3. Ensure all DLLs match your target platform
```

## 💡 Best Practices

### 1. Minimize Dependencies

```csharp
// ❌ Bad: Taking dependency on huge library for one function
using EntireGameEngine; // 50MB DLL
var result = EntireGameEngine.MathHelper.Add(2, 3);

// ✅ Good: Small, focused dependency
using SimpleMath; // 10KB DLL
var result = SimpleMath.Add(2, 3);
```

### 2. Version Dependencies Explicitly

```xml
<!-- ❌ Bad: No version specified -->
<package id="Newtonsoft.Json" />

<!-- ✅ Good: Explicit version -->
<package id="Newtonsoft.Json" version="13.0.1" />
```

### 3. Document Dependencies

```markdown
# Project Dependencies

## Runtime Dependencies
- .NET Framework 4.7.2 or higher
- Newtonsoft.Json 13.0.1
- System.Numerics.Vectors 4.5.0

## Optional Dependencies
- MathNet.Numerics (for advanced calculations)

## Development Dependencies
- NUnit 3.13 (testing)
- Moq 4.16 (mocking)
```

### 4. Use NuGet for External Libraries

```
✅ Do: Install-Package Newtonsoft.Json
❌ Don't: Download DLL from random website
```

### 5. Keep Dependencies Updated (but carefully)

```bash
# Check for updates
dotnet list package --outdated

# Update carefully (test after each!)
dotnet add package Newtonsoft.Json

# Use specific versions for stability
dotnet add package SomeLibrary --version 1.2.3
```

## 🎓 Practical Example: WPF 3D Application

### Typical Dependency Tree

```
My3DApp.exe
│
├── Application Dependencies
│   ├── PresentationFramework.dll (WPF UI)
│   ├── PresentationCore.dll (WPF Core)
│   ├── WindowsBase.dll (WPF Base)
│   └── System.Xaml.dll (XAML support)
│
├── Your Custom DLLs
│   ├── Rendering3D.dll
│   │   └── Depends on:
│   │       ├── System.Numerics.Vectors (math)
│   │       └── SharpDX (DirectX wrapper)
│   │
│   ├── Geometry3D.dll
│   │   └── Depends on:
│   │       └── System.Numerics.Vectors
│   │
│   └── FileFormats.dll
│       └── Depends on:
│           └── AssimpNet (3D file loading)
│
└── NuGet Packages
    ├── Newtonsoft.Json (JSON parsing)
    ├── SharpDX (DirectX)
    ├── AssimpNet (3D model loading)
    └── MathNet.Numerics (advanced math)
```

### Recommended .csproj Structure

```xml
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>WinExe</OutputType>
    <TargetFramework>net6.0-windows</TargetFramework>
    <UseWPF>true</UseWPF>
  </PropertyGroup>

  <!-- NuGet Package Dependencies -->
  <ItemGroup>
    <PackageReference Include="Newtonsoft.Json" Version="13.0.1" />
    <PackageReference Include="System.Numerics.Vectors" Version="4.5.0" />
    <PackageReference Include="SharpDX" Version="4.2.0" />
    <PackageReference Include="SharpDX.Mathematics" Version="4.2.0" />
  </ItemGroup>

  <!-- Project References (Your Custom DLLs) -->
  <ItemGroup>
    <ProjectReference Include="..\Rendering3D\Rendering3D.csproj" />
    <ProjectReference Include="..\Geometry3D\Geometry3D.csproj" />
    <ProjectReference Include="..\FileFormats\FileFormats.csproj" />
  </ItemGroup>
</Project>
```

## 📝 Summary

### Key Concepts

✅ **Dependencies** - DLLs needed by your application
✅ **Dependency Chain** - Recursive dependencies
✅ **Assembly Probing** - How .NET finds DLLs
✅ **NuGet** - Package manager for .NET
✅ **GAC** - Global Assembly Cache (shared DLLs)
✅ **Strong Naming** - Unique assembly identity
✅ **Binding Redirects** - Version conflict resolution

### Common Issues & Solutions

| Problem | Solution |
|---------|----------|
| Missing DLL | Set Copy Local = True |
| Version mismatch | Binding redirect in app.config |
| Circular dependency | Refactor to common library |
| Platform mismatch | Match x86/x64 architecture |
| Outdated packages | Use `dotnet list package --outdated` |

### Best Practices

1. ✅ Use NuGet for external dependencies
2. ✅ Keep dependencies minimal
3. ✅ Document all dependencies
4. ✅ Use explicit versions
5. ✅ Test after updating dependencies

## 🚀 Next Steps

**Congratulations!** You've completed Part 1: DLL Fundamentals!

**Next:** [Part 2: C# DLL Development](../Part2-CSharp-DLL-Development/Lesson06-Your-First-CSharp-DLL.md)

In Part 2, you'll create your first C# DLL from scratch!

## 💪 Practice Exercise

**Task:** Analyze dependencies of a real application

**Steps:**
1. Open Visual Studio
2. Create a new WPF Application
3. Add NuGet package: `Install-Package Newtonsoft.Json`
4. View References in Solution Explorer
5. Note all dependencies (count them!)

**Extra Challenge:**
- Download ILSpy
- Open your built EXE
- View the dependency tree
- Take a screenshot

---

**Fantastic!** 🎉 You now understand DLL dependencies thoroughly!

**Continue to:** [Part 2: C# DLL Development](../Part2-CSharp-DLL-Development/)
