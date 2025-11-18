# Lesson 7: DLL Interfaces and Contracts

## 📚 Learning Objectives

By the end of this lesson, you will understand:
- What interfaces are and why they're essential for DLLs
- How to design clean API contracts
- Dependency Inversion Principle
- Plugin architecture using interfaces
- Multiple implementations of the same interface
- Interface segregation

## 🎯 What is an Interface?

An **interface** is a **contract** that defines what a class must do, without specifying how it does it.

### The Simple Analogy

Think of a power outlet:
```
Interface (Power Outlet):
- Must provide: Electricity
- Must have: 2 or 3 prongs
- Standard: 120V AC

Implementations (Devices):
- Lamp (uses electricity for light)
- Phone charger (uses electricity to charge)
- Computer (uses electricity to compute)

All different devices, same interface!
```

### Code Example

```csharp
// Interface: The CONTRACT
public interface ICalculator
{
    int Add(int a, int b);
    int Subtract(int a, int b);
}

// Implementation 1: Basic calculator
public class BasicCalculator : ICalculator
{
    public int Add(int a, int b) => a + b;
    public int Subtract(int a, int b) => a - b;
}

// Implementation 2: Logging calculator
public class LoggingCalculator : ICalculator
{
    public int Add(int a, int b)
    {
        Console.WriteLine($"Adding {a} + {b}");
        return a + b;
    }

    public int Subtract(int a, int b)
    {
        Console.WriteLine($"Subtracting {a} - {b}");
        return a - b;
    }
}

// Usage: Code depends on INTERFACE, not implementation
public void UseCalculator(ICalculator calc)  // ← Takes interface!
{
    var result = calc.Add(5, 3);  // Works with ANY ICalculator
}
```

## 🔑 Why Interfaces Are Critical for DLLs

### Problem Without Interfaces

**Scenario:** You create a DLL with a concrete class

```csharp
// Version 1.0 of YourLibrary.dll
public class DataStorage
{
    public void SaveToFile(string data)
    {
        File.WriteAllText("data.txt", data);
    }

    public string LoadFromFile()
    {
        return File.ReadAllText("data.txt");
    }
}

// Application uses it
var storage = new DataStorage();  // ← Tightly coupled!
storage.SaveToFile("Important data");
```

**What if you want to:**
- Save to database instead of file?
- Save to cloud storage?
- Add caching?
- Use different implementations for testing?

**You're stuck!** Your app depends on the concrete `DataStorage` class.

### Solution With Interfaces

```csharp
// Contracts.dll (Interface definition)
public interface IDataStorage
{
    void Save(string data);
    string Load();
}

// FileStorage.dll (Implementation 1)
public class FileStorage : IDataStorage
{
    public void Save(string data) => File.WriteAllText("data.txt", data);
    public string Load() => File.ReadAllText("data.txt");
}

// DatabaseStorage.dll (Implementation 2)
public class DatabaseStorage : IDataStorage
{
    public void Save(string data)
    {
        // Save to database
    }

    public string Load()
    {
        // Load from database
        return "";
    }
}

// CloudStorage.dll (Implementation 3)
public class CloudStorage : IDataStorage
{
    public void Save(string data)
    {
        // Upload to cloud
    }

    public string Load()
    {
        // Download from cloud
        return "";
    }
}

// Application: Depends ONLY on interface!
public class Application
{
    private IDataStorage storage;  // ← Interface!

    public Application(IDataStorage storage)
    {
        this.storage = storage;  // Any implementation works!
    }

    public void DoWork()
    {
        storage.Save("Important data");  // Doesn't know/care how it's saved
        string data = storage.Load();
    }
}

// Usage: Choose implementation at runtime
IDataStorage storage = new FileStorage();      // Use file
// IDataStorage storage = new DatabaseStorage();  // Or database
// IDataStorage storage = new CloudStorage();     // Or cloud

var app = new Application(storage);
app.DoWork();
```

**Benefits:**
✅ Can swap implementations without changing Application code
✅ Can test with mock storage
✅ Can add new storage types without modifying existing code
✅ Loosely coupled design

