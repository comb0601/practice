using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Lesson01
{
    /// <summary>
    /// LESSON 01: Hello 3D World
    ///
    /// In this lesson, you'll create your first 3D triangle!
    /// We'll learn the fundamental building blocks of WPF 3D graphics.
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            // Create our first 3D triangle
            CreateTriangle();
        }

        /// <summary>
        /// Creates a 3D triangle and adds it to the scene.
        ///
        /// Steps:
        /// 1. Create a mesh (the shape definition)
        /// 2. Define vertices (the 3 corners)
        /// 3. Define triangle indices (how vertices connect)
        /// 4. Create a material (color/appearance)
        /// 5. Combine into a geometry model
        /// 6. Add to the scene
        /// </summary>
        private void CreateTriangle()
        {
            // ============================================
            // STEP 1: Create the Mesh
            // ============================================
            // MeshGeometry3D is the fundamental shape container
            // It holds vertices and defines how they connect
            MeshGeometry3D triangleMesh = new MeshGeometry3D();

            // ============================================
            // STEP 2: Define Vertices (Positions)
            // ============================================
            // Vertices are the 3D points that define our shape
            // A triangle needs 3 vertices (3 corners)

            // Vertex 0: Top of triangle
            triangleMesh.Positions.Add(new Point3D(0, 1, 0));
            // X = 0 (centered horizontally)
            // Y = 1 (1 unit up)
            // Z = 0 (on the "flat" plane)

            // Vertex 1: Bottom-left corner
            triangleMesh.Positions.Add(new Point3D(-1, -1, 0));
            // X = -1 (1 unit to the left)
            // Y = -1 (1 unit down)
            // Z = 0 (same plane)

            // Vertex 2: Bottom-right corner
            triangleMesh.Positions.Add(new Point3D(1, -1, 0));
            // X = 1 (1 unit to the right)
            // Y = -1 (1 unit down)
            // Z = 0 (same plane)

            // ============================================
            // STEP 3: Define Triangle Indices
            // ============================================
            // Indices tell which vertices to connect to form triangles
            // IMPORTANT: Counter-clockwise order = front face

            triangleMesh.TriangleIndices.Add(0); // Start at vertex 0 (top)
            triangleMesh.TriangleIndices.Add(1); // Connect to vertex 1 (bottom-left)
            triangleMesh.TriangleIndices.Add(2); // Connect to vertex 2 (bottom-right)

            // This creates ONE triangle: 0 → 1 → 2
            // The order matters! Counter-clockwise faces the camera.

            // ============================================
            // STEP 4: Create Material (Color)
            // ============================================
            // Material determines how the surface looks
            // DiffuseMaterial = matte, non-shiny surface

            DiffuseMaterial redMaterial = new DiffuseMaterial(
                new SolidColorBrush(Colors.Red)
            );

            // Try changing Colors.Red to:
            // - Colors.Blue
            // - Colors.Green
            // - Colors.Yellow
            // - Color.FromRgb(255, 128, 0) for orange

            // ============================================
            // STEP 5: Combine Mesh + Material
            // ============================================
            // GeometryModel3D combines the shape (mesh) with appearance (material)

            GeometryModel3D triangleModel = new GeometryModel3D(
                triangleMesh,    // The shape
                redMaterial      // The color/appearance
            );

            // OPTIONAL: Make both sides visible (default is front only)
            triangleModel.BackMaterial = redMaterial;

            // ============================================
            // STEP 6: Add to Scene
            // ============================================
            // ModelVisual3D is a container for 3D models
            // We add our geometry model to it, then add it to the viewport

            ModelVisual3D visual = new ModelVisual3D();
            visual.Content = triangleModel;

            // Add to the viewport (makes it visible)
            viewport.Children.Add(visual);

            // SUCCESS! You now have a 3D triangle in your scene!
        }

        // ============================================
        // UNDERSTANDING THE COORDINATE SYSTEM
        // ============================================
        /*
         * WPF uses a RIGHT-HANDED coordinate system:
         *
         *     Y (Up)
         *     |
         *     |
         *     +------ X (Right)
         *    /
         *   /
         *  Z (Toward You)
         *
         * - X-axis: Left (-) to Right (+)
         * - Y-axis: Down (-) to Up (+)
         * - Z-axis: Away (-) to Toward you (+)
         *
         * Our triangle is at Z=0, so it's on the "wall"
         * Camera is at Z=5, looking toward Z=-1 (into the scene)
         */

        // ============================================
        // EXPERIMENT IDEAS
        // ============================================
        /*
         * 1. CHANGE SIZE:
         *    Multiply all coordinates by 2 to make it bigger
         *    Example: (0, 1, 0) becomes (0, 2, 0)
         *
         * 2. MOVE IT:
         *    Add 1 to all X values to move it right
         *    Example: (0, 1, 0) becomes (1, 1, 0)
         *
         * 3. ROTATE IT:
         *    Change Z values to make it tilt
         *    Example: Top at Z=0.5, bottom at Z=-0.5
         *
         * 4. ADD MORE TRIANGLES:
         *    Copy this code and create a second triangle
         *    with different positions and colors
         *
         * 5. MAKE A SQUARE:
         *    Add a 4th vertex and create 2 triangles
         *    that share an edge (forms a square)
         */

        // ============================================
        // COMMON MISTAKES
        // ============================================
        /*
         * MISTAKE 1: Black screen
         * - Check camera position (should be positive Z)
         * - Check that lights exist
         * - Check vertices aren't all at the same point
         *
         * MISTAKE 2: Can't see triangle
         * - Wrong winding order (swap indices)
         * - Camera looking wrong direction
         * - Triangle is too far from camera
         *
         * MISTAKE 3: Triangle is backwards
         * - Indices in clockwise order (should be counter-clockwise)
         * - Add BackMaterial to see both sides
         */
    }
}
