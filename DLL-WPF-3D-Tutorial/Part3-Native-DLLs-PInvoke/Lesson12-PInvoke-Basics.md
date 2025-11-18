# Lesson 12: P/Invoke Basics

## 📚 Learning Objectives

By the end of this lesson, you will understand:
- Platform Invocation Services (P/Invoke) fundamentals
- DllImport attribute and its parameters
- Calling Windows API functions from C#
- Basic type marshaling (int, bool, string)
- Calling conventions (StdCall, Cdecl)
- Error handling with GetLastError
- Creating C# wrappers for native functions

## 🎯 What is P/Invoke?

**P/Invoke (Platform Invocation Services)** is the mechanism that allows managed C# code to call unmanaged functions in native DLLs.

### The Bridge Between Worlds

```
C# Managed Code
      ↓
[DllImport] attribute
      ↓
P/Invoke Marshaler (converts data)
      ↓
Native DLL Function
      ↓
Result
      ↓
P/Invoke Marshaler (converts back)
      ↓
C# Managed Code
```

## 🔧 Your First P/Invoke Call

### Example: Windows MessageBox

```csharp
using System;
using System.Runtime.InteropServices;

class Program
{
    // Import the MessageBox function from user32.dll
    [DllImport("user32.dll", CharSet = CharSet.Unicode)]
    static extern int MessageBox(
        IntPtr hWnd,
        string text,
        string caption,
        uint type);

    static void Main()
    {
        // Call the native Windows function
        MessageBox(
            IntPtr.Zero,
            "Hello from P/Invoke!",
            "My First P/Invoke",
            0);  // MB_OK
    }
}
```

**Run this and you'll see a native Windows message box!**

### Breaking Down the Code

```csharp
[DllImport("user32.dll", CharSet = CharSet.Unicode)]
//         │              │
//         │              └─ How to handle strings
//         └──────────────── Which DLL to load

static extern int MessageBox(...)
//     │      │   │
//     │      │   └─ Function name (must match DLL export)
//     │      └───── Return type
//     └──────────── Native function (no implementation)
```

## 📋 The DllImport Attribute

### Basic Syntax

```csharp
[DllImport("library.dll")]
static extern ReturnType FunctionName(parameters);
```

### DllImport Parameters

| Parameter | Description | Example |
|-----------|-------------|---------|
| **DLL Name** | Which DLL to load | `"kernel32.dll"` |
| **CharSet** | String encoding | `CharSet.Unicode` |
| **EntryPoint** | Function name in DLL | `EntryPoint = "RealName"` |
| **CallingConvention** | How to call | `CallingConvention.Cdecl` |
| **SetLastError** | Enable GetLastError | `SetLastError = true` |
| **ExactSpelling** | No name decoration | `ExactSpelling = true` |

### Complete Example

```csharp
[DllImport("user32.dll",
    CharSet = CharSet.Unicode,
    SetLastError = true,
    CallingConvention = CallingConvention.StdCall,
    ExactSpelling = true)]
static extern bool SetWindowTextW(IntPtr hWnd, string text);
```

## 🔤 CharSet and String Handling

### Understanding CharSet

Windows API functions come in two versions:
- **ANSI version:** Ends with 'A' (MessageBoxA) - 8-bit chars
- **Unicode version:** Ends with 'W' (MessageBoxW) - 16-bit chars

```csharp
// Automatic selection based on CharSet
[DllImport("user32.dll", CharSet = CharSet.Auto)]
static extern int MessageBox(...);  
// Calls MessageBoxA or MessageBoxW based on OS

// Explicitly Unicode
[DllImport("user32.dll", CharSet = CharSet.Unicode)]
static extern int MessageBox(...);
// Always calls MessageBoxW

// Explicitly ANSI
[DllImport("user32.dll", CharSet = CharSet.Ansi)]
static extern int MessageBox(...);
// Always calls MessageBoxA
```

### CharSet Options

| CharSet | Behavior | Use When |
|---------|----------|----------|
| **Auto** | System decides (Unicode on NT) | General use |
| **Unicode** | UTF-16 (2 bytes/char) | Modern Windows |
| **Ansi** | ASCII (1 byte/char) | Legacy systems |
| **None** | No marshaling | Binary data |

### Example: String Marshaling

