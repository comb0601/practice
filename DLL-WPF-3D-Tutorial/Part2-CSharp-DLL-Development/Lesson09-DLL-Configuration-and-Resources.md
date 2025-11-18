# Lesson 9: DLL Configuration and Resources

## 📚 Learning Objectives

By the end of this lesson, you will understand:
- Configuration files for DLLs
- Embedded resources in assemblies
- Resource files (.resx)
- Satellite assemblies for localization
- Custom configuration sections
- Accessing resources at runtime
- Best practices for managing resources

## 🎯 Why Configuration and Resources?

DLLs often need:
- **Configuration:** Database connection strings, API keys, settings
- **Resources:** Images, icons, text files, data files
- **Localization:** Translations for multiple languages

## ⚙️ DLL Configuration

### Configuration File Basics

**Problem:** DLLs don't have their own .config files!

```
YourApp.exe has: YourApp.exe.config ✅
YourLibrary.dll has: ??? ❌ No config file!
```

**Solution:** DLLs use the **host application's** config file.

### Example Structure

```
MyApplication/
├── MyApp.exe
├── MyApp.exe.config        ← Configuration file
└── MyLibrary.dll           ← Uses MyApp.exe.config
```

### Reading Configuration in DLL

**MyApp.exe.config:**

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <appSettings>
    <add key="DatabaseServer" value="localhost" />
    <add key="DatabaseName" value="MyDatabase" />
    <add key="MaxRetries" value="3" />
    <add key="Timeout" value="30" />
  </appSettings>

  <connectionStrings>
    <add name="DefaultConnection"
         connectionString="Server=localhost;Database=MyDB;Trusted_Connection=True;"
         providerName="System.Data.SqlClient" />
  </connectionStrings>
</configuration>
```

**MyLibrary.dll Code:**

```csharp
using System;
using System.Configuration;

namespace MyLibrary
{
    public class DatabaseHelper
    {
        public string GetConnectionString()
        {
            // Read from host app's config file
            return ConfigurationManager.ConnectionStrings["DefaultConnection"].ConnectionString;
        }

        public int GetMaxRetries()
        {
            string value = ConfigurationManager.AppSettings["MaxRetries"];
            return int.Parse(value);
        }

        public void Connect()
        {
            string connString = GetConnectionString();
            int maxRetries = GetMaxRetries();

            Console.WriteLine($"Connecting to: {connString}");
            Console.WriteLine($"Max retries: {maxRetries}");
        }
    }
}
```

**Important:** Add reference to `System.Configuration.dll`

```xml
<!-- In .csproj -->
<ItemGroup>
  <Reference Include="System.Configuration" />
</ItemGroup>
```

### Custom Configuration Sections

**Define Custom Section:**

```csharp
using System.Configuration;

namespace MyLibrary.Configuration
{
    // Configuration section
    public class RenderingConfigSection : ConfigurationSection
    {
        [ConfigurationProperty("quality", DefaultValue = "High", IsRequired = false)]
        public string Quality
        {
            get { return (string)this["quality"]; }
            set { this["quality"] = value; }
        }

        [ConfigurationProperty("antiAliasing", DefaultValue = true, IsRequired = false)]
        public bool AntiAliasing
        {
            get { return (bool)this["antiAliasing"]; }
            set { this["antiAliasing"] = value; }
        }

        [ConfigurationProperty("maxFPS", DefaultValue = 60, IsRequired = false)]
        public int MaxFPS
        {
            get { return (int)this["maxFPS"]; }
            set { this["maxFPS"] = value; }
        }
    }

    // Configuration element for collection
    public class ModelElement : ConfigurationElement
    {
        [ConfigurationProperty("name", IsRequired = true, IsKey = true)]
        public string Name
        {
            get { return (string)this["name"]; }
            set { this["name"] = value; }
        }

        [ConfigurationProperty("path", IsRequired = true)]
        public string Path
        {
            get { return (string)this["path"]; }
            set { this["path"] = value; }
        }
    }

    // Configuration collection
    public class ModelCollection : ConfigurationElementCollection
    {
        protected override ConfigurationElement CreateNewElement()
        {
            return new ModelElement();
        }

