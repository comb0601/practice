using System.Collections.ObjectModel;
using System.Windows.Media.Media3D;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Microsoft.Win32;
using VPP.Core.Engine;
using VPP.Core.Interfaces;
using VPP.Core.Models;
using VPP.Core.Services;
using VPP.Plugins.PointCloud;
using VPP.Plugins.PointCloud.Models;

namespace VPP.App.ViewModels;

public partial class MainViewModel : ObservableObject
{
    private readonly PluginService _pluginService;
    private readonly ExecutionEngine _executionEngine;

    [ObservableProperty] private NodeGraph _graph = new();
    [ObservableProperty] private ObservableCollection<NodeViewModel> _nodes = new();
    [ObservableProperty] private ObservableCollection<ConnectionViewModel> _connections = new();
    [ObservableProperty] private ObservableCollection<string> _availableNodes = new();
    [ObservableProperty] private string _selectedNodeType = "";
    [ObservableProperty] private NodeViewModel? _selectedNode;
    [ObservableProperty] private string _statusMessage = "Ready";
    [ObservableProperty] private bool _isExecuting;
    [ObservableProperty] private Point3DCollection _pointCloudPoints = new();
    [ObservableProperty] private string _inspectionResult = "";
    [ObservableProperty] private bool _inspectionPass;
    [ObservableProperty] private double _detectedRadius;
    [ObservableProperty] private Point3D _detectedCenter;

    public MainViewModel()
    {
        _pluginService = new PluginService();
        _executionEngine = new ExecutionEngine();

        // Load built-in plugins
        _pluginService.LoadFromAssembly(typeof(PointCloudPlugin).Assembly);

        // Populate available nodes
        foreach (var (name, category, _) in _pluginService.GetAvailableNodes())
        {
            AvailableNodes.Add($"{category}/{name}");
        }

        _executionEngine.NodeExecuting += (s, e) =>
            StatusMessage = $"Executing: {e.Node.Name}";
        _executionEngine.NodeExecuted += (s, e) =>
            StatusMessage = e.Result?.Success == true ? $"Completed: {e.Node.Name}" : $"Failed: {e.Node.Name}";
    }

    [RelayCommand]
    private void AddNode()
    {
        if (string.IsNullOrEmpty(SelectedNodeType)) return;

        var nodeName = SelectedNodeType.Split('/').Last();
        var node = _pluginService.CreateNode(nodeName);
        if (node == null) return;

        if (node is NodeBase nb)
        {
            nb.X = 100 + Nodes.Count * 200;
            nb.Y = 100;
        }

        Graph.AddNode(node);
        Nodes.Add(new NodeViewModel(node));
        StatusMessage = $"Added node: {node.Name}";
    }

    [RelayCommand]
    private async Task ExecuteGraph()
    {
        if (IsExecuting) return;
        IsExecuting = true;
        StatusMessage = "Executing graph...";

        try
        {
            var result = await _executionEngine.ExecuteAsync(Graph);

            if (result.Success)
            {
                StatusMessage = "Execution completed successfully";
                UpdateVisualization();
            }
            else
            {
                var failedNode = result.NodeResults.FirstOrDefault(r => !r.Value.Success);
                StatusMessage = $"Execution failed: {failedNode.Value?.ErrorMessage}";
            }
        }
        catch (Exception ex)
        {
            StatusMessage = $"Error: {ex.Message}";
        }
        finally
        {
            IsExecuting = false;
        }
    }

    [RelayCommand]
    private void LoadPointCloud()
    {
        var dialog = new OpenFileDialog
        {
            Filter = "Point Cloud Files|*.ply;*.pcd;*.xyz;*.csv;*.txt|All Files|*.*",
            Title = "Select Point Cloud File"
        };

        if (dialog.ShowDialog() == true)
        {
            // Find import node and set file path
            var importNode = Graph.Nodes.FirstOrDefault(n => n.Name == "Import Point Cloud");
            if (importNode != null)
            {
                var pathPort = importNode.InputPorts.FirstOrDefault(p => p.Name == "FilePath");
                if (pathPort != null)
                    pathPort.Value = dialog.FileName;
                StatusMessage = $"Loaded: {System.IO.Path.GetFileName(dialog.FileName)}";
            }
            else
            {
                StatusMessage = "Add 'Import Point Cloud' node first";
            }
        }
    }

