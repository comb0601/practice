# Lesson 10: Testing and Debugging DLLs

## 📚 Learning Objectives

By the end of this lesson, you will understand:
- Unit testing class libraries with NUnit, xUnit, and MSTest
- Test-Driven Development (TDD) for DLLs
- Mocking dependencies with Moq
- Debugging techniques for DLLs
- Debug vs Release builds
- PDB files and symbols
- Performance profiling
- Code coverage analysis

## 🎯 Why Test DLLs?

DLLs are reusable components used by multiple applications. Testing ensures:
- ✅ Functionality works correctly
- ✅ No breaking changes between versions
- ✅ Edge cases are handled
- ✅ Dependencies work as expected
- ✅ Performance is acceptable

## 🧪 Unit Testing Frameworks

### Three Popular Frameworks

| Framework | Pros | Cons |
|-----------|------|------|
| **MSTest** | Built into Visual Studio | Less features |
| **NUnit** | Rich assertions, mature | Separate test runner |
| **xUnit** | Modern, used by .NET team | Different syntax |

We'll use **xUnit** (most modern).

## 🛠️ Setting Up Unit Tests

### Step 1: Create Test Project

```
Solution Explorer:
  Right-click Solution → Add → New Project
  Choose: xUnit Test Project
  Name: MathLibrary.Tests
```

**Project Structure:**

```
Solution: MathLibrarySolution
│
├── MathLibrary (Class Library)
│   ├── Calculator.cs
│   └── GeometryHelper.cs
│
└── MathLibrary.Tests (xUnit Test Project)
    ├── CalculatorTests.cs
    └── GeometryHelperTests.cs
```

### Step 2: Add Reference to DLL

```
In MathLibrary.Tests project:
  Right-click Dependencies → Add Project Reference
  Check: MathLibrary
  Click OK
```

### Step 3: Install NuGet Packages

```
Already installed by template:
- xunit
- xunit.runner.visualstudio
- Microsoft.NET.Test.Sdk

Optional (we'll add):
- Moq (mocking framework)
- FluentAssertions (better assertions)
```

```bash
# Package Manager Console
Install-Package Moq
Install-Package FluentAssertions
```

## ✍️ Writing Your First Test

### Calculator.cs (Code to Test)

```csharp
namespace MathLibrary
{
    public class Calculator
    {
        public int Add(int a, int b)
        {
            return a + b;
        }

        public int Subtract(int a, int b)
        {
            return a - b;
        }

        public int Multiply(int a, int b)
        {
            return a * b;
        }

        public double Divide(int a, int b)
        {
            if (b == 0)
                throw new DivideByZeroException("Cannot divide by zero");

            return (double)a / b;
        }

        public int Factorial(int n)
        {
            if (n < 0)
                throw new ArgumentException("Number must be non-negative");

            if (n == 0 || n == 1)
                return 1;

            return n * Factorial(n - 1);
        }
    }
}
```

### CalculatorTests.cs (Unit Tests)

