using Newtonsoft.Json;
using VPP.Core.Models;
using VPP.Core.Interfaces;

namespace VPP.Core.Services;

public class GraphSerializer
{
    private readonly PluginService _pluginService;

    public GraphSerializer(PluginService pluginService)
    {
        _pluginService = pluginService;
    }

    public string Serialize(NodeGraph graph)
    {
        var data = new GraphData
        {
            Version = graph.Version,
            Name = graph.Name,
            Nodes = graph.Nodes.Select(n => new NodeData
            {
                Id = n.Id,
                Type = n.Name,
                X = (n as NodeBase)?.X ?? 0,
                Y = (n as NodeBase)?.Y ?? 0,
                Properties = n.InputPorts.ToDictionary(
                    p => p.Name,
                    p => p.Value)
            }).ToList(),
            Connections = graph.Connections.Select(c => new ConnectionData
            {
                SourceNodeId = c.SourceNodeId,
                SourcePortId = c.SourcePortId,
                TargetNodeId = c.TargetNodeId,
                TargetPortId = c.TargetPortId
            }).ToList()
        };

        return JsonConvert.SerializeObject(data, Formatting.Indented);
    }

    public NodeGraph? Deserialize(string json)
    {
        var data = JsonConvert.DeserializeObject<GraphData>(json);
        if (data == null) return null;

        var graph = new NodeGraph { Name = data.Name, Version = data.Version };

        var nodeMap = new Dictionary<string, INode>();

        foreach (var nodeData in data.Nodes)
        {
            var node = _pluginService.CreateNode(nodeData.Type);
            if (node == null) continue;

            if (node is NodeBase nb)
            {
                nb.X = nodeData.X;
                nb.Y = nodeData.Y;
            }

            // Restore properties
            foreach (var (name, value) in nodeData.Properties)
            {
                var port = node.InputPorts.FirstOrDefault(p => p.Name == name);
                if (port != null && value != null)
                    port.Value = Convert.ChangeType(value, port.DataType);
            }

            graph.AddNode(node);
            nodeMap[nodeData.Id] = node;
        }

        foreach (var connData in data.Connections)
        {
            if (!nodeMap.TryGetValue(connData.SourceNodeId, out var sourceNode)) continue;
            if (!nodeMap.TryGetValue(connData.TargetNodeId, out var targetNode)) continue;

            var sourcePort = sourceNode.OutputPorts.FirstOrDefault(p => p.Id == connData.SourcePortId);
            var targetPort = targetNode.InputPorts.FirstOrDefault(p => p.Id == connData.TargetPortId);

            if (sourcePort != null && targetPort != null)
                graph.Connect(sourceNode, sourcePort, targetNode, targetPort);
        }

        return graph;
    }
}

public class GraphData
{
    public string Version { get; set; } = "1.0";
    public string Name { get; set; } = "";
    public List<NodeData> Nodes { get; set; } = new();
    public List<ConnectionData> Connections { get; set; } = new();
}

public class NodeData
{
    public string Id { get; set; } = "";
    public string Type { get; set; } = "";
    public double X { get; set; }
    public double Y { get; set; }
    public Dictionary<string, object?> Properties { get; set; } = new();
}

public class ConnectionData
{
    public string SourceNodeId { get; set; } = "";
    public string SourcePortId { get; set; } = "";
    public string TargetNodeId { get; set; } = "";
    public string TargetPortId { get; set; } = "";
}
