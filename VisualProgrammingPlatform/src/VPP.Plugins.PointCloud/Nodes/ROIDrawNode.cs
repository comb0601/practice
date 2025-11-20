using System.Numerics;
using VPP.Core.Attributes;
using VPP.Core.Models;
using VPP.Plugins.PointCloud.Models;

namespace VPP.Plugins.PointCloud.Nodes;

[NodeInfo("ROI Draw", "Point Cloud/ROI", "Create a 3D region of interest")]
public class ROIDrawNode : NodeBase
{
    public ROIDrawNode()
    {
        AddInput<float>("CenterX", required: false, defaultValue: 0f);
        AddInput<float>("CenterY", required: false, defaultValue: 0f);
        AddInput<float>("CenterZ", required: false, defaultValue: 0f);
        AddInput<float>("SizeX", required: false, defaultValue: 10f);
        AddInput<float>("SizeY", required: false, defaultValue: 10f);
        AddInput<float>("SizeZ", required: false, defaultValue: 10f);
        AddInput<float>("Radius", required: false, defaultValue: 5f);
        AddInput<string>("Shape", required: false, defaultValue: "Box"); // Box, Cylinder, Sphere

        AddOutput<ROI3D>("ROI");
    }

    protected override Task ExecuteCoreAsync(CancellationToken cancellationToken)
    {
        var centerX = GetInput<float>("CenterX");
        var centerY = GetInput<float>("CenterY");
        var centerZ = GetInput<float>("CenterZ");
        var sizeX = GetInput<float>("SizeX");
        var sizeY = GetInput<float>("SizeY");
        var sizeZ = GetInput<float>("SizeZ");
        var radius = GetInput<float>("Radius");
        var shapeStr = GetInput<string>("Shape") ?? "Box";

        var shape = shapeStr.ToLower() switch
        {
            "cylinder" => ROIShape.Cylinder,
            "sphere" => ROIShape.Sphere,
            _ => ROIShape.Box
        };

        var roi = new ROI3D
        {
            Center = new Vector3(centerX, centerY, centerZ),
            Size = new Vector3(sizeX, sizeY, sizeZ),
            Shape = shape,
            Radius = radius
        };

        SetOutput("ROI", roi);

        return Task.CompletedTask;
    }
}
