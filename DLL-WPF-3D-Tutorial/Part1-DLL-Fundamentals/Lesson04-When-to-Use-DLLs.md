# Lesson 4: When to Use DLLs

## 📚 Learning Objectives

By the end of this lesson, you will understand:
- Design patterns that benefit from DLLs
- When DLLs are the right choice
- Architectural decisions around DLLs
- Real-world scenarios and case studies
- Best practices for DLL usage
- Common mistakes to avoid

## 🎯 Design Patterns for DLL Architecture

### Pattern 1: Plugin Architecture

**What:** Applications that allow third-party extensions

**Example: Photo Editor**
```
PhotoEditor.exe
Plugins/
├── BlurFilter.dll
├── SharpenFilter.dll
├── VintageEffect.dll
└── WatermarkTool.dll
```

**Implementation:**
```csharp
// IPlugin.dll - Interface definition
public interface IPhotoPlugin
{
    string Name { get; }
    string Description { get; }
    Bitmap ProcessImage(Bitmap input);
}

// BlurFilter.dll - Plugin implementation
public class BlurFilter : IPhotoPlugin
{
    public string Name => "Blur Filter";
    public string Description => "Applies Gaussian blur";

    public Bitmap ProcessImage(Bitmap input)
    {
        // Blur logic here
        return blurredImage;
    }
}

// PhotoEditor.exe - Host application
public class PluginManager
{
    private List<IPhotoPlugin> plugins = new List<IPhotoPlugin>();

    public void LoadPlugins(string pluginDirectory)
    {
        var dllFiles = Directory.GetFiles(pluginDirectory, "*.dll");

        foreach (var dllFile in dllFiles)
        {
            Assembly assembly = Assembly.LoadFrom(dllFile);

            foreach (Type type in assembly.GetTypes())
            {
                if (typeof(IPhotoPlugin).IsAssignableFrom(type) && !type.IsInterface)
                {
                    var plugin = (IPhotoPlugin)Activator.CreateInstance(type);
                    plugins.Add(plugin);
                }
            }
        }
    }

    public void ApplyPlugin(string pluginName, Bitmap image)
    {
        var plugin = plugins.Find(p => p.Name == pluginName);
        plugin?.ProcessImage(image);
    }
}
```

**Benefits:**
✅ Users can add new filters without changing the main app
✅ Third-party developers can create plugins
✅ Easy to enable/disable features (just add/remove DLL)

**Use When:**
- Extensibility is key
- Third-party development expected
- Features need to be optional

### Pattern 2: Layered Architecture

**What:** Separate application into logical layers

**Example: Enterprise Application**
```
Application Architecture:

┌─────────────────────────────────────┐
│  PresentationLayer.exe (WPF UI)     │
└───────────────┬─────────────────────┘
                │
┌───────────────▼─────────────────────┐
│  BusinessLogic.dll                  │
│  - Order processing                 │
│  - Inventory management             │
└───────────────┬─────────────────────┘
                │
┌───────────────▼─────────────────────┐
│  DataAccess.dll                     │
│  - Database queries                 │
│  - ORM mappings                     │
└───────────────┬─────────────────────┘
                │
           [Database]
```

**Implementation:**
```csharp
// DataAccess.dll
public class OrderRepository
{
    public List<Order> GetOrders()
    {
        // Database logic
    }

    public void SaveOrder(Order order)
    {
        // Save to database
    }
}

// BusinessLogic.dll (references DataAccess.dll)
public class OrderService
{
    private OrderRepository repository = new OrderRepository();

    public void ProcessOrder(Order order)
    {
        // Business rules
        if (order.Total > 0)
        {
            repository.SaveOrder(order);
        }
    }
}

// PresentationLayer.exe (references BusinessLogic.dll)
public partial class MainWindow : Window
{
    private OrderService orderService = new OrderService();

    private void PlaceOrder_Click(object sender, RoutedEventArgs e)
    {
        var order = CreateOrderFromUI();
        orderService.ProcessOrder(order);
    }
}
```

