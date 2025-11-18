# Lesson 2: How DLLs Work

## 📚 Learning Objectives

By the end of this lesson, you will understand:
- The internal structure of DLL files
- How Windows loads DLLs into memory
- The DLL entry point (DllMain)
- Export and import mechanisms
- The linking and loading process
- Memory mapping and address spaces

## 🔍 The DLL File Structure

### PE Format (Portable Executable)

DLL files use the **PE (Portable Executable)** format, the same format used by EXE files. Let's break down what's inside:

```
┌─────────────────────────────────────┐
│         MyLibrary.dll               │
├─────────────────────────────────────┤
│ DOS Header                          │ ← Legacy compatibility
├─────────────────────────────────────┤
│ PE Header                           │ ← File information
│  - Machine type (x86/x64)           │
│  - Number of sections               │
│  - Timestamp                        │
├─────────────────────────────────────┤
│ Optional Header                     │ ← Important metadata
│  - Entry point address              │
│  - Image base address               │
│  - Section alignment                │
├─────────────────────────────────────┤
│ Section Table                       │ ← Directory of sections
├─────────────────────────────────────┤
│ .text Section                       │ ← Your CODE lives here
│  - All executable code              │
│  - Your functions                   │
├─────────────────────────────────────┤
│ .data Section                       │ ← Initialized DATA
│  - Global variables                 │
│  - Static variables                 │
├─────────────────────────────────────┤
│ .rdata Section                      │ ← Read-only DATA
│  - String literals                  │
│  - Constants                        │
├─────────────────────────────────────┤
│ .bss Section                        │ ← Uninitialized DATA
│  - Uninitialized globals            │
├─────────────────────────────────────┤
│ .rsrc Section                       │ ← RESOURCES
│  - Icons, images                    │
│  - Dialogs, menus                   │
├─────────────────────────────────────┤
│ .reloc Section                      │ ← Relocation table
│  - Address fix-up information       │
├─────────────────────────────────────┤
│ Import Table                        │ ← DLLs this DLL needs
│  - kernel32.dll                     │
│  - user32.dll                       │
├─────────────────────────────────────┤
│ Export Table                        │ ← Functions this DLL provides
│  - MyFunction1                      │
│  - MyFunction2                      │
│  - MyClass.Method1                  │
└─────────────────────────────────────┘
```

### Key Components Explained

#### 1. **DOS Header**
```
Why it exists: Backwards compatibility
Contains: "This program cannot be run in DOS mode"
Purpose: If you try to run in DOS, shows error message
```

#### 2. **PE Header**
```
Contains:
- Signature: "PE\0\0" (identifies it as PE file)
- Machine type: x86, x64, ARM, etc.
- Number of sections
- Characteristics: Is it a DLL or EXE?
```

#### 3. **Code Section (.text)**
Your actual compiled code:
```
Machine code (binary):
0x55 0x8B 0xEC 0x83 0xEC 0x40 ...

Represents:
push ebp
mov ebp, esp
sub esp, 40h
...
```

#### 4. **Export Table**
Lists all functions/classes that other programs can use:
```
Export Table:
  Function Name: "Add"
  Address: 0x1000
  Ordinal: 1

  Function Name: "Subtract"
  Address: 0x1050
  Ordinal: 2
```

#### 5. **Import Table**
Lists all external DLLs this DLL depends on:
```
Import Table:
  DLL: kernel32.dll
    - GetCurrentThread
    - GetCurrentProcess

  DLL: msvcrt.dll
    - malloc
    - free
```

## 🔄 The DLL Loading Process

### Step-by-Step: What Happens When You Load a DLL

Let's trace what happens when your program executes:
```csharp
var result = MyLibrary.Calculator.Add(5, 3);
```

### Step 1: Application Starts

```
┌────────────────────────────────────────┐
│ Windows Process Manager                │
│                                        │
│ 1. User double-clicks YourApp.exe     │
│ 2. Create new process                 │
│ 3. Allocate virtual address space     │
│ 4. Load YourApp.exe into memory       │
└────────────────────────────────────────┘
```

### Step 2: Dependency Analysis

Windows PE Loader examines YourApp.exe:

```
PE Loader reads Import Table:
┌─────────────────────────────────┐
│ YourApp.exe imports:            │
│                                 │
│ ✓ kernel32.dll                  │
│ ✓ mscoree.dll (.NET runtime)    │
│ ✓ MyLibrary.dll (YOUR DLL!)     │
└─────────────────────────────────┘
```

### Step 3: Locate DLLs

Search order (Windows searches in this sequence):

```
1. Application Directory
   └→ C:\MyApp\MyLibrary.dll ← FOUND!

2. System32 (if not found above)
   └→ C:\Windows\System32\

3. System Directory
   └→ C:\Windows\System\

4. Windows Directory
   └→ C:\Windows\

5. Current Directory
   └→ [Current working directory]

6. PATH Environment Variable
   └→ All directories in %PATH%
```

### Step 4: Load DLL into Memory

```
Memory Layout:

High Memory (0xFFFFFFFF)
│
├─────────────────────────┐
│ Kernel Memory           │ ← Windows kernel
├─────────────────────────┤
│                         │
│ User Space              │
│                         │
│  ┌──────────────────┐   │
│  │ YourApp.exe      │   │ ← Your program
│  │ Base: 0x00400000 │   │
│  └──────────────────┘   │
│                         │
│  ┌──────────────────┐   │
│  │ MyLibrary.dll    │   │ ← Your DLL
│  │ Base: 0x10000000 │   │ ← Loaded here!
│  └──────────────────┘   │
│                         │
│  ┌──────────────────┐   │
│  │ kernel32.dll     │   │ ← System DLL
│  │ Base: 0x77000000 │   │
│  └──────────────────┘   │
│                         │
├─────────────────────────┤
Low Memory (0x00000000)
```

### Step 5: Call DllMain (Entry Point)

Every DLL has an **entry point** called `DllMain`:

```c
// This function is called automatically by Windows
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // Handle to DLL module
    DWORD fdwReason,     // Reason for calling function
    LPVOID lpvReserved   // Reserved
)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // DLL is being loaded into a process
            // Initialize your DLL here
            printf("DLL is being loaded!\n");
            break;

        case DLL_THREAD_ATTACH:
            // A new thread is created
            break;

        case DLL_THREAD_DETACH:
            // A thread exits cleanly
            break;

        case DLL_PROCESS_DETACH:
            // DLL is being unloaded
            // Cleanup here
            printf("DLL is being unloaded!\n");
            break;
    }
    return TRUE; // Success
}
```

### Step 6: Resolve Imports

Windows links function calls to actual addresses:

```
Before Resolution (in your EXE):
  Call MyLibrary.Add  → Address: ????????

After Resolution:
  Call MyLibrary.Add  → Address: 0x10001000 ← Now points to actual code!
```

### Step 7: Function Call

Now your code can call DLL functions:

```csharp
// Your code
var result = MyLibrary.Calculator.Add(5, 3);

Under the hood:
1. Push parameters (5, 3) onto stack
2. Jump to address 0x10001000
3. Execute Add() function in DLL
4. Return result (8)
5. Pop parameters from stack
```

## 🧠 Memory Management

### How DLLs Share Memory

```
Process 1 Memory Space          Process 2 Memory Space
┌─────────────────────┐        ┌─────────────────────┐
│                     │        │                     │
│  App1.exe           │        │  App2.exe           │
│  Base: 0x00400000   │        │  Base: 0x00400000   │
│                     │        │                     │
│  MyLib.dll          │        │  MyLib.dll          │
│  Base: 0x10000000   │        │  Base: 0x10000000   │
│                     │        │                     │
│  CODE (shared!)  ←──┼────────┼──→ Same physical    │
│  DATA (separate)    │        │    memory!          │
│                     │        │    DATA (separate)  │
└─────────────────────┘        └─────────────────────┘
```

**Key Concept:**
- **Code section** (.text): Shared in physical memory (read-only)
- **Data section** (.data): Separate for each process (read-write)

### Copy-on-Write

Windows uses **Copy-on-Write** for efficiency:

```
Initial State (both apps use DLL):
App1 → [DLL Code Page] ← App2 (Same physical page!)

App1 modifies data:
App1 → [DLL Code Page] ← App2 (Still shared)
App1 → [Data Copy 1]
App2 → [Data Copy 2]  (Now separate!)
```

