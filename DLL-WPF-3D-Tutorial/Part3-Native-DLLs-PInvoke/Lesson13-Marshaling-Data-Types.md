# Lesson 13: Marshaling Data Types

## 📚 Learning Objectives

- Master complex data type marshaling between C# and native code
- Handle structures, arrays, and pointers correctly  
- Understand string encoding and buffer management
- Work with callbacks and function pointers
- Use custom marshaling attributes
- Implement safe marshaling patterns

## 🎯 What is Marshaling?

**Marshaling** is the process of converting data between managed (.NET) and unmanaged (native) memory representations.

```
C# String "Hello"
      ↓
Marshaler converts
      ↓
char* "Hello\0" (C string)
      ↓
Native function uses it
      ↓
Marshaler converts back
      ↓
C# String "Hello"
```

## 🔢 Structure Marshaling

### Simple Structure

```csharp
// C++ structure:
// struct Point {
//     int x;
//     int y;
// };

// C# equivalent:
[StructLayout(LayoutKind.Sequential)]
public struct Point
{
    public int X;
    public int Y;
}

[DllImport("MyDll.dll")]
static extern void DrawPoint(Point point);

// Usage:
Point p = new Point { X = 10, Y = 20 };
DrawPoint(p);  // Marshaled automatically!
```

### Complex Structure with Strings

```csharp
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
public struct Person
{
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
    public string Name;
    
    public int Age;
    
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
    public int[] Scores;
}

[DllImport("MyDll.dll")]
static extern void ProcessPerson(ref Person person);
```

### LayoutKind Options

```csharp
// Sequential: Members laid out in order
[StructLayout(LayoutKind.Sequential)]
struct SequentialStruct { }

// Explicit: Specify exact offset
[StructLayout(LayoutKind.Explicit)]
struct ExplicitStruct
{
    [FieldOffset(0)]
    public int IntValue;
    
    [FieldOffset(0)]  // Same offset = union
    public float FloatValue;
}

// Auto: CLR decides layout (don't use for P/Invoke!)
[StructLayout(LayoutKind.Auto)]
struct AutoStruct { }  // ❌ Don't use with P/Invoke
```

## 🔤 String Marshaling

### String Types

```csharp
// ANSI string (char*)
[DllImport("MyDll.dll", CharSet = CharSet.Ansi)]
static extern void ProcessStringA(string str);

// Unicode string (wchar_t*)
[DllImport("MyDll.dll", CharSet = CharSet.Unicode)]
static extern void ProcessStringW(string str);

// BSTR (COM string)
[DllImport("MyDll.dll")]
static extern void ProcessBSTR(
    [MarshalAs(UnmanagedType.BStr)] string str);
```

### StringBuilder for Output

```csharp
[DllImport("kernel32.dll", CharSet = CharSet.Auto)]
static extern uint GetWindowText(
    IntPtr hWnd,
    [Out] StringBuilder lpString,
    int nMaxCount);

// Usage:
StringBuilder sb = new StringBuilder(256);
GetWindowText(handle, sb, sb.Capacity);
string text = sb.ToString();
```

### Fixed-Length String Buffers

```csharp
[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct FixedString
{
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
    public string FilePath;  // Like char[260] in C
}
```

## 📦 Array Marshaling

### Simple Arrays

```csharp
// C++: void ProcessArray(int* arr, int count)
[DllImport("MyDll.dll")]
static extern void ProcessArray(
    [In, Out] int[] array,
    int count);

// Usage:
int[] numbers = { 1, 2, 3, 4, 5 };
ProcessArray(numbers, numbers.Length);
```

### Multidimensional Arrays

```csharp
// C++: void ProcessMatrix(int matrix[3][3])
[DllImport("MyDll.dll")]
static extern void ProcessMatrix(
    [In, Out, MarshalAs(UnmanagedType.LPArray, SizeConst = 9)]
    int[] matrix);

// Usage (flatten 2D to 1D):
int[] matrix = new int[9];  // 3x3 matrix
ProcessMatrix(matrix);
```

### Array of Structures

```csharp
[DllImport("MyDll.dll")]
static extern void ProcessPoints(
    [In, Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]
    Point[] points,
    int count);

// Usage:
Point[] points = new Point[10];
ProcessPoints(points, points.Length);
```

## 🎯 Pointer Marshaling

### IntPtr for Pointers

```csharp
// C++: void* AllocateMemory(size_t size)
[DllImport("MyDll.dll")]
static extern IntPtr AllocateMemory(int size);

// C++: void FreeMemory(void* ptr)
[DllImport("MyDll.dll")]
static extern void FreeMemory(IntPtr ptr);

// Usage:
IntPtr ptr = AllocateMemory(1024);
try
{
    // Use memory
}
finally
{
    FreeMemory(ptr);
}
```

### ref and out Parameters