        protected override object GetElementKey(ConfigurationElement element)
        {
            return ((ModelElement)element).Name;
        }

        public ModelElement this[int index]
        {
            get { return (ModelElement)BaseGet(index); }
        }
    }

    // Extended config section with collection
    public class Scene3DConfigSection : ConfigurationSection
    {
        [ConfigurationProperty("models")]
        public ModelCollection Models
        {
            get { return (ModelCollection)this["models"]; }
        }
    }
}
```

**Use in app.config:**

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>

  <!-- Register custom sections -->
  <configSections>
    <section name="rendering"
             type="MyLibrary.Configuration.RenderingConfigSection, MyLibrary" />
    <section name="scene3D"
             type="MyLibrary.Configuration.Scene3DConfigSection, MyLibrary" />
  </configSections>

  <!-- Custom rendering configuration -->
  <rendering quality="Ultra" antiAliasing="true" maxFPS="144" />

  <!-- Custom scene configuration -->
  <scene3D>
    <models>
      <add name="Cube" path="models/cube.obj" />
      <add name="Sphere" path="models/sphere.obj" />
      <add name="Teapot" path="models/teapot.obj" />
    </models>
  </scene3D>

</configuration>
```

**Read Custom Configuration:**

```csharp
public class ConfigReader
{
    public static void ReadRenderingConfig()
    {
        var config = ConfigurationManager.GetSection("rendering") as RenderingConfigSection;

        if (config != null)
        {
            Console.WriteLine($"Quality: {config.Quality}");
            Console.WriteLine($"Anti-Aliasing: {config.AntiAliasing}");
            Console.WriteLine($"Max FPS: {config.MaxFPS}");
        }
    }

    public static void ReadScene3DConfig()
    {
        var config = ConfigurationManager.GetSection("scene3D") as Scene3DConfigSection;

        if (config != null)
        {
            foreach (ModelElement model in config.Models)
            {
                Console.WriteLine($"Model: {model.Name} at {model.Path}");
            }
        }
    }
}
```

## 📦 Embedded Resources

### What Are Embedded Resources?

Files **compiled into** your DLL as binary data.

**Benefits:**
- ✅ Single file deployment (DLL contains everything)
- ✅ Can't be accidentally deleted
- ✅ Can't be modified by users
- ✅ Protected from tampering

**Use Cases:**
- Default images/icons
- Template files
- Data files
- Localized strings

### Adding Embedded Resources

**Step 1: Add File to Project**

```
Solution Explorer:
  Right-click project → Add → Existing Item
  Select file (e.g., Logo.png)
```

**Step 2: Set Build Action**

```
Select Logo.png in Solution Explorer
Properties Window:
  Build Action: Embedded Resource
```

**Visual Studio Project Structure:**

```
MyLibrary/
├── Images/
│   ├── Logo.png (Build Action: Embedded Resource)
│   └── Icon.ico (Build Action: Embedded Resource)
├── Data/
│   └── DefaultSettings.xml (Build Action: Embedded Resource)
└── Templates/
    └── ReportTemplate.html (Build Action: Embedded Resource)
```

### Accessing Embedded Resources

**Method 1: GetManifestResourceStream**

```csharp
using System;
using System.IO;
using System.Reflection;

namespace MyLibrary
{
    public class ResourceHelper
    {
        public static Stream GetResourceStream(string resourceName)
        {
            // Get current assembly
            Assembly assembly = Assembly.GetExecutingAssembly();

            // Resource naming: Namespace.Folder.FileName
            // Example: MyLibrary.Images.Logo.png
            string fullName = $"MyLibrary.{resourceName}";

            return assembly.GetManifestResourceStream(fullName);
        }

        public static byte[] GetResourceBytes(string resourceName)
        {
            using (Stream stream = GetResourceStream(resourceName))
            {
                if (stream == null)
                    throw new FileNotFoundException($"Resource not found: {resourceName}");

                byte[] buffer = new byte[stream.Length];
                stream.Read(buffer, 0, buffer.Length);
                return buffer;
            }
        }

        public static string GetResourceText(string resourceName)
        {
            using (Stream stream = GetResourceStream(resourceName))
            using (StreamReader reader = new StreamReader(stream))
            {
                return reader.ReadToEnd();
            }
        }
    }
}
```

