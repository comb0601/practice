using System.Numerics;
using VPP.Core.Attributes;
using VPP.Core.Models;
using VPP.Plugins.PointCloud.Models;

namespace VPP.Plugins.PointCloud.Nodes;

[NodeInfo("Circle Detection", "Point Cloud/Detection", "Detect circle in point cloud using RANSAC")]
public class CircleDetectionNode : NodeBase
{
    public CircleDetectionNode()
    {
        AddInput<PointCloudData>("PointCloud", required: true);
        AddInput<int>("MaxIterations", required: false, defaultValue: 1000);
        AddInput<float>("DistanceThreshold", required: false, defaultValue: 0.01f);
        AddInput<float>("MinRadius", required: false, defaultValue: 0.0f);
        AddInput<float>("MaxRadius", required: false, defaultValue: float.MaxValue);

        AddOutput<CircleDetectionResult>("Result");
        AddOutput<Vector3>("Center");
        AddOutput<float>("Radius");
        AddOutput<float>("FitError");
    }

    protected override Task ExecuteCoreAsync(CancellationToken cancellationToken)
    {
        var cloud = GetInput<PointCloudData>("PointCloud");
        var maxIterations = GetInput<int>("MaxIterations");
        var threshold = GetInput<float>("DistanceThreshold");
        var minRadius = GetInput<float>("MinRadius");
        var maxRadius = GetInput<float>("MaxRadius");

        if (cloud == null || cloud.Count < 3)
            throw new ArgumentException("Need at least 3 points for circle detection");

        var result = DetectCircleRANSAC(cloud.Points, maxIterations, threshold, minRadius, maxRadius, cancellationToken);

        SetOutput("Result", result);
        SetOutput("Center", result.Center);
        SetOutput("Radius", result.Radius);
        SetOutput("FitError", result.FitError);

        return Task.CompletedTask;
    }

    private CircleDetectionResult DetectCircleRANSAC(
        List<Vector3> points, int maxIter, float threshold,
        float minRadius, float maxRadius, CancellationToken ct)
    {
        var random = new Random();
        var bestResult = new CircleDetectionResult { FitError = float.MaxValue };

        // Project to best-fit plane first (assuming mostly planar data)
        var (planeNormal, planePoint) = FitPlane(points);

        // Project points to 2D on the plane
        var (u, v) = GetPlaneAxes(planeNormal);
        var points2D = new List<Vector2>();
        foreach (var p in points)
        {
            var rel = p - planePoint;
            points2D.Add(new Vector2(Vector3.Dot(rel, u), Vector3.Dot(rel, v)));
        }

        for (int iter = 0; iter < maxIter && !ct.IsCancellationRequested; iter++)
        {
            // Pick 3 random points
            var idx = new int[3];
            idx[0] = random.Next(points2D.Count);
            do idx[1] = random.Next(points2D.Count); while (idx[1] == idx[0]);
            do idx[2] = random.Next(points2D.Count); while (idx[2] == idx[0] || idx[2] == idx[1]);

            var p1 = points2D[idx[0]];
            var p2 = points2D[idx[1]];
            var p3 = points2D[idx[2]];

            // Fit circle through 3 points
            if (!FitCircle3Points(p1, p2, p3, out var center2D, out var radius))
                continue;

            if (radius < minRadius || radius > maxRadius)
                continue;

            // Count inliers
            var inliers = new List<int>();
            float totalError = 0;
            for (int i = 0; i < points2D.Count; i++)
            {
                var dist = Math.Abs((points2D[i] - center2D).Length() - radius);
                if (dist < threshold)
                {
                    inliers.Add(i);
                    totalError += dist;
                }
            }

            if (inliers.Count > bestResult.InlierCount)
            {
                // Convert back to 3D
                var center3D = planePoint + center2D.X * u + center2D.Y * v;

                bestResult = new CircleDetectionResult
                {
                    Center = center3D,
                    Radius = radius,
                    Normal = planeNormal,
                    InlierCount = inliers.Count,
                    FitError = inliers.Count > 0 ? totalError / inliers.Count : float.MaxValue,
                    InlierPoints = inliers.Select(i => points[i]).ToList()
                };
            }
        }

        return bestResult;
    }

    private bool FitCircle3Points(Vector2 p1, Vector2 p2, Vector2 p3, out Vector2 center, out float radius)
    {
        center = Vector2.Zero;
        radius = 0;

        var ax = p1.X; var ay = p1.Y;
        var bx = p2.X; var by = p2.Y;
        var cx = p3.X; var cy = p3.Y;

        var d = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
        if (Math.Abs(d) < 1e-10) return false;

        var ux = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by)) / d;
        var uy = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax)) / d;

        center = new Vector2(ux, uy);
        radius = (p1 - center).Length();
        return true;
    }

    private (Vector3 normal, Vector3 point) FitPlane(List<Vector3> points)
    {
        // Compute centroid
        var centroid = Vector3.Zero;
        foreach (var p in points) centroid += p;
        centroid /= points.Count;

        // Compute covariance matrix and find smallest eigenvector
        float xx = 0, xy = 0, xz = 0, yy = 0, yz = 0, zz = 0;
        foreach (var p in points)
        {
            var r = p - centroid;
            xx += r.X * r.X; xy += r.X * r.Y; xz += r.X * r.Z;
            yy += r.Y * r.Y; yz += r.Y * r.Z; zz += r.Z * r.Z;
        }

        // Simple normal estimation (assuming Z is up for planar data)
        var normal = Vector3.Normalize(new Vector3(
            yz * xy - xz * yy,
            xz * xy - yz * xx,
            xx * yy - xy * xy
        ));

        if (float.IsNaN(normal.X)) normal = Vector3.UnitZ;

        return (normal, centroid);
    }

    private (Vector3 u, Vector3 v) GetPlaneAxes(Vector3 normal)
    {
        var u = Math.Abs(normal.X) < 0.9f
            ? Vector3.Normalize(Vector3.Cross(normal, Vector3.UnitX))
            : Vector3.Normalize(Vector3.Cross(normal, Vector3.UnitY));
        var v = Vector3.Cross(normal, u);
        return (u, v);
    }
}
