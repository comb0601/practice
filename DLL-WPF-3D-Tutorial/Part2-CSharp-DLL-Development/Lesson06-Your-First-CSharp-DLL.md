# Lesson 6: Your First C# DLL

## 📚 Learning Objectives

By the end of this lesson, you will:
- Create your first C# class library (DLL) from scratch
- Understand the difference between Class Library and Console/WPF apps
- Add references and use your DLL in another project
- Understand namespaces and public/private access
- Build and deploy your DLL
- Debug code in DLLs

## 🎯 What We're Building

We'll create a **MathLibrary.dll** that provides useful math functions, then use it in a console application.

### Project Structure
```
Solution: MathLibrarySolution
│
├── MathLibrary (Class Library Project)
│   └── Output: MathLibrary.dll
│
└── MathDemo (Console Application)
    └── Uses: MathLibrary.dll
```

## 🛠️ Step-by-Step: Creating Your First DLL

### Step 1: Create a New Solution

**In Visual Studio:**

```
1. File → New → Project
2. Search for "Class Library"
3. Select "Class Library (.NET Framework)" or "Class Library"
   - .NET Framework: For Windows-only apps
   - .NET / .NET Core: For cross-platform

4. Configure:
   Name: MathLibrary
   Location: C:\Projects\
   Solution name: MathLibrarySolution

5. Click "Create"
```

**What Visual Studio Creates:**

```
MathLibrary/
├── MathLibrary.csproj       ← Project file
├── Class1.cs                ← Default class file
└── Properties/
    └── AssemblyInfo.cs      ← Assembly metadata
```

### Step 2: Understand the Project File

**MathLibrary.csproj (SDK-style .NET 6+):**

```xml
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>net6.0</TargetFramework>
    <ImplicitUsings>enable</ImplicitUsings>
    <Nullable>enable</Nullable>
  </PropertyGroup>

</Project>
```

**Key Differences from EXE:**

| Property | EXE Project | DLL Project |
|----------|-------------|-------------|
| OutputType | `WinExe` or `Exe` | `Library` (or omitted) |
| Has Main()? | ✅ Required | ❌ No entry point |
| Can Run? | ✅ Yes | ❌ No |
| Output | .exe file | .dll file |

### Step 3: Rename and Design Your Class

**Rename Class1.cs to Calculator.cs:**

```
Solution Explorer:
  Right-click Class1.cs → Rename → Calculator.cs
  (Visual Studio will ask to rename the class too - click Yes!)
```

**Calculator.cs:**

```csharp
using System;

namespace MathLibrary
{
    /// <summary>
    /// Provides basic mathematical operations
    /// </summary>
    public class Calculator
    {
        /// <summary>
        /// Adds two numbers
        /// </summary>
        public int Add(int a, int b)
        {
            return a + b;
        }

        /// <summary>
        /// Subtracts two numbers
        /// </summary>
        public int Subtract(int a, int b)
        {
            return a - b;
        }

        /// <summary>
        /// Multiplies two numbers
        /// </summary>
        public int Multiply(int a, int b)
        {
            return a * b;
        }

        /// <summary>
        /// Divides two numbers
        /// </summary>
        public double Divide(int a, int b)
        {
            if (b == 0)
                throw new DivideByZeroException("Cannot divide by zero");

            return (double)a / b;
        }

        /// <summary>
        /// Calculates power
        /// </summary>
        public double Power(double baseNumber, double exponent)
        {
            return Math.Pow(baseNumber, exponent);
        }
    }
}
```

### Step 4: Add More Classes

**Create Advanced Calculator** (GeometryHelper.cs):

```csharp
using System;

namespace MathLibrary
{
    /// <summary>
    /// Provides geometry calculation functions
    /// </summary>
    public class GeometryHelper
    {
        /// <summary>
        /// Calculates the area of a circle
        /// </summary>
        public static double CircleArea(double radius)
        {
            if (radius < 0)
                throw new ArgumentException("Radius cannot be negative");

            return Math.PI * radius * radius;
        }

        /// <summary>
        /// Calculates the circumference of a circle
        /// </summary>
        public static double CircleCircumference(double radius)
        {
            if (radius < 0)
                throw new ArgumentException("Radius cannot be negative");

            return 2 * Math.PI * radius;
        }

        /// <summary>
        /// Calculates the area of a rectangle
        /// </summary>
        public static double RectangleArea(double width, double height)
        {
            return width * height;
        }

        /// <summary>
        /// Calculates the area of a triangle
        /// </summary>
        public static double TriangleArea(double baseLength, double height)
        {
            return 0.5 * baseLength * height;
        }
    }
}
```

