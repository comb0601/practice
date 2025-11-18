# Lesson 04: ETW Tracing

## Overview
This lesson demonstrates how to implement Event Tracing for Windows (ETW) in WPF applications. ETW provides a highly efficient, kernel-level event tracing mechanism that enables detailed application instrumentation with minimal performance overhead.

## Learning Objectives
- Understand ETW architecture and components
- Create custom ETW providers using EventSource
- Implement various event types (Info, Warning, Error, Critical)
- Collect and analyze ETW traces
- Learn ETW best practices for production applications
- Understand event levels, tasks, and keywords

## Application Features

### Custom ETW Event Source
The application demonstrates a complete EventSource implementation with:

**Event Source Details:**
- Name: `WPF-Learning-App`
- Unique GUID for provider identification
- Self-describing event format
- Comprehensive event catalog

### Event Categories

#### 1. Informational Events
Events that track normal application flow:
- **Application Started/Stopped**: Lifecycle events
- **User Actions**: Track user interactions
- **Data Loading**: Monitor data operations
- **Configuration Changes**: Track settings modifications

#### 2. Warning Events
Events indicating potential issues:
- **Slow Operations**: Detect performance degradation
- **Resource Thresholds**: Monitor resource usage
- **Retry Attempts**: Track resilience patterns

#### 3. Error Events
Events indicating failures:
- **Operation Failures**: Track failed operations with error codes
- **Exceptions**: Log exception details
- **Critical Errors**: System-threatening errors

#### 4. Performance Events
Events for performance analysis:
- **Operation Start/Stop**: Paired events for duration measurement
- **Task Completion**: Track task execution metrics
- **Complex Operations**: Multi-phase operation tracking

## ETW Architecture

### Event Source
An `EventSource` is a provider that emits ETW events. Key characteristics:
- Singleton pattern recommended
- Unique name and GUID
- Defines event catalog
- Minimal runtime overhead when not enabled

### Event Levels
```
Critical (1)    - System-threatening errors
Error (2)       - Operation failures
Warning (3)     - Potential issues
Informational (4) - Normal flow events
Verbose (5)     - Detailed diagnostic events
```

### Event Components
- **Event ID**: Unique identifier for event type
- **Level**: Severity/importance
- **Keywords**: Categorization for filtering
- **Tasks**: Logical grouping of related events
- **Opcodes**: Event relationship (Start, Stop, Info, etc.)
- **Message**: Event description template
- **Payload**: Event-specific data

## Prerequisites

### Required NuGet Packages
```xml
<PackageReference Include="Microsoft.Diagnostics.Tracing.TraceEvent" Version="3.1.18" />
<PackageReference Include="System.Diagnostics.DiagnosticSource" Version="9.0.0" />
```

### Collection Tools
1. **PerfView** (Recommended)
   - Download: https://github.com/microsoft/perfview/releases
   - Free, powerful, Microsoft-supported

2. **Windows Performance Recorder (WPR)**
   - Built into Windows
   - Part of Windows Performance Toolkit

3. **Logman** (Command-line)
   - Built into Windows
   - Scriptable trace collection

## How to Use

### Running the Application

```bash
# Build the application
dotnet build

# Run the application
dotnet run
```

### Generating Events

1. **Generate Informational Events**
   - Click "Log User Action" to track user interactions
   - Click "Log Data Loaded" to simulate data operations
   - Click "Log Configuration Change" to track settings changes

2. **Generate Warning Events**
   - Click "Log Slow Operation" to detect performance issues
   - Click "Log Resource Warning" to track resource usage
   - Click "Log Retry Attempt" to monitor resilience

3. **Generate Error Events**
   - Click "Log Operation Failed" to track failures
   - Click "Log Exception" to log exception details
   - Click "Log Critical Error" for critical issues

4. **Generate Performance Events**
   - Click "Start/Stop Operation" for duration tracking
   - Click "Log with Duration" for completed tasks
   - Click "Simulate Complex Operation" for multi-phase tracking

### Collecting ETW Traces

#### Method 1: PerfView (Recommended)

