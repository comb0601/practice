# Lesson 03: Performance Counters

## Overview
This lesson demonstrates how to use Windows Performance Counters in WPF applications for monitoring both system-level and custom application metrics. Performance counters provide real-time insight into application and system performance.

## Learning Objectives
- Understand Windows Performance Counter architecture
- Monitor built-in system performance counters
- Create and manage custom performance counters
- Implement real-time performance monitoring in WPF
- Learn different performance counter types and their use cases

## Application Features

### System Performance Monitoring
The application monitors several key system performance counters:

1. **CPU Usage**
   - Counter: `Processor \ % Processor Time (_Total)`
   - Type: `PerformanceCounterType.RawFraction`
   - Shows overall CPU utilization across all cores

2. **Available Memory**
   - Counter: `Memory \ Available MBytes`
   - Type: `PerformanceCounterType.NumberOfItems32`
   - Displays available physical memory in megabytes

3. **Disk Activity**
   - Counter: `PhysicalDisk(_Total) \ % Disk Time`
   - Type: `PerformanceCounterType.RawFraction`
   - Measures disk activity percentage

4. **Process Metrics**
   - Working Set: Memory used by the current process
   - Private Bytes: Private memory allocation
   - Thread Count: Number of active threads

### Custom Performance Counters
The application demonstrates creating custom application-specific counters:

1. **Operations/sec**
   - Type: `RateOfCountsPerSecond32`
   - Tracks the rate of operations processed per second
   - Automatically calculates the rate

2. **Total Operations**
   - Type: `NumberOfItems64`
   - Cumulative count of all operations
   - Supports large numbers (64-bit)

3. **Average Operation Time**
   - Type: `AverageTimer32` with `AverageBase`
   - Calculates average time per operation
   - Requires a base counter for calculation

4. **Active Connections**
   - Type: `NumberOfItems32`
   - Tracks current number of active connections
   - Simple gauge counter

## Performance Counter Types

### Common Counter Types

| Type | Description | Use Case |
|------|-------------|----------|
| `NumberOfItems32` | Simple 32-bit counter | Current count values |
| `NumberOfItems64` | Simple 64-bit counter | Large count values |
| `RateOfCountsPerSecond32` | Rate calculation | Operations per second |
| `AverageTimer32` | Average time calculation | Average operation duration |
| `AverageBase` | Base for average counters | Required for average calculations |
| `CounterTimer` | Percentage time | Time-based percentages |
| `RawFraction` | Simple fraction | CPU usage, etc. |

## Prerequisites

### Administrator Privileges
Creating and deleting custom performance counters requires **Administrator privileges**. The application includes an app.manifest that requests elevation.

### Required NuGet Packages
- `System.Diagnostics.PerformanceCounter` (v9.0.0)

## How to Use

### Running the Application

1. **Run as Administrator**
   ```bash
   # Build and run with administrator privileges
   dotnet build
   # Right-click the executable and select "Run as Administrator"
   ```

2. **Start Monitoring**
   - Click "Start Monitoring" to begin real-time counter updates
   - System counters update every second
   - Monitor CPU, memory, disk, and process metrics

### Creating Custom Counters

1. **Create Counter Category**
   - Click "Create Custom Counters"
   - A new performance counter category "WPF Learning App" will be created
   - All custom counters are registered with Windows

2. **View in Performance Monitor**
   ```bash
   # Open Windows Performance Monitor
   perfmon.exe
   ```
   - Add counters from the "WPF Learning App" category
   - Monitor custom metrics alongside system counters

### Simulating Activity

1. **Simulate Operations**
   - Click "Simulate Operation" to trigger a simulated operation
   - Random duration between 10-500ms
   - Updates Operations/sec and Average Time counters

2. **Manage Connections**
   - Click "Add Connection" to increment active connections
   - Click "Remove Connection" to decrement
   - See real-time updates in Performance Monitor

### Cleanup

1. **Delete Custom Counters**
   - Click "Delete Custom Counters" when done
   - Removes the entire counter category
   - Requires administrator privileges

## Key Concepts

### Counter Initialization
```csharp
// System counter (read-only)
var cpuCounter = new PerformanceCounter(
    "Processor",           // Category
    "% Processor Time",    // Counter name
    "_Total"              // Instance name
);

// Custom counter (read-write)
var customCounter = new PerformanceCounter(
    "MyCategory",
    "MyCounter",
    false                 // Read-write mode
);
```

