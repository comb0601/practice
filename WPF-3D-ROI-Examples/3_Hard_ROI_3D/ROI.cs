using System;
using System.Text.Json.Serialization;
using System.Windows.Media;

namespace Hard_ROI_3D
{
    public enum ROIShape
    {
        Box,
        Sphere,
        Cylinder,
        Cone,
        Torus
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

        [JsonIgnore]
        public Color Color { get; set; }

        // For JSON serialization
        public string ColorString
        {
            get => Color.ToString();
            set => Color = (Color)ColorConverter.ConvertFromString(value);
        }

        public double Opacity { get; set; }
        public bool Visible { get; set; }
        public bool ShowWireframe { get; set; }

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
            Visible = true;
            ShowWireframe = true;
        }

        public double GetVolume()
        {
            switch (Shape)
            {
                case ROIShape.Box:
                    return Width * Height * Depth;
                case ROIShape.Sphere:
                    double radius = Width / 2;
                    return (4.0 / 3.0) * Math.PI * Math.Pow(radius, 3);
                case ROIShape.Cylinder:
                    radius = Width / 2;
                    return Math.PI * Math.Pow(radius, 2) * Height;
                case ROIShape.Cone:
                    radius = Width / 2;
                    return (1.0 / 3.0) * Math.PI * Math.Pow(radius, 2) * Height;
                case ROIShape.Torus:
                    double majorRadius = Width / 2;
                    double minorRadius = Depth / 4;
                    return 2 * Math.PI * Math.PI * majorRadius * Math.Pow(minorRadius, 2);
                default:
                    return 0;
            }
        }

        public override string ToString()
        {
            return $"{Name} ({Shape})";
        }
    }
}
