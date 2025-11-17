using System;
using System.Windows.Media;

namespace Middle_ROI_3D
{
    public enum ROIShape
    {
        Box,
        Sphere,
        Cylinder
    }

    public class ROI
    {
        public string Name { get; set; }
        public ROIShape Shape { get; set; }
        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }
        public double Width { get; set; }
        public double Height { get; set; }
        public double Depth { get; set; }
        public double RotationX { get; set; }
        public double RotationY { get; set; }
        public double RotationZ { get; set; }
        public Color Color { get; set; }
        public double Opacity { get; set; }

        public ROI(string name)
        {
            Name = name;
            Shape = ROIShape.Box;
            X = 0;
            Y = 0;
            Z = 0;
            Width = 2;
            Height = 2;
            Depth = 2;
            RotationX = 0;
            RotationY = 0;
            RotationZ = 0;
            Color = Colors.Red;
            Opacity = 0.5;
        }

        public override string ToString()
        {
            return $"{Name} ({Shape})";
        }
    }
}
