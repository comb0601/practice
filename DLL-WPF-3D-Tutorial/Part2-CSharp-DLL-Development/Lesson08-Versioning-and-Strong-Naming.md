# Lesson 8: Versioning and Strong Naming

## 📚 Learning Objectives

By the end of this lesson, you will understand:
- Assembly version numbers and their meanings
- How to version your DLLs properly
- Strong naming and digital signatures
- Side-by-side execution of different versions
- Breaking vs non-breaking changes
- Semantic versioning principles
- Assembly binding redirects

## 🎯 Understanding Assembly Versions

### What is Assembly Versioning?

Every .NET assembly (DLL or EXE) has a **version number** that identifies which iteration of the code it represents.

### Version Number Format

```
Major.Minor.Build.Revision
  │     │     │       │
  │     │     │       └─ Revision (bug fixes, patches)
  │     │     └───────── Build number (daily/continuous builds)
  │     └─────────────── Minor version (new features, backwards compatible)
  └───────────────────── Major version (breaking changes)

Example: 2.5.1.0
- Major: 2
- Minor: 5
- Build: 1
- Revision: 0
```

### Version Types in .NET

.NET assemblies can have multiple version numbers:

```csharp
// AssemblyInfo.cs or .csproj

[assembly: AssemblyVersion("1.0.0.0")]
// Used by CLR for binding
// Breaking change → increment major

[assembly: AssemblyFileVersion("1.0.0.0")]
// Windows file version (shown in file properties)
// Can increment on every build

[assembly: AssemblyInformationalVersion("1.0.0-beta")]
// Display version (can be any string)
// Shown to users, marketing version
```

**Differences:**

| Version Type | Used By | When to Change | Format |
|--------------|---------|----------------|--------|
| **AssemblyVersion** | CLR for binding | Breaking changes | 1.0.0.0 |
| **AssemblyFileVersion** | Windows Explorer | Every build | 1.2.345.6789 |
| **AssemblyInformationalVersion** | Display/marketing | Releases | 1.0.0-beta |

## 🔧 Setting Versions

### Method 1: AssemblyInfo.cs (.NET Framework)

**Properties/AssemblyInfo.cs:**

```csharp
using System.Reflection;
using System.Runtime.InteropServices;

// General Information
[assembly: AssemblyTitle("MathLibrary")]
[assembly: AssemblyDescription("Mathematical operations library")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("Your Company Name")]
[assembly: AssemblyProduct("MathLibrary")]
[assembly: AssemblyCopyright("Copyright © 2024 Your Company")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]

// COM settings
[assembly: ComVisible(false)]
[assembly: Guid("a1b2c3d4-e5f6-7890-abcd-ef1234567890")]

// Version information
[assembly: AssemblyVersion("1.0.0.0")]
[assembly: AssemblyFileVersion("1.0.0.0")]
[assembly: AssemblyInformationalVersion("1.0.0")]
```

### Method 2: Project File (.NET Core/.NET 5+)

**MathLibrary.csproj:**

```xml
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>net6.0</TargetFramework>

    <!-- Assembly Information -->
    <AssemblyName>MathLibrary</AssemblyName>
    <RootNamespace>MathLibrary</RootNamespace>

    <!-- Versioning -->
    <Version>1.0.0</Version>
    <AssemblyVersion>1.0.0.0</AssemblyVersion>
    <FileVersion>1.0.0.0</FileVersion>
    <InformationalVersion>1.0.0</InformationalVersion>

    <!-- Metadata -->
    <Authors>Your Name</Authors>
    <Company>Your Company</Company>
    <Product>Math Library</Product>
    <Description>A comprehensive mathematical operations library</Description>
    <Copyright>Copyright © 2024 Your Company</Copyright>

    <!-- NuGet Package Info -->
    <PackageId>YourCompany.MathLibrary</PackageId>
    <PackageVersion>1.0.0</PackageVersion>
    <PackageTags>math;calculator;geometry</PackageTags>
    <PackageProjectUrl>https://github.com/yourcompany/mathlibrary</PackageProjectUrl>
    <RepositoryUrl>https://github.com/yourcompany/mathlibrary</RepositoryUrl>
    <PackageLicenseExpression>MIT</PackageLicenseExpression>
  </PropertyGroup>

</Project>
```

### Method 3: Auto-increment Version

```xml
<!-- Auto-increment build and revision -->
<PropertyGroup>
  <Version>1.0.*</Version>
</PropertyGroup>

<!-- Or in AssemblyInfo.cs -->
[assembly: AssemblyVersion("1.0.*")]
```

**Note:** Auto-increment is not recommended for production because it creates unpredictable version numbers.

## 📋 Semantic Versioning (SemVer)

### The SemVer Standard

**Format:** `MAJOR.MINOR.PATCH[-PRERELEASE][+BUILD]`

