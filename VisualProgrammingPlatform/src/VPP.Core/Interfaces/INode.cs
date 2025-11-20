namespace VPP.Core.Interfaces;

public interface INode
{
    string Id { get; }
    string Name { get; }
    string Category { get; }
    string Description { get; }

    IReadOnlyList<IPort> InputPorts { get; }
    IReadOnlyList<IPort> OutputPorts { get; }

    Task<NodeResult> ExecuteAsync(CancellationToken cancellationToken = default);
    bool Validate(out string[] errors);
    void Reset();
}

public interface IPort
{
    string Id { get; }
    string Name { get; }
    Type DataType { get; }
    PortDirection Direction { get; }
    bool IsConnected { get; }
    object? Value { get; set; }
    object? DefaultValue { get; }
    bool IsRequired { get; }
}

public enum PortDirection
{
    Input,
    Output
}

public class NodeResult
{
    public bool Success { get; init; }
    public string? ErrorMessage { get; init; }
    public TimeSpan ExecutionTime { get; init; }
    public Dictionary<string, object?> OutputValues { get; init; } = new();

    public static NodeResult Ok(Dictionary<string, object?>? outputs = null) =>
        new() { Success = true, OutputValues = outputs ?? new() };

    public static NodeResult Fail(string error) =>
        new() { Success = false, ErrorMessage = error };
}
