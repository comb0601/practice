# Module 6: Data Visualization with ROI (15 hours)

## Overview

Apply ROI techniques to real-world data visualization: volume rendering, slice visualization, isosurfaces, and statistical analysis. Build medical imaging and scientific visualization applications.

## Learning Objectives

- Volume rendering basics
- ROI-based data filtering
- 2D slice visualization from 3D data
- Isosurface extraction and rendering
- Statistical analysis within ROI

## Lessons

### Lesson 6.1 - Volume Rendering Basics (3 hours)

**Topics:**
- 3D scalar field representation
- Voxel data structures
- Direct volume rendering
- Transfer functions
- Opacity mapping

**Voxel Data Structure:**
```csharp
public class VolumeData
{
    public int SizeX { get; set; }
    public int SizeY { get; set; }
    public int SizeZ { get; set; }
    public float[] Data { get; set; }  // Flattened 3D array

    public float GetVoxel(int x, int y, int z)
    {
        int index = x + y * SizeX + z * SizeX * SizeY;
        return Data[index];
    }

    public void SetVoxel(int x, int y, int z, float value)
    {
        int index = x + y * SizeX + z * SizeX * SizeY;
        Data[index] = value;
    }

    public static VolumeData LoadFromFile(string path)
    {
        // Load raw, DICOM, or other volume format
        // Implementation depends on format
        return new VolumeData();
    }
}
```

**Project:** Volume data viewer with basic rendering

---

### Lesson 6.2 - ROI-based Filtering (3 hours)

**Topics:**
- Filter data by ROI
- Multi-ROI filtering (AND/OR operations)
- Dynamic filtering
- Performance optimization
- GPU acceleration concepts

**ROI Filtering:**
```csharp
public class ROIDataFilter
{
    public List<DataPoint> FilterByROI(List<DataPoint> data, ROI3D roi)
    {
        return data.Where(p => roi.ContainsPoint(p.Position)).ToList();
    }

    public List<DataPoint> FilterByMultipleROIs(List<DataPoint> data, List<ROI3D> rois, FilterMode mode)
    {
        switch (mode)
        {
            case FilterMode.Any:
                return data.Where(p => rois.Any(r => r.ContainsPoint(p.Position))).ToList();

            case FilterMode.All:
                return data.Where(p => rois.All(r => r.ContainsPoint(p.Position))).ToList();

            case FilterMode.None:
                return data.Where(p => !rois.Any(r => r.ContainsPoint(p.Position))).ToList();

            default:
                return data;
        }
    }

    public Dictionary<ROI3D, List<DataPoint>> GroupByROI(List<DataPoint> data, List<ROI3D> rois)
    {
        var result = new Dictionary<ROI3D, List<DataPoint>>();

        foreach (var roi in rois)
        {
            result[roi] = FilterByROI(data, roi);
        }

        return result;
    }
}

public enum FilterMode { Any, All, None }

public class DataPoint
{
    public Point3D Position { get; set; }
    public double Value { get; set; }
    public Color VisualizationColor { get; set; }
}
```

**Project:** Interactive data filtering application

---

### Lesson 6.3 - Slice Visualization (3 hours)

**Topics:**
- Extract 2D slices from 3D volume
- Orthogonal slice planes (Axial, Sagittal, Coronal)
- Oblique slicing
- Multi-planar reconstruction (MPR)
- Slice through ROI

**Slice Extraction:**
```csharp
public class SliceExtractor
{
    public ImageData ExtractSlice(VolumeData volume, SlicePlane plane, int sliceIndex)
    {
        int width, height;
        float[] sliceData;

        switch (plane)
        {
            case SlicePlane.Axial:  // XY plane (Z constant)
                width = volume.SizeX;
                height = volume.SizeY;
                sliceData = new float[width * height];

                for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++)
                {
                    sliceData[x + y * width] = volume.GetVoxel(x, y, sliceIndex);
                }
                break;

            case SlicePlane.Sagittal:  // YZ plane (X constant)
                width = volume.SizeZ;
                height = volume.SizeY;
                sliceData = new float[width * height];

                for (int y = 0; y < height; y++)
                for (int z = 0; z < width; z++)
                {
                    sliceData[z + y * width] = volume.GetVoxel(sliceIndex, y, z);
                }
                break;

            case SlicePlane.Coronal:  // XZ plane (Y constant)
                width = volume.SizeX;
                height = volume.SizeZ;
                sliceData = new float[width * height];

                for (int z = 0; z < height; z++)
                for (int x = 0; x < width; x++)
                {
                    sliceData[x + z * width] = volume.GetVoxel(x, sliceIndex, z);
                }
                break;

            default:
                return new ImageData();
        }

        return new ImageData { Width = width, Height = height, Data = sliceData };
    }

    public Model3D Create3DSlicePlane(VolumeData volume, SlicePlane plane, int sliceIndex, ROI3D? roi = null)
    {
        var imageData = ExtractSlice(volume, plane, sliceIndex);
        var texture = CreateTextureFromImageData(imageData);

        // Create plane geometry at the correct position
        var mesh = CreateSlicePlaneMesh(volume, plane, sliceIndex);

        return new GeometryModel3D
        {
            Geometry = mesh,
            Material = new DiffuseMaterial(new ImageBrush(texture))
        };
    }
}

public enum SlicePlane { Axial, Sagittal, Coronal }

public class ImageData
{
    public int Width { get; set; }
    public int Height { get; set; }
    public float[] Data { get; set; }
}
```

**Project:** Multi-planar reconstruction viewer with ROI overlay

---

### Lesson 6.4 - Isosurface Rendering (3 hours)