```csharp
using Xunit;
using MathLibrary;
using System;
using FluentAssertions;

namespace MathLibrary.Tests
{
    public class CalculatorTests
    {
        private readonly Calculator calculator;

        // Constructor runs before each test
        public CalculatorTests()
        {
            calculator = new Calculator();
        }

        // Test naming: MethodName_Scenario_ExpectedResult
        [Fact]
        public void Add_TwoPositiveNumbers_ReturnsSum()
        {
            // Arrange
            int a = 5;
            int b = 3;

            // Act
            int result = calculator.Add(a, b);

            // Assert
            Assert.Equal(8, result);
        }

        [Fact]
        public void Add_TwoNegativeNumbers_ReturnsNegativeSum()
        {
            // Arrange & Act
            int result = calculator.Add(-5, -3);

            // Assert
            Assert.Equal(-8, result);
        }

        [Fact]
        public void Add_PositiveAndNegative_ReturnsDifference()
        {
            // Using FluentAssertions (more readable)
            int result = calculator.Add(5, -3);
            result.Should().Be(2);
        }

        [Theory]  // Data-driven test
        [InlineData(5, 3, 8)]
        [InlineData(-5, -3, -8)]
        [InlineData(0, 0, 0)]
        [InlineData(100, -50, 50)]
        public void Add_VariousInputs_ReturnsCorrectSum(int a, int b, int expected)
        {
            // Act
            int result = calculator.Add(a, b);

            // Assert
            result.Should().Be(expected);
        }

        [Fact]
        public void Divide_ByZero_ThrowsException()
        {
            // Assert that exception is thrown
            Action act = () => calculator.Divide(10, 0);

            act.Should().Throw<DivideByZeroException>()
               .WithMessage("Cannot divide by zero");
        }

        [Fact]
        public void Divide_ValidNumbers_ReturnsQuotient()
        {
            double result = calculator.Divide(10, 3);

            result.Should().BeApproximately(3.333, 0.001);
        }

        [Fact]
        public void Factorial_Zero_ReturnsOne()
        {
            calculator.Factorial(0).Should().Be(1);
        }

        [Fact]
        public void Factorial_Five_Returns120()
        {
            calculator.Factorial(5).Should().Be(120);
        }

        [Fact]
        public void Factorial_NegativeNumber_ThrowsException()
        {
            Action act = () => calculator.Factorial(-1);

            act.Should().Throw<ArgumentException>()
               .WithMessage("Number must be non-negative");
        }
    }
}
```

### Running Tests

```
Visual Studio:
  Test → Run All Tests

Or:
  Test Explorer window (View → Test Explorer)
  Click "Run All"

Output:
  ✓ Add_TwoPositiveNumbers_ReturnsSum (2 ms)
  ✓ Add_TwoNegativeNumbers_ReturnsNegativeSum (1 ms)
  ✓ Add_PositiveAndNegative_ReturnsDifference (1 ms)
  ✓ Add_VariousInputs_ReturnsCorrectSum (5 ms)
  ✓ Divide_ByZero_ThrowsException (3 ms)
  ✓ Divide_ValidNumbers_ReturnsQuotient (2 ms)
  ✓ Factorial_Zero_ReturnsOne (1 ms)
  ✓ Factorial_Five_Returns120 (1 ms)
  ✓ Factorial_NegativeNumber_ThrowsException (2 ms)

  Total: 9 tests, 9 passed, 0 failed
```

## 🎭 Mocking with Moq

### Why Mock?

Testing code that has **dependencies** is hard:

```csharp
public class DatabaseService
{
    public void SaveData(string data)
    {
        // Talks to real database - hard to test!
    }
}

public class DataProcessor
{
    private DatabaseService database;

    public DataProcessor(DatabaseService db)
    {
        this.database = db;
    }

    public void Process(string data)
    {
        // Process data
        string processed = data.ToUpper();

        // Save to database
        database.SaveData(processed);  // ← How to test this?
    }
}
```

**Solution:** Mock the dependency!

### Example: Testing with Mocks

**Code to Test (uses interface):**

```csharp
// Interface for database
public interface IDataRepository
{
    void Save(string data);
    string Load(int id);
    bool Exists(int id);
}

// Business logic class
public class DataService
{
    private readonly IDataRepository repository;

    public DataService(IDataRepository repository)
    {
        this.repository = repository;
    }

    public void SaveProcessedData(string data)
    {
        if (string.IsNullOrEmpty(data))
            throw new ArgumentException("Data cannot be empty");

        string processed = data.ToUpper().Trim();
        repository.Save(processed);
    }

    public string GetData(int id)
    {
        if (!repository.Exists(id))
            throw new InvalidOperationException($"Data with ID {id} not found");

        return repository.Load(id);
    }
}
```

**Unit Tests with Moq:**