## 🏗️ Building a Complete Plugin Architecture

Let's build a real-world example: A 3D Model Import system for WPF.

### Step 1: Define the Contract (Interfaces.dll)

```csharp
// IModelImporter.cs
using System;
using System.Collections.Generic;

namespace ModelImport.Contracts
{
    /// <summary>
    /// Represents 3D model data
    /// </summary>
    public class Model3D
    {
        public string Name { get; set; }
        public List<Vertex> Vertices { get; set; }
        public List<Triangle> Triangles { get; set; }
        public List<Material> Materials { get; set; }
    }

    public class Vertex
    {
        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }
    }

    public class Triangle
    {
        public int Vertex1Index { get; set; }
        public int Vertex2Index { get; set; }
        public int Vertex3Index { get; set; }
    }

    public class Material
    {
        public string Name { get; set; }
        public string TexturePath { get; set; }
    }

    /// <summary>
    /// Contract for 3D model importers
    /// </summary>
    public interface IModelImporter
    {
        /// <summary>
        /// Supported file extensions (e.g., ".obj", ".fbx")
        /// </summary>
        string[] SupportedExtensions { get; }

        /// <summary>
        /// Display name of this importer
        /// </summary>
        string Name { get; }

        /// <summary>
        /// Description of the importer
        /// </summary>
        string Description { get; }

        /// <summary>
        /// Checks if this importer can handle the given file
        /// </summary>
        bool CanImport(string filePath);

        /// <summary>
        /// Imports a 3D model from file
        /// </summary>
        Model3D Import(string filePath);
    }
}
```

### Step 2: Create Implementations (Plugin DLLs)

**ObjImporter.dll:**

```csharp
using ModelImport.Contracts;
using System;
using System.IO;
using System.Linq;

namespace ObjImporter
{
    public class ObjModelImporter : IModelImporter
    {
        public string[] SupportedExtensions => new[] { ".obj" };

        public string Name => "Wavefront OBJ Importer";

        public string Description => "Imports .obj 3D model files";

        public bool CanImport(string filePath)
        {
            return filePath.EndsWith(".obj", StringComparison.OrdinalIgnoreCase);
        }

        public Model3D Import(string filePath)
        {
            var model = new Model3D
            {
                Name = Path.GetFileNameWithoutExtension(filePath),
                Vertices = new List<Vertex>(),
                Triangles = new List<Triangle>(),
                Materials = new List<Material>()
            };

            foreach (var line in File.ReadLines(filePath))
            {
                if (line.StartsWith("v "))
                {
                    // Parse vertex
                    var parts = line.Split(' ');
                    model.Vertices.Add(new Vertex
                    {
                        X = double.Parse(parts[1]),
                        Y = double.Parse(parts[2]),
                        Z = double.Parse(parts[3])
                    });
                }
                else if (line.StartsWith("f "))
                {
                    // Parse face
                    var parts = line.Split(' ');
                    model.Triangles.Add(new Triangle
                    {
                        Vertex1Index = int.Parse(parts[1].Split('/')[0]) - 1,
                        Vertex2Index = int.Parse(parts[2].Split('/')[0]) - 1,
                        Vertex3Index = int.Parse(parts[3].Split('/')[0]) - 1
                    });
                }
            }

            return model;
        }
    }
}
```

**FbxImporter.dll:**

```csharp
using ModelImport.Contracts;

namespace FbxImporter
{
    public class FbxModelImporter : IModelImporter
    {
        public string[] SupportedExtensions => new[] { ".fbx" };

        public string Name => "FBX Importer";

        public string Description => "Imports Autodesk FBX files";

        public bool CanImport(string filePath)
        {
            return filePath.EndsWith(".fbx", StringComparison.OrdinalIgnoreCase);
        }

        public Model3D Import(string filePath)
        {
            // Implementation using FBX SDK or library
            // This is simplified for demonstration
            var model = new Model3D
            {
                Name = Path.GetFileNameWithoutExtension(filePath),
                Vertices = new List<Vertex>(),
                Triangles = new List<Triangle>(),
                Materials = new List<Material>()
            };

            // FBX parsing logic here...

            return model;
        }
    }
}
```

