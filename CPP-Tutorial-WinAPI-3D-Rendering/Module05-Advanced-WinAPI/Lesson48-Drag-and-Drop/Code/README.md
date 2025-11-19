# Drag and Drop - Code Examples

This directory contains 20 complete Windows API applications demonstrating Drag and Drop.

## Overview

Lesson 48 covers drag and drop in the Windows API, providing practical examples of Windows application development.

## Compilation

### Using MinGW:
```bash
g++ -std=c++17 filename.cpp -o program.exe -lgdi32 -luser32 -mwindows
```

For GDI+ applications:
```bash
g++ -std=c++17 filename.cpp -o program.exe -lgdi32 -luser32 -lgdiplus -lole32 -mwindows
```

### Using Microsoft Visual C++:
```bash
cl filename.cpp /Fe:program.exe user32.lib gdi32.lib
```

For GDI+ applications:
```bash
cl filename.cpp /Fe:program.exe user32.lib gdi32.lib gdiplus.lib
```

## Execution

Run the compiled executable:
```bash
./program.exe
```

Or simply double-click the .exe file in Windows Explorer.

## Topics Covered

This lesson covers:
- Drag and Drop concepts and implementation
- Windows API programming techniques
- Message handling and window procedures
- Graphics and user interface design
- Event-driven programming patterns

## Learning Objectives

After studying these examples, you will:
- Understand Windows application architecture
- Implement drag and drop in your Windows programs
- Handle Windows messages effectively
- Create professional Windows applications
- Use Windows API functions correctly

## File Organization

Each file is a complete, runnable Windows application that demonstrates specific aspects of drag and drop:
- **Complete programs**: Include WinMain, message loops, and window procedures
- **Production-ready**: Follow Windows programming best practices
- **Well-documented**: Detailed comments explaining WinAPI calls
- **Incremental complexity**: From basic to advanced examples

## Platform Requirements

- **OS**: Windows 7 or later
- **Compiler**: MinGW-w64 or Microsoft Visual C++
- **Libraries**: User32.lib, GDI32.lib (GDIPlus.lib for advanced graphics)

## Additional Resources

For more information, refer to:
- Microsoft Windows API documentation
- MSDN Windows Development guides
- Lesson materials in the parent directory

---
**Note**: These are Windows-specific applications and require a Windows environment to compile and run.