```csharp
// C++ DLL function:
// extern "C" __declspec(dllexport) void PrintString(const char* str);

// C# P/Invoke (ANSI):
[DllImport("MyDll.dll", CharSet = CharSet.Ansi)]
static extern void PrintString(string str);

// C++ DLL function:
// extern "C" __declspec(dllexport) void PrintStringW(const wchar_t* str);

// C# P/Invoke (Unicode):
[DllImport("MyDll.dll", CharSet = CharSet.Unicode)]
static extern void PrintStringW(string str);
```

## 📞 Calling Conventions

### What is a Calling Convention?

Defines how parameters are passed and who cleans up the stack.

### Common Calling Conventions

**1. StdCall (Default for Windows API)**
```csharp
[DllImport("kernel32.dll", CallingConvention = CallingConvention.StdCall)]
static extern bool Beep(uint frequency, uint duration);
```

**2. Cdecl (Used by C runtime functions)**
```csharp
[DllImport("msvcrt.dll", CallingConvention = CallingConvention.Cdecl)]
static extern int printf(string format, __arglist);
```

**3. FastCall**
```csharp
[DllImport("MyDll.dll", CallingConvention = CallingConvention.FastCall)]
static extern int FastFunction(int a, int b);
```

### Comparison

| Convention | Who Cleans Stack | Parameter Order | Use Case |
|------------|------------------|-----------------|----------|
| **StdCall** | Callee | Right to left | Windows API |
| **Cdecl** | Caller | Right to left | C functions |
| **FastCall** | Callee | Registers first | Performance |

### Example: printf with Cdecl

```csharp
using System;
using System.Runtime.InteropServices;

class Program
{
    [DllImport("msvcrt.dll", CallingConvention = CallingConvention.Cdecl)]
    static extern int printf(string format, __arglist);

    static void Main()
    {
        printf("Hello %s!\n", __arglist("World"));
        printf("Number: %d\n", __arglist(42));
    }
}
```

## 🔢 Basic Type Marshaling

### Simple Types (Blittable)

These types have the same representation in managed and native code:

| C# Type | C/C++ Type | Size |
|---------|------------|------|
| `byte` | `unsigned char` | 1 byte |
| `sbyte` | `char` | 1 byte |
| `short` | `short` | 2 bytes |
| `ushort` | `unsigned short` | 2 bytes |
| `int` | `int` | 4 bytes |
| `uint` | `unsigned int` | 4 bytes |
| `long` | `__int64` | 8 bytes |
| `ulong` | `unsigned __int64` | 8 bytes |
| `float` | `float` | 4 bytes |
| `double` | `double` | 8 bytes |
| `IntPtr` | `void*` | 4/8 bytes |

```csharp
// C++ function:
// extern "C" __declspec(dllexport) int Add(int a, int b);

// C# P/Invoke:
[DllImport("MathLib.dll")]
static extern int Add(int a, int b);

// Usage:
int result = Add(5, 3);  // Returns 8
```

### Boolean Types

```csharp
// C++ uses int for bool (0 = false, non-zero = true)
[DllImport("MyDll.dll")]
[return: MarshalAs(UnmanagedType.Bool)]
static extern bool IsEven(int number);

// Win32 BOOL is 4 bytes
[DllImport("kernel32.dll", SetLastError = true)]
[return: MarshalAs(UnmanagedType.Bool)]
static extern bool CloseHandle(IntPtr handle);
```

### String Types

```csharp
// C-style null-terminated string (char*)
[DllImport("MyDll.dll")]
static extern void PrintMessage(string message);

// StringBuilder for output strings
[DllImport("kernel32.dll", CharSet = CharSet.Auto)]
static extern uint GetCurrentDirectory(
    uint bufferLength,
    [Out] StringBuilder buffer);

// Usage:
StringBuilder sb = new StringBuilder(260);  // MAX_PATH
GetCurrentDirectory((uint)sb.Capacity, sb);
Console.WriteLine($"Current directory: {sb}");
```

## 🛠️ Practical Examples

### Example 1: Windows Beep

```csharp
using System;
using System.Runtime.InteropServices;

class BeepExample
{
    [DllImport("kernel32.dll", SetLastError = true)]
    static extern bool Beep(uint frequency, uint duration);

    static void Main()
    {
        Console.WriteLine("Playing musical notes...");

        // C note (261.63 Hz)
        Beep(262, 200);
        System.Threading.Thread.Sleep(50);

        // E note (329.63 Hz)
        Beep(330, 200);
        System.Threading.Thread.Sleep(50);

        // G note (392.00 Hz)
        Beep(392, 200);
        System.Threading.Thread.Sleep(50);

        // C note (523.25 Hz)
        Beep(523, 400);

        Console.WriteLine("Done!");
    }
}
```