**Benefits:**
✅ Clear separation of concerns
✅ Can update database logic without touching UI
✅ Testable (mock each layer)
✅ Reusable (BusinessLogic.dll in web and desktop app)

**Use When:**
- Large, complex applications
- Multiple developers/teams
- Need to support multiple frontends (WPF, Web, Mobile)

### Pattern 3: Shared Utilities

**What:** Common code used by many applications

**Example: Company-wide Utilities**
```
Multiple Applications:
├── SalesApp.exe
├── InventoryApp.exe
├── ReportingApp.exe
└── HRApp.exe

All use:
└── CompanyUtilities.dll
    ├── Logging
    ├── Configuration
    ├── Authentication
    └── Error handling
```

**Implementation:**
```csharp
// CompanyUtilities.dll
public static class Logger
{
    public static void LogInfo(string message)
    {
        File.AppendAllText("app.log", $"[INFO] {message}\n");
    }

    public static void LogError(string message)
    {
        File.AppendAllText("app.log", $"[ERROR] {message}\n");
    }
}

public static class ConfigManager
{
    public static string GetSetting(string key)
    {
        return ConfigurationManager.AppSettings[key];
    }
}

// Used by all apps:
// SalesApp.exe
Logger.LogInfo("Sales application started");
var dbConnection = ConfigManager.GetSetting("DatabaseConnection");

// InventoryApp.exe
Logger.LogInfo("Inventory application started");
var apiKey = ConfigManager.GetSetting("APIKey");
```

**Benefits:**
✅ Consistency across applications
✅ Fix bug once, all apps benefit
✅ Shared best practices

**Use When:**
- Multiple applications in organization
- Common functionality needed everywhere
- Want consistent behavior

### Pattern 4: Runtime Selection

**What:** Choose implementation at runtime

**Example: Database Provider Selection**
```
YourApp.exe

Can use:
├── SqlServerProvider.dll
├── MySqlProvider.dll
├── PostgresProvider.dll
└── OracleProvider.dll

User chooses in config file!
```

**Implementation:**
```csharp
// IDataProvider.dll - Interface
public interface IDataProvider
{
    void Connect(string connectionString);
    DataTable ExecuteQuery(string sql);
}

// SqlServerProvider.dll
public class SqlServerProvider : IDataProvider
{
    public void Connect(string connectionString)
    {
        // SQL Server connection logic
    }

    public DataTable ExecuteQuery(string sql)
    {
        // SQL Server query
    }
}

// MySqlProvider.dll
public class MySqlProvider : IDataProvider
{
    public void Connect(string connectionString)
    {
        // MySQL connection logic
    }

    public DataTable ExecuteQuery(string sql)
    {
        // MySQL query
    }
}

// YourApp.exe - Runtime selection
public class DataProviderFactory
{
    public static IDataProvider CreateProvider()
    {
        string providerType = ConfigurationManager.AppSettings["DatabaseProvider"];

        switch (providerType)
        {
            case "SqlServer":
                return LoadProvider<SqlServerProvider>("SqlServerProvider.dll");
            case "MySql":
                return LoadProvider<MySqlProvider>("MySqlProvider.dll");
            default:
                throw new Exception("Unknown provider");
        }
    }

    private static T LoadProvider<T>(string dllName) where T : IDataProvider
    {
        Assembly assembly = Assembly.LoadFrom(dllName);
        Type type = typeof(T);
        return (T)Activator.CreateInstance(type);
    }
}

// Usage
IDataProvider provider = DataProviderFactory.CreateProvider();
provider.Connect("connection string");
```

**Benefits:**
✅ Flexible deployment
✅ Support multiple scenarios without recompiling
✅ Customers can choose their preferred option

**Use When:**
- Multiple implementations of same functionality
- Customer-specific requirements
- Different deployment scenarios

### Pattern 5: Versioned Components

