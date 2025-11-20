using System.Numerics;
using VPP.Core.Attributes;
using VPP.Core.Models;
using VPP.Plugins.PointCloud.Models;

namespace VPP.Plugins.PointCloud.Nodes;

[NodeInfo("ROI Filter", "Point Cloud/Filter", "Filter points within a 3D region of interest")]
public class ROIFilterNode : NodeBase
{
    public ROIFilterNode()
    {
        AddInput<PointCloudData>("PointCloud", required: true);
        AddInput<ROI3D>("ROI", required: true);
        AddOutput<PointCloudData>("FilteredCloud");
        AddOutput<int>("PointCount");
    }

    protected override Task ExecuteCoreAsync(CancellationToken cancellationToken)
    {
        var cloud = GetInput<PointCloudData>("PointCloud");
        var roi = GetInput<ROI3D>("ROI");

        if (cloud == null) throw new ArgumentNullException(nameof(cloud));
        if (roi == null) throw new ArgumentNullException(nameof(roi));

        var filtered = new PointCloudData();

        for (int i = 0; i < cloud.Points.Count; i++)
        {
            var point = cloud.Points[i];
            if (IsInROI(point, roi))
            {
                filtered.Points.Add(point);
                if (cloud.Colors != null && i < cloud.Colors.Count)
                {
                    filtered.Colors ??= new List<Vector3>();
                    filtered.Colors.Add(cloud.Colors[i]);
                }
            }
        }

        filtered.ComputeBoundingBox();
        SetOutput("FilteredCloud", filtered);
        SetOutput("PointCount", filtered.Count);

        return Task.CompletedTask;
    }

    private bool IsInROI(Vector3 point, ROI3D roi)
    {
        var diff = point - roi.Center;

        return roi.Shape switch
        {
            ROIShape.Box =>
                Math.Abs(diff.X) <= roi.Size.X / 2 &&
                Math.Abs(diff.Y) <= roi.Size.Y / 2 &&
                Math.Abs(diff.Z) <= roi.Size.Z / 2,

            ROIShape.Cylinder =>
                Math.Sqrt(diff.X * diff.X + diff.Y * diff.Y) <= roi.Radius &&
                Math.Abs(diff.Z) <= roi.Size.Z / 2,

            ROIShape.Sphere =>
                diff.Length() <= roi.Radius,

            _ => false
        };
    }
}