```csharp
// C++: void GetValue(int* outValue)
[DllImport("MyDll.dll")]
static extern void GetValue(out int value);

// C++: void ModifyValue(int* value)
[DllImport("MyDll.dll")]
static extern void ModifyValue(ref int value);

// Usage:
int result;
GetValue(out result);  // out: must be assigned

int value = 10;
ModifyValue(ref value);  // ref: modify existing
```

## 🔄 Callback Functions

### Defining Callback Delegates

```csharp
// C++: typedef void (*Callback)(int value);
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void MyCallback(int value);

// C++: void RegisterCallback(Callback cb)
[DllImport("MyDll.dll", CallingConvention = CallingConvention.Cdecl)]
static extern void RegisterCallback(MyCallback callback);

// Usage:
MyCallback callback = (value) =>
{
    Console.WriteLine($"Callback called with: {value}");
};

RegisterCallback(callback);
// ⚠️ Keep callback alive! GC might collect it
```

### Keeping Callbacks Alive

```csharp
public class CallbackManager
{
    private MyCallback callback;  // Keep reference!
    
    public void Setup()
    {
        callback = OnCallback;
        RegisterCallback(callback);
    }
    
    private void OnCallback(int value)
    {
        Console.WriteLine($"Value: {value}");
    }
}
```

## 🛡️ Safe Marshaling with SafeHandle

### Creating a SafeHandle

```csharp
using Microsoft.Win32.SafeHandles;

public class SafeFileHandle : SafeHandleZeroOrMinusOneIsInvalid
{
    public SafeFileHandle() : base(true) { }
    
    protected override bool ReleaseHandle()
    {
        return CloseHandle(handle);
    }
    
    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool CloseHandle(IntPtr handle);
}

// Usage:
[DllImport("kernel32.dll", SetLastError = true)]
static extern SafeFileHandle CreateFile(...);

using (SafeFileHandle handle = CreateFile(...))
{
    // Use handle safely
    // Automatically disposed and closed!
}
```

## 📋 Complete Practical Example

```csharp
using System;
using System.Runtime.InteropServices;
using System.Text;

namespace MarshalingExample
{
    // Structure matching C++ definition
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Employee
    {
        public int Id;
        
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 50)]
        public string Name;
        
        public double Salary;
        
        [MarshalAs(UnmanagedType.Bool)]
        public bool IsActive;
    }
    
    // Callback delegate
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void EmployeeCallback(ref Employee emp);
    
    class Program
    {
        // P/Invoke declarations
        [DllImport("EmployeeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern bool CreateEmployee(ref Employee emp);
        
        [DllImport("EmployeeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void GetEmployees(
            [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]
            Employee[] employees,
            int count);
        
        [DllImport("EmployeeLib.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void EnumerateEmployees(EmployeeCallback callback);
        
        static void Main()
        {
            // Create employee
            Employee emp = new Employee
            {
                Id = 1,
                Name = "John Doe",
                Salary = 50000.0,
                IsActive = true
            };
            
            if (CreateEmployee(ref emp))
            {
                Console.WriteLine("Employee created successfully!");
            }
            
            // Get multiple employees
            Employee[] employees = new Employee[10];
            GetEmployees(employees, employees.Length);
            
            // Use callback
            EmployeeCallback callback = (ref Employee e) =>
            {
                Console.WriteLine($"{e.Id}: {e.Name} - ${e.Salary}");
            };
            
            EnumerateEmployees(callback);
        }
    }
}
```

## 💡 Best Practices

```csharp
// ✅ DO: Use StructLayout for structures
[StructLayout(LayoutKind.Sequential)]
public struct MyStruct { }

// ✅ DO: Specify CharSet for strings
[DllImport("MyDll.dll", CharSet = CharSet.Unicode)]

// ✅ DO: Use StringBuilder for output strings
StringBuilder sb = new StringBuilder(256);

// ✅ DO: Keep callback delegates alive
private MyCallback callback;  // Field, not local variable

// ❌ DON'T: Use auto layout with P/Invoke
[StructLayout(LayoutKind.Auto)]  // ❌ Bad for P/Invoke

// ❌ DON'T: Assume structure size
// Always use Marshal.SizeOf() or sizeof()

// ❌ DON'T: Forget to specify array size
[MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
```

## 📝 Summary

✅ Structures need StructLayout attribute
✅ Use StringBuilder for output strings
✅ Arrays marshal automatically with correct attributes
✅ Callbacks need UnmanagedFunctionPointer
✅ SafeHandle for automatic resource management
✅ Always match C++ structure layout exactly

## 🚀 Next Steps

**Lesson 14:** Creating C++ DLLs for C# - Build native libraries optimized for C# consumption

## 💪 Exercises

1. Create structures matching Windows API types (RECT, POINT)
2. Implement string marshaling with different CharSets
3. Create callback system for progress reporting
4. Build SafeHandle wrapper for native resource

---

**Excellent!** 🎉 You now master data marshaling!

**Next:** [Lesson 14: Creating C++ DLLs for C#](Lesson14-Creating-CPP-DLLs-for-CSharp.md)
