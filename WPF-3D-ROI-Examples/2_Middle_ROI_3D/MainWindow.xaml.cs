using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Middle_ROI_3D
{
    public partial class MainWindow : Window
    {
        private List<ROI> roiList = new List<ROI>();
        private ROI? selectedROI = null;
        private int roiCounter = 1;
        private bool isUpdating = false;

        public MainWindow()
        {
            InitializeComponent();
            CreateGrid();

            // Add initial ROI
            AddROI_Click(null, null);
        }

        private void CreateGrid()
        {
            var gridGroup = new Model3DGroup();

            // Create grid lines
            for (int i = -10; i <= 10; i++)
            {
                gridGroup.Children.Add(CreateLine(
                    new Point3D(-10, 0, i), new Point3D(10, 0, i),
                    i == 0 ? Colors.DimGray : Colors.DarkGray, i == 0 ? 0.03 : 0.015));

                gridGroup.Children.Add(CreateLine(
                    new Point3D(i, 0, -10), new Point3D(i, 0, 10),
                    i == 0 ? Colors.DimGray : Colors.DarkGray, i == 0 ? 0.03 : 0.015));
            }

            // Axis lines
            gridGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(4, 0, 0), Colors.Red, 0.06));
            gridGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(0, 4, 0), Colors.Green, 0.06));
            gridGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(0, 0, 4), Colors.Blue, 0.06));

            gridModel.Content = gridGroup;
        }

        private GeometryModel3D CreateLine(Point3D start, Point3D end, Color color, double thickness)
        {
            var mesh = new MeshGeometry3D();
            var direction = end - start;
            direction.Normalize();

            var perpendicular = Vector3D.CrossProduct(direction, new Vector3D(0, 1, 0));
            if (perpendicular.Length < 0.001)
                perpendicular = Vector3D.CrossProduct(direction, new Vector3D(1, 0, 0));
            perpendicular.Normalize();

            var p1 = start + perpendicular * thickness;
            var p2 = start - perpendicular * thickness;
            var p3 = end + perpendicular * thickness;
            var p4 = end - perpendicular * thickness;

            mesh.Positions.Add(p1);
            mesh.Positions.Add(p2);
            mesh.Positions.Add(p3);
            mesh.Positions.Add(p4);

            mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(2);
            mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(3); mesh.TriangleIndices.Add(2);

            return new GeometryModel3D(mesh, new DiffuseMaterial(new SolidColorBrush(color)));
        }

        private void UpdateAllROIs()
        {
            var group = new Model3DGroup();

            foreach (var roi in roiList)
            {
                var roiModel = CreateROIModel(roi);
                if (roiModel != null)
                    group.Children.Add(roiModel);
            }

            roiContainer.Content = group;
        }

        private Model3D CreateROIModel(ROI roi)
        {
            var group = new Model3DGroup();

            MeshGeometry3D mesh;
            switch (roi.Shape)
            {
                case ROIShape.Box:
                    mesh = CreateBoxMesh(roi.Width, roi.Height, roi.Depth);
                    break;
                case ROIShape.Sphere:
                    mesh = CreateSphereMesh(roi.Width / 2, 20);
                    break;
                case ROIShape.Cylinder:
                    mesh = CreateCylinderMesh(roi.Width / 2, roi.Height, 20);
                    break;
                default:
                    return group;
            }

            var color = roi.Color;
            color.A = (byte)(roi.Opacity * 255);
            var material = new DiffuseMaterial(new SolidColorBrush(color));
            var specular = new SpecularMaterial(new SolidColorBrush(Colors.White), 30);
            var materialGroup = new MaterialGroup();
            materialGroup.Children.Add(material);
            materialGroup.Children.Add(specular);

            var geometry = new GeometryModel3D(mesh, materialGroup);

            // Apply transformations
            var transformGroup = new Transform3DGroup();
            transformGroup.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(new Vector3D(1, 0, 0), roi.RotationX)));
            transformGroup.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(new Vector3D(0, 1, 0), roi.RotationY)));
            transformGroup.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(new Vector3D(0, 0, 1), roi.RotationZ)));
            transformGroup.Children.Add(new TranslateTransform3D(roi.X, roi.Y, roi.Z));
            geometry.Transform = transformGroup;

            group.Children.Add(geometry);

            return group;
        }

        private MeshGeometry3D CreateBoxMesh(double width, double height, double depth)
        {
            var mesh = new MeshGeometry3D();
            double w = width / 2, h = height / 2, d = depth / 2;

            Point3D[] vertices = new Point3D[]
            {
                new Point3D(-w, -h, -d), new Point3D(w, -h, -d), new Point3D(w, h, -d), new Point3D(-w, h, -d),
                new Point3D(-w, -h, d), new Point3D(w, -h, d), new Point3D(w, h, d), new Point3D(-w, h, d)
            };

            foreach (var v in vertices) mesh.Positions.Add(v);

            int[] indices = { 0,1,2, 0,2,3, 5,4,7, 5,7,6, 4,0,3, 4,3,7, 1,5,6, 1,6,2, 3,2,6, 3,6,7, 4,5,1, 4,1,0 };
            foreach (var i in indices) mesh.TriangleIndices.Add(i);

            return mesh;
        }

        private MeshGeometry3D CreateSphereMesh(double radius, int divisions)
        {
            var mesh = new MeshGeometry3D();

            for (int lat = 0; lat <= divisions; lat++)
            {
                double theta = lat * Math.PI / divisions;
                double sinTheta = Math.Sin(theta);
                double cosTheta = Math.Cos(theta);

                for (int lon = 0; lon <= divisions; lon++)
                {
                    double phi = lon * 2 * Math.PI / divisions;
                    double sinPhi = Math.Sin(phi);
                    double cosPhi = Math.Cos(phi);

                    double x = cosPhi * sinTheta;
                    double y = cosTheta;
                    double z = sinPhi * sinTheta;

                    mesh.Positions.Add(new Point3D(x * radius, y * radius, z * radius));
                }
            }

            for (int lat = 0; lat < divisions; lat++)
            {
                for (int lon = 0; lon < divisions; lon++)
                {
                    int first = (lat * (divisions + 1)) + lon;
                    int second = first + divisions + 1;

                    mesh.TriangleIndices.Add(first);
                    mesh.TriangleIndices.Add(second);
                    mesh.TriangleIndices.Add(first + 1);

                    mesh.TriangleIndices.Add(second);
                    mesh.TriangleIndices.Add(second + 1);
                    mesh.TriangleIndices.Add(first + 1);
                }
            }

            return mesh;
        }

        private MeshGeometry3D CreateCylinderMesh(double radius, double height, int divisions)
        {
            var mesh = new MeshGeometry3D();
            double halfHeight = height / 2;

            // Top and bottom centers
            mesh.Positions.Add(new Point3D(0, halfHeight, 0));
            mesh.Positions.Add(new Point3D(0, -halfHeight, 0));

            // Create vertices
            for (int i = 0; i <= divisions; i++)
            {
                double angle = 2 * Math.PI * i / divisions;
                double x = radius * Math.Cos(angle);
                double z = radius * Math.Sin(angle);

                mesh.Positions.Add(new Point3D(x, halfHeight, z));
                mesh.Positions.Add(new Point3D(x, -halfHeight, z));
            }

            // Top cap
            for (int i = 0; i < divisions; i++)
            {
                mesh.TriangleIndices.Add(0);
                mesh.TriangleIndices.Add(2 + i * 2);
                mesh.TriangleIndices.Add(2 + (i + 1) * 2);
            }

            // Bottom cap
            for (int i = 0; i < divisions; i++)
            {
                mesh.TriangleIndices.Add(1);
                mesh.TriangleIndices.Add(3 + (i + 1) * 2);
                mesh.TriangleIndices.Add(3 + i * 2);
            }

            // Sides
            for (int i = 0; i < divisions; i++)
            {
                int topCurrent = 2 + i * 2;
                int bottomCurrent = 3 + i * 2;
                int topNext = 2 + (i + 1) * 2;
                int bottomNext = 3 + (i + 1) * 2;

                mesh.TriangleIndices.Add(topCurrent);
                mesh.TriangleIndices.Add(bottomCurrent);
                mesh.TriangleIndices.Add(topNext);

                mesh.TriangleIndices.Add(bottomCurrent);
                mesh.TriangleIndices.Add(bottomNext);
                mesh.TriangleIndices.Add(topNext);
            }

            return mesh;
        }

        private void AddROI_Click(object? sender, RoutedEventArgs? e)
        {
            var roi = new ROI($"ROI_{roiCounter++}")
            {
                X = (roiList.Count % 3 - 1) * 3,
                Z = (roiList.Count / 3 - 1) * 3,
                Color = GetColorForIndex(roiList.Count)
            };

            roiList.Add(roi);
            roiListBox.Items.Add(roi);
            roiListBox.SelectedItem = roi;

            UpdateAllROIs();
        }

        private Color GetColorForIndex(int index)
        {
            Color[] colors = { Colors.Red, Colors.Green, Colors.Blue, Colors.Yellow,
                             Colors.Cyan, Colors.Magenta, Colors.Orange, Colors.Purple };
            return colors[index % colors.Length];
        }

        private void RemoveROI_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null && roiList.Count > 1)
            {
                roiList.Remove(selectedROI);
                roiListBox.Items.Remove(selectedROI);
                selectedROI = null;
                UpdateAllROIs();
            }
        }

        private void ROIList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (roiListBox.SelectedItem is ROI roi)
            {
                selectedROI = roi;
                LoadROIProperties(roi);
            }
        }

        private void LoadROIProperties(ROI roi)
        {
            isUpdating = true;

            shapeCombo.SelectedIndex = (int)roi.Shape;
            sliderPosX.Value = roi.X;
            sliderPosY.Value = roi.Y;
            sliderPosZ.Value = roi.Z;
            sliderWidth.Value = roi.Width;
            sliderHeight.Value = roi.Height;
            sliderDepth.Value = roi.Depth;
            sliderRotX.Value = roi.RotationX;
            sliderRotY.Value = roi.RotationY;
            sliderRotZ.Value = roi.RotationZ;
            sliderOpacity.Value = roi.Opacity * 100;

            // Set color combo
            for (int i = 0; i < colorCombo.Items.Count; i++)
            {
                var item = (ComboBoxItem)colorCombo.Items[i];
                var itemColor = (Color)ColorConverter.ConvertFromString(item.Tag.ToString()!);
                if (itemColor.R == roi.Color.R && itemColor.G == roi.Color.G && itemColor.B == roi.Color.B)
                {
                    colorCombo.SelectedIndex = i;
                    break;
                }
            }

            UpdatePropertyDisplay();
            isUpdating = false;
        }

        private void UpdatePropertyDisplay()
        {
            txtPosX.Text = sliderPosX.Value.ToString("F1");
            txtPosY.Text = sliderPosY.Value.ToString("F1");
            txtPosZ.Text = sliderPosZ.Value.ToString("F1");
            txtWidth.Text = sliderWidth.Value.ToString("F1");
            txtHeight.Text = sliderHeight.Value.ToString("F1");
            txtDepth.Text = sliderDepth.Value.ToString("F1");
            txtRotX.Text = sliderRotX.Value.ToString("F0") + "°";
            txtRotY.Text = sliderRotY.Value.ToString("F0") + "°";
            txtRotZ.Text = sliderRotZ.Value.ToString("F0") + "°";
            txtOpacity.Text = sliderOpacity.Value.ToString("F0") + "%";
        }

        private void Property_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null) return;

            selectedROI.X = sliderPosX.Value;
            selectedROI.Y = sliderPosY.Value;
            selectedROI.Z = sliderPosZ.Value;
            selectedROI.Width = sliderWidth.Value;
            selectedROI.Height = sliderHeight.Value;
            selectedROI.Depth = sliderDepth.Value;
            selectedROI.RotationX = sliderRotX.Value;
            selectedROI.RotationY = sliderRotY.Value;
            selectedROI.RotationZ = sliderRotZ.Value;

            UpdatePropertyDisplay();
            UpdateAllROIs();
        }

        private void Shape_Changed(object sender, SelectionChangedEventArgs e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null) return;

            selectedROI.Shape = (ROIShape)shapeCombo.SelectedIndex;
            UpdateAllROIs();
            roiListBox.Items.Refresh();
        }

        private void Color_Changed(object sender, SelectionChangedEventArgs e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null || colorCombo.SelectedItem == null) return;

            var item = (ComboBoxItem)colorCombo.SelectedItem;
            selectedROI.Color = (Color)ColorConverter.ConvertFromString(item.Tag.ToString()!);
            UpdateAllROIs();
        }

        private void Opacity_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null) return;

            selectedROI.Opacity = sliderOpacity.Value / 100.0;
            txtOpacity.Text = sliderOpacity.Value.ToString("F0") + "%";
            UpdateAllROIs();
        }

        private void Camera_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!IsLoaded) return;

            double angle = sliderCameraAngle.Value * Math.PI / 180.0;
            double distance = 15;
            double height = sliderCameraHeight.Value;

            camera.Position = new Point3D(
                Math.Sin(angle) * distance,
                height,
                Math.Cos(angle) * distance);

            camera.LookDirection = new Vector3D(
                -camera.Position.X,
                -camera.Position.Y,
                -camera.Position.Z);

            txtCameraAngle.Text = sliderCameraAngle.Value.ToString("F0") + "°";
            txtCameraHeight.Text = height.ToString("F1");
        }
    }
}