```
Examples:
1.0.0           - Initial release
1.0.1           - Bug fix
1.1.0           - New feature (backwards compatible)
2.0.0           - Breaking change
1.0.0-alpha     - Pre-release (alpha)
1.0.0-beta.2    - Pre-release (beta 2)
1.0.0+20230501  - Build metadata
```

### When to Increment

```
Given version number MAJOR.MINOR.PATCH:

MAJOR: Increment when you make incompatible API changes
  ❌ Breaking: Removed a public method
  ❌ Breaking: Changed method signature
  ❌ Breaking: Renamed a class
  Example: 1.5.3 → 2.0.0

MINOR: Increment when you add functionality in a backwards-compatible manner
  ✅ Added: New public method
  ✅ Added: New class
  ✅ Added: Optional parameter with default value
  Example: 1.5.3 → 1.6.0

PATCH: Increment when you make backwards-compatible bug fixes
  ✅ Fixed: Bug in existing method
  ✅ Fixed: Performance improvement
  ✅ Fixed: Security patch
  Example: 1.5.3 → 1.5.4
```

### Practical Example

```csharp
// Version 1.0.0 - Initial release
namespace MathLibrary
{
    public class Calculator
    {
        public int Add(int a, int b)
        {
            return a + b;
        }
    }
}

// Version 1.1.0 - Added new method (MINOR increment)
namespace MathLibrary
{
    public class Calculator
    {
        public int Add(int a, int b)
        {
            return a + b;
        }

        public int Subtract(int a, int b)  // ← NEW METHOD (backwards compatible)
        {
            return a - b;
        }
    }
}

// Version 1.1.1 - Fixed bug (PATCH increment)
namespace MathLibrary
{
    public class Calculator
    {
        public int Add(int a, int b)
        {
            // Fixed: Now handles overflow properly
            checked
            {
                return a + b;
            }
        }

        public int Subtract(int a, int b)
        {
            return a - b;
        }
    }
}

// Version 2.0.0 - Breaking change (MAJOR increment)
namespace MathLibrary
{
    public class Calculator
    {
        // BREAKING: Changed return type from int to double
        public double Add(double a, double b)  // ← BREAKING CHANGE
        {
            return a + b;
        }

        public double Subtract(double a, double b)
        {
            return a - b;
        }
    }
}
```

## 🔐 Strong Naming

### What is a Strong Name?

A **strong name** is a unique identity for an assembly consisting of:
1. Simple text name
2. Version number
3. Culture information (if provided)
4. Public key token (cryptographic signature)

### Why Use Strong Names?

```
✅ Uniqueness: No two assemblies can have the same strong name
✅ Security: Ensures assembly hasn't been tampered with
✅ GAC: Required for Global Assembly Cache
✅ Version control: Side-by-side execution of different versions
✅ Trust: Verify the publisher
```

### Creating a Strong Name Key

**Step 1: Generate Key Pair**

```batch
# Open Developer Command Prompt
sn -k MyLibrary.snk

# Creates MyLibrary.snk (Strong Name Key file)
# Contains both public and private keys
# Keep this file SECRET!
```

**Step 2: Sign Your Assembly**

**For .NET Framework (AssemblyInfo.cs):**

```csharp
using System.Reflection;

[assembly: AssemblyKeyFile("MyLibrary.snk")]
```

**For .NET Core/.NET 5+ (.csproj):**

```xml
<PropertyGroup>
  <SignAssembly>true</SignAssembly>
  <AssemblyOriginatorKeyFile>MyLibrary.snk</AssemblyOriginatorKeyFile>
  <DelaySign>false</DelaySign>
</PropertyGroup>
```

**Step 3: Build**

```
Build → Build Solution

Your DLL is now strong-named!
```

### Viewing Strong Name Information

```batch
# View public key token
sn -T MathLibrary.dll

Output:
Microsoft (R) .NET Framework Strong Name Utility  Version 4.0.30319.0
Copyright (c) Microsoft Corporation.  All rights reserved.

Public key token is b77a5c561934e089
```

### Delay Signing (for Open Source)

**Problem:** You don't want to distribute private key to all developers

**Solution:** Delay signing

```xml
<PropertyGroup>
  <SignAssembly>true</SignAssembly>
  <AssemblyOriginatorKeyFile>PublicKey.snk</AssemblyOriginatorKeyFile>
  <DelaySign>true</DelaySign>
</PropertyGroup>
```

**Process:**
1. Developers use public key only (no signature yet)
2. Build system signs with private key during release
3. Developers must disable strong name verification for testing

```batch
# Disable verification for development
sn -Vr MathLibrary.dll
```

## 🔄 Side-by-Side Execution

### The Problem