```bash
# Start collection for your provider
PerfView.exe collect -OnlyProviders=*WPF-Learning-App

# Perform actions in the application

# Press 'S' in PerfView to stop collection

# Open the generated .etl file to analyze
```

**PerfView Commands:**
```bash
# Collect only specific provider
PerfView collect -OnlyProviders=*WPF-Learning-App

# Collect with specific keywords
PerfView collect -OnlyProviders=*WPF-Learning-App:@StacksEnabled=true

# Collect to specific file
PerfView collect -OnlyProviders=*WPF-Learning-App -DataFile:mytrace.etl
```

#### Method 2: Windows Performance Recorder (WPR)

```bash
# Start recording
wpr -start GeneralProfile -recordTempTo C:\temp

# Perform actions in the application

# Stop recording
wpr -stop mytrace.etl

# Open with Windows Performance Analyzer
wpa mytrace.etl
```

#### Method 3: Logman (Command-line)

```bash
# Get the provider GUID from the application UI
# Start trace session
logman start mysession -p {GUID} -o trace.etl -ets

# Perform actions in the application

# Stop trace session
logman stop mysession -ets

# View with tracerpt or PerfView
```

**Advanced Logman:**
```bash
# With specific event level
logman start mysession -p {GUID} 0 4 -o trace.etl -ets
# Level: 4 = Informational and above

# With keywords
logman start mysession -p {GUID} 0x0001 0 -o trace.etl -ets
# Keywords: 0x0001 = Performance
```

### Analyzing Traces

#### PerfView Analysis

1. **Open Trace File**
   - Double-click the .etl file
   - Or: PerfView.exe mytrace.etl

2. **View Events**
   - Expand "Events" node
   - Find "WPF-Learning-App" provider
   - Double-click to see event list

3. **Filter Events**
   - Use event level filter
   - Filter by event ID
   - Search in event text

4. **Export Data**
   - Right-click event list
   - Export to CSV/Excel
   - Analyze in external tools

#### Windows Performance Analyzer (WPA)

1. **Load Trace**
   ```bash
   wpa mytrace.etl
   ```

2. **Add Graph**
   - Drag "Generic Events" to analysis view
   - Filter by provider name

3. **Analyze Patterns**
   - View event timeline
   - Correlate with other system events
   - Create custom views

## Key Concepts

### Creating an EventSource

```csharp
[EventSource(Name = "MyApp-Component")]
public sealed class MyEventSource : EventSource
{
    public static readonly MyEventSource Instance = new();

    private MyEventSource() : base(EventSourceSettings.EtwSelfDescribingEventFormat)
    {
    }

    [Event(1, Level = EventLevel.Informational, Message = "User {0} logged in")]
    public void UserLoggedIn(string userName)
    {
        if (IsEnabled())
        {
            WriteEvent(1, userName);
        }
    }
}
```

### Event Attributes

```csharp
[Event(
    eventId,                          // Unique ID
    Level = EventLevel.Informational, // Severity level
    Message = "Template {0}",         // Message template
    Opcode = EventOpcode.Start,       // Event relationship
    Task = Tasks.MyTask,              // Logical grouping
    Keywords = Keywords.Performance   // Filtering category
)]
```

### Start/Stop Event Pairs

```csharp
[Event(1, Opcode = EventOpcode.Start, Task = Tasks.Operation)]
public void OperationStarted(int id, string name)
{
    WriteEvent(1, id, name);
}

[Event(2, Opcode = EventOpcode.Stop, Task = Tasks.Operation)]
public void OperationCompleted(int id, string name, bool success)
{
    WriteEvent(2, id, name, success);
}
```

PerfView and WPA automatically calculate duration between paired events.

### Custom Keywords

```csharp
public static class Keywords
{
    public const EventKeywords Performance = (EventKeywords)0x0001;
    public const EventKeywords Diagnostic = (EventKeywords)0x0002;
    public const EventKeywords UserAction = (EventKeywords)0x0004;
}

[Event(1, Keywords = Keywords.Performance)]
public void PerformanceEvent() { }
```