**What:** Maintain multiple versions simultaneously

**Example: API Versioning**
```
API Application:
├── APICore.exe
├── API_v1.dll
├── API_v2.dll
└── API_v3.dll

Clients can call:
- api.com/v1/users (uses API_v1.dll)
- api.com/v2/users (uses API_v2.dll)
- api.com/v3/users (uses API_v3.dll)
```

**Benefits:**
✅ Backward compatibility
✅ Gradual migration
✅ Different clients use different versions

## 🔍 Specific Use Cases for WPF 3D Applications

### Use Case 1: 3D Rendering Engine DLL

**Scenario:** Building a WPF 3D rendering library

**Architecture:**
```
YourWPF3DApp.exe
│
├── Rendering3D.dll        ← Core rendering engine
│   ├── Scene management
│   ├── Camera systems
│   └── Render loop
│
├── Geometry.dll           ← 3D geometry utilities
│   ├── Mesh generation
│   ├── Primitive shapes
│   └── Geometry operations
│
├── Materials.dll          ← Material system
│   ├── Shaders
│   ├── Textures
│   └── Lighting
│
└── Physics.dll            ← Optional physics
    ├── Collision detection
    └── Rigid body dynamics
```

**Why DLLs?**
```
✅ Update rendering engine without changing app
✅ Reuse in multiple 3D applications
✅ Optional features (physics can be excluded)
✅ Third-party can extend (new materials, effects)
✅ Easier testing (test each DLL independently)
```

**Example Code:**
```csharp
// Rendering3D.dll
public class Scene3D
{
    public Camera Camera { get; set; }
    public List<Mesh3D> Meshes { get; set; }

    public void Render(Viewport3D viewport)
    {
        // Rendering logic
    }
}

// Geometry.dll
public class MeshGenerator
{
    public static Mesh3D CreateSphere(double radius, int segments)
    {
        // Generate sphere mesh
    }

    public static Mesh3D CreateCube(double size)
    {
        // Generate cube mesh
    }
}

// YourWPF3DApp.exe
using Rendering3D;
using Geometry;

public partial class MainWindow : Window
{
    private Scene3D scene = new Scene3D();

    private void CreateScene()
    {
        scene.Camera = new Camera();

        // Use Geometry DLL to create meshes
        var sphere = MeshGenerator.CreateSphere(1.0, 32);
        scene.Meshes.Add(sphere);

        // Render using Rendering3D DLL
        scene.Render(viewport3D);
    }
}
```

### Use Case 2: CAD Application

**Scenario:** Computer-Aided Design software

**Architecture:**
```
CADApp.exe
│
├── CADCore.dll            ← Core CAD functionality
├── DrawingTools.dll       ← 2D drawing tools
├── Modeling3D.dll         ← 3D modeling
├── FileFormats.dll        ← Import/Export (DWG, DXF, STL)
├── Rendering.dll          ← Visualization
└── Analysis.dll           ← Structural analysis
```

**Benefits:**
```
✅ Modular: Can license features separately
✅ Extensible: Add new file format DLL
✅ Updateable: Improve rendering without touching core
✅ Optional: Analysis DLL for premium users only
```

### Use Case 3: Game Engine

**Scenario:** 3D game with WPF tools

**Architecture:**
```
GameEngine.exe
│
├── Graphics3D.dll         ← 3D rendering
├── Audio.dll              ← Sound engine
├── Physics.dll            ← Physics simulation
├── AI.dll                 ← AI behaviors
├── Networking.dll         ← Multiplayer
│
Tools (WPF):
├── LevelEditor.exe        ← Uses Graphics3D.dll
├── ModelViewer.exe        ← Uses Graphics3D.dll
└── ParticleEditor.exe     ← Uses Graphics3D.dll
```

**Benefits:**
```
✅ Same rendering DLL for game and tools
✅ Update physics without recompiling game
✅ Can swap Graphics3D.dll for DirectX/OpenGL versions
```

