using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using Microsoft.Win32;

namespace Hard_ROI_3D
{
    public partial class MainWindow : Window
    {
        private List<ROI> roiList = new List<ROI>();
        private ROI? selectedROI = null;
        private int roiCounter = 1;
        private bool isUpdating = false;

        // Camera control
        private Point lastMousePosition;
        private bool isRotatingCamera = false;
        private double cameraAngle = 45;
        private double cameraElevation = 30;
        private double cameraDistance = 20;

        // Visibility flags
        private bool showGrid = true;
        private bool showAxes = true;
        private bool wireframeMode = false;

        public MainWindow()
        {
            InitializeComponent();
            CreateScene();
            UpdateCamera();

            // Add initial ROI
            AddDefaultROI();
        }

        private void CreateScene()
        {
            CreateGrid();
            CreateAxes();
        }

        private void CreateGrid()
        {
            var gridGroup = new Model3DGroup();

            for (int i = -10; i <= 10; i++)
            {
                gridGroup.Children.Add(CreateLine(
                    new Point3D(-10, 0, i), new Point3D(10, 0, i),
                    i % 5 == 0 ? Colors.Gray : Colors.DarkGray,
                    i % 5 == 0 ? 0.025 : 0.012));

                gridGroup.Children.Add(CreateLine(
                    new Point3D(i, 0, -10), new Point3D(i, 0, 10),
                    i % 5 == 0 ? Colors.Gray : Colors.DarkGray,
                    i % 5 == 0 ? 0.025 : 0.012));
            }

            gridModel.Content = gridGroup;
        }

        private void CreateAxes()
        {
            var axesGroup = new Model3DGroup();

            // X axis - Red
            axesGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(5, 0, 0), Colors.Red, 0.08));
            axesGroup.Children.Add(CreateCone(new Point3D(5, 0, 0), new Vector3D(1, 0, 0), 0.2, 0.4, Colors.Red));