**StlImporter.dll:**

```csharp
using ModelImport.Contracts;

namespace StlImporter
{
    public class StlModelImporter : IModelImporter
    {
        public string[] SupportedExtensions => new[] { ".stl" };

        public string Name => "STL Importer";

        public string Description => "Imports STL (stereolithography) files";

        public bool CanImport(string filePath)
        {
            return filePath.EndsWith(".stl", StringComparison.OrdinalIgnoreCase);
        }

        public Model3D Import(string filePath)
        {
            // STL parsing logic
            var model = new Model3D { /* ... */ };
            return model;
        }
    }
}
```

### Step 3: Plugin Manager (Main Application)

```csharp
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using ModelImport.Contracts;

namespace ModelViewer
{
    public class ImporterPluginManager
    {
        private List<IModelImporter> importers = new List<IModelImporter>();

        /// <summary>
        /// Loads all importer plugins from the Plugins folder
        /// </summary>
        public void LoadPlugins(string pluginDirectory = "Plugins")
        {
            if (!Directory.Exists(pluginDirectory))
            {
                Directory.CreateDirectory(pluginDirectory);
                return;
            }

            // Find all DLL files
            var dllFiles = Directory.GetFiles(pluginDirectory, "*.dll");

            foreach (var dllFile in dllFiles)
            {
                try
                {
                    // Load the assembly
                    Assembly assembly = Assembly.LoadFrom(dllFile);

                    // Find all types that implement IModelImporter
                    var importerTypes = assembly.GetTypes()
                        .Where(t => typeof(IModelImporter).IsAssignableFrom(t) && !t.IsInterface);

                    foreach (var type in importerTypes)
                    {
                        // Create instance
                        var importer = (IModelImporter)Activator.CreateInstance(type);
                        importers.Add(importer);

                        Console.WriteLine($"Loaded plugin: {importer.Name}");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Failed to load plugin {dllFile}: {ex.Message}");
                }
            }
        }

        /// <summary>
        /// Gets all loaded importers
        /// </summary>
        public IEnumerable<IModelImporter> GetImporters()
        {
            return importers;
        }

        /// <summary>
        /// Finds the appropriate importer for a file
        /// </summary>
        public IModelImporter FindImporter(string filePath)
        {
            return importers.FirstOrDefault(i => i.CanImport(filePath));
        }

        /// <summary>
        /// Imports a model using the appropriate plugin
        /// </summary>
        public Model3D ImportModel(string filePath)
        {
            var importer = FindImporter(filePath);

            if (importer == null)
            {
                throw new NotSupportedException($"No importer found for file: {filePath}");
            }

            Console.WriteLine($"Using importer: {importer.Name}");
            return importer.Import(filePath);
        }
    }
}
```

### Step 4: Using the Plugin System

```csharp
// In your WPF Application or Console App
using System;
using ModelViewer;

class Program
{
    static void Main(string[] args)
    {
        var pluginManager = new ImporterPluginManager();

        // Load all plugins from the Plugins folder
        pluginManager.LoadPlugins();

        // List available importers
        Console.WriteLine("Available Importers:");
        foreach (var importer in pluginManager.GetImporters())
        {
            Console.WriteLine($"  - {importer.Name}");
            Console.WriteLine($"    Extensions: {string.Join(", ", importer.SupportedExtensions)}");
            Console.WriteLine($"    {importer.Description}");
            Console.WriteLine();
        }

        // Import a model
        try
        {
            string modelFile = "models/teapot.obj";
            var model = pluginManager.ImportModel(modelFile);

            Console.WriteLine($"Imported model: {model.Name}");
            Console.WriteLine($"  Vertices: {model.Vertices.Count}");
            Console.WriteLine($"  Triangles: {model.Triangles.Count}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }
}
```

### Directory Structure

```
YourApp.exe
Plugins/
├── ObjImporter.dll       ← Plugin 1
├── FbxImporter.dll       ← Plugin 2
└── StlImporter.dll       ← Plugin 3

Contracts/
└── ModelImport.Contracts.dll  ← Shared interface
```

