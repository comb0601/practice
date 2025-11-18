# Lesson 03: Task Parallel Library (TPL)

## Overview
This lesson demonstrates the Task Parallel Library (TPL), PLINQ, and TPL Dataflow for building high-performance parallel applications in WPF.

## Learning Objectives
- Master Parallel.For and Parallel.ForEach for data parallelism
- Leverage PLINQ for declarative parallel queries
- Build processing pipelines with TPL Dataflow
- Coordinate complex task workflows

## Key Concepts

### 1. Parallel Loops
- **Parallel.For**: Parallelize for loops across multiple cores
- **Parallel.ForEach**: Process collections in parallel
- **ParallelOptions**: Configure max degree of parallelism
- **Cancellation**: Stop parallel operations gracefully

### 2. PLINQ (Parallel LINQ)
- **AsParallel()**: Convert LINQ queries to parallel execution
- **AsOrdered()**: Preserve order in parallel queries
- **WithDegreeOfParallelism()**: Control parallel execution
- **Aggregations**: Parallel sum, average, grouping

### 3. TPL Dataflow
- **ActionBlock**: Process items with actions
- **TransformBlock**: Transform data through pipeline
- **BufferBlock**: Queue data between blocks
- **Pipeline Patterns**: Build multi-stage data processors

### 4. Task Coordination
- **ContinueWith**: Chain tasks together
- **Task.WhenAll/WhenAny**: Coordinate multiple tasks
- **Barrier**: Synchronize parallel operations
- **TaskFactory**: Create tasks with custom options

## Code Structure

```
TaskParallelLibrary/
├── TaskParallelLibrary.csproj    # .NET 9.0 with Dataflow package
├── App.xaml                      # Application resources
├── App.xaml.cs                   # Exception handling
├── MainWindow.xaml               # Interactive UI with 4 tabs
└── MainWindow.xaml.cs            # TPL implementations
```

## Features

### Tab 1: Parallel Loops
- **Performance Comparison**: Sequential vs Parallel.For
- **Parallel.ForEach**: Process image collections
- **Configurable Parallelism**: Adjust max degree
- **Cancellation**: Stop parallel operations
- **Real-time Progress**: Track parallel execution

### Tab 2: PLINQ
- **Basic PLINQ**: AsParallel() demonstrations
- **Query Operations**: Filter, map, aggregation, grouping
- **Order Preservation**: AsOrdered() comparison
- **Performance Metrics**: Compare LINQ vs PLINQ speedup
- **Configuration**: Degree of parallelism tuning

### Tab 3: TPL Dataflow
- **Basic Blocks**: ActionBlock, TransformBlock, BufferBlock
- **Pipeline Processing**: Multi-stage data pipeline
- **Advanced Blocks**: BroadcastBlock, BatchBlock, JoinBlock
- **Visual Progress**: Track items through pipeline stages
- **Real-time Updates**: See data flow in action

### Tab 4: Task Coordination
- **Task Continuations**: Simple and conditional
- **Barrier Pattern**: Synchronize parallel phases
- **TaskFactory Options**: LongRunning, AttachedToParent
- **Performance Comparison**: Compare all parallel patterns

## Building and Running

### Prerequisites
- .NET 9.0 SDK or later
- Windows OS
- System.Threading.Tasks.Dataflow package (auto-installed)

### Build
```bash
cd Module04-UIThreadOptimization/Lesson03-TaskParallelLibrary
dotnet build
```

### Run
```bash
dotnet run
```

## Interactive Demonstrations

### 1. Parallel Loops
1. Adjust iteration count slider (100K - 10M)
2. Click "Sequential For" - see baseline time
3. Click "Parallel.For" - observe speedup
4. Click "Compare Both" - see detailed comparison
5. Adjust "Max Degree of Parallelism" slider
6. Test cancellation with long-running operations

### 2. PLINQ
1. Click "LINQ (Sequential)" - baseline performance
2. Click "PLINQ (Parallel)" - see parallel speedup
3. Click "Compare Performance" - detailed metrics
4. Try "Filter & Map", "Aggregation", "Grouping"
5. Toggle "Preserve Order" - see performance impact
6. Adjust degree of parallelism

### 3. TPL Dataflow
1. Click basic block demos - see simple patterns
2. Adjust "Items to process" slider
3. Start pipeline - watch multi-stage processing
4. Observe parallel stages working simultaneously
5. Try advanced blocks - broadcast, batch, join

### 4. Task Coordination
1. Test simple continuations
2. Run multiple continuations
3. Try conditional continuations
4. Execute barrier pattern - see synchronization
5. Run full comparison - compare all methods

## Best Practices Demonstrated

### ✅ DO
- Use Parallel.For/ForEach for CPU-intensive operations
- Use PLINQ for declarative parallel data processing
- Use TPL Dataflow for complex processing pipelines
- Configure MaxDegreeOfParallelism appropriately
- Support cancellation for long operations
- Use Interlocked for thread-safe counters
- Batch UI updates from parallel operations

### ❌ DON'T
- Use parallel loops for I/O-bound operations (use async instead)
- Parallelize very small workloads (overhead exceeds benefit)
- Access UI elements directly from parallel tasks
- Ignore exceptions in parallel code
- Over-subscribe cores (too much parallelism)
- Use locks in tight parallel loops

## Performance Tips

### 1. When to Use Parallel Loops
```csharp
// GOOD: CPU-intensive work
Parallel.For(0, 1000000, i => {
    var result = ComplexCalculation(i);
});

// BAD: I/O-bound work
Parallel.For(0, 100, async i => {
    await File.ReadAllTextAsync($"file{i}.txt"); // Use async instead!
});
```

