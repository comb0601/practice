using System.Numerics;
using VPP.Core.Attributes;
using VPP.Core.Models;
using VPP.Plugins.PointCloud.Models;

namespace VPP.Plugins.PointCloud.Nodes;

[NodeInfo("Spec Inspection", "Point Cloud/Inspection", "Inspect measurements against specifications")]
public class InspectionNode : NodeBase
{
    public InspectionNode()
    {
        AddInput<CircleDetectionResult>("CircleResult", required: true);
        AddInput<float>("RadiusMin", required: true);
        AddInput<float>("RadiusMax", required: true);
        AddInput<float>("CenterXMin", required: false, defaultValue: float.MinValue);
        AddInput<float>("CenterXMax", required: false, defaultValue: float.MaxValue);
        AddInput<float>("CenterYMin", required: false, defaultValue: float.MinValue);
        AddInput<float>("CenterYMax", required: false, defaultValue: float.MaxValue);
        AddInput<float>("MaxFitError", required: false, defaultValue: 0.1f);
        AddInput<int>("MinInliers", required: false, defaultValue: 10);

        AddOutput<InspectionResult>("Result");
        AddOutput<bool>("Pass");
        AddOutput<string>("Message");
    }

    protected override Task ExecuteCoreAsync(CancellationToken cancellationToken)
    {
        var circle = GetInput<CircleDetectionResult>("CircleResult");
        var radiusMin = GetInput<float>("RadiusMin");
        var radiusMax = GetInput<float>("RadiusMax");
        var centerXMin = GetInput<float>("CenterXMin");
        var centerXMax = GetInput<float>("CenterXMax");
        var centerYMin = GetInput<float>("CenterYMin");
        var centerYMax = GetInput<float>("CenterYMax");
        var maxFitError = GetInput<float>("MaxFitError");
        var minInliers = GetInput<int>("MinInliers");

        if (circle == null)
            throw new ArgumentNullException(nameof(circle));

        var result = new InspectionResult
        {
            Measurements = new Dictionary<string, double>
            {
                ["Radius"] = circle.Radius,
                ["CenterX"] = circle.Center.X,
                ["CenterY"] = circle.Center.Y,
                ["CenterZ"] = circle.Center.Z,
                ["FitError"] = circle.FitError,
                ["InlierCount"] = circle.InlierCount
            }
        };

        // Check specifications
        if (circle.Radius < radiusMin)
            result.Failures.Add($"Radius {circle.Radius:F3} < Min {radiusMin:F3}");
        if (circle.Radius > radiusMax)
            result.Failures.Add($"Radius {circle.Radius:F3} > Max {radiusMax:F3}");

        if (circle.Center.X < centerXMin)
            result.Failures.Add($"Center X {circle.Center.X:F3} < Min {centerXMin:F3}");
        if (circle.Center.X > centerXMax)
            result.Failures.Add($"Center X {circle.Center.X:F3} > Max {centerXMax:F3}");

        if (circle.Center.Y < centerYMin)
            result.Failures.Add($"Center Y {circle.Center.Y:F3} < Min {centerYMin:F3}");
        if (circle.Center.Y > centerYMax)
            result.Failures.Add($"Center Y {circle.Center.Y:F3} > Max {centerYMax:F3}");

        if (circle.FitError > maxFitError)
            result.Failures.Add($"Fit error {circle.FitError:F4} > Max {maxFitError:F4}");

        if (circle.InlierCount < minInliers)
            result.Failures.Add($"Inlier count {circle.InlierCount} < Min {minInliers}");

        result.Pass = result.Failures.Count == 0;
        result.Message = result.Pass
            ? $"PASS - Radius: {circle.Radius:F3}, Center: ({circle.Center.X:F3}, {circle.Center.Y:F3})"
            : $"FAIL - {string.Join("; ", result.Failures)}";

        SetOutput("Result", result);
        SetOutput("Pass", result.Pass);
        SetOutput("Message", result.Message);

        return Task.CompletedTask;
    }
}
