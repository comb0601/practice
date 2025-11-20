using System.Diagnostics;
using VPP.Core.Interfaces;
using VPP.Core.Attributes;
using System.Reflection;

namespace VPP.Core.Models;

public abstract class NodeBase : INode
{
    public string Id { get; } = Guid.NewGuid().ToString();
    public virtual string Name => GetType().GetCustomAttribute<NodeInfoAttribute>()?.Name ?? GetType().Name;
    public virtual string Category => GetType().GetCustomAttribute<NodeInfoAttribute>()?.Category ?? "General";
    public virtual string Description => GetType().GetCustomAttribute<NodeInfoAttribute>()?.Description ?? "";

    private readonly List<Port> _inputPorts = new();
    private readonly List<Port> _outputPorts = new();

    public IReadOnlyList<IPort> InputPorts => _inputPorts;
    public IReadOnlyList<IPort> OutputPorts => _outputPorts;

    public double X { get; set; }
    public double Y { get; set; }

    protected Port AddInput<T>(string name, bool required = true, T? defaultValue = default)
    {
        var port = new Port(name, typeof(T), PortDirection.Input, required, defaultValue);
        _inputPorts.Add(port);
        return port;
    }

    protected Port AddOutput<T>(string name)
    {
        var port = new Port(name, typeof(T), PortDirection.Output, false, default);
        _outputPorts.Add(port);
        return port;
    }

    protected T? GetInput<T>(string name) =>
        _inputPorts.FirstOrDefault(p => p.Name == name)?.Value is T val ? val : default;

    protected void SetOutput<T>(string name, T value)
    {
        var port = _outputPorts.FirstOrDefault(p => p.Name == name);
        if (port != null) port.Value = value;
    }

    public async Task<NodeResult> ExecuteAsync(CancellationToken cancellationToken = default)
    {
        var sw = Stopwatch.StartNew();
        try
        {
            if (!Validate(out var errors))
                return NodeResult.Fail(string.Join("; ", errors));

            await ExecuteCoreAsync(cancellationToken);

            var outputs = _outputPorts.ToDictionary(p => p.Name, p => p.Value);
            return new NodeResult
            {
                Success = true,
                ExecutionTime = sw.Elapsed,
                OutputValues = outputs
            };
        }
        catch (Exception ex)
        {
            return new NodeResult
            {
                Success = false,
                ErrorMessage = ex.Message,
                ExecutionTime = sw.Elapsed
            };
        }
    }

    protected abstract Task ExecuteCoreAsync(CancellationToken cancellationToken);

    public virtual bool Validate(out string[] errors)
    {
        var errs = new List<string>();
        foreach (var port in _inputPorts.Where(p => p.IsRequired && p.Value == null && !p.IsConnected))
            errs.Add($"Required input '{port.Name}' is not connected");
        errors = errs.ToArray();
        return errs.Count == 0;
    }

    public void Reset()
    {
        foreach (var port in _outputPorts) port.Value = null;
    }
}

public class Port : IPort
{
    public string Id { get; } = Guid.NewGuid().ToString();
    public string Name { get; }
    public Type DataType { get; }
    public PortDirection Direction { get; }
    public bool IsConnected { get; set; }
    public object? Value { get; set; }
    public object? DefaultValue { get; }
    public bool IsRequired { get; }

    public Port(string name, Type dataType, PortDirection direction, bool required, object? defaultValue)
    {
        Name = name;
        DataType = dataType;
        Direction = direction;
        IsRequired = required;
        DefaultValue = defaultValue;
        Value = defaultValue;
    }
}