**Create a Constants class** (MathConstants.cs):

```csharp
namespace MathLibrary
{
    /// <summary>
    /// Mathematical constants
    /// </summary>
    public static class MathConstants
    {
        public const double PI = 3.14159265358979323846;
        public const double E = 2.71828182845904523536;
        public const double GoldenRatio = 1.61803398874989484820;

        public static readonly double SquareRootOf2 = 1.41421356237309504880;
        public static readonly double SquareRootOf3 = 1.73205080756887729352;
    }
}
```

### Step 5: Understanding Access Modifiers

**Public vs Private:**

```csharp
namespace MathLibrary
{
    public class Calculator  // ← PUBLIC: Accessible from other assemblies
    {
        // PUBLIC method: Can be called from outside the DLL
        public int Add(int a, int b)
        {
            return a + b;
        }

        // PRIVATE method: Only accessible within this class
        private int ValidateInput(int value)
        {
            if (value < 0)
                throw new ArgumentException("Value must be positive");
            return value;
        }

        // INTERNAL method: Accessible only within this DLL
        internal int InternalCalculation(int x)
        {
            return x * 2;
        }
    }

    // INTERNAL class: Not visible outside this DLL
    internal class InternalHelper
    {
        public void DoSomething()
        {
            // Even though this method is public,
            // the class is internal, so not visible outside DLL
        }
    }
}
```

**Access Modifier Summary:**

| Modifier | Visible To |
|----------|-----------|
| `public` | Everyone (inside and outside DLL) |
| `private` | Only the containing class |
| `internal` | Only within the same DLL |
| `protected` | Class and derived classes |
| `protected internal` | Same DLL or derived classes |
| `private protected` | Same DLL AND derived classes |

### Step 6: Build the DLL

**Build the project:**

```
Visual Studio:
  Build → Build Solution (or press Ctrl+Shift+B)

Output window shows:
  1>------ Build started: Project: MathLibrary ------
  1>  MathLibrary -> C:\Projects\MathLibrarySolution\MathLibrary\bin\Debug\net6.0\MathLibrary.dll
  ========== Build: 1 succeeded, 0 failed ==========
```

**Output Files:**

```
bin/Debug/net6.0/
├── MathLibrary.dll          ← Your compiled DLL!
├── MathLibrary.pdb          ← Debug symbols
├── MathLibrary.deps.json    ← Dependency info
└── MathLibrary.xml          ← XML documentation (if enabled)
```

### Step 7: Configure Assembly Information

**Edit Properties:**

**For .NET Framework (AssemblyInfo.cs):**

```csharp
using System.Reflection;
using System.Runtime.InteropServices;

// General Information
[assembly: AssemblyTitle("MathLibrary")]
[assembly: AssemblyDescription("A library for mathematical operations")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("Your Company")]
[assembly: AssemblyProduct("MathLibrary")]
[assembly: AssemblyCopyright("Copyright © 2024")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]

// Version information
[assembly: AssemblyVersion("1.0.0.0")]
[assembly: AssemblyFileVersion("1.0.0.0")]

// COM Visible
[assembly: ComVisible(false)]

// GUID for the library
[assembly: Guid("12345678-1234-1234-1234-123456789012")]
```

**For .NET 6+ (.csproj):**

```xml
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>net6.0</TargetFramework>

    <!-- Assembly Information -->
    <AssemblyName>MathLibrary</AssemblyName>
    <Version>1.0.0</Version>
    <Authors>Your Name</Authors>
    <Company>Your Company</Company>
    <Product>MathLibrary</Product>
    <Description>A library for mathematical operations</Description>
    <Copyright>Copyright © 2024</Copyright>

    <!-- Generate XML documentation -->
    <GenerateDocumentationFile>true</GenerateDocumentationFile>

    <!-- Package Information (for NuGet) -->
    <PackageId>MathLibrary</PackageId>
    <PackageTags>math;calculator;geometry</PackageTags>
  </PropertyGroup>

</Project>
```