            // Y axis - Green
            axesGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(0, 5, 0), Colors.Green, 0.08));
            axesGroup.Children.Add(CreateCone(new Point3D(0, 5, 0), new Vector3D(0, 1, 0), 0.2, 0.4, Colors.Green));

            // Z axis - Blue
            axesGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(0, 0, 5), Colors.Blue, 0.08));
            axesGroup.Children.Add(CreateCone(new Point3D(0, 0, 5), new Vector3D(0, 0, 1), 0.2, 0.4, Colors.Blue));

            axesModel.Content = axesGroup;
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

            mesh.Positions.Add(p1); mesh.Positions.Add(p2);
            mesh.Positions.Add(p3); mesh.Positions.Add(p4);

            mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(2);
            mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(3); mesh.TriangleIndices.Add(2);

            return new GeometryModel3D(mesh, new DiffuseMaterial(new SolidColorBrush(color)));
        }

        private GeometryModel3D CreateCone(Point3D tip, Vector3D direction, double radius, double height, Color color)
        {
            var mesh = CreateConeMesh(radius, height, 12);
            var material = new DiffuseMaterial(new SolidColorBrush(color));
            var model = new GeometryModel3D(mesh, material);

            // Orient the cone
            var transform = new Transform3DGroup();
            var rotationAxis = Vector3D.CrossProduct(new Vector3D(0, 1, 0), direction);
            if (rotationAxis.Length > 0.001)
            {
                var angle = Vector3D.AngleBetween(new Vector3D(0, 1, 0), direction);
                transform.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(rotationAxis, angle)));
            }
            transform.Children.Add(new TranslateTransform3D(tip.X, tip.Y, tip.Z));
            model.Transform = transform;

            return model;
        }

        private void AddDefaultROI()
        {
            var roi = new ROI($"ROI_{roiCounter++}");
            roiList.Add(roi);
            roiListBox.Items.Add(roi);
            roiListBox.SelectedItem = roi;
            UpdateAllROIs();
            UpdateStatusBar();
        }

        private void UpdateAllROIs()
        {
            var group = new Model3DGroup();

            foreach (var roi in roiList)
            {
                if (roi.Visible)
                {
                    var roiModel = CreateROIModel(roi);
                    if (roiModel != null)
                        group.Children.Add(roiModel);
                }
            }

            roiContainer.Content = group;
        }

        private Model3D CreateROIModel(ROI roi)
        {
            var group = new Model3DGroup();

            MeshGeometry3D mesh = roi.Shape switch
            {
                ROIShape.Box => CreateBoxMesh(roi.Width, roi.Height, roi.Depth),
                ROIShape.Sphere => CreateSphereMesh(roi.Width / 2, 24),
                ROIShape.Cylinder => CreateCylinderMesh(roi.Width / 2, roi.Height, 24),
                ROIShape.Cone => CreateConeMesh(roi.Width / 2, roi.Height, 24),
                ROIShape.Torus => CreateTorusMesh(roi.Width / 2, roi.Depth / 4, 20, 12),
                _ => CreateBoxMesh(roi.Width, roi.Height, roi.Depth)
            };

            var color = roi.Color;
            color.A = (byte)(roi.Opacity * 255);

            if (!wireframeMode)
            {
                var material = new DiffuseMaterial(new SolidColorBrush(color));
                var specular = new SpecularMaterial(new SolidColorBrush(Colors.White), 40);
                var materialGroup = new MaterialGroup();
                materialGroup.Children.Add(material);
                materialGroup.Children.Add(specular);

                var geometry = new GeometryModel3D(mesh, materialGroup);
                geometry.Transform = CreateROITransform(roi);
                group.Children.Add(geometry);
            }

            // Wireframe
            if (roi.ShowWireframe || wireframeMode)
            {
                var wireframe = CreateWireframeFromMesh(mesh, roi);
                if (wireframe != null)
                    group.Children.Add(wireframe);
            }

            return group;
        }

        private Transform3DGroup CreateROITransform(ROI roi)
        {
            var transformGroup = new Transform3DGroup();
            transformGroup.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(new Vector3D(1, 0, 0), roi.RotationX)));
            transformGroup.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(new Vector3D(0, 1, 0), roi.RotationY)));
            transformGroup.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(new Vector3D(0, 0, 1), roi.RotationZ)));
            transformGroup.Children.Add(new TranslateTransform3D(roi.X, roi.Y, roi.Z));
            return transformGroup;
        }

        private Model3D? CreateWireframeFromMesh(MeshGeometry3D mesh, ROI roi)
        {
            var wireGroup = new Model3DGroup();
            var edgeColor = roi.Color;
            edgeColor.A = 255;

            // Sample edges for wireframe (simplified)
            for (int i = 0; i < mesh.TriangleIndices.Count; i += 3)
            {
                var p1 = mesh.Positions[mesh.TriangleIndices[i]];
                var p2 = mesh.Positions[mesh.TriangleIndices[i + 1]];
                var p3 = mesh.Positions[mesh.TriangleIndices[i + 2]];

                if (i % 6 == 0) // Draw every other edge to reduce clutter
                {
                    var line1 = CreateLine(p1, p2, edgeColor, 0.015);
                    line1.Transform = CreateROITransform(roi);
                    wireGroup.Children.Add(line1);
                }
            }

            return wireGroup;
        }

        private MeshGeometry3D CreateBoxMesh(double width, double height, double depth)
        {
            var mesh = new MeshGeometry3D();
            double w = width / 2, h = height / 2, d = depth / 2;

            Point3D[] vertices = new[]
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
                    double x = Math.Cos(phi) * sinTheta;
                    double y = cosTheta;
                    double z = Math.Sin(phi) * sinTheta;

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

            mesh.Positions.Add(new Point3D(0, halfHeight, 0));
            mesh.Positions.Add(new Point3D(0, -halfHeight, 0));

            for (int i = 0; i <= divisions; i++)
            {
                double angle = 2 * Math.PI * i / divisions;
                double x = radius * Math.Cos(angle);
                double z = radius * Math.Sin(angle);

                mesh.Positions.Add(new Point3D(x, halfHeight, z));
                mesh.Positions.Add(new Point3D(x, -halfHeight, z));
            }

            for (int i = 0; i < divisions; i++)
            {
                mesh.TriangleIndices.Add(0);
                mesh.TriangleIndices.Add(2 + i * 2);
                mesh.TriangleIndices.Add(2 + (i + 1) * 2);

                mesh.TriangleIndices.Add(1);
                mesh.TriangleIndices.Add(3 + (i + 1) * 2);
                mesh.TriangleIndices.Add(3 + i * 2);

                mesh.TriangleIndices.Add(2 + i * 2);
                mesh.TriangleIndices.Add(3 + i * 2);
                mesh.TriangleIndices.Add(2 + (i + 1) * 2);
                mesh.TriangleIndices.Add(3 + i * 2);
                mesh.TriangleIndices.Add(3 + (i + 1) * 2);
                mesh.TriangleIndices.Add(2 + (i + 1) * 2);
            }

            return mesh;
        }

        private MeshGeometry3D CreateConeMesh(double radius, double height, int divisions)
        {
            var mesh = new MeshGeometry3D();
            mesh.Positions.Add(new Point3D(0, height / 2, 0)); // Tip
            mesh.Positions.Add(new Point3D(0, -height / 2, 0)); // Base center

            for (int i = 0; i <= divisions; i++)
            {
                double angle = 2 * Math.PI * i / divisions;
                double x = radius * Math.Cos(angle);
                double z = radius * Math.Sin(angle);
                mesh.Positions.Add(new Point3D(x, -height / 2, z));
            }

            // Tip triangles
            for (int i = 0; i < divisions; i++)
            {
                mesh.TriangleIndices.Add(0);
                mesh.TriangleIndices.Add(2 + i);
                mesh.TriangleIndices.Add(2 + (i + 1));
            }

            // Base triangles
            for (int i = 0; i < divisions; i++)
            {
                mesh.TriangleIndices.Add(1);
                mesh.TriangleIndices.Add(2 + (i + 1));
                mesh.TriangleIndices.Add(2 + i);
            }

            return mesh;
        }

        private MeshGeometry3D CreateTorusMesh(double majorRadius, double minorRadius, int majorDivisions, int minorDivisions)
        {
            var mesh = new MeshGeometry3D();

            for (int i = 0; i <= majorDivisions; i++)
            {
                double u = 2 * Math.PI * i / majorDivisions;
                for (int j = 0; j <= minorDivisions; j++)
                {
                    double v = 2 * Math.PI * j / minorDivisions;

                    double x = (majorRadius + minorRadius * Math.Cos(v)) * Math.Cos(u);
                    double y = minorRadius * Math.Sin(v);
                    double z = (majorRadius + minorRadius * Math.Cos(v)) * Math.Sin(u);

                    mesh.Positions.Add(new Point3D(x, y, z));
                }
            }

            for (int i = 0; i < majorDivisions; i++)
            {
                for (int j = 0; j < minorDivisions; j++)
                {
                    int first = i * (minorDivisions + 1) + j;
                    int second = first + minorDivisions + 1;

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

        // Event Handlers
        private void AddROI_Click(object sender, RoutedEventArgs e)
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
            UpdateStatusBar();
        }

        private Color GetColorForIndex(int index)
        {
            Color[] colors = { Colors.Red, Colors.Green, Colors.Blue, Colors.Yellow,
                             Colors.Cyan, Colors.Magenta, Colors.Orange, Colors.Purple };
            return colors[index % colors.Length];
        }

        private void RemoveROI_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null)
            {
                roiList.Remove(selectedROI);
                roiListBox.Items.Remove(selectedROI);
                selectedROI = roiList.FirstOrDefault();
                if (selectedROI != null)
                    roiListBox.SelectedItem = selectedROI;
                UpdateAllROIs();
                UpdateStatusBar();
            }
        }

        private void DuplicateROI_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null)
            {
                var newROI = new ROI($"ROI_{roiCounter++}")
                {
                    Shape = selectedROI.Shape,
                    X = selectedROI.X + 1,
                    Y = selectedROI.Y,
                    Z = selectedROI.Z + 1,
                    Width = selectedROI.Width,
                    Height = selectedROI.Height,
                    Depth = selectedROI.Depth,
                    RotationX = selectedROI.RotationX,
                    RotationY = selectedROI.RotationY,
                    RotationZ = selectedROI.RotationZ,
                    Color = selectedROI.Color,
                    Opacity = selectedROI.Opacity,
                    Visible = true,
                    ShowWireframe = selectedROI.ShowWireframe
                };

                roiList.Add(newROI);
                roiListBox.Items.Add(newROI);
                roiListBox.SelectedItem = newROI;
                UpdateAllROIs();
                UpdateStatusBar();
            }
        }

        private void MoveUp_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null)
            {
                int index = roiList.IndexOf(selectedROI);
                if (index > 0)
                {
                    roiList.RemoveAt(index);
                    roiList.Insert(index - 1, selectedROI);
                    roiListBox.Items.RemoveAt(index);
                    roiListBox.Items.Insert(index - 1, selectedROI);
                    roiListBox.SelectedItem = selectedROI;
                }
            }
        }

        private void MoveDown_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null)
            {
                int index = roiList.IndexOf(selectedROI);
                if (index < roiList.Count - 1)
                {
                    roiList.RemoveAt(index);
                    roiList.Insert(index + 1, selectedROI);
                    roiListBox.Items.RemoveAt(index);
                    roiListBox.Items.Insert(index + 1, selectedROI);
                    roiListBox.SelectedItem = selectedROI;
                }
            }
        }

        private void ROIList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (roiListBox.SelectedItem is ROI roi)
            {
                selectedROI = roi;
                LoadROIProperties(roi);
                UpdateROIInfo();
            }
        }

        private void LoadROIProperties(ROI roi)
        {
            isUpdating = true;

            txtName.Text = roi.Name;
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
            chkWireframe.IsChecked = roi.ShowWireframe;

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
            txtPosX.Text = sliderPosX.Value.ToString("F2");
            txtPosY.Text = sliderPosY.Value.ToString("F2");
            txtPosZ.Text = sliderPosZ.Value.ToString("F2");
            txtWidth.Text = sliderWidth.Value.ToString("F2");
            txtHeight.Text = sliderHeight.Value.ToString("F2");
            txtDepth.Text = sliderDepth.Value.ToString("F2");
            txtRotX.Text = sliderRotX.Value.ToString("F0") + "°";
            txtRotY.Text = sliderRotY.Value.ToString("F0") + "°";
            txtRotZ.Text = sliderRotZ.Value.ToString("F0") + "°";
            txtOpacity.Text = sliderOpacity.Value.ToString("F0") + "%";
        }

        private void UpdateROIInfo()
        {
            if (selectedROI != null)
            {
                txtVolume.Text = $"Volume: {selectedROI.GetVolume():F2}";
                txtCenter.Text = $"Center: ({selectedROI.X:F1}, {selectedROI.Y:F1}, {selectedROI.Z:F1})";
            }
        }

        private void Name_Changed(object sender, TextChangedEventArgs e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null) return;
            selectedROI.Name = txtName.Text;
            roiListBox.Items.Refresh();
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
            UpdateROIInfo();
            UpdateAllROIs();
        }

        private void Shape_Changed(object sender, SelectionChangedEventArgs e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null) return;
            selectedROI.Shape = (ROIShape)shapeCombo.SelectedIndex;
            UpdateAllROIs();
            UpdateROIInfo();
            roiListBox.Items.Refresh();
        }

        private void Color_Changed(object sender, SelectionChangedEventArgs e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null || colorCombo.SelectedItem == null) return;
            var item = (ComboBoxItem)colorCombo.SelectedItem;
            selectedROI.Color = (Color)ColorConverter.ConvertFromString(item.Tag.ToString()!);
            UpdateAllROIs();
            roiListBox.Items.Refresh();
        }

        private void Opacity_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null) return;
            selectedROI.Opacity = sliderOpacity.Value / 100.0;
            txtOpacity.Text = sliderOpacity.Value.ToString("F0") + "%";
            UpdateAllROIs();
        }

        private void Wireframe_Changed(object sender, RoutedEventArgs e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null) return;
            selectedROI.ShowWireframe = chkWireframe.IsChecked == true;
            UpdateAllROIs();
        }

        private void Visibility_Changed(object sender, RoutedEventArgs e)
        {
            UpdateAllROIs();
            UpdateStatusBar();
        }

        private void Clipping_Changed(object sender, RoutedEventArgs e)
        {
            UpdateClippingPlane();
        }

        private void ClipPlane_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!IsLoaded) return;
            txtClipY.Text = sliderClipY.Value.ToString("F2");
            UpdateClippingPlane();
        }

        private void UpdateClippingPlane()
        {
            if (chkEnableClipping?.IsChecked == true)
            {
                var mesh = CreateBoxMesh(20, 0.05, 20);
                var material = new DiffuseMaterial(new SolidColorBrush(Color.FromArgb(50, 255, 255, 0)));
                var model = new GeometryModel3D(mesh, material);
                var transform = new TranslateTransform3D(0, sliderClipY.Value, 0);
                model.Transform = transform;

                clippingPlaneModel.Content = model;
            }
            else
            {
                clippingPlaneModel.Content = null;
            }
        }

        // Camera Control
        private void Viewport_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            lastMousePosition = e.GetPosition(viewport3D);
            isRotatingCamera = true;
            viewport3D.CaptureMouse();
        }

        private void Viewport_MouseMove(object sender, MouseEventArgs e)
        {
            if (isRotatingCamera)
            {
                Point currentPosition = e.GetPosition(viewport3D);
                double dx = currentPosition.X - lastMousePosition.X;
                double dy = currentPosition.Y - lastMousePosition.Y;

                cameraAngle += dx * 0.5;
                cameraElevation -= dy * 0.5;

                cameraElevation = Math.Max(-89, Math.Min(89, cameraElevation));

                UpdateCamera();
                lastMousePosition = currentPosition;
            }
        }

        private void Viewport_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            isRotatingCamera = false;
            viewport3D.ReleaseMouseCapture();
        }

        private void Viewport_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            cameraDistance -= e.Delta * 0.01;
            cameraDistance = Math.Max(5, Math.Min(50, cameraDistance));
            UpdateCamera();
        }

        private void UpdateCamera()
        {
            double angleRad = cameraAngle * Math.PI / 180;
            double elevationRad = cameraElevation * Math.PI / 180;

            double x = cameraDistance * Math.Cos(elevationRad) * Math.Sin(angleRad);
            double y = cameraDistance * Math.Sin(elevationRad);
            double z = cameraDistance * Math.Cos(elevationRad) * Math.Cos(angleRad);

            camera.Position = new Point3D(x, y, z);
            camera.LookDirection = new Vector3D(-x, -y, -z);

            cameraInfoText.Text = $"Camera: {cameraAngle:F0}°, {cameraElevation:F0}°";
        }

        private void UpdateStatusBar()
        {
            int visibleCount = roiList.Count(r => r.Visible);
            statusText.Text = $"Ready | Camera: Orbit Mode | ROIs: {visibleCount}/{roiList.Count}";
            roiCountText.Text = $"ROIs: {roiList.Count}";
        }

        // Menu Handlers
        private void NewScene_Click(object sender, RoutedEventArgs e)
        {
            roiList.Clear();
            roiListBox.Items.Clear();
            roiCounter = 1;
            AddDefaultROI();
        }

        private void SaveScene_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new SaveFileDialog
            {
                Filter = "ROI Scene (*.json)|*.json",
                DefaultExt = "json"
            };

            if (dialog.ShowDialog() == true)
            {
                var json = JsonSerializer.Serialize(roiList, new JsonSerializerOptions { WriteIndented = true });
                File.WriteAllText(dialog.FileName, json);
                MessageBox.Show("Scene saved successfully!", "Save", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

        private void LoadScene_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new OpenFileDialog
            {
                Filter = "ROI Scene (*.json)|*.json",
                DefaultExt = "json"
            };

            if (dialog.ShowDialog() == true)
            {
                try
                {
                    var json = File.ReadAllText(dialog.FileName);
                    var loadedROIs = JsonSerializer.Deserialize<List<ROI>>(json);

                    if (loadedROIs != null)
                    {
                        roiList.Clear();
                        roiListBox.Items.Clear();

                        foreach (var roi in loadedROIs)
                        {
                            roiList.Add(roi);
                            roiListBox.Items.Add(roi);
                        }

                        if (roiList.Count > 0)
                            roiListBox.SelectedIndex = 0;

                        UpdateAllROIs();
                        UpdateStatusBar();
                        MessageBox.Show("Scene loaded successfully!", "Load", MessageBoxButton.OK, MessageBoxImage.Information);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error loading scene: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private void ExportView_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Export functionality would save a screenshot here.", "Export", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void ToggleGrid_Click(object sender, RoutedEventArgs e)
        {
            showGrid = !showGrid;
            gridModel.Content = showGrid ? gridModel.Content : null;
            if (showGrid) CreateGrid();
        }

        private void ToggleAxes_Click(object sender, RoutedEventArgs e)
        {
            showAxes = !showAxes;
            axesModel.Content = showAxes ? axesModel.Content : null;
            if (showAxes) CreateAxes();
        }

        private void ToggleWireframe_Click(object sender, RoutedEventArgs e)
        {
            wireframeMode = !wireframeMode;
            UpdateAllROIs();
        }

        private void MeasureDistance_Click(object sender, RoutedEventArgs e)
        {
            if (roiList.Count >= 2)
            {
                var roi1 = roiList[0];
                var roi2 = roiList[1];
                double distance = Math.Sqrt(
                    Math.Pow(roi2.X - roi1.X, 2) +
                    Math.Pow(roi2.Y - roi1.Y, 2) +
                    Math.Pow(roi2.Z - roi1.Z, 2));

                MessageBox.Show($"Distance between {roi1.Name} and {roi2.Name}: {distance:F2}",
                    "Measurement", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            else
            {
                MessageBox.Show("Need at least 2 ROIs to measure distance.", "Measurement",
                    MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }

        private void ShowStats_Click(object sender, RoutedEventArgs e)
        {
            double totalVolume = roiList.Where(r => r.Visible).Sum(r => r.GetVolume());
            string stats = $"Total ROIs: {roiList.Count}\n" +
                          $"Visible ROIs: {roiList.Count(r => r.Visible)}\n" +
                          $"Total Volume: {totalVolume:F2}\n\n" +
                          $"Shapes:\n" +
                          $"  Boxes: {roiList.Count(r => r.Shape == ROIShape.Box)}\n" +
                          $"  Spheres: {roiList.Count(r => r.Shape == ROIShape.Sphere)}\n" +
                          $"  Cylinders: {roiList.Count(r => r.Shape == ROIShape.Cylinder)}\n" +
                          $"  Cones: {roiList.Count(r => r.Shape == ROIShape.Cone)}\n" +
                          $"  Tori: {roiList.Count(r => r.Shape == ROIShape.Torus)}";

            MessageBox.Show(stats, "Statistics", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