## ⚖️ Decision Matrix: Should You Use a DLL?

### ✅ Use DLL When:

| Scenario | Why DLL? |
|----------|----------|
| **Code used by 2+ applications** | Avoid duplication |
| **Large library (>1MB)** | Memory sharing beneficial |
| **Frequent updates** | Update DLL, not all apps |
| **Plugin/extension system** | Dynamic loading |
| **Optional features** | Include/exclude DLL |
| **Third-party integration** | Standard interface |
| **Language interop** | C++ DLL from C# |

### ❌ Don't Use DLL When:

| Scenario | Why Not? |
|----------|----------|
| **Tiny utility (<10KB)** | Overhead not worth it |
| **Used in 1 app only** | No reuse benefit |
| **Never updates** | No flexibility needed |
| **Performance critical** | Avoid indirection |
| **Simple deployment critical** | 1 EXE easier |

## 💡 Best Practices

### 1. Design Clear Interfaces

**Bad:**
```csharp
// Kitchen sink DLL - does everything
public class UtilityLibrary
{
    public void DoEverything() { }
}
```

**Good:**
```csharp
// Focused DLLs with clear purpose
// MathUtilities.dll
public class MathHelper
{
    public double Calculate(double x, double y) { }
}

// StringUtilities.dll
public class StringHelper
{
    public string Format(string input) { }
}
```

### 2. Version Your DLLs

```csharp
// AssemblyInfo.cs
[assembly: AssemblyVersion("1.2.3.4")]
//                          │ │ │ └─ Build
//                          │ │ └─── Revision
//                          │ └───── Minor version
//                          └─────── Major version

// Breaking changes → Increment MAJOR (1.0 → 2.0)
// New features → Increment MINOR (1.0 → 1.1)
// Bug fixes → Increment REVISION (1.0.0 → 1.0.1)
```

### 3. Document Dependencies

```xml
<!-- Create a manifest or documentation -->
<Application>
  <Name>My3DApp</Name>
  <Dependencies>
    <DLL name="Rendering3D.dll" version="1.0.0" required="true" />
    <DLL name="Physics.dll" version="2.1.0" required="false" />
  </Dependencies>
</Application>
```

### 4. Handle Missing DLLs Gracefully

```csharp
public class PluginLoader
{
    public IPlugin LoadPlugin(string dllPath)
    {
        try
        {
            if (!File.Exists(dllPath))
            {
                Logger.LogWarning($"Plugin not found: {dllPath}");
                return null;
            }

            Assembly assembly = Assembly.LoadFrom(dllPath);
            // ... load plugin
        }
        catch (Exception ex)
        {
            Logger.LogError($"Failed to load plugin: {ex.Message}");
            return null;
        }
    }
}
```

### 5. Use Dependency Injection

```csharp
// Define interface
public interface IRenderer
{
    void Render(Scene scene);
}

// Multiple implementations in DLLs
public class DirectXRenderer : IRenderer { }  // DirectXRenderer.dll
public class OpenGLRenderer : IRenderer { }   // OpenGLRenderer.dll

// Application chooses at runtime
public class Application
{
    private IRenderer renderer;

    public Application(IRenderer renderer)
    {
        this.renderer = renderer;  // Injected!
    }

    public void Run()
    {
        renderer.Render(scene);
    }
}

// Startup
var renderer = CreateRenderer(); // From config
var app = new Application(renderer);
```

## 📊 Real-World Case Studies

### Case Study 1: Microsoft Office

**Problem:** Word, Excel, PowerPoint have many shared features

**Solution:** Shared DLLs
```
Office Installation:
├── WINWORD.EXE (Word)
├── EXCEL.EXE (Excel)
├── POWERPNT.EXE (PowerPoint)
│
Shared DLLs:
├── mso.dll         ← Office core features
├── vbe7.dll        ← VBA macro engine
├── graph.dll       ← Chart engine
└── msspell.dll     ← Spell checker
```