### Example 2: Getting System Time

```csharp
using System;
using System.Runtime.InteropServices;

class SystemTimeExample
{
    [StructLayout(LayoutKind.Sequential)]
    struct SYSTEMTIME
    {
        public ushort wYear;
        public ushort wMonth;
        public ushort wDayOfWeek;
        public ushort wDay;
        public ushort wHour;
        public ushort wMinute;
        public ushort wSecond;
        public ushort wMilliseconds;
    }

    [DllImport("kernel32.dll")]
    static extern void GetSystemTime(out SYSTEMTIME st);

    static void Main()
    {
        SYSTEMTIME st;
        GetSystemTime(out st);

        Console.WriteLine($"UTC Time:");
        Console.WriteLine($"  Year: {st.wYear}");
        Console.WriteLine($"  Month: {st.wMonth}");
        Console.WriteLine($"  Day: {st.wDay}");
        Console.WriteLine($"  Hour: {st.wHour}");
        Console.WriteLine($"  Minute: {st.wMinute}");
        Console.WriteLine($"  Second: {st.wSecond}");
    }
}
```

### Example 3: File Operations

```csharp
using System;
using System.Runtime.InteropServices;
using System.Text;

class FileExample
{
    const uint GENERIC_WRITE = 0x40000000;
    const uint CREATE_ALWAYS = 2;
    static readonly IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);

    [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    static extern IntPtr CreateFile(
        string fileName,
        uint desiredAccess,
        uint shareMode,
        IntPtr securityAttributes,
        uint creationDisposition,
        uint flagsAndAttributes,
        IntPtr templateFile);

    [DllImport("kernel32.dll", SetLastError = true)]
    static extern bool WriteFile(
        IntPtr hFile,
        byte[] buffer,
        uint numberOfBytesToWrite,
        out uint numberOfBytesWritten,
        IntPtr overlapped);

    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    static extern bool CloseHandle(IntPtr hObject);

    static void Main()
    {
        // Create file
        IntPtr handle = CreateFile(
            "test.txt",
            GENERIC_WRITE,
            0,
            IntPtr.Zero,
            CREATE_ALWAYS,
            0,
            IntPtr.Zero);

        if (handle == INVALID_HANDLE_VALUE)
        {
            Console.WriteLine($"Error creating file: {Marshal.GetLastWin32Error()}");
            return;
        }

        // Write data
        string data = "Hello from P/Invoke!\r\n";
        byte[] bytes = Encoding.ASCII.GetBytes(data);
        uint bytesWritten;

        if (WriteFile(handle, bytes, (uint)bytes.Length, out bytesWritten, IntPtr.Zero))
        {
            Console.WriteLine($"Wrote {bytesWritten} bytes");
        }

        // Close handle
        CloseHandle(handle);
        Console.WriteLine("File created successfully!");
    }
}
```

## ⚠️ Error Handling

### Using SetLastError

```csharp
[DllImport("kernel32.dll", SetLastError = true)]
static extern IntPtr CreateFile(...);

// Call function
IntPtr handle = CreateFile(...);

// Check for error
if (handle == INVALID_HANDLE_VALUE)
{
    int error = Marshal.GetLastWin32Error();
    Console.WriteLine($"Error code: {error}");

    // Get error message
    string message = new Win32Exception(error).Message;
    Console.WriteLine($"Error: {message}");
}
```

### Complete Error Handling Example

```csharp
using System;
using System.ComponentModel;
using System.Runtime.InteropServices;

class ErrorHandlingExample
{
    [DllImport("kernel32.dll", SetLastError = true)]
    static extern bool Beep(uint frequency, uint duration);

    static void SafeBeep(uint frequency, uint duration)
    {
        if (!Beep(frequency, duration))
        {
            int error = Marshal.GetLastWin32Error();
            throw new Win32Exception(error, 
                $"Beep failed with error code: {error}");
        }
    }

    static void Main()
    {
        try
        {
            SafeBeep(1000, 200);
            Console.WriteLine("Beep successful!");
        }
        catch (Win32Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }
}
```

## 🎁 Creating a Helper Class

### Wrapper Pattern