```
Application needs:
- OldLibrary.dll version 1.0
- NewLibrary.dll version 2.0

But OldLibrary.dll depends on CommonLibrary.dll version 1.0
And NewLibrary.dll depends on CommonLibrary.dll version 2.0

Without strong names:
❌ Only one version of CommonLibrary.dll can load
❌ Version conflict!
```

### The Solution: Strong Names

```
With strong names:

CommonLibrary, Version=1.0, PublicKeyToken=abc123
CommonLibrary, Version=2.0, PublicKeyToken=abc123

Both can coexist in the same application!
CLR treats them as different assemblies.
```

### Example Scenario

```csharp
// Your application
using OldLibrary;  // Uses CommonLibrary v1.0
using NewLibrary;  // Uses CommonLibrary v2.0

public class MyApp
{
    public void DoWork()
    {
        var old = new OldLibrary.Helper();  // Loads CommonLibrary v1.0
        var newHelper = new NewLibrary.Helper();  // Loads CommonLibrary v2.0

        // Both versions loaded simultaneously!
    }
}
```

## 🔀 Assembly Binding Redirects

### The Version Mismatch Problem

```
Your app references:
- YourLibrary.dll which needs Newtonsoft.Json v12.0

You install:
- AnotherLibrary.dll which needs Newtonsoft.Json v13.0

Runtime error:
Could not load file or assembly 'Newtonsoft.Json, Version=12.0.0.0'
```

### Solution: Binding Redirect

**app.config or web.config:**

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <runtime>
    <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1">

      <!-- Redirect all versions of Newtonsoft.Json to 13.0 -->
      <dependentAssembly>
        <assemblyIdentity name="Newtonsoft.Json"
                          publicKeyToken="30ad4fe6b2a6aeed"
                          culture="neutral" />
        <bindingRedirect oldVersion="0.0.0.0-13.0.0.0"
                         newVersion="13.0.1.0" />
      </dependentAssembly>

      <!-- Redirect multiple versions of your DLL -->
      <dependentAssembly>
        <assemblyIdentity name="MathLibrary"
                          publicKeyToken="b77a5c561934e089"
                          culture="neutral" />
        <bindingRedirect oldVersion="1.0.0.0-1.5.0.0"
                         newVersion="2.0.0.0" />
      </dependentAssembly>

    </assemblyBinding>
  </runtime>
</configuration>
```

**What this does:**
- Any request for Newtonsoft.Json v0.0 through v13.0 → loads v13.0.1
- Any request for MathLibrary v1.0 through v1.5 → loads v2.0

### Auto-Generate Binding Redirects

```xml
<!-- In .csproj -->
<PropertyGroup>
  <AutoGenerateBindingRedirects>true</AutoGenerateBindingRedirects>
</PropertyGroup>
```

Visual Studio will automatically generate redirects during build!

## 📊 Version Compatibility Strategies

### Strategy 1: Lock to Specific Version

```csharp
// Require exact version
[assembly: AssemblyVersion("1.0.0.0")]

// Clients must reference exactly 1.0.0.0
```

**Pros:** Complete control, no surprises
**Cons:** Clients must update references for every version

### Strategy 2: Lock Major/Minor Only

```csharp
// Version: 1.2.*.*
[assembly: AssemblyVersion("1.2.0.0")]

// Allow any 1.2.x.x version to be substituted
```

**Pros:** Patch releases don't break clients
**Cons:** Must maintain backwards compatibility in patches

### Strategy 3: Lock Major Only

```csharp
// Version: 1.*.*.*
[assembly: AssemblyVersion("1.0.0.0")]

// Allow any 1.x.x.x version
```

**Pros:** Maximum flexibility
**Cons:** Must maintain backwards compatibility always

### Recommendation

```
For libraries you control:
  Use Strategy 2 (lock major.minor)

For public libraries/NuGet:
  Use Strategy 1 (lock all four)
  Use SemVer strictly
```

## 🧪 Practical Example: Versioning Workflow

### Scenario: You maintain MathLibrary.dll

**Version 1.0.0 - Initial Release**

```csharp
namespace MathLibrary
{
    public class Calculator
    {
        public int Add(int a, int b) => a + b;
        public int Subtract(int a, int b) => a - b;
    }
}

// AssemblyVersion: 1.0.0.0
```

**Bug Found! Version 1.0.1 - Bug Fix Release**

```csharp
namespace MathLibrary
{
    public class Calculator
    {
        public int Add(int a, int b)
        {
            // BUG FIX: Handle overflow
            checked { return a + b; }
        }

        public int Subtract(int a, int b) => a - b;
    }
}

// AssemblyVersion: 1.0.0.0 (NO CHANGE - compatible!)
// AssemblyFileVersion: 1.0.1.0
// AssemblyInformationalVersion: 1.0.1
```

**New Feature! Version 1.1.0**

```csharp
namespace MathLibrary
{
    public class Calculator
    {
        public int Add(int a, int b)
        {
            checked { return a + b; }
        }