**Benefits:**
- Spell checker used by all apps
- Fix spell bug once → all apps fixed
- Saves disk space (3 apps, 1 spell checker)

### Case Study 2: Adobe Photoshop

**Plugin Architecture:**
```
Photoshop.exe
Plug-ins/
├── Filters/
│   ├── Blur.dll
│   ├── Sharpen.dll
│   └── Noise.dll
├── Import/
│   ├── RAW.dll
│   ├── TIFF.dll
│   └── PSD.dll
└── Effects/
    ├── Lighting.dll
    └── 3D.dll
```

**Benefits:**
- Third-party plugins
- Users choose which to install
- Can update plugins independently

### Case Study 3: Your WPF 3D Application

**Scenario:** Building a 3D model viewer for architecture

**Recommended Architecture:**
```
ArchitectureViewer.exe         ← Main WPF application
│
├── Core3DEngine.dll           ← Your rendering DLL
│   ├── Scene graph
│   ├── Camera control
│   └── Rendering pipeline
│
├── FileFormats.dll            ← Import/export
│   ├── OBJ importer
│   ├── FBX importer
│   └── STL exporter
│
├── MaterialSystem.dll         ← Materials
│   ├── PBR materials
│   └── Texture management
│
└── Measurements.dll           ← Optional feature
    └── Distance/area calculations
```

**Why This Works:**
```
✅ Can build multiple viewers using Core3DEngine.dll
✅ Add new file format → just add new DLL
✅ Measurements.dll optional (premium feature)
✅ Update rendering without touching file I/O
✅ Easy to test each component
```

## 📝 Summary

### When to Use DLLs

**Perfect for:**
- Code reuse across multiple applications
- Plugin architectures
- Modular, large applications
- Updatable components
- Optional features
- Third-party integration

**Not ideal for:**
- Single-use, small utilities
- Performance-critical hot paths
- Simple, standalone tools
- When deployment complexity is a concern

### Key Principles

1. **Separation of Concerns:** Each DLL has one clear purpose
2. **Dependency Management:** Minimize and document dependencies
3. **Versioning:** Always version your DLLs
4. **Interface Design:** Stable, well-documented APIs
5. **Error Handling:** Gracefully handle missing DLLs

### For Your WPF 3D Project

**Recommended:**
```
✅ Create Rendering3D.dll for core engine
✅ Separate Geometry.dll for mesh operations
✅ Plugin DLLs for import/export formats
✅ Optional Physics.dll for advanced features
```

## 🚀 Next Steps

In **Lesson 5: DLL Dependencies**, we'll explore:
- Understanding dependency chains
- The GAC (Global Assembly Cache)
- Managing NuGet packages
- Resolving dependency conflicts
- Tools for analyzing dependencies

## 💪 Practice Exercise

**Design Exercise:**

You're building a **WPF 3D Floor Plan Designer** for interior designers.

**Requirements:**
- Load 3D models of furniture
- Place furniture in rooms
- Render realistic materials
- Export to PDF
- Calculate costs (optional premium feature)

**Task:** Design the DLL architecture

**Your answer should include:**
1. List of DLLs needed
2. What each DLL does
3. Why you chose DLLs vs monolithic app
4. Dependency diagram

**Example Answer:**
```
MainApp.exe
├── Rendering3D.dll (3D visualization)
├── FurnitureLibrary.dll (furniture models)
├── FloorPlanEngine.dll (room logic)
├── MaterialsDB.dll (textures, materials)
├── PDFExporter.dll (export feature)
└── CostCalculator.dll (optional premium)

Rationale:
- Can reuse Rendering3D.dll in other 3D apps
- Furniture library can be updated separately
- PDF exporter can be replaced/updated
- Cost calculator optional → DLL can be excluded for free version
```

---

**Excellent work!** 🎉 You now know when and why to use DLLs!

**Next Lesson:** [Lesson 5: DLL Dependencies](Lesson05-DLL-Dependencies.md)