**Usage:**

```csharp
// Load image
byte[] imageData = ResourceHelper.GetResourceBytes("Images.Logo.png");

// Load text file
string template = ResourceHelper.GetResourceText("Templates.ReportTemplate.html");

// Use in WPF
using (Stream stream = ResourceHelper.GetResourceStream("Images.Logo.png"))
{
    BitmapImage bitmap = new BitmapImage();
    bitmap.BeginInit();
    bitmap.StreamSource = stream;
    bitmap.CacheOption = BitmapCacheOption.OnLoad;
    bitmap.EndInit();

    myImage.Source = bitmap;
}
```

**Method 2: List All Resources**

```csharp
public static void ListEmbeddedResources()
{
    Assembly assembly = Assembly.GetExecutingAssembly();
    string[] resourceNames = assembly.GetManifestResourceNames();

    Console.WriteLine("Embedded Resources:");
    foreach (string name in resourceNames)
    {
        Console.WriteLine($"  - {name}");
    }
}

// Output:
// Embedded Resources:
//   - MyLibrary.Images.Logo.png
//   - MyLibrary.Images.Icon.ico
//   - MyLibrary.Data.DefaultSettings.xml
//   - MyLibrary.Templates.ReportTemplate.html
```

### WPF-Specific Resource Access

**Pack URI in XAML:**

```xml
<!-- In WPF, use pack:// URI -->
<Window x:Class="MyApp.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
    <StackPanel>
        <!-- From same assembly -->
        <Image Source="pack://application:,,,/Images/Logo.png" />

        <!-- From different assembly (MyLibrary.dll) -->
        <Image Source="pack://application:,,,/MyLibrary;component/Images/Logo.png" />
    </StackPanel>
</Window>
```

**Pack URI in Code:**

```csharp
// WPF code-behind
using System;
using System.Windows.Media.Imaging;

public void LoadEmbeddedImage()
{
    // From same assembly
    var uri = new Uri("pack://application:,,,/Images/Logo.png");
    myImage.Source = new BitmapImage(uri);

    // From MyLibrary.dll
    var uri2 = new Uri("pack://application:,,,/MyLibrary;component/Images/Icon.ico");
    myIcon.Source = new BitmapImage(uri2);
}
```

## 📄 Resource Files (.resx)

### What Are .resx Files?

XML-based resource files for:
- Strings (localization)
- Images
- Icons
- Other file types

**Advantage:** Visual Studio designer support!

### Creating a Resource File

**Step 1: Add Resource File**

```
Right-click project → Add → New Item → Resources File
Name: Strings.resx
```

**Step 2: Add Resources in Designer**

```
Strings.resx opens in designer:

Name              | Value                | Comment
------------------|----------------------|------------------
WelcomeMessage    | Welcome to 3D Viewer | Shown on startup
ErrorTitle        | Error                | Error dialog title
SaveSuccess       | File saved!          | Success message
```

**Step 3: Generated Code**

Visual Studio generates `Strings.Designer.cs`:

```csharp
namespace MyLibrary {
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute(...)]
    internal class Strings {

        private static global::System.Resources.ResourceManager resourceMan;

        internal static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp =
                        new global::System.Resources.ResourceManager(
                            "MyLibrary.Strings", typeof(Strings).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }

        internal static string WelcomeMessage {
            get {
                return ResourceManager.GetString("WelcomeMessage", resourceCulture);
            }
        }

        internal static string ErrorTitle {
            get {
                return ResourceManager.GetString("ErrorTitle", resourceCulture);
            }
        }

        // ... more properties
    }
}
```

**Step 4: Use Resources**

```csharp
using MyLibrary;

public void ShowWelcome()
{
    MessageBox.Show(Strings.WelcomeMessage);
}

public void ShowError(string message)
{
    MessageBox.Show(message, Strings.ErrorTitle, MessageBoxButton.OK, MessageBoxImage.Error);
}
```

### Adding Images to .resx

**In Resource Designer:**

```
1. Click "Add Resource" dropdown
2. Select "Add Existing File"
3. Choose image file
4. Image embedded in .resx
```