```csharp
using Moq;
using Xunit;
using FluentAssertions;

public class DataServiceTests
{
    [Fact]
    public void SaveProcessedData_ValidData_CallsRepositorySave()
    {
        // Arrange
        var mockRepository = new Mock<IDataRepository>();
        var service = new DataService(mockRepository.Object);

        // Act
        service.SaveProcessedData("  hello world  ");

        // Assert: Verify Save was called with "HELLO WORLD"
        mockRepository.Verify(r => r.Save("HELLO WORLD"), Times.Once);
    }

    [Fact]
    public void SaveProcessedData_EmptyData_ThrowsException()
    {
        // Arrange
        var mockRepository = new Mock<IDataRepository>();
        var service = new DataService(mockRepository.Object);

        // Act & Assert
        Action act = () => service.SaveProcessedData("");

        act.Should().Throw<ArgumentException>()
           .WithMessage("Data cannot be empty");
    }

    [Fact]
    public void GetData_ExistingId_ReturnsData()
    {
        // Arrange
        var mockRepository = new Mock<IDataRepository>();

        // Setup mock behavior
        mockRepository.Setup(r => r.Exists(1)).Returns(true);
        mockRepository.Setup(r => r.Load(1)).Returns("Test Data");

        var service = new DataService(mockRepository.Object);

        // Act
        string result = service.GetData(1);

        // Assert
        result.Should().Be("Test Data");
        mockRepository.Verify(r => r.Exists(1), Times.Once);
        mockRepository.Verify(r => r.Load(1), Times.Once);
    }

    [Fact]
    public void GetData_NonExistingId_ThrowsException()
    {
        // Arrange
        var mockRepository = new Mock<IDataRepository>();
        mockRepository.Setup(r => r.Exists(999)).Returns(false);

        var service = new DataService(mockRepository.Object);

        // Act & Assert
        Action act = () => service.GetData(999);

        act.Should().Throw<InvalidOperationException>()
           .WithMessage("Data with ID 999 not found");
    }
}
```

### Advanced Mocking Scenarios

```csharp
public class AdvancedMockingExamples
{
    [Fact]
    public void MockReturnsSequence()
    {
        var mock = new Mock<IDataRepository>();

        // Return different values on successive calls
        mock.SetupSequence(r => r.Load(It.IsAny<int>()))
            .Returns("First")
            .Returns("Second")
            .Returns("Third");

        mock.Object.Load(1).Should().Be("First");
        mock.Object.Load(1).Should().Be("Second");
        mock.Object.Load(1).Should().Be("Third");
    }

    [Fact]
    public void MockThrowsException()
    {
        var mock = new Mock<IDataRepository>();

        // Setup to throw exception
        mock.Setup(r => r.Load(It.IsAny<int>()))
            .Throws(new InvalidOperationException("Database error"));

        Action act = () => mock.Object.Load(1);

        act.Should().Throw<InvalidOperationException>()
           .WithMessage("Database error");
    }

    [Fact]
    public void MockWithCallback()
    {
        var mock = new Mock<IDataRepository>();
        string savedData = null;

        // Capture the argument
        mock.Setup(r => r.Save(It.IsAny<string>()))
            .Callback<string>(data => savedData = data);

        var service = new DataService(mock.Object);
        service.SaveProcessedData("test");

        savedData.Should().Be("TEST");
    }
}
```

## 🐛 Debugging DLL Code

### Debugging from Test Project

**Set Breakpoint:**

```
1. Open Calculator.cs in MathLibrary
2. Click margin next to line to set breakpoint (red dot)
3. Run tests in Debug mode: Test → Debug All Tests
4. Debugger stops at breakpoint
5. Inspect variables, step through code
```

### Debugging from Host Application

**Scenario:** DLL used by WPF app

```
1. Set MathDemo (WPF app) as startup project
2. Set breakpoints in MathLibrary.dll code
3. Press F5 to debug
4. When WPF app calls DLL code, debugger breaks
```

### Debug Output

```csharp
using System.Diagnostics;

public class Calculator
{
    public int Add(int a, int b)
    {
        // Write to Debug output window
        Debug.WriteLine($"Add called: {a} + {b}");

        int result = a + b;

        Debug.WriteLine($"Result: {result}");

        return result;
    }
}

// View output: Debug → Windows → Output → Show output from: Debug
```

### Conditional Debugging

```csharp
public class Calculator
{
    [Conditional("DEBUG")]
    public void LogDebugInfo(string message)
    {
        Debug.WriteLine($"[DEBUG] {message}");
        // Only compiled in DEBUG builds!
    }

    public int Add(int a, int b)
    {
        LogDebugInfo($"Adding {a} + {b}");
        return a + b;
    }
}
```