## 📊 Export Mechanisms

### How Functions Become Available

For a function to be called from outside the DLL, it must be **exported**.

### Method 1: Using __declspec(dllexport) in C++

```cpp
// In MyLibrary.dll (C++)
__declspec(dllexport) int Add(int a, int b)
{
    return a + b;
}

// This adds "Add" to the Export Table
```

### Method 2: Using .def File

```
; MyLibrary.def
LIBRARY   MyLibrary
EXPORTS
   Add
   Subtract
   Multiply
```

### Method 3: C# Automatic Export

```csharp
// In C# DLLs, all public classes/methods are automatically "exported"
public class Calculator
{
    public int Add(int a, int b) // Automatically available!
    {
        return a + b;
    }
}
```

### Viewing Exports

You can view a DLL's exports using tools:

#### Using dumpbin (Visual Studio)
```batch
dumpbin /EXPORTS MyLibrary.dll

Output:
    ordinal hint RVA      name
          1    0 00001000 Add
          2    1 00001050 Subtract
```

#### Using Dependency Walker
```
Shows:
- All exported functions
- All imported DLLs
- Dependency tree
```

## 🔗 Import Mechanisms

### How Programs Use DLL Functions

### Static (Load-Time) Linking

Your program knows about the DLL at compile time:

```csharp
// C# example
using MyLibrary; // Reference added at compile time

var calc = new Calculator();
var result = calc.Add(5, 3);

// At runtime:
// - Windows automatically loads MyLibrary.dll
// - Resolves Calculator and Add addresses
// - Everything happens automatically!
```

### Dynamic (Runtime) Loading

Your program loads the DLL manually at runtime:

```csharp
// C# example using reflection
Assembly dll = Assembly.LoadFrom("MyLibrary.dll");
Type calcType = dll.GetType("MyLibrary.Calculator");
object calcInstance = Activator.CreateInstance(calcType);
MethodInfo addMethod = calcType.GetMethod("Add");
object result = addMethod.Invoke(calcInstance, new object[] { 5, 3 });

// Gives you more control but more complex!
```

For native DLLs in C#:

```csharp
// P/Invoke: Import native DLL function
[DllImport("user32.dll", CharSet = CharSet.Auto)]
public static extern int MessageBox(
    IntPtr hWnd,
    string text,
    string caption,
    uint type
);

// Usage
MessageBox(IntPtr.Zero, "Hello from DLL!", "Title", 0);
```

## ⚙️ Address Relocation

### The Rebasing Problem

Every DLL has a **preferred base address**:

```
MyLibrary.dll prefers: 0x10000000
```

**Problem:** What if that address is already occupied?

```
Memory:
0x10000000 ← AnotherDLL.dll is HERE!
           ← MyLibrary.dll wants HERE too!
```

**Solution:** Relocation (rebasing)

```
Windows loader:
1. Checks if 0x10000000 is available
2. If NOT, loads DLL at different address (e.g., 0x11000000)
3. Fixes all internal addresses using .reloc section
```

### How Relocation Works

DLL code contains addresses:
```asm
; Original code (base 0x10000000)
mov eax, [0x10001234]  ; Read from address 0x10001234

; After rebasing to 0x11000000
mov eax, [0x11001234]  ; Address updated! (+0x01000000)
```

The `.reloc` section tells Windows which addresses to update:
```
Relocation Table:
  Offset: 0x1000 (needs fixing)
  Offset: 0x1050 (needs fixing)
  Offset: 0x2000 (needs fixing)
```

## 🎯 DLL Versioning

### Version Information

DLLs contain version info:

```
File Properties:
  File Version: 1.2.3.4
  Product Version: 1.2
  Copyright: (C) 2024
  Description: My Amazing Library
```

### Checking DLL Version in C#

```csharp
using System.Diagnostics;

var dllPath = "MyLibrary.dll";
var versionInfo = FileVersionInfo.GetVersionInfo(dllPath);

Console.WriteLine($"File Version: {versionInfo.FileVersion}");
Console.WriteLine($"Product Version: {versionInfo.ProductVersion}");
Console.WriteLine($"Description: {versionInfo.FileDescription}");
```

## 🔍 DLL Loading States