```csharp
using System;
using System.Runtime.InteropServices;
using System.Text;

/// <summary>
/// Wrapper for Windows kernel functions
/// </summary>
public static class Kernel32
{
    const uint GENERIC_READ = 0x80000000;
    const uint GENERIC_WRITE = 0x40000000;
    const uint CREATE_ALWAYS = 2;
    const uint OPEN_EXISTING = 3;

    [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    private static extern IntPtr CreateFile(
        string fileName,
        uint desiredAccess,
        uint shareMode,
        IntPtr securityAttributes,
        uint creationDisposition,
        uint flagsAndAttributes,
        IntPtr templateFile);

    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool CloseHandle(IntPtr hObject);

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool Beep(uint frequency, uint duration);

    // Public wrapper methods
    public static IntPtr CreateFileForWrite(string fileName)
    {
        IntPtr handle = CreateFile(
            fileName,
            GENERIC_WRITE,
            0,
            IntPtr.Zero,
            CREATE_ALWAYS,
            0,
            IntPtr.Zero);

        if (handle == new IntPtr(-1))
        {
            throw new System.ComponentModel.Win32Exception(
                Marshal.GetLastWin32Error());
        }

        return handle;
    }

    public static void Close(IntPtr handle)
    {
        if (!CloseHandle(handle))
        {
            throw new System.ComponentModel.Win32Exception(
                Marshal.GetLastWin32Error());
        }
    }

    public static void PlayBeep(uint frequency, uint duration)
    {
        if (!Beep(frequency, duration))
        {
            throw new System.ComponentModel.Win32Exception(
                Marshal.GetLastWin32Error());
        }
    }
}

// Usage:
class Program
{
    static void Main()
    {
        try
        {
            Kernel32.PlayBeep(800, 200);
            Console.WriteLine("Beep played successfully!");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }
}
```

## 📝 Best Practices

### ✅ DO:

```csharp
// Use SetLastError for Win32 functions
[DllImport("kernel32.dll", SetLastError = true)]
static extern bool CloseHandle(IntPtr handle);

// Use CharSet for string functions
[DllImport("user32.dll", CharSet = CharSet.Auto)]
static extern int MessageBox(...);

// Check return values
IntPtr handle = CreateFile(...);
if (handle == INVALID_HANDLE_VALUE)
{
    // Handle error
}

// Use StringBuilder for output strings
StringBuilder sb = new StringBuilder(256);
GetWindowText(handle, sb, sb.Capacity);
```

### ❌ DON'T:

```csharp
// Don't ignore SetLastError
[DllImport("kernel32.dll")]  // Missing SetLastError = true
static extern bool CloseHandle(IntPtr handle);

// Don't ignore errors
CreateFile(...);  // No error checking!

// Don't use string for output
[DllImport("kernel32.dll")]
static extern void GetWindowText(IntPtr hwnd, string buffer, int maxCount);
// Use StringBuilder instead!

// Don't assume calling convention
[DllImport("MyDll.dll")]  // Might be wrong!
static extern int MyFunction(...);
// Specify CallingConvention explicitly
```

## 📊 Summary

### What We Learned

✅ P/Invoke allows C# to call native DLLs
✅ DllImport attribute specifies the DLL and function
✅ CharSet controls string marshaling (Ansi, Unicode, Auto)
✅ Calling conventions determine parameter passing
✅ SetLastError enables error checking
✅ Basic types marshal automatically
✅ StringBuilder for output strings
✅ Always check return values and errors

### Key Attributes

```csharp
[DllImport("library.dll",
    CharSet = CharSet.Auto,           // String encoding
    CallingConvention = CallingConvention.StdCall,  // How to call
    SetLastError = true,              // Enable GetLastError
    EntryPoint = "RealFunctionName",  // Actual name in DLL
    ExactSpelling = true)]            // No name decoration
static extern ReturnType FunctionName(parameters);
```

## 🚀 Next Steps

In **Lesson 13: Marshaling Data Types**, we'll learn:
- Complex structure marshaling
- Array and pointer handling
- In/Out/Ref parameters
- String builder usage
- Callback functions
- Custom marshaling

## 💪 Practice Exercises

### Exercise 1: Windows API
Call these Windows functions:
```csharp
1. GetComputerName - Get computer name
2. GetUserName - Get logged-in user
3. Sleep - Pause execution
```

### Exercise 2: File Operations
Use Windows API to:
```csharp
1. Create a file (CreateFile)
2. Write data (WriteFile)
3. Read data (ReadFile)
4. Close handle (CloseHandle)
```

### Exercise 3: Error Handling
```csharp
Create SafeWin32 class with:
- Proper error handling
- Exception throwing
- Win32Exception usage
```

---

**Excellent!** 🎉 You now understand P/Invoke basics!

**Next Lesson:** [Lesson 13: Marshaling Data Types](Lesson13-Marshaling-Data-Types.md)