Filter collection by keywords:
```bash
# Collect only Performance events
logman start mysession -p {GUID} 0x0001 0 -o trace.etl -ets
```

### Custom Tasks

```csharp
public static class Tasks
{
    public const EventTask Database = (EventTask)1;
    public const EventTask UI = (EventTask)2;
    public const EventTask Network = (EventTask)3;
}

[Event(1, Task = Tasks.Database)]
public void DatabaseEvent() { }
```

## Best Practices

### 1. Event Source Design

**Do:**
- Use singleton pattern for EventSource
- Define unique, sequential event IDs
- Use descriptive event names and messages
- Group related events with Tasks
- Use Keywords for filtering

**Don't:**
- Create multiple instances
- Reuse event IDs
- Use complex types in payloads
- Log sensitive data (passwords, PII)

### 2. Event Levels

Use appropriate levels:
- **Critical**: Application cannot continue
- **Error**: Operation failed, but app continues
- **Warning**: Potential issue, degraded performance
- **Informational**: Normal flow, milestones
- **Verbose**: Detailed diagnostic data

### 3. Performance Considerations

**Enable Checks:**
```csharp
if (IsEnabled(EventLevel.Verbose, EventKeywords.Performance))
{
    WriteEvent(eventId, data);
}
```

- Always check `IsEnabled()` before expensive operations
- ETW has minimal overhead when not collecting
- Avoid string formatting before `IsEnabled()` check
- Use value types in payloads when possible

### 4. Event Payload Design

**Supported Types:**
- Primitives: int, long, bool, double, etc.
- string
- Guid, DateTime, TimeSpan
- Enums

**Not Supported:**
- Custom classes/structs
- Collections (arrays, lists)
- Nullable types

**Workaround for Complex Data:**
```csharp
// Instead of passing an object
public void UserEvent(User user)
{
    WriteEvent(1, user.Name, user.Id, user.Role);
}
```

### 5. Production Usage

**Recommendations:**
- Always include ETW in applications
- Use standard event levels consistently
- Document event catalog
- Version your EventSource (use name versioning)
- Test trace collection before production

**Deployment:**
- No special deployment needed
- EventSource is always available
- Collection requires admin rights (typically)
- Zero overhead when not collecting

## Common Scenarios

### Application Performance Monitoring

```csharp
// Track operation duration
int opId = GetNextId();
eventSource.OperationStarted(opId, "ProcessData");

try
{
    ProcessData();
    eventSource.OperationCompleted(opId, "ProcessData", true);
}
catch (Exception ex)
{
    eventSource.OperationCompleted(opId, "ProcessData", false);
    eventSource.ExceptionOccurred(ex.GetType().Name, ex.Message, ex.StackTrace);
}
```

### User Behavior Tracking

```csharp
eventSource.UserActionPerformed(
    action: "ButtonClick",
    target: "SaveButton",
    userName: Environment.UserName
);
```

### Resource Monitoring

```csharp
if (memoryUsage > threshold)
{
    eventSource.ResourceThresholdExceeded(
        "Memory",
        memoryUsage,
        threshold
    );
}
```

### Multi-Phase Operations

```csharp
int opId = GetNextId();
eventSource.OperationStarted(opId, "ComplexTask");

eventSource.PhaseCompleted(opId, "LoadData");
// ... load data

eventSource.PhaseCompleted(opId, "ProcessData");
// ... process data

eventSource.PhaseCompleted(opId, "SaveResults");
// ... save results

eventSource.OperationCompleted(opId, "ComplexTask", true);
```

## Comparison: ETW vs Performance Counters

| Feature | ETW | Performance Counters |
|---------|-----|---------------------|
| **Overhead** | Minimal (when not collecting) | Moderate (always active) |
| **Flexibility** | High (structured events) | Low (numeric values only) |
| **Real-time** | Limited | Excellent |
| **Historical** | Excellent (trace files) | Limited |
| **Detail Level** | Very detailed | Summary metrics only |
| **Setup Required** | None | Category creation (admin) |
| **Production** | Ideal | Good for dashboards |
| **Analysis Tools** | PerfView, WPA | PerfMon, PowerShell |