### 1. Not Loaded
```
DLL file exists on disk
Not in memory
No process uses it
```

### 2. Loaded
```
DLL mapped into process memory
DllMain(DLL_PROCESS_ATTACH) called
Ready for use
Reference count = 1
```

### 3. Shared
```
Multiple processes use same DLL
Code pages shared in physical memory
Each process has own data
Reference count > 1
```

### 4. Unloading
```
Last process stops using DLL
DllMain(DLL_PROCESS_DETACH) called
Memory unmapped
Reference count = 0
```

## 💡 Reference Counting

Windows tracks how many processes use a DLL:

```
Time T0:
  MyLibrary.dll RefCount = 0 (not loaded)

App1 starts (uses MyLibrary.dll):
  MyLibrary.dll RefCount = 1 (loaded)

App2 starts (uses MyLibrary.dll):
  MyLibrary.dll RefCount = 2 (shared)

App1 exits:
  MyLibrary.dll RefCount = 1 (still in memory)

App2 exits:
  MyLibrary.dll RefCount = 0 (unloaded from memory)
```

## 🛠️ Practical Example: Tracing DLL Load

Let's trace a real application:

### Code
```csharp
using System;
using MyLibrary;

class Program
{
    static void Main()
    {
        Console.WriteLine("App started");

        // This line triggers DLL load!
        var calc = new Calculator();

        Console.WriteLine("Calculator created");

        var result = calc.Add(5, 3);

        Console.WriteLine($"Result: {result}");
    }
}
```

### What Happens Behind the Scenes

```
1. Main() starts
   └→ Console.WriteLine runs
       └→ System.Console.dll already loaded (.NET)

2. new Calculator() executes
   └→ CLR checks: Is MyLibrary.dll loaded? NO
   └→ CLR calls Windows Loader
       └→ Loader searches for MyLibrary.dll
       └→ Found in app directory
       └→ Loader maps DLL into memory
       └→ Loader calls DllMain(DLL_PROCESS_ATTACH)
       └→ CLR initializes type metadata

3. calc.Add(5, 3) executes
   └→ MyLibrary.dll already loaded
   └→ Direct function call
   └→ Returns 8

4. Main() exits
   └→ .NET GC runs
   └→ Process terminates
   └→ Windows unloads all DLLs
   └→ DllMain(DLL_PROCESS_DETACH) called
```

## 📝 Summary

### What We Learned

✅ DLL files use PE (Portable Executable) format
✅ DLLs contain code, data, and resource sections
✅ Export table lists available functions
✅ Import table lists required DLLs
✅ Windows loads DLLs automatically or on-demand
✅ DllMain is the DLL entry point
✅ Code is shared, data is per-process
✅ Reference counting manages DLL lifetime

### Key Concepts

1. **PE Format** - Standard structure for EXE/DLL
2. **Loading** - Windows loader maps DLL into memory
3. **Exports** - Functions available to others
4. **Imports** - External dependencies
5. **Memory Mapping** - Efficient sharing
6. **Relocation** - Address adjustment when needed

## 🚀 Next Steps

In **Lesson 3: DLL vs Static Libraries**, we'll explore:
- Static vs dynamic linking comparison
- When to use each approach
- Pros and cons
- Performance implications
- Build configurations

## 💪 Practice Exercises

### Exercise 1: Examine a DLL
Use Windows tools to examine `kernel32.dll`:
1. Location: `C:\Windows\System32\kernel32.dll`
2. Right-click → Properties → Details tab
3. Note the version, size, and description

### Exercise 2: View Exports (if you have Visual Studio)
```batch
cd "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\[version]\bin\Hostx64\x64"
dumpbin /EXPORTS C:\Windows\System32\kernel32.dll
```
Count how many functions are exported!

### Exercise 3: Trace DLL Loading
Use Process Monitor (download from Microsoft):
1. Download Process Monitor
2. Filter by Process Name = "YourApp.exe"
3. Filter by Operation = "Load Image"
4. See all DLLs being loaded!

---

**Excellent!** 🎉 You now understand the internals of how DLLs work!

**Next Lesson:** [Lesson 3: DLL vs Static Libraries](Lesson03-DLL-vs-Static-Libraries.md)