## 🔧 Using Your DLL in Another Project

### Step 8: Create a Console Application

**Add new project to solution:**

```
1. Right-click Solution → Add → New Project
2. Choose "Console App"
3. Name: MathDemo
4. Click Create
```

### Step 9: Add Reference to Your DLL

**Method 1: Project Reference (Recommended)**

```
In MathDemo project:
1. Right-click "Dependencies" or "References"
2. Add Reference...
3. Click "Projects" tab
4. Check "MathLibrary"
5. Click OK
```

**Result in .csproj:**

```xml
<ItemGroup>
  <ProjectReference Include="..\MathLibrary\MathLibrary.csproj" />
</ItemGroup>
```

**Method 2: DLL Reference**

```
1. Right-click "Dependencies" → Add Reference
2. Click "Browse"
3. Navigate to MathLibrary\bin\Debug\net6.0\
4. Select MathLibrary.dll
5. Click OK
```

### Step 10: Use Your DLL

**Program.cs in MathDemo:**

```csharp
using System;
using MathLibrary;  // ← Import your namespace!

namespace MathDemo
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("=== Math Library Demo ===\n");

            // Create calculator instance
            Calculator calc = new Calculator();

            // Test basic operations
            Console.WriteLine("--- Basic Operations ---");
            Console.WriteLine($"5 + 3 = {calc.Add(5, 3)}");
            Console.WriteLine($"5 - 3 = {calc.Subtract(5, 3)}");
            Console.WriteLine($"5 × 3 = {calc.Multiply(5, 3)}");
            Console.WriteLine($"5 ÷ 3 = {calc.Divide(5, 3):F2}");
            Console.WriteLine($"2 ^ 8 = {calc.Power(2, 8)}");

            // Test geometry functions
            Console.WriteLine("\n--- Geometry ---");
            Console.WriteLine($"Circle area (r=5): {GeometryHelper.CircleArea(5):F2}");
            Console.WriteLine($"Circle circumference (r=5): {GeometryHelper.CircleCircumference(5):F2}");
            Console.WriteLine($"Rectangle area (4×6): {GeometryHelper.RectangleArea(4, 6)}");

            // Use constants
            Console.WriteLine("\n--- Constants ---");
            Console.WriteLine($"PI = {MathConstants.PI}");
            Console.WriteLine($"e = {MathConstants.E}");
            Console.WriteLine($"Golden Ratio = {MathConstants.GoldenRatio}");

            // Test error handling
            Console.WriteLine("\n--- Error Handling ---");
            try
            {
                calc.Divide(10, 0);
            }
            catch (DivideByZeroException ex)
            {
                Console.WriteLine($"Error caught: {ex.Message}");
            }

            Console.WriteLine("\nPress any key to exit...");
            Console.ReadKey();
        }
    }
}
```

### Step 11: Run the Application

```
Press F5 (or Ctrl+F5 for no debugging)

Output:
=== Math Library Demo ===

--- Basic Operations ---
5 + 3 = 8
5 - 3 = 2
5 × 3 = 15
5 ÷ 3 = 1.67
2 ^ 8 = 256

--- Geometry ---
Circle area (r=5): 78.54
Circle circumference (r=5): 31.42
Rectangle area (4×6): 24

--- Constants ---
PI = 3.14159265358979
e = 2.71828182845905
Golden Ratio = 1.61803398874989

--- Error Handling ---
Error caught: Cannot divide by zero

Press any key to exit...
```

## 🐛 Debugging Your DLL

### Setting Breakpoints in DLL Code

**Steps:**

```
1. Open Calculator.cs in MathLibrary project
2. Click in the margin next to a line (e.g., inside Add method)
   → Red dot appears (breakpoint)

3. Set MathDemo as startup project:
   Right-click MathDemo → Set as Startup Project

4. Press F5 to debug

5. When code hits calc.Add(5, 3) in MathDemo,
   debugger will step INTO your DLL code!

6. You can inspect variables, step through code, etc.
```

### Debugging Tips

```csharp
// Add debug output
public int Add(int a, int b)
{
    System.Diagnostics.Debug.WriteLine($"Add called: {a} + {b}");
    return a + b;
}

// View in Output window → Debug
```

## 📦 Distributing Your DLL

### Option 1: Copy DLL Manually

