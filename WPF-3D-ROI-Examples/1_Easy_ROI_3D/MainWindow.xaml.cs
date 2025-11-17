using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Easy_ROI_3D
{
    public partial class MainWindow : Window
    {
        private double roiX = 0;
        private double roiY = 0;
        private double roiZ = 0;
        private double roiWidth = 2;
        private double roiHeight = 2;
        private double roiDepth = 2;
        private Color roiColor = Color.FromArgb(128, 255, 0, 0);

        public MainWindow()
        {
            InitializeComponent();
            CreateGrid();
            UpdateROI();
        }

        private void CreateGrid()
        {
            var gridGroup = new Model3DGroup();

            // Create grid lines on XZ plane
            for (int i = -5; i <= 5; i++)
            {
                // Lines parallel to X axis
                gridGroup.Children.Add(CreateLine(
                    new Point3D(-5, 0, i),
                    new Point3D(5, 0, i),
                    Colors.Gray, 0.02));

                // Lines parallel to Z axis
                gridGroup.Children.Add(CreateLine(
                    new Point3D(i, 0, -5),
                    new Point3D(i, 0, 5),
                    Colors.Gray, 0.02));
            }

            // Axis lines
            gridGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(3, 0, 0), Colors.Red, 0.05));    // X axis
            gridGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(0, 3, 0), Colors.Green, 0.05));  // Y axis
            gridGroup.Children.Add(CreateLine(new Point3D(0, 0, 0), new Point3D(0, 0, 3), Colors.Blue, 0.05));   // Z axis

            gridModel.Content = gridGroup;
        }

        private GeometryModel3D CreateLine(Point3D start, Point3D end, Color color, double thickness)
        {
            var mesh = new MeshGeometry3D();
            var direction = end - start;
            var length = direction.Length;
            direction.Normalize();

            // Create a simple cylinder for the line
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

            var material = new DiffuseMaterial(new SolidColorBrush(color));
            return new GeometryModel3D(mesh, material);
        }

        private void UpdateROI()
        {
            var roiGroup = new Model3DGroup();

            // Create a box geometry for the ROI
            var mesh = CreateBoxMesh(roiWidth, roiHeight, roiDepth);

            // Create semi-transparent material
            var material = new DiffuseMaterial(new SolidColorBrush(roiColor));
            var specular = new SpecularMaterial(new SolidColorBrush(Colors.White), 50);
            var materialGroup = new MaterialGroup();
            materialGroup.Children.Add(material);
            materialGroup.Children.Add(specular);

            var geometry = new GeometryModel3D(mesh, materialGroup);

            // Apply transform to position the ROI
            var transform = new TranslateTransform3D(roiX, roiY, roiZ);
            geometry.Transform = transform;

            roiGroup.Children.Add(geometry);

            // Add wireframe edges
            roiGroup.Children.Add(CreateWireframeBox(roiWidth, roiHeight, roiDepth, roiX, roiY, roiZ));

            roiModel.Content = roiGroup;
        }

        private MeshGeometry3D CreateBoxMesh(double width, double height, double depth)
        {
            var mesh = new MeshGeometry3D();

            double w = width / 2;
            double h = height / 2;
            double d = depth / 2;

            // Define the 8 vertices of the box
            Point3D[] vertices = new Point3D[]
            {
                new Point3D(-w, -h, -d), // 0
                new Point3D(w, -h, -d),  // 1
                new Point3D(w, h, -d),   // 2
                new Point3D(-w, h, -d),  // 3
                new Point3D(-w, -h, d),  // 4
                new Point3D(w, -h, d),   // 5
                new Point3D(w, h, d),    // 6
                new Point3D(-w, h, d)    // 7
            };

            // Add all vertices
            foreach (var vertex in vertices)
                mesh.Positions.Add(vertex);

            // Define the 6 faces (2 triangles per face)
            int[] indices = new int[]
            {
                // Front face
                0, 1, 2, 0, 2, 3,
                // Back face
                5, 4, 7, 5, 7, 6,
                // Left face
                4, 0, 3, 4, 3, 7,
                // Right face
                1, 5, 6, 1, 6, 2,
                // Top face
                3, 2, 6, 3, 6, 7,
                // Bottom face
                4, 5, 1, 4, 1, 0
            };

            foreach (var index in indices)
                mesh.TriangleIndices.Add(index);

            return mesh;
        }

        private Model3DGroup CreateWireframeBox(double width, double height, double depth, double x, double y, double z)
        {
            var wireframe = new Model3DGroup();
            double w = width / 2;
            double h = height / 2;
            double d = depth / 2;

            Point3D[] corners = new Point3D[]
            {
                new Point3D(x - w, y - h, z - d),
                new Point3D(x + w, y - h, z - d),
                new Point3D(x + w, y + h, z - d),
                new Point3D(x - w, y + h, z - d),
                new Point3D(x - w, y - h, z + d),
                new Point3D(x + w, y - h, z + d),
                new Point3D(x + w, y + h, z + d),
                new Point3D(x - w, y + h, z + d)
            };

            // Bottom face
            wireframe.Children.Add(CreateLine(corners[0], corners[1], Colors.White, 0.02));
            wireframe.Children.Add(CreateLine(corners[1], corners[2], Colors.White, 0.02));
            wireframe.Children.Add(CreateLine(corners[2], corners[3], Colors.White, 0.02));
            wireframe.Children.Add(CreateLine(corners[3], corners[0], Colors.White, 0.02));

            // Top face
            wireframe.Children.Add(CreateLine(corners[4], corners[5], Colors.White, 0.02));
            wireframe.Children.Add(CreateLine(corners[5], corners[6], Colors.White, 0.02));
            wireframe.Children.Add(CreateLine(corners[6], corners[7], Colors.White, 0.02));
            wireframe.Children.Add(CreateLine(corners[7], corners[4], Colors.White, 0.02));

            // Vertical edges
            wireframe.Children.Add(CreateLine(corners[0], corners[4], Colors.White, 0.02));
            wireframe.Children.Add(CreateLine(corners[1], corners[5], Colors.White, 0.02));
            wireframe.Children.Add(CreateLine(corners[2], corners[6], Colors.White, 0.02));
            wireframe.Children.Add(CreateLine(corners[3], corners[7], Colors.White, 0.02));

            return wireframe;
        }

        private void Position_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!IsLoaded) return;

            roiX = sliderPosX.Value;
            roiY = sliderPosY.Value;
            roiZ = sliderPosZ.Value;

            txtPosX.Text = roiX.ToString("F1");
            txtPosY.Text = roiY.ToString("F1");
            txtPosZ.Text = roiZ.ToString("F1");

            UpdateROI();
        }

        private void Size_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!IsLoaded) return;

            roiWidth = sliderWidth.Value;
            roiHeight = sliderHeight.Value;
            roiDepth = sliderDepth.Value;

            txtWidth.Text = roiWidth.ToString("F1");
            txtHeight.Text = roiHeight.ToString("F1");
            txtDepth.Text = roiDepth.ToString("F1");

            UpdateROI();
        }

        private void CameraAngle_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!IsLoaded) return;

            double angle = sliderCameraAngle.Value * Math.PI / 180.0;
            double distance = 10;
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
        }

        private void CameraHeight_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!IsLoaded) return;

            double angle = sliderCameraAngle.Value * Math.PI / 180.0;
            double distance = 10;
            double height = sliderCameraHeight.Value;

            camera.Position = new Point3D(
                Math.Sin(angle) * distance,
                height,
                Math.Cos(angle) * distance);

            camera.LookDirection = new Vector3D(
                -camera.Position.X,
                -camera.Position.Y,
                -camera.Position.Z);

            txtCameraHeight.Text = height.ToString("F1");
        }

        private void Color_Changed(object sender, SelectionChangedEventArgs e)
        {
            if (!IsLoaded || colorCombo.SelectedItem == null) return;

            var item = (ComboBoxItem)colorCombo.SelectedItem;
            var colorString = item.Tag.ToString();
            if (colorString != null)
            {
                roiColor = (Color)ColorConverter.ConvertFromString(colorString);
                UpdateROI();
            }
        }

        private void Reset_Click(object sender, RoutedEventArgs e)
        {
            sliderPosX.Value = 0;
            sliderPosY.Value = 0;
            sliderPosZ.Value = 0;
            sliderWidth.Value = 2;
            sliderHeight.Value = 2;
            sliderDepth.Value = 2;
            sliderCameraAngle.Value = 0;
            sliderCameraHeight.Value = 5;
            colorCombo.SelectedIndex = 0;
        }
    }
}