**Topics:**
- Marching cubes algorithm
- Isosurface extraction
- ROI-constrained isosurfaces
- Surface smoothing
- Multiple isosurface levels

**Isosurface Extraction:**
```csharp
public class IsosurfaceExtractor
{
    public MeshGeometry3D ExtractIsosurface(VolumeData volume, float isoValue, ROI3D? roi = null)
    {
        var mesh = new MeshGeometry3D();
        var marchingCubes = new MarchingCubes();

        // Determine bounds
        int minX = roi != null ? (int)roi.MinX : 0;
        int maxX = roi != null ? (int)roi.MaxX : volume.SizeX - 1;
        int minY = roi != null ? (int)roi.MinY : 0;
        int maxY = roi != null ? (int)roi.MaxY : volume.SizeY - 1;
        int minZ = roi != null ? (int)roi.MinZ : 0;
        int maxZ = roi != null ? (int)roi.MaxZ : volume.SizeZ - 1;

        // Process each voxel cube
        for (int z = minZ; z < maxZ; z++)
        for (int y = minY; y < maxY; y++)
        for (int x = minX; x < maxX; x++)
        {
            var cube = GetVoxelCube(volume, x, y, z);
            var triangles = marchingCubes.ProcessCube(cube, isoValue);

            foreach (var triangle in triangles)
            {
                AddTriangle(mesh, triangle);
            }
        }

        return mesh;
    }

    private VoxelCube GetVoxelCube(VolumeData volume, int x, int y, int z)
    {
        return new VoxelCube
        {
            Values = new float[8]
            {
                volume.GetVoxel(x, y, z),
                volume.GetVoxel(x+1, y, z),
                volume.GetVoxel(x+1, y+1, z),
                volume.GetVoxel(x, y+1, z),
                volume.GetVoxel(x, y, z+1),
                volume.GetVoxel(x+1, y, z+1),
                volume.GetVoxel(x+1, y+1, z+1),
                volume.GetVoxel(x, y+1, z+1)
            },
            Position = new Point3D(x, y, z)
        };
    }
}
```

**Project:** Isosurface viewer with ROI filtering

---

### Lesson 6.5 - Statistical Analysis in ROI (3 hours)

**Topics:**
- Histogram calculation
- Distribution analysis
- Comparative statistics (multi-ROI)
- Time-series analysis
- Export and reporting

**Statistical Analysis:**
```csharp
public class ROIStatisticsAnalyzer
{
    public DetailedStatistics AnalyzeROI(VolumeData volume, ROI3D roi, int numBins = 256)
    {
        var values = new List<float>();

        // Collect all values in ROI
        for (int z = (int)roi.MinZ; z <= roi.MaxZ && z < volume.SizeZ; z++)
        for (int y = (int)roi.MinY; y <= roi.MaxY && y < volume.SizeY; y++)
        for (int x = (int)roi.MinX; x <= roi.MaxX && x < volume.SizeX; x++)
        {
            if (roi.ContainsPoint(new Point3D(x, y, z)))
            {
                values.Add(volume.GetVoxel(x, y, z));
            }
        }

        return CalculateStatistics(values, numBins);
    }

    private DetailedStatistics CalculateStatistics(List<float> values, int numBins)
    {
        if (values.Count == 0)
            return new DetailedStatistics();

        var sorted = values.OrderBy(v => v).ToList();

        var stats = new DetailedStatistics
        {
            Count = values.Count,
            Min = sorted.First(),
            Max = sorted.Last(),
            Mean = values.Average(),
            Median = GetMedian(sorted),
            StdDev = CalculateStdDev(values),
            Histogram = CalculateHistogram(values, numBins)
        };

        stats.Percentile25 = GetPercentile(sorted, 0.25);
        stats.Percentile75 = GetPercentile(sorted, 0.75);

        return stats;
    }

    private double[] CalculateHistogram(List<float> values, int numBins)
    {
        double min = values.Min();
        double max = values.Max();
        double binSize = (max - min) / numBins;

        var histogram = new double[numBins];

        foreach (var value in values)
        {
            int binIndex = (int)((value - min) / binSize);
            if (binIndex >= numBins) binIndex = numBins - 1;
            histogram[binIndex]++;
        }

        // Normalize
        for (int i = 0; i < numBins; i++)
            histogram[i] /= values.Count;

        return histogram;
    }

    public ComparisonReport CompareROIs(VolumeData volume, List<ROI3D> rois)
    {
        var report = new ComparisonReport();

        foreach (var roi in rois)
        {
            report.ROIStatistics[roi.Name] = AnalyzeROI(volume, roi);
        }

        return report;
    }
}

public class DetailedStatistics
{
    public int Count { get; set; }
    public double Min { get; set; }
    public double Max { get; set; }
    public double Mean { get; set; }
    public double Median { get; set; }
    public double StdDev { get; set; }
    public double Percentile25 { get; set; }
    public double Percentile75 { get; set; }
    public double[] Histogram { get; set; }
}

public class ComparisonReport
{
    public Dictionary<string, DetailedStatistics> ROIStatistics { get; set; } = new();

    public string GenerateReport()
    {
        // Generate formatted report
        return "ROI Comparison Report...";
    }
}
```

**Project:** Complete ROI statistical analysis tool with reporting

---

## Real-World Applications

This module prepares you for:
- Medical image analysis
- Scientific data visualization
- Quality control systems
- Research data processing
- 3D measurement tools

## Next Module

**Module 7: Performance and Optimization** - Make your applications fast and efficient!

---

**Time Investment:** 15 hours total
**Difficulty:** Advanced
