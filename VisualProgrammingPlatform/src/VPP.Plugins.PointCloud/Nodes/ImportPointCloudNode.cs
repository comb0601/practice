using System.Globalization;
using System.Numerics;
using VPP.Core.Attributes;
using VPP.Core.Models;
using VPP.Plugins.PointCloud.Models;

namespace VPP.Plugins.PointCloud.Nodes;

[NodeInfo("Import Point Cloud", "Point Cloud/IO", "Import point cloud from PLY, PCD, XYZ, or CSV file")]
public class ImportPointCloudNode : NodeBase
{
    public ImportPointCloudNode()
    {
        AddInput<string>("FilePath", required: true);
        AddOutput<PointCloudData>("PointCloud");
    }

    protected override async Task ExecuteCoreAsync(CancellationToken cancellationToken)
    {
        var filePath = GetInput<string>("FilePath");
        if (string.IsNullOrEmpty(filePath) || !File.Exists(filePath))
            throw new FileNotFoundException($"File not found: {filePath}");

        var extension = Path.GetExtension(filePath).ToLower();
        var pointCloud = extension switch
        {
            ".ply" => await LoadPlyAsync(filePath, cancellationToken),
            ".pcd" => await LoadPcdAsync(filePath, cancellationToken),
            ".xyz" or ".csv" or ".txt" => await LoadXyzAsync(filePath, cancellationToken),
            _ => throw new NotSupportedException($"Unsupported format: {extension}")
        };

        pointCloud.ComputeBoundingBox();
        SetOutput("PointCloud", pointCloud);
    }

    private async Task<PointCloudData> LoadXyzAsync(string path, CancellationToken ct)
    {
        var data = new PointCloudData();
        var lines = await File.ReadAllLinesAsync(path, ct);

        foreach (var line in lines)
        {
            if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;
            var parts = line.Split(new[] { ' ', ',', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            if (parts.Length >= 3 &&
                float.TryParse(parts[0], NumberStyles.Float, CultureInfo.InvariantCulture, out var x) &&
                float.TryParse(parts[1], NumberStyles.Float, CultureInfo.InvariantCulture, out var y) &&
                float.TryParse(parts[2], NumberStyles.Float, CultureInfo.InvariantCulture, out var z))
            {
                data.Points.Add(new Vector3(x, y, z));
            }
        }
        return data;
    }

    private async Task<PointCloudData> LoadPlyAsync(string path, CancellationToken ct)
    {
        var data = new PointCloudData();
        var lines = await File.ReadAllLinesAsync(path, ct);
        var headerEnd = false;
        var vertexCount = 0;

        foreach (var line in lines)
        {
            if (!headerEnd)
            {
                if (line.StartsWith("element vertex"))
                    vertexCount = int.Parse(line.Split(' ')[2]);
                if (line == "end_header")
                    headerEnd = true;
                continue;
            }

            var parts = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);
            if (parts.Length >= 3 &&
                float.TryParse(parts[0], NumberStyles.Float, CultureInfo.InvariantCulture, out var x) &&
                float.TryParse(parts[1], NumberStyles.Float, CultureInfo.InvariantCulture, out var y) &&
                float.TryParse(parts[2], NumberStyles.Float, CultureInfo.InvariantCulture, out var z))
            {
                data.Points.Add(new Vector3(x, y, z));
            }
        }
        return data;
    }

    private async Task<PointCloudData> LoadPcdAsync(string path, CancellationToken ct)
    {
        var data = new PointCloudData();
        var lines = await File.ReadAllLinesAsync(path, ct);
        var dataStarted = false;

        foreach (var line in lines)
        {
            if (line.StartsWith("DATA"))
            {
                dataStarted = true;
                continue;
            }
            if (!dataStarted) continue;

            var parts = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);
            if (parts.Length >= 3 &&
                float.TryParse(parts[0], NumberStyles.Float, CultureInfo.InvariantCulture, out var x) &&
                float.TryParse(parts[1], NumberStyles.Float, CultureInfo.InvariantCulture, out var y) &&
                float.TryParse(parts[2], NumberStyles.Float, CultureInfo.InvariantCulture, out var z))
            {
                data.Points.Add(new Vector3(x, y, z));
            }
        }
        return data;
    }
}
