# Visual Programming Platform - Point Cloud Inspection

A WPF-based visual programming platform for point cloud processing with 3D visualization, circle detection, and pass/fail inspection.

## Features

- **Node-Based Graph Editor**: Drag-and-drop visual programming interface
- **Point Cloud Import**: Support for PLY, PCD, XYZ, CSV formats
- **3D ROI Drawing**: Define regions of interest (Box, Cylinder, Sphere)
- **Circle Detection**: RANSAC-based circle fitting algorithm
- **Spec Inspection**: Pass/fail inspection with configurable tolerances
- **3D Visualization**: Real-time point cloud rendering with HelixToolkit

## Architecture

- **MVVM Pattern**: Complete separation of UI and logic
- **Plugin Architecture**: Extensible node system via reflection
- **Composite Pattern**: Hierarchical node structure
- **Command Pattern**: Undo/redo support ready

## Project Structure

```
VisualProgrammingPlatform/
├── src/
│   ├── VPP.Core/              # Core engine and interfaces
│   │   ├── Interfaces/        # INode, IPlugin, IPort
│   │   ├── Models/            # NodeBase, NodeGraph, Connection
│   │   ├── Engine/            # ExecutionEngine
│   │   └── Services/          # PluginService, GraphSerializer
│   ├── VPP.Plugins.PointCloud/ # Point cloud processing nodes
│   │   ├── Models/            # PointCloudData, ROI3D, etc.
│   │   └── Nodes/             # Import, ROI, Circle Detection, Inspection
│   └── VPP.App/               # WPF Application
│       ├── ViewModels/        # MainViewModel
│       ├── Views/             # MainWindow
│       └── Controls/          # Custom WPF controls
```

## Workflow

1. **Import Point Cloud**: Load 3D point cloud data from file
2. **Draw ROI**: Define a 3D region of interest (box, cylinder, sphere)
3. **Filter Points**: Extract points within the ROI
4. **Detect Circle**: Find circle in the filtered points using RANSAC
5. **Inspect Spec**: Check if detected circle meets specifications
6. **Pass/Fail Result**: Display inspection result

## Node Types

### Point Cloud/IO
- **Import Point Cloud**: Load PLY, PCD, XYZ, CSV files

### Point Cloud/ROI
- **ROI Draw**: Create 3D region of interest

### Point Cloud/Filter
- **ROI Filter**: Filter points within ROI

### Point Cloud/Detection
- **Circle Detection**: RANSAC-based circle fitting

### Point Cloud/Inspection
- **Spec Inspection**: Check measurements against tolerances

## Configuration

### Circle Detection Parameters
- **MaxIterations**: RANSAC iterations (default: 1000)
- **DistanceThreshold**: Inlier distance threshold (default: 0.01)
- **MinRadius/MaxRadius**: Valid radius range

### Inspection Specifications
- **RadiusMin/RadiusMax**: Acceptable radius range
- **CenterX/Y Min/Max**: Center position tolerances
- **MaxFitError**: Maximum fitting error
- **MinInliers**: Minimum inlier count

## Building

```bash
dotnet build VisualProgrammingPlatform.sln
```

## Running

```bash
dotnet run --project src/VPP.App/VPP.App.csproj
```

## Usage

1. Click **Create Workflow** to set up the default inspection pipeline
2. Click **Load Point Cloud** to import your point cloud file
3. Adjust ROI and inspection parameters in the node properties
4. Click **Execute** to run the inspection
5. View results in the 3D visualization panel

## Technology Stack

- .NET 8.0 / C# 12
- WPF with XAML
- CommunityToolkit.Mvvm
- HelixToolkit.Wpf (3D visualization)
- Newtonsoft.Json

## Extending

### Adding New Nodes

1. Create a class inheriting from `NodeBase`
2. Add `[NodeInfo]` attribute with name, category, description
3. Define input/output ports in constructor
4. Implement `ExecuteCoreAsync` method
5. Register in your plugin's `NodeTypes` list

```csharp
[NodeInfo("My Node", "Category", "Description")]
public class MyNode : NodeBase
{
    public MyNode()
    {
        AddInput<float>("Input", required: true);
        AddOutput<float>("Output");
    }

    protected override Task ExecuteCoreAsync(CancellationToken ct)
    {
        var input = GetInput<float>("Input");
        SetOutput("Output", input * 2);
        return Task.CompletedTask;
    }
}
```

## License

MIT License
