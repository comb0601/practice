# Lesson 97: Debugging Advanced Techniques

**Duration**: 6 hours
**Difficulty**: Advanced
**Prerequisites**: Lessons 1-96

## Table of Contents
1. GDB Advanced Usage
2. Memory Debugging
3. Performance Profiling
4. Core Dumps
5. Remote Debugging

---

## 1. GDB Commands

```bash
# Start with symbols
g++ -g -O0 program.cpp -o program
gdb ./program

# Common commands
(gdb) break main          # Set breakpoint
(gdb) run arg1 arg2       # Run with args
(gdb) next                # Step over
(gdb) step                # Step into
(gdb) continue            # Continue execution
(gdb) print variable      # Print variable
(gdb) backtrace           # Show call stack
(gdb) info locals         # Show local variables
(gdb) watch variable      # Set watchpoint
(gdb) condition 1 x==5    # Conditional breakpoint
```

---

## 2. Valgrind Memory Analysis

```bash
# Memory leak detection
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         ./program

# Thread analysis
valgrind --tool=helgrind ./program

# Cache profiling
valgrind --tool=cachegrind ./program
```

---

## 3. Address Sanitizer

```cpp
// Compile with sanitizer
g++ -fsanitize=address -g program.cpp -o program

// Run - it will detect:
// - Use after free
// - Heap buffer overflow
// - Stack buffer overflow
// - Memory leaks
./program
```

---

## 4. Performance Profiling

```bash
# gprof
g++ -pg program.cpp -o program
./program
gprof program gmon.out > analysis.txt

# perf (Linux)
perf record ./program
perf report
```

---

## Summary

✅ **GDB**: Interactive debugging
✅ **Valgrind**: Memory analysis
✅ **Sanitizers**: Runtime checks
✅ **Profiling**: Performance analysis