**Usage:**

```csharp
// Get image from resources
System.Drawing.Image logo = Resources.Logo;

// For WPF
BitmapImage bmp = ConvertToBitmapImage(Resources.Logo);

public BitmapImage ConvertToBitmapImage(System.Drawing.Image image)
{
    using (var memory = new MemoryStream())
    {
        image.Save(memory, System.Drawing.Imaging.ImageFormat.Png);
        memory.Position = 0;

        var bitmapImage = new BitmapImage();
        bitmapImage.BeginInit();
        bitmapImage.StreamSource = memory;
        bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
        bitmapImage.EndInit();
        bitmapImage.Freeze();

        return bitmapImage;
    }
}
```

## 🌍 Localization with Satellite Assemblies

### Creating Multi-Language Support

**Step 1: Default Language (English)**

```
Strings.resx:
WelcomeMessage = Welcome to 3D Viewer
```

**Step 2: French Translation**

```
Create: Strings.fr.resx
WelcomeMessage = Bienvenue dans le visualiseur 3D
```

**Step 3: German Translation**

```
Create: Strings.de.resx
WelcomeMessage = Willkommen im 3D-Viewer
```

**Step 4: Spanish Translation**

```
Create: Strings.es.resx
WelcomeMessage = Bienvenido al visor 3D
```

### File Structure

```
MyLibrary/
├── Strings.resx (default - English)
├── Strings.fr.resx (French)
├── Strings.de.resx (German)
└── Strings.es.resx (Spanish)
```

### Build Output (Satellite Assemblies)

```
bin/Debug/
├── MyLibrary.dll (main assembly)
├── fr/
│   └── MyLibrary.resources.dll (French satellite)
├── de/
│   └── MyLibrary.resources.dll (German satellite)
└── es/
    └── MyLibrary.resources.dll (Spanish satellite)
```

### Using Localized Resources

```csharp
using System.Globalization;
using System.Threading;

public class LocalizationDemo
{
    public static void ChangeLanguage(string cultureName)
    {
        // Set culture
        CultureInfo culture = new CultureInfo(cultureName);
        Thread.CurrentThread.CurrentCulture = culture;
        Thread.CurrentThread.CurrentUICulture = culture;

        // Resources automatically loaded from satellite assembly!
        Console.WriteLine(Strings.WelcomeMessage);
    }

    public static void Demo()
    {
        ChangeLanguage("en-US");  // Welcome to 3D Viewer
        ChangeLanguage("fr-FR");  // Bienvenue dans le visualiseur 3D
        ChangeLanguage("de-DE");  // Willkommen im 3D-Viewer
        ChangeLanguage("es-ES");  // Bienvenido al visor 3D
    }
}
```

### WPF Localization

**App.xaml.cs:**

```csharp
public partial class App : Application
{
    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);

        // Set language based on user preference
        string language = Properties.Settings.Default.Language;
        SetLanguage(language);
    }

    public static void SetLanguage(string cultureName)
    {
        CultureInfo culture = new CultureInfo(cultureName);

        Thread.CurrentThread.CurrentCulture = culture;
        Thread.CurrentThread.CurrentUICulture = culture;

        // Force WPF to use new culture
        FrameworkElement.LanguageProperty.OverrideMetadata(
            typeof(FrameworkElement),
            new FrameworkPropertyMetadata(
                XmlLanguage.GetLanguage(CultureInfo.CurrentCulture.IetfLanguageTag)));
    }
}
```

**MainWindow.xaml (data binding):**

```xml
<Window x:Class="MyApp.MainWindow"
        xmlns:p="clr-namespace:MyLibrary.Properties;assembly=MyLibrary">
    <StackPanel>
        <!-- Bind to resource strings -->
        <TextBlock Text="{x:Static p:Strings.WelcomeMessage}" />

        <ComboBox SelectionChanged="Language_Changed">
            <ComboBoxItem Tag="en-US">English</ComboBoxItem>
            <ComboBoxItem Tag="fr-FR">Français</ComboBoxItem>
            <ComboBoxItem Tag="de-DE">Deutsch</ComboBoxItem>
            <ComboBoxItem Tag="es-ES">Español</ComboBoxItem>
        </ComboBox>
    </StackPanel>
</Window>
```