## 🎨 Interface Design Principles

### 1. Interface Segregation Principle

**Bad: One huge interface**
```csharp
// ❌ Fat interface - forces implementations to do too much
public interface IShape3D
{
    double CalculateArea();
    double CalculateVolume();
    double CalculatePerimeter();
    void Render();
    void Export(string format);
    void Import(string format);
    void Animate();
    void ApplyTexture(string texturePath);
}

// Problem: Simple shapes must implement everything!
public class Sphere : IShape3D
{
    public double CalculateArea() { /* OK */ }
    public double CalculateVolume() { /* OK */ }
    public double CalculatePerimeter() { return 0; } // Doesn't make sense!
    public void Render() { /* OK */ }
    public void Export(string format) { throw new NotImplementedException(); }
    public void Import(string format) { throw new NotImplementedException(); }
    public void Animate() { throw new NotImplementedException(); }
    public void ApplyTexture(string texturePath) { throw new NotImplementedException(); }
}
```

**Good: Segregated interfaces**
```csharp
// ✅ Small, focused interfaces
public interface IShape3D
{
    double CalculateArea();
    double CalculateVolume();
}

public interface IRenderable
{
    void Render();
}

public interface IImportable
{
    void Import(string format);
}

public interface IExportable
{
    void Export(string format);
}

public interface IAnimatable
{
    void Animate();
}

public interface ITexturable
{
    void ApplyTexture(string texturePath);
}

// Now implement only what you need!
public class Sphere : IShape3D, IRenderable, ITexturable
{
    public double CalculateArea() { /* ... */ }
    public double CalculateVolume() { /* ... */ }
    public void Render() { /* ... */ }
    public void ApplyTexture(string texturePath) { /* ... */ }
    // No need to implement animation, import/export, etc.!
}
```

### 2. Dependency Inversion Principle

**High-level modules should not depend on low-level modules. Both should depend on abstractions.**

**Bad:**
```csharp
// ❌ Direct dependency on concrete class
public class Scene3D
{
    private FileLogger logger = new FileLogger();  // ← Tightly coupled!

    public void AddModel(Model model)
    {
        logger.Log("Model added");  // Can't use different logger!
    }
}
```

**Good:**
```csharp
// ✅ Depend on interface
public interface ILogger
{
    void Log(string message);
}

public class FileLogger : ILogger
{
    public void Log(string message)
    {
        File.AppendAllText("log.txt", message + "\n");
    }
}

public class ConsoleLogger : ILogger
{
    public void Log(string message)
    {
        Console.WriteLine(message);
    }
}

public class Scene3D
{
    private ILogger logger;  // ← Depends on interface!

    public Scene3D(ILogger logger)
    {
        this.logger = logger;  // Inject any logger!
    }

    public void AddModel(Model model)
    {
        logger.Log("Model added");  // Works with any ILogger!
    }
}

// Usage:
var scene1 = new Scene3D(new FileLogger());
var scene2 = new Scene3D(new ConsoleLogger());
```

### 3. Design by Contract

```csharp
/// <summary>
/// Interface for 3D rendering engines
/// </summary>
public interface IRenderEngine
{
    /// <summary>
    /// Initializes the rendering engine
    /// </summary>
    /// <exception cref="RenderException">If initialization fails</exception>
    void Initialize();

    /// <summary>
    /// Renders the scene
    /// </summary>
    /// <param name="scene">Scene to render (must not be null)</param>
    /// <exception cref="ArgumentNullException">If scene is null</exception>
    /// <exception cref="RenderException">If rendering fails</exception>
    void Render(Scene scene);

    /// <summary>
    /// Gets the frames per second
    /// </summary>
    /// <returns>FPS (always >= 0)</returns>
    double GetFPS();

    /// <summary>
    /// Releases all resources
    /// </summary>
    void Dispose();
}
```

## 💪 Advanced Interface Patterns

### Pattern 1: Factory Interface

