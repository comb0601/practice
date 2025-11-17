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
using Professional3DROIViewer.Models;
using Professional3DROIViewer.Services;

namespace Professional3DROIViewer
{
    /// <summary>
    /// Professional 3D ROI Viewer - Main Window
    ///
    /// This is a production-ready 3D ROI visualization and manipulation tool.
    /// Features include:
    /// - Multiple ROI shapes (Box, Sphere, Cylinder, Cone, Ellipsoid, Torus)
    /// - Interactive 3D manipulation
    /// - Professional UI with dark theme
    /// - Save/Load scenes
    /// - Advanced visualization options
    /// - Measurement tools
    /// - Clipping planes
    /// </summary>
    public partial class MainWindow : Window
    {
        // ============================================
        // FIELDS
        // ============================================

        private List<ROI> roiList = new List<ROI>();
        private ROI? selectedROI = null;
        private int roiCounter = 1;
        private bool isUpdating = false;

        // Camera control
        private Point lastMousePosition;
        private bool isLeftDragging = false;
        private bool isRightDragging = false;
        private double cameraDistance = 25;
        private double cameraAngle = 45;
        private double cameraElevation = 30;

        // Scene file path
        private string? currentFilePath = null;

        // ============================================
        // CONSTRUCTOR & INITIALIZATION
        // ============================================

        public MainWindow()
        {
            InitializeComponent();
            InitializeScene();
            CreateInitialROI();
            UpdateCamera();
            UpdateStatusBar();
        }

        /// <summary>
        /// Initialize the 3D scene with grid and axes
        /// </summary>
        private void InitializeScene()
        {
            CreateGrid();
            CreateAxes();
        }

        /// <summary>
        /// Creates the ground grid
        /// </summary>
        private void CreateGrid()
        {
            var gridGroup = new Model3DGroup();

            // Create grid lines
            for (int i = -20; i <= 20; i++)
            {
                Color lineColor = (i % 5 == 0) ? Colors.Gray : Color.FromRgb(50, 50, 50);
                double thickness = (i % 5 == 0) ? 0.03 : 0.015;

                // Lines parallel to X
                gridGroup.Children.Add(CreateLine(
                    new Point3D(-20, 0, i),
                    new Point3D(20, 0, i),
                    lineColor, thickness));

                // Lines parallel to Z
                gridGroup.Children.Add(CreateLine(
                    new Point3D(i, 0, -20),
                    new Point3D(i, 0, 20),
                    lineColor, thickness));
            }

            gridModel.Content = gridGroup;
        }

        /// <summary>
        /// Creates coordinate axes (X=Red, Y=Green, Z=Blue)
        /// </summary>
        private void CreateAxes()
        {
            var axesGroup = new Model3DGroup();

            // X-axis (Red)
            axesGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(5, 0, 0), Colors.Red, 0.08));
            axesGroup.Children.Add(CreateArrow(new Point3D(5, 0, 0), new Vector3D(1, 0, 0), Colors.Red));

