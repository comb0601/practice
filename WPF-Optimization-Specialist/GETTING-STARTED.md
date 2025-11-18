# Getting Started - WPF Optimization Specialist Course

Welcome to the WPF Optimization Specialist course! This guide will help you prepare for and navigate through the 300-hour specialist training program.

## Before You Begin

### Prerequisites Check

Before starting this course, ensure you meet these prerequisites:

#### Technical Knowledge
- [ ] **WPF Fundamentals** - Strong understanding of WPF architecture, controls, and XAML
- [ ] **C# Proficiency** - Advanced C# knowledge (generics, delegates, events, LINQ)
- [ ] **MVVM Pattern** - Experience implementing MVVM in production applications
- [ ] **Data Binding** - Deep understanding of WPF data binding
- [ ] **Async/Await** - Basic async programming knowledge
- [ ] **.NET Framework** - Understanding of CLR and .NET runtime concepts

#### Experience Level
- [ ] **2+ years** of WPF development experience
- [ ] Built at least one **production WPF application**
- [ ] Encountered **performance issues** in real applications
- [ ] Basic familiarity with **profiling tools**

### Recommended Preparation

If you need to strengthen any prerequisites:

1. **WPF Fundamentals:** Complete the [WPF-3D-ROI-Course](../WPF-3D-ROI-Course/) first
2. **C# Skills:** Review "C# in Depth" by Jon Skeet
3. **MVVM:** Study "Pro MVVM in WPF" by Gary Hall
4. **Async:** Read "Concurrency in C# Cookbook" by Stephen Cleary

## System Requirements

### Hardware
- **OS:** Windows 10 or Windows 11 (64-bit)
- **Processor:** Intel i5/AMD Ryzen 5 or better (for profiling)
- **RAM:** 16GB minimum (32GB recommended for profiling)
- **Storage:** 50GB free space (for Visual Studio and sample data)
- **GPU:** Dedicated GPU recommended (for Module 3)
- **Display:** 1920x1080 or higher resolution

### Software Requirements

#### Essential Tools
1. **Visual Studio 2022** (Professional or Enterprise recommended)
   - Workload: .NET desktop development
   - Component: .NET 9.0 SDK
   - Component: Performance profiling tools
   - Download: https://visualstudio.microsoft.com/

2. **.NET 9.0 SDK**
   - Included with Visual Studio 2022
   - Standalone: https://dotnet.microsoft.com/download

3. **Git** (for cloning repository and version control)
   - Download: https://git-scm.com/

#### Profiling Tools

**Free Tools:**
- [ ] **PerfView** - ETW-based performance analysis
  - Download: https://github.com/microsoft/perfview/releases

- [ ] **Visual Studio Performance Profiler** (included)

**Recommended Paid Tools (Trial Available):**
- [ ] **JetBrains dotTrace** - CPU and thread profiling
  - Trial: https://www.jetbrains.com/profiler/download/

- [ ] **JetBrains dotMemory** - Memory profiling
  - Trial: https://www.jetbrains.com/dotmemory/download/

**Optional Tools:**
- [ ] **NVIDIA Nsight** (for NVIDIA GPUs) - Graphics debugging
- [ ] **AMD Radeon GPU Profiler** (for AMD GPUs)
- [ ] **Windows Performance Toolkit** - Advanced diagnostics

## Installation Guide

### Step 1: Install Visual Studio 2022

```bash
# Download and install Visual Studio 2022
# Select workload: ".NET desktop development"
# Select: .NET 9.0 SDK
# Select: Performance profiling tools
```

### Step 2: Verify .NET 9.0 Installation

```bash
dotnet --version
# Should show: 9.0.x or higher
```

### Step 3: Install PerfView

```bash
# Download from GitHub releases
# Extract to C:\Tools\PerfView (or your preferred location)
# Add to PATH for easy access
```

### Step 4: Install dotTrace/dotMemory (Optional)

```bash
# Download JetBrains Toolbox
# Install dotTrace and dotMemory
# Start 30-day trial
```

### Step 5: Clone Course Repository

```bash
git clone <repository-url>
cd WPF-Optimization-Specialist
```

