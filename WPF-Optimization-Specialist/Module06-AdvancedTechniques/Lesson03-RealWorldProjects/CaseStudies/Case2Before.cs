using System;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace RealWorldProjectsApp.CaseStudies
{
    /// <summary>
    /// BEFORE: Non-optimized image loading
    /// Issues: Full-size images loaded, no caching, memory leaks, slow
    /// </summary>
    public class Case2Before : UserControl
    {
        private ListBox _listBox;
        private ObservableCollection<ImageItem> _images = new ObservableCollection<ImageItem>();
        private Random _random = new Random();

        public Case2Before()
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

            // BAD: No virtualization
            VirtualizingPanel.SetIsVirtualizing(_listBox, false);

            // BAD: Complex template without optimization
            _listBox.ItemTemplate = new DataTemplate
            {
                VisualTree = CreateImageTemplate()
            };

            var itemsPanelTemplate = new ItemsPanelTemplate();
            var factory = new FrameworkElementFactory(typeof(WrapPanel));
            itemsPanelTemplate.VisualTree = factory;
            _listBox.ItemsPanel = itemsPanelTemplate;

            Content = _listBox;
        }

        private FrameworkElementFactory CreateImageTemplate()
        {
            var border = new FrameworkElementFactory(typeof(Border));
            border.SetValue(Border.MarginProperty, new Thickness(5));
            border.SetValue(Border.BorderBrushProperty, Brushes.Gray);
            border.SetValue(Border.BorderThicknessProperty, new Thickness(1));

            var image = new FrameworkElementFactory(typeof(Image));
            // BAD: No DecodePixelWidth - loads full resolution!
            image.SetBinding(Image.SourceProperty, new System.Windows.Data.Binding("ImageSource"));
            image.SetValue(Image.StretchProperty, Stretch.UniformToFill);

            border.AppendChild(image);
            return border;
        }

        public void LoadImages(int count, int size)
        {
            _images.Clear();

            for (int i = 0; i < count; i++)
            {
                // BAD: Create full-size BitmapImage without optimization
                var bitmap = CreateLargeImage(size);
                _images.Add(new ImageItem
                {
                    ImageSource = bitmap,
                    Name = $"Image {i + 1}"
                });
            }
        }

        private BitmapSource CreateLargeImage(int displaySize)
        {
            // Simulate creating a large image (e.g., 4000x3000)
            int largeWidth = 4000;
            int largeHeight = 3000;

            var bitmap = new WriteableBitmap(largeWidth, largeHeight, 96, 96, PixelFormats.Bgra32, null);

            // Fill with random colors
            byte[] pixels = new byte[largeWidth * largeHeight * 4];
            _random.NextBytes(pixels);
            bitmap.WritePixels(new Int32Rect(0, 0, largeWidth, largeHeight), pixels, largeWidth * 4, 0);

            // BAD: Return full-size bitmap - wastes memory!
            return bitmap;
        }

        public void Clear()
        {
            // BAD: No proper disposal
            _images.Clear();
        }

        public class ImageItem
        {
            public BitmapSource? ImageSource { get; set; }
            public string Name { get; set; } = string.Empty;
        }
    }
}