```csharp
// Factory creates objects that implement an interface
public interface IShape3DFactory
{
    IShape3D CreateSphere(double radius);
    IShape3D CreateCube(double size);
    IShape3D CreateCylinder(double radius, double height);
}

public class SimpleShapeFactory : IShape3DFactory
{
    public IShape3D CreateSphere(double radius)
    {
        return new SimpleSphere(radius);
    }

    public IShape3D CreateCube(double size)
    {
        return new SimpleCube(size);
    }

    public IShape3D CreateCylinder(double radius, double height)
    {
        return new SimpleCylinder(radius, height);
    }
}
```

### Pattern 2: Strategy Pattern

```csharp
// Different rendering strategies
public interface IRenderStrategy
{
    void Render(Scene3D scene);
}

public class WireframeRenderer : IRenderStrategy
{
    public void Render(Scene3D scene)
    {
        // Render as wireframe
    }
}

public class SolidRenderer : IRenderStrategy
{
    public void Render(Scene3D scene)
    {
        // Render as solid
    }
}

public class RayTracedRenderer : IRenderStrategy
{
    public void Render(Scene3D scene)
    {
        // Ray tracing
    }
}

// Usage: Choose strategy at runtime
public class Renderer
{
    private IRenderStrategy strategy;

    public void SetStrategy(IRenderStrategy strategy)
    {
        this.strategy = strategy;
    }

    public void Render(Scene3D scene)
    {
        strategy.Render(scene);
    }
}
```

### Pattern 3: Observer Pattern

```csharp
public interface ISceneObserver
{
    void OnModelAdded(Model3D model);
    void OnModelRemoved(Model3D model);
    void OnSceneCleared();
}

public class Scene3D
{
    private List<ISceneObserver> observers = new List<ISceneObserver>();

    public void AddObserver(ISceneObserver observer)
    {
        observers.Add(observer);
    }

    public void AddModel(Model3D model)
    {
        // Add model logic...

        // Notify all observers
        foreach (var observer in observers)
        {
            observer.OnModelAdded(model);
        }
    }
}

// Usage:
public class UIUpdater : ISceneObserver
{
    public void OnModelAdded(Model3D model)
    {
        Console.WriteLine($"UI: Model {model.Name} added");
    }

    public void OnModelRemoved(Model3D model)
    {
        Console.WriteLine($"UI: Model {model.Name} removed");
    }

    public void OnSceneCleared()
    {
        Console.WriteLine("UI: Scene cleared");
    }
}
```

## 📝 Summary

### What We Learned

✅ Interfaces define contracts (what, not how)
✅ Interfaces enable loose coupling
✅ Plugin architectures rely on interfaces
✅ Interface Segregation: Small, focused interfaces
✅ Dependency Inversion: Depend on abstractions
✅ Multiple implementations of same interface

### Key Benefits for DLLs

1. **Flexibility:** Swap implementations without changing code
2. **Testability:** Mock interfaces for unit tests
3. **Extensibility:** Add plugins without modifying core
4. **Maintainability:** Changes to implementation don't affect contract
5. **Versioning:** Interfaces provide stable API

## 🚀 Next Steps

In **Lesson 8: Versioning and Strong Naming**, we'll learn:
- Assembly versioning strategies
- Strong naming assemblies
- Side-by-side execution
- Backwards compatibility
- Breaking vs non-breaking changes

## 💪 Practice Exercises

### Exercise 1: Create a Logger System

Create a logging library with interfaces:

```csharp
// ILogger interface
// FileLogger implementation
// ConsoleLogger implementation
// DatabaseLogger implementation
// MultiLogger (logs to multiple destinations)
```

### Exercise 2: Plugin Architecture for Export

Create an export system:

```csharp
// IExporter interface
// PdfExporter.dll
// ImageExporter.dll
// ExcelExporter.dll
// ExportManager (loads plugins)
```

### Exercise 3: 3D Rendering Strategies

Design interfaces for different rendering techniques:

```csharp
// IRenderPipeline interface
// Forward rendering implementation
// Deferred rendering implementation
// Ray tracing implementation
```

---

**Fantastic!** 🎉 You now understand how to design DLLs with interfaces!

**Next Lesson:** [Lesson 8: Versioning and Strong Naming](Lesson08-Versioning-and-Strong-Naming.md)