        public int Subtract(int a, int b) => a - b;

        // NEW FEATURE: Multiplication
        public int Multiply(int a, int b) => a * b;
    }
}

// AssemblyVersion: 1.1.0.0 (MINOR increment)
// AssemblyFileVersion: 1.1.0.0
// AssemblyInformationalVersion: 1.1.0
```

**Breaking Change! Version 2.0.0**

```csharp
namespace MathLibrary
{
    public class Calculator
    {
        // BREAKING: Changed from int to double
        public double Add(double a, double b) => a + b;
        public double Subtract(double a, double b) => a - b;
        public double Multiply(double a, double b) => a * b;
    }
}

// AssemblyVersion: 2.0.0.0 (MAJOR increment)
// AssemblyFileVersion: 2.0.0.0
// AssemblyInformationalVersion: 2.0.0
```

## 🔍 Checking Version at Runtime

```csharp
using System;
using System.Reflection;

public class VersionChecker
{
    public static void DisplayVersionInfo()
    {
        // Get current assembly
        Assembly assembly = Assembly.GetExecutingAssembly();

        // Get version information
        Version version = assembly.GetName().Version;

        Console.WriteLine($"Assembly Version: {version}");
        Console.WriteLine($"  Major: {version.Major}");
        Console.WriteLine($"  Minor: {version.Minor}");
        Console.WriteLine($"  Build: {version.Build}");
        Console.WriteLine($"  Revision: {version.Revision}");

        // Get file version
        string fileVersion = assembly.GetCustomAttribute<AssemblyFileVersionAttribute>()?.Version;
        Console.WriteLine($"File Version: {fileVersion}");

        // Get informational version
        string infoVersion = assembly.GetCustomAttribute<AssemblyInformationalVersionAttribute>()?.InformationalVersion;
        Console.WriteLine($"Informational Version: {infoVersion}");
    }

    public static bool IsVersionCompatible(string requiredVersion)
    {
        Assembly assembly = Assembly.GetExecutingAssembly();
        Version current = assembly.GetName().Version;
        Version required = new Version(requiredVersion);

        // Check if major versions match
        return current.Major == required.Major;
    }
}

// Usage
VersionChecker.DisplayVersionInfo();

if (!VersionChecker.IsVersionCompatible("1.0.0.0"))
{
    Console.WriteLine("Version mismatch!");
}
```

## 📝 Best Practices Checklist

### ✅ DO:

```
✓ Use semantic versioning (SemVer)
✓ Increment MAJOR for breaking changes
✓ Increment MINOR for new features
✓ Increment PATCH for bug fixes
✓ Strong name assemblies intended for GAC
✓ Strong name assemblies used by multiple clients
✓ Document version changes in CHANGELOG
✓ Test version upgrades thoroughly
```

### ❌ DON'T:

```
✗ Use auto-increment versions in production
✗ Make breaking changes in MINOR versions
✗ Share private key files (.snk)
✗ Commit .snk files to public repos
✗ Skip versions (1.0 → 3.0)
✗ Reuse version numbers
✗ Forget binding redirects
```

## 📝 Summary

### What We Learned

✅ Assembly versions have four parts: Major.Minor.Build.Revision
✅ Three version types: AssemblyVersion, FileVersion, InformationalVersion
✅ Semantic Versioning: MAJOR.MINOR.PATCH
✅ Strong naming provides unique identity and security
✅ Side-by-side execution requires strong names
✅ Binding redirects resolve version conflicts
✅ Version changes communicate compatibility

### Key Takeaways

1. **Version properly:** Follow SemVer strictly
2. **Breaking changes:** Always increment MAJOR
3. **Strong names:** Use for shared libraries
4. **Test upgrades:** Ensure backwards compatibility
5. **Document:** Maintain CHANGELOG.md

## 🚀 Next Steps

In **Lesson 9: DLL Configuration and Resources**, we'll learn:
- App.config and web.config
- Embedded resources
- Resource files (.resx)
- Configuration sections
- Localization

## 💪 Practice Exercises

### Exercise 1: Version a Library

Create MathLibrary and release versions:
1. Version 1.0.0 - Add, Subtract
2. Version 1.0.1 - Fix overflow bug
3. Version 1.1.0 - Add Multiply
4. Version 2.0.0 - Change to double

### Exercise 2: Strong Name It

1. Generate .snk file
2. Sign your MathLibrary
3. Install to GAC
4. Reference from another app

### Exercise 3: Binding Redirect

1. Create two versions of a library
2. Make an app reference v1
3. Update to v2
4. Add binding redirect
5. Verify it works

---

**Great work!** 🎉 You now understand versioning and strong naming!

**Next Lesson:** [Lesson 9: DLL Configuration and Resources](Lesson09-DLL-Configuration-and-Resources.md)