    [RelayCommand]
    private void CreateDefaultWorkflow()
    {
        // Clear existing
        Graph = new NodeGraph();
        Nodes.Clear();
        Connections.Clear();

        // Create workflow: Import -> ROI -> Circle Detection -> Inspection
        var importNode = _pluginService.CreateNode("Import Point Cloud") as NodeBase;
        var roiDrawNode = _pluginService.CreateNode("ROI Draw") as NodeBase;
        var roiFilterNode = _pluginService.CreateNode("ROI Filter") as NodeBase;
        var circleNode = _pluginService.CreateNode("Circle Detection") as NodeBase;
        var inspectNode = _pluginService.CreateNode("Spec Inspection") as NodeBase;

        if (importNode == null || roiDrawNode == null || roiFilterNode == null ||
            circleNode == null || inspectNode == null) return;

        // Position nodes
        importNode.X = 50; importNode.Y = 150;
        roiDrawNode.X = 50; roiDrawNode.Y = 350;
        roiFilterNode.X = 300; roiFilterNode.Y = 200;
        circleNode.X = 550; circleNode.Y = 200;
        inspectNode.X = 800; inspectNode.Y = 200;

        // Set default values
        roiDrawNode.InputPorts.First(p => p.Name == "SizeX").Value = 100f;
        roiDrawNode.InputPorts.First(p => p.Name == "SizeY").Value = 100f;
        roiDrawNode.InputPorts.First(p => p.Name == "SizeZ").Value = 50f;

        inspectNode.InputPorts.First(p => p.Name == "RadiusMin").Value = 5f;
        inspectNode.InputPorts.First(p => p.Name == "RadiusMax").Value = 15f;

        // Add to graph
        Graph.AddNode(importNode);
        Graph.AddNode(roiDrawNode);
        Graph.AddNode(roiFilterNode);
        Graph.AddNode(circleNode);
        Graph.AddNode(inspectNode);

        // Connect nodes
        Graph.Connect(importNode, importNode.OutputPorts[0], roiFilterNode, roiFilterNode.InputPorts[0]);
        Graph.Connect(roiDrawNode, roiDrawNode.OutputPorts[0], roiFilterNode, roiFilterNode.InputPorts[1]);
        Graph.Connect(roiFilterNode, roiFilterNode.OutputPorts[0], circleNode, circleNode.InputPorts[0]);
        Graph.Connect(circleNode, circleNode.OutputPorts[0], inspectNode, inspectNode.InputPorts[0]);

        // Create view models
        Nodes.Add(new NodeViewModel(importNode));
        Nodes.Add(new NodeViewModel(roiDrawNode));
        Nodes.Add(new NodeViewModel(roiFilterNode));
        Nodes.Add(new NodeViewModel(circleNode));
        Nodes.Add(new NodeViewModel(inspectNode));

        foreach (var conn in Graph.Connections)
        {
            Connections.Add(new ConnectionViewModel(conn, Graph));
        }

        StatusMessage = "Created default workflow";
    }

    private void UpdateVisualization()
    {
        // Get point cloud data from import node
        var importNode = Graph.Nodes.FirstOrDefault(n => n.Name == "Import Point Cloud");
        if (importNode != null)
        {
            var cloudPort = importNode.OutputPorts.FirstOrDefault(p => p.Name == "PointCloud");
            if (cloudPort?.Value is PointCloudData cloud)
            {
                PointCloudPoints.Clear();
                foreach (var pt in cloud.Points)
                {
                    PointCloudPoints.Add(new Point3D(pt.X, pt.Y, pt.Z));
                }
            }
        }

        // Get inspection result
        var inspectNode = Graph.Nodes.FirstOrDefault(n => n.Name == "Spec Inspection");
        if (inspectNode != null)
        {
            var passPort = inspectNode.OutputPorts.FirstOrDefault(p => p.Name == "Pass");
            var msgPort = inspectNode.OutputPorts.FirstOrDefault(p => p.Name == "Message");

            if (passPort?.Value is bool pass)
                InspectionPass = pass;
            if (msgPort?.Value is string msg)
                InspectionResult = msg;
        }

        // Get circle detection result
        var circleNode = Graph.Nodes.FirstOrDefault(n => n.Name == "Circle Detection");
        if (circleNode != null)
        {
            var radiusPort = circleNode.OutputPorts.FirstOrDefault(p => p.Name == "Radius");
            var centerPort = circleNode.OutputPorts.FirstOrDefault(p => p.Name == "Center");

            if (radiusPort?.Value is float radius)
                DetectedRadius = radius;
            if (centerPort?.Value is System.Numerics.Vector3 center)
                DetectedCenter = new Point3D(center.X, center.Y, center.Z);
        }
    }
}

public partial class NodeViewModel : ObservableObject
{
    public INode Node { get; }

    [ObservableProperty] private double _x;
    [ObservableProperty] private double _y;
    [ObservableProperty] private bool _isSelected;
    [ObservableProperty] private bool _isExecuting;

    public string Name => Node.Name;
    public string Category => Node.Category;
    public IReadOnlyList<IPort> InputPorts => Node.InputPorts;
    public IReadOnlyList<IPort> OutputPorts => Node.OutputPorts;

    public NodeViewModel(INode node)
    {
        Node = node;
        if (node is NodeBase nb)
        {
            X = nb.X;
            Y = nb.Y;
        }
    }
}

public class ConnectionViewModel
{
    public Connection Connection { get; }
    public Point Start { get; set; }
    public Point End { get; set; }

    public ConnectionViewModel(Connection connection, NodeGraph graph)
    {
        Connection = connection;
        // Calculate positions based on node positions (simplified)
    }
}

public struct Point
{
    public double X { get; set; }
    public double Y { get; set; }
}
