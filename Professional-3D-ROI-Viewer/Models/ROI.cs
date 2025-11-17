using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Text.Json.Serialization;
using System.Windows.Media;

namespace Professional3DROIViewer.Models
{
    /// <summary>
    /// ROI Shape Types
    /// </summary>
    public enum ROIShape
    {
        Box,
        Sphere,
        Cylinder,
        Cone,
        Ellipsoid,
        Torus,
        CustomMesh
    }

    /// <summary>
    /// Region of Interest (ROI) Data Model
    /// Represents a 3D region with position, size, rotation, and visual properties
    /// </summary>
    public class ROI : INotifyPropertyChanged
    {
        private string _name = "ROI";
        private ROIShape _shape = ROIShape.Box;
        private double _x, _y, _z;
        private double _width = 2.0;
        private double _height = 2.0;
        private double _depth = 2.0;
        private double _rotationX, _rotationY, _rotationZ;
        private Color _color = Colors.Red;
        private double _opacity = 0.7;
        private bool _visible = true;
        private bool _locked = false;
        private bool _showWireframe = true;
        private string _layer = "Default";

        // ============================================
        // Basic Properties
        // ============================================

        public string Name
        {
            get => _name;
            set { _name = value; OnPropertyChanged(); }
        }

        public ROIShape Shape
        {
            get => _shape;
            set { _shape = value; OnPropertyChanged(); }
        }

        // ============================================
        // Position Properties
        // ============================================

        public double X
        {
            get => _x;
            set { _x = value; OnPropertyChanged(); }
        }

        public double Y
        {
            get => _y;
            set { _y = value; OnPropertyChanged(); }
        }

        public double Z
        {
            get => _z;
            set { _z = value; OnPropertyChanged(); }
        }

        // ============================================
        // Size Properties
        // ============================================

        public double Width
        {
            get => _width;
            set { _width = Math.Max(0.1, value); OnPropertyChanged(); }
        }

        public double Height
        {
            get => _height;
            set { _height = Math.Max(0.1, value); OnPropertyChanged(); }
        }

        public double Depth
        {
            get => _depth;
            set { _depth = Math.Max(0.1, value); OnPropertyChanged(); }
        }

        // ============================================
        // Rotation Properties (Degrees)
        // ============================================

        public double RotationX
        {
            get => _rotationX;
            set { _rotationX = value % 360; OnPropertyChanged(); }
        }

        public double RotationY
        {
            get => _rotationY;
            set { _rotationY = value % 360; OnPropertyChanged(); }
        }

        public double RotationZ
        {
            get => _rotationZ;
            set { _rotationZ = value % 360; OnPropertyChanged(); }
        }

        // ============================================
        // Visual Properties
        // ============================================

        [JsonIgnore]
        public Color Color
        {
            get => _color;
            set { _color = value; OnPropertyChanged(); }
        }

        // For JSON serialization
        public string ColorString
        {
            get => _color.ToString();
            set => _color = (Color)ColorConverter.ConvertFromString(value);
        }

        public double Opacity
        {
            get => _opacity;
            set { _opacity = Math.Clamp(value, 0, 1); OnPropertyChanged(); }
        }

        public bool Visible
        {
            get => _visible;
            set { _visible = value; OnPropertyChanged(); }
        }

        public bool Locked
        {
            get => _locked;
            set { _locked = value; OnPropertyChanged(); }
        }

        public bool ShowWireframe
        {
            get => _showWireframe;
            set { _showWireframe = value; OnPropertyChanged(); }
        }

        public string Layer
        {
            get => _layer;
            set { _layer = value ?? "Default"; OnPropertyChanged(); }
        }

        // ============================================
        // Metadata
        // ============================================

        public string Id { get; set; } = Guid.NewGuid().ToString();
        public DateTime CreatedAt { get; set; } = DateTime.Now;
        public DateTime ModifiedAt { get; set; } = DateTime.Now;
        public string Notes { get; set; } = string.Empty;