## 🔧 Debug vs Release Builds

### Build Configurations

| Configuration | Debug | Release |
|---------------|-------|---------|
| **Optimizations** | Off | On |
| **Debug Symbols** | Full | PDB only |
| **Assertions** | Active | Removed |
| **Performance** | Slower | Faster |
| **File Size** | Larger | Smaller |

### Configuration Settings

```xml
<!-- .csproj -->
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <!-- Debug Configuration -->
    <Configuration Condition=" '$(Configuration)' == '' ">Debug</Configuration>
  </PropertyGroup>

  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' ">
    <DebugSymbols>true</DebugSymbols>
    <DebugType>full</DebugType>
    <Optimize>false</Optimize>
    <DefineConstants>DEBUG;TRACE</DefineConstants>
  </PropertyGroup>

  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Release|AnyCPU' ">
    <DebugType>pdbonly</DebugType>
    <Optimize>true</Optimize>
    <DefineConstants>TRACE</DefineConstants>
  </PropertyGroup>

</Project>
```

### Conditional Compilation

```csharp
public class Calculator
{
    public int Add(int a, int b)
    {
#if DEBUG
        // Only in Debug builds
        Console.WriteLine($"Debug: Add({a}, {b})");
        ValidateInputs(a, b);
#endif

        return a + b;
    }

#if DEBUG
    private void ValidateInputs(int a, int b)
    {
        Debug.Assert(a >= 0, "a should be non-negative");
        Debug.Assert(b >= 0, "b should be non-negative");
    }
#endif
}
```

## 📊 Performance Profiling

### Built-in Performance Profiler

**Visual Studio Profiler:**

```
1. Debug → Performance Profiler
2. Select:
   - CPU Usage
   - Memory Usage
   - .NET Object Allocation
3. Click Start
4. Use your application
5. Stop profiling
6. Analyze results
```

### BenchmarkDotNet (for precise benchmarks)

```bash
Install-Package BenchmarkDotNet
```

```csharp
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;

[MemoryDiagnoser]
public class CalculatorBenchmarks
{
    private Calculator calculator = new Calculator();

    [Benchmark]
    public int AddMethod()
    {
        return calculator.Add(5, 3);
    }

    [Benchmark]
    public int MultiplyMethod()
    {
        return calculator.Multiply(5, 3);
    }

    [Benchmark]
    public int FactorialSmall()
    {
        return calculator.Factorial(5);
    }

    [Benchmark]
    public int FactorialLarge()
    {
        return calculator.Factorial(10);
    }
}

// Run benchmarks
class Program
{
    static void Main(string[] args)
    {
        var summary = BenchmarkRunner.Run<CalculatorBenchmarks>();
    }
}
```

**Output:**

```
|          Method |      Mean |    Error |   StdDev | Allocated |
|---------------- |----------:|---------:|---------:|----------:|
|       AddMethod |  1.234 ns | 0.012 ns | 0.011 ns |       0 B |
|  MultiplyMethod |  1.289 ns | 0.015 ns | 0.014 ns |       0 B |
| FactorialSmall  | 12.456 ns | 0.125 ns | 0.117 ns |       0 B |
| FactorialLarge  | 45.789 ns | 0.456 ns | 0.427 ns |       0 B |
```

## 📈 Code Coverage

### What is Code Coverage?

**Code coverage** measures what percentage of your code is executed by tests.

### Enable Code Coverage

**Visual Studio Enterprise:**

```
1. Test → Analyze Code Coverage → All Tests
2. View Code Coverage Results window
3. See percentage covered per file/method
```

**For Visual Studio Community (free alternative):**

```bash
# Install coverlet
dotnet add package coverlet.collector
dotnet add package coverlet.msbuild

# Run tests with coverage
dotnet test /p:CollectCoverage=true
```

### Reading Coverage Reports

```
MathLibrary.dll:
  Calculator.cs: 85% covered
    - Add(): 100% ✓
    - Subtract(): 100% ✓
    - Multiply(): 75% ⚠ (missing edge case)
    - Divide(): 90% ⚠ (exception path not tested)
    - Factorial(): 60% ❌ (needs more tests)
```

