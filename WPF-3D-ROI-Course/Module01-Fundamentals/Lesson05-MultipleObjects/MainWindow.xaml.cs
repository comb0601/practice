using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace Lesson05_MultipleObjects;

public partial class MainWindow : Window
{
    private Model3DGroup? boardGroup;
    private Model3DGroup? piecesGroup;
    private int totalObjects = 0;

    public MainWindow()
    {
        InitializeComponent();
        CreateScene();
        UpdateSceneTree();
        UpdateStatistics();
    }

    private void CreateScene()
    {
        var rootGroup = new Model3DGroup();

        // Create chess board
        boardGroup = CreateChessBoard();
        rootGroup.Children.Add(boardGroup);

        // Create chess pieces
        piecesGroup = CreateChessPieces();
        rootGroup.Children.Add(piecesGroup);

        sceneRoot.Content = rootGroup;
    }

    private Model3DGroup CreateChessBoard()
    {
        var group = new Model3DGroup();
        double squareSize = 1.0;
        double boardThickness = 0.2;

        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                bool isWhite = (row + col) % 2 == 0;
                Color color = isWhite ? Colors.White : Colors.DarkGray;

                var square = CreateSquare(
                    col * squareSize - 4,
                    -boardThickness,
                    row * squareSize - 4,
                    squareSize,
                    boardThickness,
                    color);

                group.Children.Add(square);
                totalObjects++;
            }
        }

        return group;
    }

    private GeometryModel3D CreateSquare(double x, double y, double z, double size, double height, Color color)
    {
        var mesh = new MeshGeometry3D();

        // Top face vertices
        mesh.Positions.Add(new Point3D(x, y + height, z));
        mesh.Positions.Add(new Point3D(x + size, y + height, z));
        mesh.Positions.Add(new Point3D(x + size, y + height, z + size));
        mesh.Positions.Add(new Point3D(x, y + height, z + size));

        // Bottom face vertices
        mesh.Positions.Add(new Point3D(x, y, z));
        mesh.Positions.Add(new Point3D(x + size, y, z));
        mesh.Positions.Add(new Point3D(x + size, y, z + size));
        mesh.Positions.Add(new Point3D(x, y, z + size));

        // Top face
        mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(2);
        mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(2); mesh.TriangleIndices.Add(3);

        // Bottom face
        mesh.TriangleIndices.Add(4); mesh.TriangleIndices.Add(6); mesh.TriangleIndices.Add(5);
        mesh.TriangleIndices.Add(4); mesh.TriangleIndices.Add(7); mesh.TriangleIndices.Add(6);

        // Side faces
        mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(5); mesh.TriangleIndices.Add(1);
        mesh.TriangleIndices.Add(0); mesh.TriangleIndices.Add(4); mesh.TriangleIndices.Add(5);

        mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(6); mesh.TriangleIndices.Add(2);
        mesh.TriangleIndices.Add(1); mesh.TriangleIndices.Add(5); mesh.TriangleIndices.Add(6);

        mesh.TriangleIndices.Add(2); mesh.TriangleIndices.Add(7); mesh.TriangleIndices.Add(3);
        mesh.TriangleIndices.Add(2); mesh.TriangleIndices.Add(6); mesh.TriangleIndices.Add(7);

        mesh.TriangleIndices.Add(3); mesh.TriangleIndices.Add(4); mesh.TriangleIndices.Add(0);
        mesh.TriangleIndices.Add(3); mesh.TriangleIndices.Add(7); mesh.TriangleIndices.Add(4);

        return new GeometryModel3D
        {
            Geometry = mesh,
            Material = new DiffuseMaterial(new SolidColorBrush(color))
        };
    }

    private Model3DGroup CreateChessPieces()
    {
        var group = new Model3DGroup();

        // Create simple pieces (cylinders) at starting positions
        // White pieces (row 0 and 1)
        for (int col = 0; col < 8; col++)
        {
            // Pawns
            group.Children.Add(CreatePiece(col - 3.5, 1, Colors.WhiteSmoke, 0.3, 0.8));
            totalObjects++;

            // Back row pieces
            group.Children.Add(CreatePiece(col - 3.5, 0, Colors.WhiteSmoke, 0.4, 1.2));
            totalObjects++;
        }

        // Black pieces (row 6 and 7)
        for (int col = 0; col < 8; col++)
        {
            // Pawns
            group.Children.Add(CreatePiece(col - 3.5, -2, Colors.Black, 0.3, 0.8));
            totalObjects++;

            // Back row pieces
            group.Children.Add(CreatePiece(col - 3.5, -3, Colors.Black, 0.4, 1.2));
            totalObjects++;
        }

        return group;
    }

    private GeometryModel3D CreatePiece(double x, double z, Color color, double radius, double height)
    {
        var mesh = CreateCylinderMesh(new Point3D(x, 0, z), radius, height, 12);

        var material = new MaterialGroup();
        material.Children.Add(new DiffuseMaterial(new SolidColorBrush(color)));
        material.Children.Add(new SpecularMaterial(Brushes.White, 30));

        return new GeometryModel3D
        {
            Geometry = mesh,
            Material = material
        };
    }

    private MeshGeometry3D CreateCylinderMesh(Point3D center, double radius, double height, int segments)
    {
        var mesh = new MeshGeometry3D();
        double angleStep = 2 * Math.PI / segments;

        // Bottom circle
        for (int i = 0; i < segments; i++)
        {
            double angle = i * angleStep;
            mesh.Positions.Add(new Point3D(
                center.X + radius * Math.Cos(angle),
                center.Y,
                center.Z + radius * Math.Sin(angle)));
        }

        // Top circle
        for (int i = 0; i < segments; i++)
        {
            double angle = i * angleStep;
            mesh.Positions.Add(new Point3D(
                center.X + radius * Math.Cos(angle),
                center.Y + height,
                center.Z + radius * Math.Sin(angle)));
        }

        // Side triangles
        for (int i = 0; i < segments; i++)
        {
            int next = (i + 1) % segments;

            mesh.TriangleIndices.Add(i);
            mesh.TriangleIndices.Add(i + segments);
            mesh.TriangleIndices.Add(next);

            mesh.TriangleIndices.Add(next);
            mesh.TriangleIndices.Add(i + segments);
            mesh.TriangleIndices.Add(next + segments);
        }

        // Top and bottom caps
        mesh.Positions.Add(new Point3D(center.X, center.Y, center.Z));
        mesh.Positions.Add(new Point3D(center.X, center.Y + height, center.Z));

        int bottomCenter = mesh.Positions.Count - 2;
        int topCenter = mesh.Positions.Count - 1;

        for (int i = 0; i < segments; i++)
        {
            int next = (i + 1) % segments;

            mesh.TriangleIndices.Add(bottomCenter);
            mesh.TriangleIndices.Add(next);
            mesh.TriangleIndices.Add(i);

            mesh.TriangleIndices.Add(topCenter);
            mesh.TriangleIndices.Add(i + segments);
            mesh.TriangleIndices.Add(next + segments);
        }

        return mesh;
    }

    private void UpdateSceneTree()
    {
        sceneTree.Items.Clear();

        var root = new TreeViewItem { Header = "Scene Root", IsExpanded = true };

        var boardItem = new TreeViewItem { Header = $"Chess Board ({boardGroup?.Children.Count ?? 0} squares)" };
        root.Items.Add(boardItem);

        var piecesItem = new TreeViewItem { Header = $"Chess Pieces ({piecesGroup?.Children.Count ?? 0} pieces)" };
        root.Items.Add(piecesItem);

        sceneTree.Items.Add(root);
    }

    private void UpdateStatistics()
    {
        txtObjectCount.Text = totalObjects.ToString();
        txtStats.Text = $"Board Squares: 64\nPieces: {piecesGroup?.Children.Count ?? 0}\nTotal Objects: {totalObjects}";
    }

    private void TopView_Click(object sender, RoutedEventArgs e)
    {
        camera.Position = new Point3D(0, 12, 0);
        camera.LookDirection = new Vector3D(0, -12, 0);
    }

    private void SideView_Click(object sender, RoutedEventArgs e)
    {
        camera.Position = new Point3D(12, 4, 0);
        camera.LookDirection = new Vector3D(-12, -4, 0);
    }

    private void IsoView_Click(object sender, RoutedEventArgs e)
    {
        camera.Position = new Point3D(6, 8, 6);
        camera.LookDirection = new Vector3D(-6, -8, -6);
    }

    private void Toggle_Changed(object sender, RoutedEventArgs e)
    {
        if (boardGroup != null)
            boardGroup.Transform = chkShowBoard.IsChecked == true
                ? Transform3D.Identity
                : new ScaleTransform3D(0, 0, 0);

        if (piecesGroup != null)
            piecesGroup.Transform = chkShowPieces.IsChecked == true
                ? Transform3D.Identity
                : new ScaleTransform3D(0, 0, 0);
    }
}
