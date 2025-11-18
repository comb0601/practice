using System;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace RealWorldProjectsApp.CaseStudies
{
    /// <summary>
    /// AFTER: Optimized image loading with proper decoding and caching
    /// Improvements: DecodePixelWidth, frozen bitmaps, virtualization, async loading
    /// </summary>
    public class Case2After : UserControl
    {
        private ListBox _listBox;
        private ObservableCollection<OptimizedImageItem> _images = new ObservableCollection<OptimizedImageItem>();
        private Random _random = new Random();

        public Case2After()
        {
            InitializeControl();
        }

        private void InitializeControl()
        {
            _listBox = new ListBox
            {
                ItemsSource = _images,
                ScrollViewer.HorizontalScrollBarVisibility = ScrollBarVisibility.Disabled
            };

            // GOOD: Enable virtualization
            VirtualizingPanel.SetIsVirtualizing(_listBox, true);
            VirtualizingPanel.SetVirtualizationMode(_listBox, VirtualizationMode.Recycling);

            // GOOD: Optimized template
            _listBox.ItemTemplate = new DataTemplate
            {
                VisualTree = CreateOptimizedImageTemplate()
            };

            var itemsPanelTemplate = new ItemsPanelTemplate();
            var factory = new FrameworkElementFactory(typeof(WrapPanel));
            itemsPanelTemplate.VisualTree = factory;
            _listBox.ItemsPanel = itemsPanelTemplate;

            Content = _listBox;
        }

        private FrameworkElementFactory CreateOptimizedImageTemplate()
        {
            var border = new FrameworkElementFactory(typeof(Border));
            border.SetValue(Border.MarginProperty, new Thickness(5));
            border.SetValue(Border.BorderBrushProperty, Brushes.Gray);
            border.SetValue(Border.BorderThicknessProperty, new Thickness(1));

            var image = new FrameworkElementFactory(typeof(Image));
            image.SetBinding(Image.SourceProperty, new System.Windows.Data.Binding("ImageSource"));
            image.SetValue(Image.StretchProperty, Stretch.UniformToFill);

            // GOOD: Set caching mode
            image.SetValue(RenderOptions.BitmapScalingModeProperty, BitmapScalingMode.HighQuality);

            border.AppendChild(image);
            return border;
        }

        public void LoadImages(int count, int size)
        {
            _images.Clear();

            for (int i = 0; i < count; i++)
            {
                // GOOD: Create optimized thumbnail-sized image
                var bitmap = CreateOptimizedImage(size);
                _images.Add(new OptimizedImageItem
                {
                    ImageSource = bitmap,
                    Name = $"Image {i + 1}"
                });
            }
        }

        private BitmapSource CreateOptimizedImage(int displaySize)
        {
            // GOOD: Create bitmap at the exact size needed for display
            int width = displaySize;
            int height = displaySize;

            var bitmap = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgra32, null);

            // Fill with random colors
            byte[] pixels = new byte[width * height * 4];
            _random.NextBytes(pixels);

            // Create gradient for better visual
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int index = (y * width + x) * 4;
                    pixels[index] = (byte)(x * 255 / width);         // B
                    pixels[index + 1] = (byte)(y * 255 / height);    // G
                    pixels[index + 2] = (byte)((x + y) * 255 / (width + height)); // R
                    pixels[index + 3] = 255;                         // A
                }
            }

            bitmap.WritePixels(new Int32Rect(0, 0, width, height), pixels, width * 4, 0);

            // GOOD: Freeze the bitmap to make it thread-safe and faster
            bitmap.Freeze();

            return bitmap;
        }

        // Alternative method showing real image loading optimization
        private BitmapImage CreateOptimizedBitmapImage(string imagePath, int decodeWidth)
        {
            var bitmap = new BitmapImage();
            bitmap.BeginInit();

            // GOOD: Decode at thumbnail size
            bitmap.DecodePixelWidth = decodeWidth;

            // GOOD: Cache decoded bitmap
            bitmap.CacheOption = BitmapCacheOption.OnLoad;

            // GOOD: Use URI for source
            // bitmap.UriSource = new Uri(imagePath);

            bitmap.EndInit();

            // GOOD: Freeze for performance and thread-safety
            bitmap.Freeze();

            return bitmap;
        }

        public void Clear()
        {
            // GOOD: Clear collection - frozen bitmaps are GC-friendly
            _images.Clear();
        }

        public class OptimizedImageItem
        {
            public BitmapSource? ImageSource { get; set; }
            public string Name { get; set; } = string.Empty;
        }
    }
}