        // ============================================
        // Calculated Properties
        // ============================================

        [JsonIgnore]
        public double Volume
        {
            get
            {
                return Shape switch
                {
                    ROIShape.Box => Width * Height * Depth,
                    ROIShape.Sphere => (4.0 / 3.0) * Math.PI * Math.Pow(Width / 2, 3),
                    ROIShape.Cylinder => Math.PI * Math.Pow(Width / 2, 2) * Height,
                    ROIShape.Cone => (1.0 / 3.0) * Math.PI * Math.Pow(Width / 2, 2) * Height,
                    ROIShape.Ellipsoid => (4.0 / 3.0) * Math.PI * (Width / 2) * (Height / 2) * (Depth / 2),
                    ROIShape.Torus => 2 * Math.PI * Math.PI * (Width / 2) * Math.Pow(Depth / 4, 2),
                    _ => 0
                };
            }
        }

        [JsonIgnore]
        public double SurfaceArea
        {
            get
            {
                return Shape switch
                {
                    ROIShape.Box => 2 * (Width * Height + Width * Depth + Height * Depth),
                    ROIShape.Sphere => 4 * Math.PI * Math.Pow(Width / 2, 2),
                    _ => 0 // Simplified
                };
            }
        }

        [JsonIgnore]
        public string Center => $"({X:F2}, {Y:F2}, {Z:F2})";

        // ============================================
        // Methods
        // ============================================

        /// <summary>
        /// Creates a deep copy of this ROI
        /// </summary>
        public ROI Clone()
        {
            return new ROI
            {
                Name = Name + "_Copy",
                Shape = Shape,
                X = X,
                Y = Y,
                Z = Z,
                Width = Width,
                Height = Height,
                Depth = Depth,
                RotationX = RotationX,
                RotationY = RotationY,
                RotationZ = RotationZ,
                Color = Color,
                Opacity = Opacity,
                Visible = Visible,
                Locked = Locked,
                ShowWireframe = ShowWireframe,
                Layer = Layer,
                Notes = Notes
            };
        }

        /// <summary>
        /// Checks if this ROI overlaps with another (simplified bounding box check)
        /// </summary>
        public bool OverlapsWith(ROI other)
        {
            // Simplified AABB collision detection
            double thisMinX = X - Width / 2;
            double thisMaxX = X + Width / 2;
            double thisMinY = Y - Height / 2;
            double thisMaxY = Y + Height / 2;
            double thisMinZ = Z - Depth / 2;
            double thisMaxZ = Z + Depth / 2;

            double otherMinX = other.X - other.Width / 2;
            double otherMaxX = other.X + other.Width / 2;
            double otherMinY = other.Y - other.Height / 2;
            double otherMaxY = other.Y + other.Height / 2;
            double otherMinZ = other.Z - other.Depth / 2;
            double otherMaxZ = other.Z + other.Depth / 2;

            return (thisMinX <= otherMaxX && thisMaxX >= otherMinX) &&
                   (thisMinY <= otherMaxY && thisMaxY >= otherMinY) &&
                   (thisMinZ <= otherMaxZ && thisMaxZ >= otherMinZ);
        }

        /// <summary>
        /// Gets the bounding box dimensions
        /// </summary>
        public (double minX, double maxX, double minY, double maxY, double minZ, double maxZ) GetBoundingBox()
        {
            return (
                X - Width / 2, X + Width / 2,
                Y - Height / 2, Y + Height / 2,
                Z - Depth / 2, Z + Depth / 2
            );
        }

        // ============================================
        // INotifyPropertyChanged Implementation
        // ============================================

        public event PropertyChangedEventHandler? PropertyChanged;

        protected void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            ModifiedAt = DateTime.Now;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        // ============================================
        // ToString Override
        // ============================================

        public override string ToString()
        {
            return $"{Name} ({Shape}) - {Center}";
        }
    }
}