**Aim for:** 80-90% coverage for libraries

## 🧪 Test-Driven Development (TDD)

### TDD Cycle: Red-Green-Refactor

```
1. RED: Write failing test
   ↓
2. GREEN: Write minimal code to pass
   ↓
3. REFACTOR: Improve code quality
   ↓
   Repeat
```

### TDD Example

**Step 1: Write Test First (RED)**

```csharp
[Fact]
public void CalculateCircleArea_Radius5_ReturnsCorrectArea()
{
    var geometry = new GeometryHelper();

    double area = geometry.CalculateCircleArea(5);

    area.Should().BeApproximately(78.54, 0.01);
}

// Compile error: GeometryHelper doesn't exist yet!
```

**Step 2: Write Minimal Code (GREEN)**

```csharp
public class GeometryHelper
{
    public double CalculateCircleArea(double radius)
    {
        return Math.PI * radius * radius;
    }
}

// Test passes! ✓
```

**Step 3: Refactor**

```csharp
public class GeometryHelper
{
    public double CalculateCircleArea(double radius)
    {
        if (radius < 0)
            throw new ArgumentException("Radius cannot be negative");

        return Math.PI * radius * radius;
    }
}

// Add test for negative radius
[Fact]
public void CalculateCircleArea_NegativeRadius_ThrowsException()
{
    var geometry = new GeometryHelper();

    Action act = () => geometry.CalculateCircleArea(-1);

    act.Should().Throw<ArgumentException>();
}
```

## 📝 Best Practices

### Testing Best Practices

```
✅ DO:
- Write tests before or alongside code (TDD)
- Test one thing per test
- Use descriptive test names
- Follow Arrange-Act-Assert pattern
- Test edge cases and error conditions
- Aim for 80%+ code coverage
- Run tests before committing code

❌ DON'T:
- Test private methods directly
- Write tests that depend on each other
- Use real databases/file systems in unit tests
- Ignore failing tests
- Test implementation details
```

### Debugging Best Practices

```
✅ DO:
- Use breakpoints strategically
- Watch variables and expressions
- Step through code methodically
- Use Debug.WriteLine for tracing
- Enable exception breaking
- Keep PDB files with DLL for debugging

❌ DON'T:
- Debug without understanding the problem
- Ignore warnings
- Ship Debug builds to production
- Forget to remove debug code
```

## 📝 Summary

### What We Learned

✅ Unit testing with xUnit framework
✅ Writing testable code with interfaces
✅ Mocking dependencies with Moq
✅ Debugging DLL code effectively
✅ Debug vs Release configurations
✅ Performance profiling techniques
✅ Code coverage analysis
✅ Test-Driven Development (TDD)

### Key Tools

- **xUnit:** Modern testing framework
- **Moq:** Mocking framework
- **FluentAssertions:** Readable assertions
- **BenchmarkDotNet:** Performance testing
- **Visual Studio Profiler:** Performance analysis
- **Code Coverage:** Quality metrics

## 🚀 Next Steps

**Congratulations!** You've completed Part 2: C# DLL Development!

**Next:** [Part 3: Native DLLs & P/Invoke](../Part3-Native-DLLs-PInvoke/Lesson11-Understanding-Native-vs-Managed.md)

## 💪 Practice Exercises

### Exercise 1: Comprehensive Testing

Test the GeometryHelper class:
```csharp
- CircleArea (positive, zero, negative)
- RectangleArea (positive, zero, negative)
- TriangleArea (positive, invalid dimensions)
- Aim for 100% code coverage
```

### Exercise 2: Mocking Practice

Create and test:
```csharp
- IFileSystem interface
- FileProcessor class (uses IFileSystem)
- Tests with mocked file system
- Verify all method calls
```

### Exercise 3: TDD Challenge

Use TDD to build:
```csharp
- StringValidator class
- Methods: IsEmail, IsPhoneNumber, IsValidPassword
- Write tests FIRST
- Then implement
```

---

**Fantastic!** 🎉 You're now a DLL testing and debugging expert!

**Continue to:** [Part 3: Native DLLs & P/Invoke](../Part3-Native-DLLs-PInvoke/)