**Use ETW when:**
- Detailed diagnostic data needed
- Investigating specific issues
- Performance analysis and profiling
- Structured event logging
- Minimal production overhead required

**Use Performance Counters when:**
- Real-time monitoring dashboards
- Simple numeric metrics
- Integration with existing monitoring
- Standard Windows monitoring tools

## Integration with Other Tools

### Application Insights
```csharp
// ETW events can feed into Application Insights
// Use EventSource -> DiagnosticSource -> AppInsights
```

### Elasticsearch/Kibana
```csharp
// Export ETW traces to Elasticsearch
// Use logstash or custom exporters
```

### Custom Dashboards
```csharp
// Real-time ETW consumption
// Use TraceEvent library for custom listeners
```

## Troubleshooting

### Issue: Events Not Appearing

**Possible Causes:**
1. Provider not enabled in collection
2. Event level filter too restrictive
3. Application not generating events

**Solutions:**
```bash
# List active sessions
logman query -ets

# Verify provider GUID
# Check application UI for correct GUID

# Collect at Verbose level
logman start mysession -p {GUID} 0 5 -o trace.etl -ets
```

### Issue: Too Many Events

**Solutions:**
1. Use event level filtering
2. Filter by keywords
3. Use more specific event generation
4. Increase buffer size

```bash
# Collect only Warning and above
logman start mysession -p {GUID} 0 3 -o trace.etl -ets
```

### Issue: Trace Files Too Large

**Solutions:**
1. Use circular logging
2. Filter at collection time
3. Limit collection duration
4. Use event keywords for filtering

```bash
# Circular buffer (100 MB)
logman start mysession -p {GUID} -o trace.etl -max 100 -mode circular -ets
```

## Additional Resources

### Microsoft Documentation
- [EventSource Class](https://docs.microsoft.com/en-us/dotnet/api/system.diagnostics.tracing.eventsource)
- [ETW Overview](https://docs.microsoft.com/en-us/windows/win32/etw/about-event-tracing)
- [PerfView Documentation](https://github.com/microsoft/perfview/blob/main/documentation/README.md)

### Tools
- **PerfView**: https://github.com/microsoft/perfview
- **Windows Performance Toolkit**: https://docs.microsoft.com/en-us/windows-hardware/test/wpt/
- **TraceEvent Library**: https://www.nuget.org/packages/Microsoft.Diagnostics.Tracing.TraceEvent

### Related Lessons
- **Lesson 01**: Profiling Tools - Using profilers for analysis
- **Lesson 02**: Memory Profiling - Memory-specific analysis
- **Lesson 03**: Performance Counters - Numeric performance metrics

## Exercises

1. **Add Custom Events**
   - Create events for your domain
   - Implement custom keywords
   - Define custom tasks

2. **Trace Collection**
   - Collect traces during various scenarios
   - Analyze patterns in PerfView
   - Create performance baselines

3. **Production Simulation**
   - Implement comprehensive instrumentation
   - Collect traces under load
   - Identify performance bottlenecks

4. **Custom Analysis**
   - Write custom trace analyzer using TraceEvent
   - Create real-time event listener
   - Build custom monitoring dashboard

## Summary

Event Tracing for Windows (ETW) provides:
- Highly efficient event logging mechanism
- Minimal overhead in production
- Detailed, structured event data
- Powerful analysis tools
- System-wide event correlation

This lesson demonstrated:
- Creating custom EventSource providers
- Implementing various event types and levels
- Collecting traces with multiple tools
- Analyzing performance and behavior patterns
- Best practices for production use

ETW is essential for:
- Production diagnostics
- Performance analysis
- Application monitoring
- Issue investigation
- Long-term trend analysis

## Next Steps

Continue to **Module 02** or explore advanced ETW topics:
- Activity IDs for distributed tracing
- Channel-based logging
- Manifest-based providers
- Custom trace consumers
- Integration with monitoring systems