## Course Structure Overview

The course is organized into 6 modules with 22 lessons:

```
WPF-Optimization-Specialist/
├── Module01-PerformanceProfiling/    (60h)
│   ├── Lesson01-ProfilingTools/
│   ├── Lesson02-MemoryProfiling/
│   ├── Lesson03-PerformanceCounters/
│   └── Lesson04-ETWTracing/
├── Module02-MemoryOptimization/      (50h)
├── Module03-RenderingOptimization/   (60h)
├── Module04-UIThreadOptimization/    (40h)
├── Module05-DataBindingOptimization/ (40h)
└── Module06-AdvancedTechniques/      (50h)
```

## Learning Path Options

### Option 1: Sequential (Recommended for Most)
Follow modules 1-6 in order. Best for comprehensive understanding.

**Timeline:** 20-30 weeks at 10-15 hours/week

### Option 2: Focus-Based
Start with Module 1, then choose based on your needs:

- **Memory Issues?** → Module 1 → Module 2 → Module 4
- **Rendering Slow?** → Module 1 → Module 3 → Module 4
- **Data-Heavy App?** → Module 1 → Module 5 → Module 2

**Timeline:** 15-20 weeks at 15 hours/week

### Option 3: Intensive
Full-time focused study.

**Timeline:** 10-12 weeks at 25-30 hours/week

## How to Use Each Lesson

Every lesson follows this structure:

### 1. Read the Lesson README
- **Location:** `ModuleXX-Topic/LessonXX-Topic/README.md`
- **Contains:**
  - Learning objectives
  - Theory and concepts
  - Code explanations
  - Best practices
  - Common pitfalls

### 2. Review the Code
- **Location:** `ModuleXX-Topic/LessonXX-Topic/`
- **Files:**
  - `.csproj` - Project file
  - `App.xaml` - Application entry
  - `MainWindow.xaml` - UI definition
  - `MainWindow.xaml.cs` - Application logic
  - Additional helper classes

### 3. Build and Run
```bash
cd ModuleXX-Topic/LessonXX-Topic
dotnet build
dotnet run
```

### 4. Experiment
- Modify the code
- Add breakpoints
- Run profilers
- Measure performance
- Try variations

### 5. Complete Exercises
- Each lesson has 2-4 exercises
- Solutions provided (but try first!)
- Document your findings

### 6. Take Notes
- Keep a learning journal
- Document optimization patterns
- Track your progress

## Study Tips

### Time Management
- **Set a schedule:** Dedicate specific hours each week
- **Use focus blocks:** 2-hour deep work sessions
- **Take breaks:** Pomodoro technique (25min work, 5min break)
- **Review regularly:** Revisit previous modules

### Effective Learning
- **Hands-on first:** Run code before reading theory
- **Profile everything:** Measure before and after
- **Take notes:** Document patterns and insights
- **Build projects:** Apply to real scenarios
- **Ask questions:** Use forums and community

### Profiling Best Practices
- **Always use Release builds** when profiling
- **Use realistic data sets** (production-like volumes)
- **Establish baselines** before optimizing
- **Change one thing at a time**
- **Document all measurements**

## Module Completion Checklist

For each module, ensure you:

- [ ] Read all lesson READMEs thoroughly
- [ ] Built and ran all lesson applications
- [ ] Completed all hands-on exercises
- [ ] Practiced with profiling tools
- [ ] Took comprehensive notes
- [ ] Passed module quiz (80%+)
- [ ] Applied concepts to a personal project

## Assessment Information

### Module Quizzes
- **Format:** Multiple choice + practical questions
- **Duration:** 30-60 minutes per module
- **Passing Score:** 80%
- **Retakes:** Unlimited (but learn from mistakes!)

### Final Exam
- **Format:** Comprehensive exam covering all modules
- **Duration:** 90 minutes
- **Questions:** 50
- **Passing Score:** 85%

### Capstone Project
- **Duration:** 20 hours
- **Format:** Optimize a provided WPF application
- **Deliverables:**
  - Optimized application
  - Performance analysis report
  - Documentation of changes

## Getting Help

### Resources