## 🔧 Advanced Configuration Techniques

### JSON Configuration (.NET Core/.NET 5+)

**.NET Core and newer use JSON instead of XML:**

```json
// appsettings.json
{
  "Rendering": {
    "Quality": "Ultra",
    "AntiAliasing": true,
    "MaxFPS": 144
  },
  "Scene3D": {
    "Models": [
      { "Name": "Cube", "Path": "models/cube.obj" },
      { "Name": "Sphere", "Path": "models/sphere.obj" }
    ]
  },
  "ConnectionStrings": {
    "DefaultConnection": "Server=localhost;Database=MyDB;"
  }
}
```

**Reading JSON Configuration:**

```csharp
using Microsoft.Extensions.Configuration;

public class ConfigService
{
    private IConfiguration configuration;

    public ConfigService()
    {
        configuration = new ConfigurationBuilder()
            .SetBasePath(Directory.GetCurrentDirectory())
            .AddJsonFile("appsettings.json", optional: false, reloadOnChange: true)
            .Build();
    }

    public string GetQuality()
    {
        return configuration["Rendering:Quality"];
    }

    public bool GetAntiAliasing()
    {
        return configuration.GetValue<bool>("Rendering:AntiAliasing");
    }

    public int GetMaxFPS()
    {
        return configuration.GetValue<int>("Rendering:MaxFPS");
    }

    public string GetConnectionString()
    {
        return configuration.GetConnectionString("DefaultConnection");
    }
}
```

**Strongly-typed Configuration:**

```csharp
public class RenderingOptions
{
    public string Quality { get; set; }
    public bool AntiAliasing { get; set; }
    public int MaxFPS { get; set; }
}

public class ConfigService
{
    public RenderingOptions GetRenderingOptions()
    {
        var options = new RenderingOptions();
        configuration.GetSection("Rendering").Bind(options);
        return options;
    }
}
```

## 📝 Best Practices

### Configuration Best Practices

```
✅ DO:
- Store connection strings in configuration
- Use custom sections for complex config
- Validate configuration on startup
- Provide sensible defaults
- Document all configuration options

❌ DON'T:
- Hard-code settings in DLL
- Store passwords in plain text
- Assume configuration is valid
- Make configuration overly complex
```

### Resource Best Practices

```
✅ DO:
- Embed resources that rarely change
- Use .resx for localization
- Name resources clearly
- Organize resources in folders
- Test resource loading

❌ DON'T:
- Embed huge files (>1MB)
- Mix content types chaotically
- Forget to set Build Action
- Use absolute paths
```

## 📝 Summary

### What We Learned

✅ DLLs use host application's configuration file
✅ Custom configuration sections for complex settings
✅ Embedded resources compiled into DLL
✅ .resx files for strings and localization
✅ Satellite assemblies for multi-language support
✅ JSON configuration for .NET Core/.NET 5+

### Key Techniques

1. **Configuration:** Read from app.config/appsettings.json
2. **Embedded Resources:** GetManifestResourceStream()
3. **Resource Files:** .resx with designer support
4. **Localization:** Satellite assemblies per culture
5. **WPF Resources:** Pack URIs for XAML

## 🚀 Next Steps

In **Lesson 10: Testing and Debugging DLLs**, we'll learn:
- Unit testing class libraries
- Mocking and dependency injection
- Debug vs Release builds
- Using PDB files
- Performance profiling

## 💪 Practice Exercises

### Exercise 1: Configuration

Create a DLL with custom config section:
```
- Graphics settings (quality, resolution, vsync)
- Read from app.config
- Validate settings
```

### Exercise 2: Embedded Resources

Create a DLL with:
```
- Embedded image files
- Embedded data file (XML/JSON)
- Load and display in WPF app
```

### Exercise 3: Localization

Create multi-language DLL:
```
- English, French, Spanish resources
- Language switcher in UI
- Test satellite assemblies
```

---

**Excellent!** 🎉 You now know how to configure and manage resources in DLLs!

**Next Lesson:** [Lesson 10: Testing and Debugging DLLs](Lesson10-Testing-and-Debugging-DLLs.md)