### Creating Counter Categories
```csharp
var counters = new CounterCreationDataCollection();

// Define counter
var counter = new CounterCreationData
{
    CounterName = "Operations/sec",
    CounterHelp = "Number of operations per second",
    CounterType = PerformanceCounterType.RateOfCountsPerSecond32
};
counters.Add(counter);

// Create category
PerformanceCounterCategory.Create(
    "MyCategory",
    "Category description",
    PerformanceCounterCategoryType.SingleInstance,
    counters
);
```

### Reading Counter Values
```csharp
// First read initializes the counter
cpuCounter.NextValue();

// Subsequent reads return actual values
float cpuUsage = cpuCounter.NextValue();

// Or read raw value directly
long rawValue = customCounter.RawValue;
```

### Writing Counter Values
```csharp
// Increment by 1
customCounter.Increment();

// Increment by specific amount
customCounter.IncrementBy(100);

// Set specific value
customCounter.RawValue = 42;
```

## Best Practices

### 1. Counter Lifecycle
- Initialize counters once at application startup
- Dispose counters properly when done
- Don't create/delete counters frequently

### 2. Reading Performance
- Read counters at appropriate intervals (1-5 seconds typical)
- First read initializes; second read provides value
- Use `NextValue()` for calculated values
- Use `RawValue` for simple counters

### 3. Security Considerations
- Creating counters requires administrator privileges
- Reading most counters doesn't require elevation
- Some system counters may require specific permissions
- Handle `UnauthorizedAccessException` gracefully

### 4. Error Handling
- Counter categories may not exist
- Counters may not be available on all systems
- Handle missing performance counter data gracefully
- Provide user feedback for permission issues

### 5. Performance Impact
- Reading counters has minimal overhead
- Update intervals should balance freshness vs. performance
- Batch counter reads when possible
- Dispose counters to release resources

## Common Issues and Solutions

### Issue: Access Denied
**Problem**: Cannot create or delete counters
**Solution**: Run application as Administrator

### Issue: Counter Not Found
**Problem**: Counter doesn't exist or is misspelled
**Solution**: Check counter name, category, and instance with Performance Monitor

### Issue: First Read Returns Zero
**Problem**: First call to `NextValue()` returns 0
**Solution**: This is expected - first read initializes the counter

### Issue: Negative or Invalid Values
**Problem**: Counter shows unexpected values
**Solution**: Ensure proper counter type and calculation method

## Integration with Other Tools

### Performance Monitor (perfmon.exe)
- View custom counters in real-time graphs
- Create custom counter sets
- Log counter data to files
- Compare multiple counters

### PowerShell
```powershell
# List counter categories
Get-Counter -ListSet *

# Get specific counter
Get-Counter "\WPF Learning App\Operations/sec"

# Continuous monitoring
Get-Counter "\WPF Learning App\*" -Continuous
```

### PerfView
- Collect performance counter data
- Analyze counter trends over time
- Export counter data for analysis

## Additional Resources

### Microsoft Documentation
- [Performance Counters Overview](https://docs.microsoft.com/en-us/dotnet/api/system.diagnostics.performancecounter)
- [PerformanceCounterType Enumeration](https://docs.microsoft.com/en-us/dotnet/api/system.diagnostics.performancecountertype)
- [Windows Performance Counters](https://docs.microsoft.com/en-us/windows/win32/perfctrs/performance-counters-portal)

### Related Lessons
- **Lesson 01**: Profiling Tools - Using profilers to identify bottlenecks
- **Lesson 02**: Memory Profiling - Analyzing memory usage patterns
- **Lesson 04**: ETW Tracing - Event-based performance monitoring

## Exercises

1. **Create Custom Counters**
   - Add a new counter type (e.g., `PercentageTimer`)
   - Implement business logic specific counters
   - Export counter data to CSV

2. **Remote Monitoring**
   - Modify code to read counters from remote machines
   - Create a dashboard for multiple servers
   - Implement alerting for threshold violations

3. **Historical Analysis**
   - Log counter values to database
   - Create trend analysis reports
   - Implement performance baselines

4. **Advanced Scenarios**
   - Multi-instance counters for multiple app instances
   - Category-level performance monitoring
   - Integration with Application Insights

## Summary

Performance Counters provide a standardized way to monitor application and system performance in Windows. This lesson demonstrated:

- Reading system performance counters for CPU, memory, and disk
- Creating custom application-specific counters
- Different counter types and their appropriate use cases
- Real-time monitoring and visualization
- Integration with Windows Performance Monitor

Performance counters are essential for:
- Production monitoring
- Performance troubleshooting
- Capacity planning
- SLA compliance
- Long-term trend analysis

## Next Steps
Proceed to **Lesson 04: ETW Tracing** to learn about Event Tracing for Windows, a more flexible and detailed performance monitoring approach.