1. **Course Materials**
   - Main README.md
   - Module READMEs
   - Lesson READMEs
   - Code comments

2. **Online Resources**
   - Microsoft Docs: https://docs.microsoft.com/wpf
   - WPF Performance Guide
   - .NET Blog: https://devblogs.microsoft.com/dotnet/

3. **Community**
   - Course discussion forums
   - Stack Overflow (tag: wpf, performance)
   - Reddit: r/dotnet, r/wpf

4. **Books** (Recommended Reading)
   - "Pro .NET Performance" - Sasha Goldshtein
   - "Writing High-Performance .NET Code" - Ben Watson
   - "CLR via C#" - Jeffrey Richter
   - "Concurrency in C# Cookbook" - Stephen Cleary

### Troubleshooting Common Issues

**Issue: Application won't build**
```bash
# Clean and rebuild
dotnet clean
dotnet restore
dotnet build
```

**Issue: Wrong .NET version**
```bash
# Check installed SDKs
dotnet --list-sdks

# Install .NET 9.0 if missing
```

**Issue: Profiler not working**
- Run Visual Studio as Administrator
- Check Windows Defender/Firewall
- Ensure Release build (not Debug)

## Recommended Study Schedule

### 15 Hours/Week Plan (20 weeks)

**Week 1-4: Module 1 (Performance Profiling)**
- Week 1: Lesson 1 (15h)
- Week 2: Lesson 2 (15h)
- Week 3: Lesson 3 (15h)
- Week 4: Lesson 4 + Quiz (15h)

**Week 5-7: Module 2 (Memory Optimization)**
- Week 5: Lessons 1-2 (15h)
- Week 6: Lessons 3-4 (15h)
- Week 7: Lesson 5 + Quiz (15h)

**Week 8-11: Module 3 (Rendering Optimization)**
- Week 8: Lessons 1-2 (15h)
- Week 9: Lesson 3 (15h)
- Week 10: Lesson 4 (15h)
- Week 11: Lesson 5 + Quiz (15h)

**Week 12-14: Module 4 (UI Thread & Async)**
- Week 12: Lessons 1-2 (15h)
- Week 13: Lessons 3-4 (15h)
- Week 14: Quiz + Review (15h)

**Week 15-17: Module 5 (Data Binding & MVVM)**
- Week 15: Lessons 1-2 (15h)
- Week 16: Lessons 3-4 (15h)
- Week 17: Quiz + Review (15h)

**Week 18-20: Module 6 (Advanced Techniques)**
- Week 18: Lessons 1-2 (15h)
- Week 19: Lesson 3 (15h)
- Week 20: Lesson 4 + Capstone Start (15h)

## Quick Start Checklist

Ready to begin? Complete this checklist:

### Prerequisites
- [ ] Windows 10/11 installed
- [ ] Visual Studio 2022 installed
- [ ] .NET 9.0 SDK verified (`dotnet --version`)
- [ ] PerfView installed
- [ ] Course repository cloned
- [ ] Prerequisites validated (WPF knowledge, etc.)

### First Steps
- [ ] Read main README.md
- [ ] Review course structure
- [ ] Choose learning path
- [ ] Set study schedule
- [ ] Join community forums

### Start Learning
- [ ] Navigate to Module 1, Lesson 1
- [ ] Read lesson README
- [ ] Build and run first application
- [ ] Complete first exercise

## Welcome Aboard!

You're about to embark on a comprehensive journey to become a WPF optimization specialist. This course will transform how you approach WPF performance.

### Remember:
- **Measure everything** - Data-driven optimization
- **Learn by doing** - Hands-on practice is key
- **Ask questions** - No question is too simple
- **Stay consistent** - Regular practice beats cramming
- **Apply knowledge** - Use on real projects

### Your First Task:
1. Verify all tools are installed
2. Navigate to `Module01-PerformanceProfiling/Lesson01-ProfilingTools/`
3. Read the README.md
4. Begin your optimization journey!

---

**Good luck, and enjoy becoming a WPF Optimization Specialist!**

Questions? Check the main README.md or reach out to the community.

Last Updated: 2025-11-18