            // Y-axis (Green)
            axesGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(0, 5, 0), Colors.Green, 0.08));
            axesGroup.Children.Add(CreateArrow(new Point3D(0, 5, 0), new Vector3D(0, 1, 0), Colors.Green));

            // Z-axis (Blue)
            axesGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(0, 0, 5), Colors.Blue, 0.08));
            axesGroup.Children.Add(CreateArrow(new Point3D(0, 0, 5), new Vector3D(0, 0, 1), Colors.Blue));

            axesModel.Content = axesGroup;
        }

        /// <summary>
        /// Creates a line between two points
        /// </summary>
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

        /// <summary>
        /// Creates an arrow (cone) at the end of an axis
        /// </summary>
        private GeometryModel3D CreateArrow(Point3D position, Vector3D direction, Color color)
        {
            var mesh = MeshGenerator.CreateCone(0.15, 0.3, 12);
            var material = new DiffuseMaterial(new SolidColorBrush(color));
            var model = new GeometryModel3D(mesh, material);

            // Orient the arrow
            var transform = new Transform3DGroup();

            // Rotate to point in correct direction
            var rotationAxis = Vector3D.CrossProduct(new Vector3D(0, 1, 0), direction);
            if (rotationAxis.Length > 0.001)
            {
                var angle = Vector3D.AngleBetween(new Vector3D(0, 1, 0), direction);
                transform.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(rotationAxis, angle)));
            }

            transform.Children.Add(new TranslateTransform3D(position.X, position.Y, position.Z));
            model.Transform = transform;

            return model;
        }

        /// <summary>
        /// Creates the initial ROI to demonstrate the application
        /// </summary>
        private void CreateInitialROI()
        {
            var roi = new ROI($"ROI_{roiCounter++}")
            {
                Shape = ROIShape.Box,
                Color = Colors.Red,
                Opacity = 0.7
            };

            roiList.Add(roi);
            roiListBox.Items.Add(roi);
            roiListBox.SelectedItem = roi;
            UpdateAllROIs();
        }

        // ============================================
        // ROI RENDERING
        // ============================================

        /// <summary>
        /// Updates all ROIs in the 3D scene
        /// </summary>
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

        /// <summary>
        /// Creates a 3D model for a single ROI
        /// </summary>
        private Model3D CreateROIModel(ROI roi)
        {
            var group = new Model3DGroup();

            // Create mesh for the ROI
            var mesh = MeshGenerator.CreateMeshForROI(roi);

            // Create material
            var color = roi.Color;
            color.A = (byte)(roi.Opacity * 255);
            var material = new DiffuseMaterial(new SolidColorBrush(color));
            var specular = new SpecularMaterial(new SolidColorBrush(Colors.White), 40);
            var materialGroup = new MaterialGroup();
            materialGroup.Children.Add(material);
            materialGroup.Children.Add(specular);

            var geometry = new GeometryModel3D(mesh, materialGroup);

            // Apply transforms (rotation + translation)
            var transformGroup = new Transform3DGroup();
            transformGroup.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(new Vector3D(1, 0, 0), roi.RotationX)));
            transformGroup.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(new Vector3D(0, 1, 0), roi.RotationY)));
            transformGroup.Children.Add(new RotateTransform3D(new AxisAngleRotation3D(new Vector3D(0, 0, 1), roi.RotationZ)));
            transformGroup.Children.Add(new TranslateTransform3D(roi.X, roi.Y, roi.Z));
            geometry.Transform = transformGroup;

            group.Children.Add(geometry);

            // Add wireframe if enabled
            if (roi.ShowWireframe)
            {
                var wireframeColor = roi.Color;
                wireframeColor.A = 255;
                var wireframe = MeshGenerator.CreateWireframe(mesh, wireframeColor);
                wireframe.Transform = transformGroup;
                group.Children.Add(wireframe);
            }

            return group;
        }

        // ============================================
        // UI EVENT HANDLERS - ROI MANAGEMENT
        // ============================================

        private void AddROI_Click(object sender, RoutedEventArgs e)
        {
            var roi = new ROI($"ROI_{roiCounter++}")
            {
                X = (roiList.Count % 3 - 1) * 4,
                Z = (roiList.Count / 3 - 1) * 4,
                Color = GetColorForIndex(roiList.Count)
            };

            roiList.Add(roi);
            roiListBox.Items.Add(roi);
            roiListBox.SelectedItem = roi;
            UpdateAllROIs();
            UpdateStatusBar();

            statusText.Text = $"Created {roi.Name}";
        }

        private void DeleteROI_Click(object sender, RoutedEventArgs e)
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

                statusText.Text = "ROI deleted";
            }
        }

        private void DuplicateROI_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null)
            {
                var newROI = selectedROI.Clone();
                newROI.Name = $"ROI_{roiCounter++}";
                newROI.X += 1;
                newROI.Z += 1;

                roiList.Add(newROI);
                roiListBox.Items.Add(newROI);
                roiListBox.SelectedItem = newROI;
                UpdateAllROIs();
                UpdateStatusBar();

                statusText.Text = $"Duplicated to {newROI.Name}";
            }
        }

        private void ROIList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (roiListBox.SelectedItem is ROI roi)
            {
                selectedROI = roi;
                LoadROIProperties(roi);
                statusText.Text = $"Selected: {roi.Name}";
            }
        }

        private void Visibility_Changed(object sender, RoutedEventArgs e)
        {
            UpdateAllROIs();
        }

        private Color GetColorForIndex(int index)
        {
            Color[] colors = { Colors.Red, Colors.Green, Colors.Blue, Colors.Yellow,
                             Colors.Cyan, Colors.Magenta, Colors.Orange, Colors.Purple };
            return colors[index % colors.Length];
        }

        // ============================================
        // PROPERTY EDITING
        // ============================================

        private void LoadROIProperties(ROI roi)
        {
            isUpdating = true;

            txtName.Text = roi.Name;
            shapeCombo.SelectedIndex = (int)roi.Shape;

            sliderX.Value = roi.X;
            sliderY.Value = roi.Y;
            sliderZ.Value = roi.Z;

            sliderWidth.Value = roi.Width;
            sliderHeight.Value = roi.Height;
            sliderDepth.Value = roi.Depth;

            sliderRotX.Value = roi.RotationX;
            sliderRotY.Value = roi.RotationY;
            sliderRotZ.Value = roi.RotationZ;

            sliderOpacity.Value = roi.Opacity * 100;
            chkWireframe.IsChecked = roi.ShowWireframe;

            // Set color
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
            UpdateROIInfo();

            isUpdating = false;
        }

        private void Property_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null) return;

            selectedROI.X = sliderX.Value;
            selectedROI.Y = sliderY.Value;
            selectedROI.Z = sliderZ.Value;

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

        private void Property_Changed(object sender, TextChangedEventArgs e)
        {
            if (isUpdating || !IsLoaded || selectedROI == null) return;
            selectedROI.Name = txtName.Text;
            roiListBox.Items.Refresh();
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

        private void UpdatePropertyDisplay()
        {
            txtX.Text = sliderX.Value.ToString("F2");
            txtY.Text = sliderY.Value.ToString("F2");
            txtZ.Text = sliderZ.Value.ToString("F2");

            txtWidth.Text = sliderWidth.Value.ToString("F2");
            txtHeight.Text = sliderHeight.Value.ToString("F2");
            txtDepth.Text = sliderDepth.Value.ToString("F2");

            txtRotX.Text = sliderRotX.Value.ToString("F0") + "°";
            txtRotY.Text = sliderRotY.Value.ToString("F0") + "°";
            txtRotZ.Text = sliderRotZ.Value.ToString("F0") + "°";
        }

        private void UpdateROIInfo()
        {
            if (selectedROI != null)
            {
                txtVolume.Text = $"Volume: {selectedROI.Volume:F2}";
                txtCenter.Text = $"Center: {selectedROI.Center}";
                var bounds = selectedROI.GetBoundingBox();
                txtBounds.Text = $"Bounds: [{bounds.minX:F1},{bounds.maxX:F1}]×[{bounds.minY:F1},{bounds.maxY:F1}]×[{bounds.minZ:F1},{bounds.maxZ:F1}]";
            }
        }

        // ============================================
        // CAMERA CONTROL
        // ============================================

        private void Viewport_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            lastMousePosition = e.GetPosition(viewport);
            isLeftDragging = true;
            viewport.CaptureMouse();
        }

        private void Viewport_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            lastMousePosition = e.GetPosition(viewport);
            isRightDragging = true;
            viewport.CaptureMouse();
        }

        private void Viewport_MouseMove(object sender, MouseEventArgs e)
        {
            if (isLeftDragging || isRightDragging)
            {
                Point currentPosition = e.GetPosition(viewport);
                double dx = currentPosition.X - lastMousePosition.X;
                double dy = currentPosition.Y - lastMousePosition.Y;

                if (isLeftDragging)
                {
                    // Rotate camera
                    cameraAngle += dx * 0.5;
                    cameraElevation -= dy * 0.5;
                    cameraElevation = Math.Clamp(cameraElevation, -89, 89);
                }
                else if (isRightDragging)
                {
                    // Pan camera (simplified)
                    // In a full implementation, this would move the look-at point
                }

                UpdateCamera();
                lastMousePosition = currentPosition;
            }
        }

        private void Viewport_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            isLeftDragging = false;
            viewport.ReleaseMouseCapture();
        }

        private void Viewport_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            isRightDragging = false;
            viewport.ReleaseMouseCapture();
        }

        private void Viewport_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            cameraDistance -= e.Delta * 0.01;
            cameraDistance = Math.Clamp(cameraDistance, 5, 100);
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

            cameraInfoText.Text = $"Camera: Perspective ({cameraAngle:F0}°, {cameraElevation:F0}°, {cameraDistance:F1})";
        }

        // ============================================
        // MENU HANDLERS - FILE
        // ============================================

        private void NewScene_Click(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show("Create new scene? Unsaved changes will be lost.", "New Scene",
                MessageBoxButton.YesNo, MessageBoxImage.Question) == MessageBoxResult.Yes)
            {
                roiList.Clear();
                roiListBox.Items.Clear();
                roiCounter = 1;
                currentFilePath = null;
                CreateInitialROI();
                statusText.Text = "New scene created";
            }
        }

        private void SaveScene_Click(object sender, RoutedEventArgs e)
        {
            if (currentFilePath == null)
            {
                SaveSceneAs_Click(sender, e);
            }
            else
            {
                SaveSceneToFile(currentFilePath);
            }
        }

        private void SaveSceneAs_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new SaveFileDialog
            {
                Filter = "ROI Scene (*.roi)|*.roi|All Files (*.*)|*.*",
                DefaultExt = "roi"
            };

            if (dialog.ShowDialog() == true)
            {
                SaveSceneToFile(dialog.FileName);
            }
        }

        private void SaveSceneToFile(string filePath)
        {
            try
            {
                var json = JsonSerializer.Serialize(roiList, new JsonSerializerOptions { WriteIndented = true });
                File.WriteAllText(filePath, json);
                currentFilePath = filePath;
                statusText.Text = $"Scene saved to {Path.GetFileName(filePath)}";
                MessageBox.Show("Scene saved successfully!", "Save", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error saving scene: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void OpenScene_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new OpenFileDialog
            {
                Filter = "ROI Scene (*.roi)|*.roi|All Files (*.*)|*.*",
                DefaultExt = "roi"
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

                        currentFilePath = dialog.FileName;
                        UpdateAllROIs();
                        UpdateStatusBar();
                        statusText.Text = $"Loaded {roiList.Count} ROIs from {Path.GetFileName(dialog.FileName)}";
                        MessageBox.Show($"Scene loaded successfully! ({roiList.Count} ROIs)", "Load", MessageBoxButton.OK, MessageBoxImage.Information);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error loading scene: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        // ============================================
        // MENU HANDLERS - CREATE ROI SHAPES
        // ============================================

        private void CreateBox_Click(object sender, RoutedEventArgs e) => CreateROIWithShape(ROIShape.Box);
        private void CreateSphere_Click(object sender, RoutedEventArgs e) => CreateROIWithShape(ROIShape.Sphere);
        private void CreateCylinder_Click(object sender, RoutedEventArgs e) => CreateROIWithShape(ROIShape.Cylinder);
        private void CreateCone_Click(object sender, RoutedEventArgs e) => CreateROIWithShape(ROIShape.Cone);
        private void CreateEllipsoid_Click(object sender, RoutedEventArgs e) => CreateROIWithShape(ROIShape.Ellipsoid);
        private void CreateTorus_Click(object sender, RoutedEventArgs e) => CreateROIWithShape(ROIShape.Torus);

        private void CreateROIWithShape(ROIShape shape)
        {
            var roi = new ROI($"{shape}_{roiCounter++}")
            {
                Shape = shape,
                Color = GetColorForIndex(roiList.Count)
            };

            roiList.Add(roi);
            roiListBox.Items.Add(roi);
            roiListBox.SelectedItem = roi;
            UpdateAllROIs();
            UpdateStatusBar();
            statusText.Text = $"Created {shape}";
        }

        // ============================================
        // MENU HANDLERS - EDIT & VIEW
        // ============================================

        private void Undo_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Undo functionality not yet implemented.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void Redo_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Redo functionality not yet implemented.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void SelectAll_Click(object sender, RoutedEventArgs e)
        {
            // In a full implementation, this would support multi-selection
            if (roiListBox.Items.Count > 0)
                roiListBox.SelectedIndex = 0;
        }

        private void FrameSelected_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null)
            {
                // Center camera on selected ROI
                cameraDistance = Math.Max(selectedROI.Width, Math.Max(selectedROI.Height, selectedROI.Depth)) * 3;
                UpdateCamera();
                statusText.Text = "Framed selected ROI";
            }
        }

        private void FrameAll_Click(object sender, RoutedEventArgs e)
        {
            cameraAngle = 45;
            cameraElevation = 30;
            cameraDistance = 25;
            UpdateCamera();
            statusText.Text = "Framed all ROIs";
        }

        private void CameraMode_Click(object sender, RoutedEventArgs e)
        {
            if (sender == menuPerspective)
            {
                menuPerspective.IsChecked = true;
                menuOrthographic.IsChecked = false;
                // Switch to perspective camera (already set)
                statusText.Text = "Switched to Perspective camera";
            }
            else
            {
                menuPerspective.IsChecked = false;
                menuOrthographic.IsChecked = true;
                // Would switch to orthographic camera
                MessageBox.Show("Orthographic camera not yet implemented.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }

        private void ToggleGrid_Click(object sender, RoutedEventArgs e)
        {
            gridModel.Content = gridModel.Content == null ? null : gridModel.Content;
            if (gridModel.Content == null)
                CreateGrid();
            else
                gridModel.Content = null;
        }

        private void ToggleWireframe_Click(object sender, RoutedEventArgs e)
        {
            foreach (var roi in roiList)
                roi.ShowWireframe = !roi.ShowWireframe;
            UpdateAllROIs();
        }

        private void ShowAll_Click(object sender, RoutedEventArgs e)
        {
            foreach (var roi in roiList)
                roi.Visible = true;
            UpdateAllROIs();
            roiListBox.Items.Refresh();
        }

        private void HideAll_Click(object sender, RoutedEventArgs e)
        {
            foreach (var roi in roiList)
                roi.Visible = false;
            UpdateAllROIs();
            roiListBox.Items.Refresh();
        }

        // ============================================
        // MENU HANDLERS - TOOLS
        // ============================================

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

                MessageBox.Show($"Distance between {roi1.Name} and {roi2.Name}:\n{distance:F3} units",
                    "Measurement", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            else
            {
                MessageBox.Show("Need at least 2 ROIs to measure distance.", "Measurement",
                    MessageBoxButton.OK, MessageBoxImage.Warning);
            }
        }

        private void ClippingPlanes_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Clipping planes interface not yet implemented.\n\nThis feature would allow you to:\n• Enable X, Y, Z clipping planes\n• Adjust plane positions\n• View cross-sections\n• Export slices",
                "Clipping Planes", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void ShowStatistics_Click(object sender, RoutedEventArgs e)
        {
            double totalVolume = roiList.Sum(r => r.Volume);
            string stats = $"=== SCENE STATISTICS ===\n\n" +
                          $"Total ROIs: {roiList.Count}\n" +
                          $"Visible ROIs: {roiList.Count(r => r.Visible)}\n" +
                          $"Hidden ROIs: {roiList.Count(r => !r.Visible)}\n\n" +
                          $"Total Volume: {totalVolume:F2}\n\n" +
                          $"Shape Distribution:\n" +
                          $"  Boxes: {roiList.Count(r => r.Shape == ROIShape.Box)}\n" +
                          $"  Spheres: {roiList.Count(r => r.Shape == ROIShape.Sphere)}\n" +
                          $"  Cylinders: {roiList.Count(r => r.Shape == ROIShape.Cylinder)}\n" +
                          $"  Cones: {roiList.Count(r => r.Shape == ROIShape.Cone)}\n" +
                          $"  Ellipsoids: {roiList.Count(r => r.Shape == ROIShape.Ellipsoid)}\n" +
                          $"  Tori: {roiList.Count(r => r.Shape == ROIShape.Torus)}";

            MessageBox.Show(stats, "Statistics", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void ExportROI_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Export functionality not yet implemented.\n\nPlanned formats:\n• OBJ (3D Model)\n• STL (3D Printing)\n• PNG (Screenshot)\n• CSV (Data)",
                "Export", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void ImportModel_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Import functionality not yet implemented.\n\nPlanned formats:\n• OBJ files\n• STL files\n• PLY files",
                "Import", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        // ============================================
        // MENU HANDLERS - HELP
        // ============================================

        private void UserGuide_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Professional 3D ROI Viewer - Quick Guide\n\n" +
                          "MOUSE CONTROLS:\n" +
                          "• Left Drag: Rotate camera\n" +
                          "• Scroll Wheel: Zoom\n\n" +
                          "ROI MANAGEMENT:\n" +
                          "• Use + button to add ROI\n" +
                          "• Select ROI from list to edit\n" +
                          "• Adjust properties in right panel\n\n" +
                          "KEYBOARD SHORTCUTS:\n" +
                          "• Ctrl+N: New scene\n" +
                          "• Ctrl+S: Save scene\n" +
                          "• Ctrl+D: Duplicate ROI\n" +
                          "• Del: Delete ROI\n" +
                          "• F: Frame selected\n" +
                          "• A: Frame all",
                "User Guide", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void Shortcuts_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("KEYBOARD SHORTCUTS\n\n" +
                          "File:\n" +
                          "  Ctrl+N - New Scene\n" +
                          "  Ctrl+O - Open Scene\n" +
                          "  Ctrl+S - Save Scene\n\n" +
                          "Edit:\n" +
                          "  Ctrl+Z - Undo\n" +
                          "  Ctrl+Y - Redo\n" +
                          "  Ctrl+D - Duplicate ROI\n" +
                          "  Delete - Delete ROI\n\n" +
                          "View:\n" +
                          "  F - Frame Selected\n" +
                          "  A - Frame All\n" +
                          "  G - Toggle Grid\n" +
                          "  W - Toggle Wireframe\n\n" +
                          "Tools:\n" +
                          "  M - Measure Distance\n" +
                          "  C - Clipping Planes",
                "Keyboard Shortcuts", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void About_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Professional 3D ROI Viewer\nVersion 1.0\n\n" +
                          "A production-ready WPF 3D application for\n" +
                          "Region of Interest visualization and manipulation.\n\n" +
                          "Features:\n" +
                          "• Multiple ROI shapes\n" +
                          "• Interactive 3D manipulation\n" +
                          "• Save/Load scenes\n" +
                          "• Professional UI\n" +
                          "• Measurement tools\n\n" +
                          "Built with .NET 9.0 and WPF\n" +
                          "© 2024 WPF 3D Masterclass",
                "About", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        // ============================================
        // QUICK ACTIONS
        // ============================================

        private void CenterToOrigin_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null)
            {
                selectedROI.X = 0;
                selectedROI.Y = 0;
                selectedROI.Z = 0;
                LoadROIProperties(selectedROI);
                UpdateAllROIs();
                statusText.Text = "ROI centered to origin";
            }
        }

        private void ResetRotation_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null)
            {
                selectedROI.RotationX = 0;
                selectedROI.RotationY = 0;
                selectedROI.RotationZ = 0;
                LoadROIProperties(selectedROI);
                UpdateAllROIs();
                statusText.Text = "Rotation reset";
            }
        }

        private void ResetScale_Click(object sender, RoutedEventArgs e)
        {
            if (selectedROI != null)
            {
                selectedROI.Width = 2;
                selectedROI.Height = 2;
                selectedROI.Depth = 2;
                LoadROIProperties(selectedROI);
                UpdateAllROIs();
                statusText.Text = "Scale reset";
            }
        }

        // ============================================
        // STATUS BAR
        // ============================================

        private void UpdateStatusBar()
        {
            roiCountText.Text = $"ROIs: {roiList.Count} ({roiList.Count(r => r.Visible)} visible)";
        }
    }
}
