using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Lesson02
{
    /// <summary>
    /// LESSON 02: Camera Systems
    ///
    /// This lesson teaches you everything about cameras in WPF 3D:
    /// - Perspective vs Orthographic cameras
    /// - Camera positioning and orientation
    /// - Field of View (FOV) and how it affects the view
    /// - Preset camera positions
    ///
    /// Run this application and experiment with all the controls!
    /// </summary>
    public partial class MainWindow : Window
    {
        private OrthographicCamera? orthographicCamera;
        private bool isUpdating = false;

        public MainWindow()
        {
            InitializeComponent();
            CreateScene();
            UpdateCameraInfo();
        }

        /// <summary>
        /// Creates a scene with multiple colored cubes to visualize camera effects
        /// </summary>
        private void CreateScene()
        {
            var group = new Model3DGroup();

            // Create a grid of cubes in different colors
            // This helps visualize how the camera sees the scene

            // Center cube - RED
            group.Children.Add(CreateCube(0, 0, 0, 2, Colors.Red));

            // Surrounding cubes - Different colors
            group.Children.Add(CreateCube(4, 0, 0, 1.5, Colors.Green));   // Right
            group.Children.Add(CreateCube(-4, 0, 0, 1.5, Colors.Blue));   // Left
            group.Children.Add(CreateCube(0, 0, 4, 1.5, Colors.Yellow));  // Back
            group.Children.Add(CreateCube(0, 0, -4, 1.5, Colors.Cyan));   // Front
            group.Children.Add(CreateCube(0, 3, 0, 1.5, Colors.Magenta)); // Top

            // Corner cubes - Gray
            group.Children.Add(CreateCube(4, 0, 4, 1, Colors.Gray));
            group.Children.Add(CreateCube(-4, 0, 4, 1, Colors.Gray));
            group.Children.Add(CreateCube(4, 0, -4, 1, Colors.Gray));
            group.Children.Add(CreateCube(-4, 0, -4, 1, Colors.Gray));

            // Ground plane (helps with orientation)
            group.Children.Add(CreateGroundPlane());

            sceneModel.Content = group;
        }

        /// <summary>
        /// Creates a single cube at the specified position
        /// </summary>
        private GeometryModel3D CreateCube(double x, double y, double z, double size, Color color)
        {
            var mesh = new MeshGeometry3D();
            double s = size / 2;

            // Define 8 vertices of a cube
            Point3D[] vertices = new[]
            {
                new Point3D(-s, -s, -s), new Point3D(s, -s, -s),
                new Point3D(s, s, -s), new Point3D(-s, s, -s),
                new Point3D(-s, -s, s), new Point3D(s, -s, s),
                new Point3D(s, s, s), new Point3D(-s, s, s)
            };

            foreach (var v in vertices)
                mesh.Positions.Add(v);

            // Define 12 triangles (6 faces * 2 triangles each)
            int[] indices = { 0,1,2, 0,2,3, 5,4,7, 5,7,6, 4,0,3, 4,3,7, 1,5,6, 1,6,2, 3,2,6, 3,6,7, 4,5,1, 4,1,0 };
            foreach (var i in indices)
                mesh.TriangleIndices.Add(i);

            // Create material with the specified color
            var material = new DiffuseMaterial(new SolidColorBrush(color));
            var specular = new SpecularMaterial(new SolidColorBrush(Colors.White), 30);
            var materialGroup = new MaterialGroup();
            materialGroup.Children.Add(material);
            materialGroup.Children.Add(specular);

            var model = new GeometryModel3D(mesh, materialGroup);

            // Position the cube
            model.Transform = new TranslateTransform3D(x, y, z);

            return model;
        }

        /// <summary>
        /// Creates a ground plane to help with orientation
        /// </summary>
        private GeometryModel3D CreateGroundPlane()
        {
            var mesh = new MeshGeometry3D();

            // Large rectangle on the XZ plane (Y = -1)
            mesh.Positions.Add(new Point3D(-10, -1, -10));
            mesh.Positions.Add(new Point3D(10, -1, -10));
            mesh.Positions.Add(new Point3D(10, -1, 10));
            mesh.Positions.Add(new Point3D(-10, -1, 10));

            mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(2);
            mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(2); mesh.TriangleIndices.Add(3);

            var material = new DiffuseMaterial(new SolidColorBrush(Color.FromRgb(40, 40, 40)));
            return new GeometryModel3D(mesh, material);
        }

        // =====================================================
        // CAMERA CONTROL METHODS
        // =====================================================

        /// <summary>
        /// Called when camera position sliders change
        /// </summary>
        private void CameraPosition_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!IsLoaded || isUpdating) return;

            // Get position from sliders
            double x = sliderPosX.Value;
            double y = sliderPosY.Value;
            double z = sliderPosZ.Value;

            // Update text displays
            txtPosX.Text = x.ToString("F1");
            txtPosY.Text = y.ToString("F1");
            txtPosZ.Text = z.ToString("F1");

            // Update active camera
            Point3D newPos = new Point3D(x, y, z);

            if (rbPerspective.IsChecked == true)
            {
                perspectiveCamera.Position = newPos;
                // Look at origin (0,0,0)
                perspectiveCamera.LookDirection = new Vector3D(-x, -y, -z);
            }
            else if (orthographicCamera != null)
            {
                orthographicCamera.Position = newPos;
                orthographicCamera.LookDirection = new Vector3D(-x, -y, -z);
            }

            UpdateCameraInfo();
        }

        /// <summary>
        /// Called when FOV/Width slider changes
        /// </summary>
        private void FOV_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!IsLoaded || isUpdating) return;

            double value = sliderFOV.Value;

            if (rbPerspective.IsChecked == true)
            {
                perspectiveCamera.FieldOfView = value;
                txtFOV.Text = value.ToString("F0") + "°";
            }
            else if (orthographicCamera != null)
            {
                orthographicCamera.Width = value / 5.0; // Scale for reasonable viewing
                txtFOV.Text = (value / 5.0).ToString("F1");
            }

            UpdateCameraInfo();
        }

        /// <summary>
        /// Switches between Perspective and Orthographic cameras
        /// </summary>
        private void CameraType_Changed(object sender, RoutedEventArgs e)
        {
            if (!IsLoaded) return;

            isUpdating = true;

            if (rbPerspective.IsChecked == true)
            {
                // Switch to Perspective
                viewport.Camera = perspectiveCamera;
                lblFOVWidth.Text = "FIELD OF VIEW";
                sliderFOV.Minimum = 10;
                sliderFOV.Maximum = 120;
                sliderFOV.Value = perspectiveCamera.FieldOfView;
                txtFOV.Text = perspectiveCamera.FieldOfView.ToString("F0") + "°";
            }
            else
            {
                // Create Orthographic camera if it doesn't exist
                if (orthographicCamera == null)
                {
                    orthographicCamera = new OrthographicCamera
                    {
                        Position = perspectiveCamera.Position,
                        LookDirection = perspectiveCamera.LookDirection,
                        UpDirection = perspectiveCamera.UpDirection,
                        Width = 12
                    };
                }

                // Switch to Orthographic
                viewport.Camera = orthographicCamera;
                lblFOVWidth.Text = "WIDTH";
                sliderFOV.Minimum = 10;
                sliderFOV.Maximum = 100;
                sliderFOV.Value = orthographicCamera.Width * 5; // Scale back
                txtFOV.Text = orthographicCamera.Width.ToString("F1");
            }

            isUpdating = false;
            UpdateCameraInfo();
        }

        // =====================================================
        // PRESET CAMERA POSITIONS
        // =====================================================

        private void PresetFront_Click(object sender, RoutedEventArgs e)
        {
            SetCameraPosition(0, 0, 15, "Front");
        }

        private void PresetBack_Click(object sender, RoutedEventArgs e)
        {
            SetCameraPosition(0, 0, -15, "Back");
        }

        private void PresetTop_Click(object sender, RoutedEventArgs e)
        {
            SetCameraPosition(0, 15, 0, "Top");
        }

        private void PresetBottom_Click(object sender, RoutedEventArgs e)
        {
            SetCameraPosition(0, -15, 0, "Bottom");
        }

        private void PresetLeft_Click(object sender, RoutedEventArgs e)
        {
            SetCameraPosition(-15, 0, 0, "Left");
        }

        private void PresetRight_Click(object sender, RoutedEventArgs e)
        {
            SetCameraPosition(15, 0, 0, "Right");
        }

        private void PresetIso_Click(object sender, RoutedEventArgs e)
        {
            // Isometric view: 45° angles
            double dist = 12;
            double val = dist / Math.Sqrt(3);
            SetCameraPosition(val, val, val, "Isometric");
        }

        /// <summary>
        /// Helper method to set camera position
        /// </summary>
        private void SetCameraPosition(double x, double y, double z, string viewName)
        {
            isUpdating = true;

            // Update sliders
            sliderPosX.Value = x;
            sliderPosY.Value = y;
            sliderPosZ.Value = z;

            // Update text
            txtPosX.Text = x.ToString("F1");
            txtPosY.Text = y.ToString("F1");
            txtPosZ.Text = z.ToString("F1");

            // Update cameras
            Point3D pos = new Point3D(x, y, z);
            Vector3D lookDir = new Vector3D(-x, -y, -z);

            perspectiveCamera.Position = pos;
            perspectiveCamera.LookDirection = lookDir;

            if (orthographicCamera != null)
            {
                orthographicCamera.Position = pos;
                orthographicCamera.LookDirection = lookDir;
            }

            isUpdating = false;
            UpdateCameraInfo();

            MessageBox.Show($"Camera set to {viewName} view!", "Preset Applied",
                MessageBoxButton.OK, MessageBoxImage.Information);
        }

        /// <summary>
        /// Updates the camera information display
        /// </summary>
        private void UpdateCameraInfo()
        {
            if (!IsLoaded) return;

            if (rbPerspective.IsChecked == true)
            {
                txtCameraType.Text = "Type: Perspective";
                txtCameraPos.Text = $"Position: ({perspectiveCamera.Position.X:F1}, {perspectiveCamera.Position.Y:F1}, {perspectiveCamera.Position.Z:F1})";
                txtCameraLook.Text = $"Looking: ({perspectiveCamera.LookDirection.X:F1}, {perspectiveCamera.LookDirection.Y:F1}, {perspectiveCamera.LookDirection.Z:F1})";
                txtCameraFOV.Text = $"FOV: {perspectiveCamera.FieldOfView:F0}°";
            }
            else if (orthographicCamera != null)
            {
                txtCameraType.Text = "Type: Orthographic";
                txtCameraPos.Text = $"Position: ({orthographicCamera.Position.X:F1}, {orthographicCamera.Position.Y:F1}, {orthographicCamera.Position.Z:F1})";
                txtCameraLook.Text = $"Looking: ({orthographicCamera.LookDirection.X:F1}, {orthographicCamera.LookDirection.Y:F1}, {orthographicCamera.LookDirection.Z:F1})";
                txtCameraFOV.Text = $"Width: {orthographicCamera.Width:F1}";
            }
        }

        // =====================================================
        // KEY LEARNING POINTS
        // =====================================================

        /*
         * UNDERSTANDING CAMERAS IN WPF 3D
         *
         * 1. POSITION (Point3D)
         *    - Where the camera is located in 3D space
         *    - Think of it as "where you're standing"
         *    - Example: (5, 3, 10) means 5 right, 3 up, 10 forward
         *
         * 2. LOOK DIRECTION (Vector3D)
         *    - Which way the camera is pointing
         *    - NOT a position, it's a DIRECTION
         *    - Example: (-1, 0, 0) means "looking left"
         *    - To look at origin from (5,3,10): direction = (0,0,0) - (5,3,10) = (-5,-3,-10)
         *
         * 3. UP DIRECTION (Vector3D)
         *    - Which way is "up" for the camera
         *    - Usually (0, 1, 0) meaning Y+ is up
         *    - Prevents camera from rolling/tilting
         *
         * 4. FIELD OF VIEW (Perspective only)
         *    - How wide the camera can see (in degrees)
         *    - 60° = normal human vision
         *    - 90° = wide angle lens
         *    - 30° = telephoto/zoomed in
         *
         * 5. WIDTH (Orthographic only)
         *    - How many units wide the view is
         *    - Larger = see more (zoomed out)
         *    - Smaller = see less (zoomed in)
         *
         * PERSPECTIVE VS ORTHOGRAPHIC
         *
         * Perspective:
         *   - Objects farther away appear smaller (realistic)
         *   - Has vanishing points
         *   - Used for: games, realistic renders, simulations
         *
         * Orthographic:
         *   - All objects same size regardless of distance
         *   - Parallel lines stay parallel (no perspective)
         *   - Used for: CAD, technical drawings, 2D-like games
         *
         * TRY THIS:
         * 1. Switch between camera types and see the difference
         * 2. Move the camera far away and close up
         * 3. Change FOV from 10° to 120° (perspective only)
         * 4. Use preset buttons to see standard views
         * 5. Understand how Position + LookDirection work together
         */
    }
}