```
Copy these files to the target application:
  MathLibrary.dll     ← Required
  MathLibrary.pdb     ← Optional (for debugging)
  MathLibrary.xml     ← Optional (for IntelliSense)
```

### Option 2: NuGet Package (Advanced)

```xml
<!-- In MathLibrary.csproj -->
<PropertyGroup>
  <GeneratePackageOnBuild>true</GeneratePackageOnBuild>
  <PackageId>MyCompany.MathLibrary</PackageId>
  <Version>1.0.0</Version>
  <Authors>Your Name</Authors>
  <Description>Math library for common operations</Description>
</PropertyGroup>
```

**Build → Creates NuGet package:**
```
bin/Debug/MyCompany.MathLibrary.1.0.0.nupkg
```

### Option 3: Strong Name (for GAC)

```
1. Generate key file:
   Developer Command Prompt:
   sn -k MathLibrary.snk

2. Add to .csproj:
   <PropertyGroup>
     <SignAssembly>true</SignAssembly>
     <AssemblyOriginatorKeyFile>MathLibrary.snk</AssemblyOriginatorKeyFile>
   </PropertyGroup>

3. Rebuild

4. Install to GAC:
   gacutil /i MathLibrary.dll
```

## 📊 Project Organization Best Practices

### Folder Structure

```
MathLibrary/
├── Core/
│   ├── Calculator.cs
│   └── AdvancedCalculator.cs
├── Geometry/
│   ├── GeometryHelper.cs
│   └── Shapes.cs
├── Constants/
│   └── MathConstants.cs
└── Utilities/
    └── ValidationHelper.cs
```

### Namespace Organization

```csharp
// Match folders to namespaces
namespace MathLibrary.Core
{
    public class Calculator { }
}

namespace MathLibrary.Geometry
{
    public class GeometryHelper { }
}

namespace MathLibrary.Constants
{
    public static class MathConstants { }
}

// Usage:
using MathLibrary.Core;
using MathLibrary.Geometry;

var calc = new Calculator();
var area = GeometryHelper.CircleArea(5);
```

## 📝 Summary

### What We Learned

✅ Created a C# Class Library project
✅ Added multiple classes with different purposes
✅ Used access modifiers (public, private, internal)
✅ Built the DLL
✅ Referenced the DLL in another project
✅ Used the DLL in a console application
✅ Debugged code inside the DLL
✅ Learned distribution options

### Key Concepts

1. **Class Library** = DLL project (OutputType: Library)
2. **Public** = Accessible from outside DLL
3. **Internal** = Only within the DLL
4. **Project Reference** = Best way to reference DLLs in same solution
5. **IntelliSense** = Works automatically with XML comments

### File Extensions

- `.dll` - The compiled library
- `.pdb` - Debug symbols
- `.xml` - Documentation file
- `.deps.json` - Dependency information

## 🚀 Next Steps

In **Lesson 7: DLL Interfaces and Contracts**, we'll learn:
- Designing with interfaces
- Creating abstraction layers
- Plugin architecture
- Dependency injection
- SOLID principles

## 💪 Practice Exercises

### Exercise 1: Extend the Math Library

Add these features to MathLibrary:

```csharp
// 1. Add a Statistics class
public class Statistics
{
    public static double Average(params double[] numbers) { }
    public static double Median(params double[] numbers) { }
    public static double StandardDeviation(params double[] numbers) { }
}

// 2. Add a Conversion class
public class UnitConverter
{
    public static double CelsiusToFahrenheit(double celsius) { }
    public static double KilometersToMiles(double km) { }
    public static double KgToPounds(double kg) { }
}
```

### Exercise 2: Create Your Own DLL

Create a **StringLibrary.dll** with:
```csharp
public class StringHelper
{
    public static string Reverse(string input) { }
    public static bool IsPalindrome(string input) { }
    public static int WordCount(string input) { }
    public static string CapitalizeWords(string input) { }
}
```

### Exercise 3: Multi-DLL Application

Create a solution with:
```
├── MathLibrary.dll (from this lesson)
├── StringLibrary.dll (from Exercise 2)
└── UtilityDemo.exe (uses both DLLs)
```

---

**Excellent!** 🎉 You've created your first C# DLL!

**Next Lesson:** [Lesson 7: DLL Interfaces and Contracts](Lesson07-DLL-Interfaces-and-Contracts.md)