### 2. PLINQ Performance
```csharp
// GOOD: Preserve order only when needed
var result = data.AsParallel()
    .Where(x => Filter(x))
    .Select(x => Transform(x));

// SLOWER: Unnecessary ordering
var result = data.AsParallel()
    .AsOrdered()  // Only if order matters!
    .Where(x => Filter(x));
```

### 3. Degree of Parallelism
```csharp
// GOOD: Let TPL decide (usually best)
Parallel.For(0, count, i => Work(i));

// GOOD: Limit for I/O-bound scenarios
var options = new ParallelOptions { MaxDegreeOfParallelism = 4 };
Parallel.For(0, count, options, i => Work(i));

// BAD: Too much parallelism
var options = new ParallelOptions { MaxDegreeOfParallelism = 1000 };
```

### 4. Dataflow Pipeline Design
```csharp
// GOOD: Configure each block appropriately
var downloadBlock = new TransformBlock<int, Data>(
    Download,
    new ExecutionDataflowBlockOptions { MaxDegreeOfParallelism = 10 }
);

var processBlock = new TransformBlock<Data, Result>(
    Process,
    new ExecutionDataflowBlockOptions { MaxDegreeOfParallelism = 4 }
);
```

## Common Pitfalls

### Shared State
```csharp
// BAD: Race condition
int counter = 0;
Parallel.For(0, 1000, i => counter++);

// GOOD: Thread-safe increment
int counter = 0;
Parallel.For(0, 1000, i => Interlocked.Increment(ref counter));
```

### UI Updates
```csharp
// BAD: Cross-thread exception
Parallel.ForEach(items, item => {
    textBlock.Text = item.ToString(); // CRASH!
});

// GOOD: Use Dispatcher
Parallel.ForEach(items, item => {
    Dispatcher.Invoke(() => textBlock.Text = item.ToString());
});
```

### Over-Parallelization
```csharp
// BAD: Too fine-grained
Parallel.For(0, 100, i => {
    DoSimpleWork(i); // Overhead > benefit
});

// GOOD: Only parallelize significant work
if (count > 1000 && IsComplexWork())
    Parallel.For(0, count, i => DoWork(i));
else
    for (int i = 0; i < count; i++) DoWork(i);
```

## Performance Comparison

### Expected Speedups (4-core CPU)
- **Parallel.For**: 2.5-3.5x faster for CPU-intensive work
- **PLINQ**: 2-3x faster for LINQ queries
- **TPL Dataflow**: Variable (depends on pipeline design)

### Efficiency Factors
- **CPU-bound work**: Best parallelization efficiency
- **Memory-bound work**: Lower efficiency (memory bandwidth limit)
- **I/O-bound work**: Use async/await instead
- **Small workloads**: Overhead may exceed benefit

## TPL Dataflow Patterns

### Simple Pipeline
```csharp
var transform = new TransformBlock<Input, Output>(Process);
var action = new ActionBlock<Output>(Save);
transform.LinkTo(action, new DataflowLinkOptions { PropagateCompletion = true });
```

### Broadcast Pattern
```csharp
var broadcast = new BroadcastBlock<Data>(x => x);
broadcast.LinkTo(target1);
broadcast.LinkTo(target2);
broadcast.LinkTo(target3);
```

### Batch Processing
```csharp
var batch = new BatchBlock<Item>(batchSize: 10);
var process = new ActionBlock<Item[]>(ProcessBatch);
batch.LinkTo(process);
```

## Key Takeaways

1. **Parallel.For/ForEach** provide easy data parallelism for CPU-intensive work
2. **PLINQ** enables declarative parallel queries with minimal code changes
3. **TPL Dataflow** excels at building complex processing pipelines
4. **Proper configuration** is critical for optimal performance
5. **Thread-safe operations** prevent race conditions and bugs
6. **Cancellation support** provides responsive user experience
7. **Batch UI updates** from parallel operations for better performance

## When to Use What

### Parallel.For/ForEach
- CPU-intensive independent iterations
- Simple parallel loops
- Known iteration count

### PLINQ
- Complex LINQ queries on large datasets
- Declarative data processing
- Aggregations and grouping

### TPL Dataflow
- Multi-stage data processing
- Producer-consumer patterns
- Complex async workflows
- Backpressure handling

### Regular async/await
- I/O-bound operations
- Network calls
- File I/O
- Database queries

## Troubleshooting

### Issue: Poor Parallel Performance
**Problem**: Parallel code slower than sequential
**Solutions**:
- Check if workload is large enough to parallelize
- Verify CPU-bound (not I/O-bound) operation
- Reduce degree of parallelism
- Minimize lock contention

### Issue: Race Conditions
**Problem**: Incorrect results with parallel code
**Solutions**:
- Use Interlocked for simple counters
- Use concurrent collections
- Add locks where necessary
- Avoid shared mutable state

### Issue: UI Freezing
**Problem**: UI still freezes during parallel operations
**Solutions**:
- Ensure parallel work runs on background threads
- Batch UI updates
- Use Dispatcher.InvokeAsync with lower priority

## Additional Resources

- [Parallel Programming in .NET](https://docs.microsoft.com/en-us/dotnet/standard/parallel-programming/)
- [PLINQ (Parallel LINQ)](https://docs.microsoft.com/en-us/dotnet/standard/parallel-programming/introduction-to-plinq)
- [TPL Dataflow Library](https://docs.microsoft.com/en-us/dotnet/standard/parallel-programming/dataflow-task-parallel-library)
- [Patterns for Parallel Programming](https://www.microsoft.com/en-us/download/details.aspx?id=19222)

## Next Steps
- Proceed to Lesson 04: UI Virtualization for large data sets
- Review Lesson 01: Async Patterns for I/O-bound operations
- Study Lesson 02: Background Workers for thread management
