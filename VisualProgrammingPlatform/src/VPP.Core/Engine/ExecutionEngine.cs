using VPP.Core.Interfaces;
using VPP.Core.Models;

namespace VPP.Core.Engine;

public class ExecutionEngine
{
    public event EventHandler<NodeExecutionEventArgs>? NodeExecuting;
    public event EventHandler<NodeExecutionEventArgs>? NodeExecuted;
    public event EventHandler<ExecutionCompletedEventArgs>? ExecutionCompleted;

    public async Task<ExecutionResult> ExecuteAsync(NodeGraph graph, CancellationToken cancellationToken = default)
    {
        var results = new Dictionary<string, NodeResult>();
        var executionOrder = graph.GetExecutionOrder().ToList();

        foreach (var node in executionOrder)
        {
            if (cancellationToken.IsCancellationRequested)
                break;

            // Transfer data through connections
            foreach (var conn in graph.Connections.Where(c => c.TargetNodeId == node.Id))
            {
                var sourceNode = graph.Nodes.First(n => n.Id == conn.SourceNodeId);
                var sourcePort = sourceNode.OutputPorts.First(p => p.Id == conn.SourcePortId);
                var targetPort = node.InputPorts.First(p => p.Id == conn.TargetPortId);
                targetPort.Value = sourcePort.Value;
            }

            NodeExecuting?.Invoke(this, new NodeExecutionEventArgs(node));
            var result = await node.ExecuteAsync(cancellationToken);
            results[node.Id] = result;
            NodeExecuted?.Invoke(this, new NodeExecutionEventArgs(node, result));

            if (!result.Success)
            {
                var execResult = new ExecutionResult { Success = false, NodeResults = results };
                ExecutionCompleted?.Invoke(this, new ExecutionCompletedEventArgs(execResult));
                return execResult;
            }
        }

        var finalResult = new ExecutionResult { Success = true, NodeResults = results };
        ExecutionCompleted?.Invoke(this, new ExecutionCompletedEventArgs(finalResult));
        return finalResult;
    }
}

public class ExecutionResult
{
    public bool Success { get; init; }
    public Dictionary<string, NodeResult> NodeResults { get; init; } = new();
}

public class NodeExecutionEventArgs : EventArgs
{
    public INode Node { get; }
    public NodeResult? Result { get; }
    public NodeExecutionEventArgs(INode node, NodeResult? result = null)
    {
        Node = node;
        Result = result;
    }
}

public class ExecutionCompletedEventArgs : EventArgs
{
    public ExecutionResult Result { get; }
    public ExecutionCompletedEventArgs(ExecutionResult result) => Result = result;
}
