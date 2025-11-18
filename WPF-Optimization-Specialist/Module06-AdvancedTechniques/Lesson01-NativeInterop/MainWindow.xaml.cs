using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;

namespace NativeInteropApp
{
    public partial class MainWindow : Window
    {
        private DispatcherTimer _gcTimer;
        private byte[]? _testImageData;
        private int _imageWidth;
        private int _imageHeight;

        #region P/Invoke Declarations

        // Optimized P/Invoke declarations with correct marshalling
        [DllImport("msvcrt.dll", CallingConvention = CallingConvention.Cdecl, SetLastError = false)]
        private static extern IntPtr memcpy(IntPtr dest, IntPtr src, UIntPtr count);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern void GetSystemInfo(out SYSTEM_INFO lpSystemInfo);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GlobalMemoryStatusEx(ref MEMORYSTATUSEX lpBuffer);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool FlashWindowEx(ref FLASHWINFO pwfi);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetPerformanceInfo(out PERFORMANCE_INFORMATION pPerformanceInformation, uint cb);

        [StructLayout(LayoutKind.Sequential)]
        private struct SYSTEM_INFO
        {
            public ushort wProcessorArchitecture;
            public ushort wReserved;
            public uint dwPageSize;
            public IntPtr lpMinimumApplicationAddress;
            public IntPtr lpMaximumApplicationAddress;
            public UIntPtr dwActiveProcessorMask;
            public uint dwNumberOfProcessors;
            public uint dwProcessorType;
            public uint dwAllocationGranularity;
            public ushort wProcessorLevel;
            public ushort wProcessorRevision;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct MEMORYSTATUSEX
        {
            public uint dwLength;
            public uint dwMemoryLoad;
            public ulong ullTotalPhys;
            public ulong ullAvailPhys;
            public ulong ullTotalPageFile;
            public ulong ullAvailPageFile;
            public ulong ullTotalVirtual;
            public ulong ullAvailVirtual;
            public ulong ullAvailExtendedVirtual;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct FLASHWINFO
        {
            public uint cbSize;
            public IntPtr hwnd;
            public uint dwFlags;
            public uint uCount;
            public uint dwTimeout;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct PERFORMANCE_INFORMATION
        {
            public uint cb;
            public UIntPtr CommitTotal;
            public UIntPtr CommitLimit;
            public UIntPtr CommitPeak;
            public UIntPtr PhysicalTotal;
            public UIntPtr PhysicalAvailable;
            public UIntPtr SystemCache;
            public UIntPtr KernelTotal;
            public UIntPtr KernelPaged;
            public UIntPtr KernelNonpaged;
            public UIntPtr PageSize;
            public uint HandleCount;
            public uint ProcessCount;
            public uint ThreadCount;
        }

        #endregion

        public MainWindow()
        {
            InitializeComponent();
            InitializeUI();
            StartGCMonitoring();
        }

        private void InitializeUI()
        {
            MemorySizeSlider.ValueChanged += (s, e) =>
            {
                long bytes = 1L << (int)e.NewValue;
                MemorySizeText.Text = FormatBytes(bytes);
            };

            IterationsSlider.ValueChanged += (s, e) =>
            {
                IterationsText.Text = ((int)e.NewValue).ToString();
            };

            MemorySizeText.Text = "1 MB";
            IterationsText.Text = "100";
        }

        private void StartGCMonitoring()
        {
            _gcTimer = new DispatcherTimer
            {
                Interval = TimeSpan.FromSeconds(1)
            };
            _gcTimer.Tick += (s, e) =>
            {
                long memoryBytes = GC.GetTotalMemory(false);
                GCMemoryText.Text = $"{memoryBytes / (1024.0 * 1024.0):F2} MB";
            };
            _gcTimer.Start();
        }

        #region Memory Copy Operations

        private void ManagedCopyBtn_Click(object sender, RoutedEventArgs e)
        {
            int size = 1 << (int)MemorySizeSlider.Value;
            int iterations = (int)IterationsSlider.Value;

            byte[] source = new byte[size];
            byte[] dest = new byte[size];
            new Random().NextBytes(source);

            var sw = Stopwatch.StartNew();
            for (int i = 0; i < iterations; i++)
            {
                Array.Copy(source, dest, size);
            }
            sw.Stop();

            MemoryCopyResults.Text = $"Managed Array.Copy:\n" +
                                    $"  Size: {FormatBytes(size)}\n" +
                                    $"  Iterations: {iterations}\n" +
                                    $"  Time: {sw.ElapsedMilliseconds} ms\n" +
                                    $"  Throughput: {(size * iterations / 1024.0 / 1024.0 / sw.Elapsed.TotalSeconds):F2} MB/s";

            UpdateStatus("Managed copy completed");
        }

        private void PInvokeCopyBtn_Click(object sender, RoutedEventArgs e)
        {
            int size = 1 << (int)MemorySizeSlider.Value;
            int iterations = (int)IterationsSlider.Value;

            byte[] source = new byte[size];
            byte[] dest = new byte[size];
            new Random().NextBytes(source);

            GCHandle sourceHandle = GCHandle.Alloc(source, GCHandleType.Pinned);
            GCHandle destHandle = GCHandle.Alloc(dest, GCHandleType.Pinned);

            try
            {
                IntPtr sourcePtr = sourceHandle.AddrOfPinnedObject();
                IntPtr destPtr = destHandle.AddrOfPinnedObject();

                var sw = Stopwatch.StartNew();
                for (int i = 0; i < iterations; i++)
                {
                    memcpy(destPtr, sourcePtr, (UIntPtr)size);
                }
                sw.Stop();

                MemoryCopyResults.Text = $"P/Invoke memcpy:\n" +
                                        $"  Size: {FormatBytes(size)}\n" +
                                        $"  Iterations: {iterations}\n" +
                                        $"  Time: {sw.ElapsedMilliseconds} ms\n" +
                                        $"  Throughput: {(size * iterations / 1024.0 / 1024.0 / sw.Elapsed.TotalSeconds):F2} MB/s\n" +
                                        $"  Note: Includes pinning overhead";

                UpdateStatus("P/Invoke copy completed");
            }
            finally
            {
                sourceHandle.Free();
                destHandle.Free();
            }
        }

        private void UnsafeCopyBtn_Click(object sender, RoutedEventArgs e)
        {
            int size = 1 << (int)MemorySizeSlider.Value;
            int iterations = (int)IterationsSlider.Value;

            byte[] source = new byte[size];
            byte[] dest = new byte[size];
            new Random().NextBytes(source);

            var sw = Stopwatch.StartNew();
            for (int i = 0; i < iterations; i++)
            {
                UnsafeMemoryCopy(source, dest);
            }
            sw.Stop();

            MemoryCopyResults.Text = $"Unsafe Pointer Copy:\n" +
                                    $"  Size: {FormatBytes(size)}\n" +
                                    $"  Iterations: {iterations}\n" +
                                    $"  Time: {sw.ElapsedMilliseconds} ms\n" +
                                    $"  Throughput: {(size * iterations / 1024.0 / 1024.0 / sw.Elapsed.TotalSeconds):F2} MB/s\n" +
                                    $"  Optimized with pointer arithmetic";

            UpdateStatus("Unsafe copy completed");
        }

        private unsafe void UnsafeMemoryCopy(byte[] source, byte[] dest)
        {
            fixed (byte* srcPtr = source)
            fixed (byte* dstPtr = dest)
            {
                byte* src = srcPtr;
                byte* dst = dstPtr;
                int length = source.Length;

                // Copy in 8-byte chunks for better performance
                long* srcLong = (long*)src;
                long* dstLong = (long*)dst;
                int longCount = length / 8;

                for (int i = 0; i < longCount; i++)
                {
                    dstLong[i] = srcLong[i];
                }

                // Copy remaining bytes
                int remaining = length % 8;
                int offset = longCount * 8;
                for (int i = 0; i < remaining; i++)
                {
                    dst[offset + i] = src[offset + i];
                }
            }
        }

        private void CompareAllCopyBtn_Click(object sender, RoutedEventArgs e)
        {
            int size = 1 << (int)MemorySizeSlider.Value;
            int iterations = (int)IterationsSlider.Value;

            byte[] source = new byte[size];
            new Random().NextBytes(source);

            // Managed
            byte[] destManaged = new byte[size];
            var sw1 = Stopwatch.StartNew();
            for (int i = 0; i < iterations; i++)
            {
                Array.Copy(source, destManaged, size);
            }
            sw1.Stop();

            // P/Invoke
            byte[] destPInvoke = new byte[size];
            GCHandle sourceHandle = GCHandle.Alloc(source, GCHandleType.Pinned);
            GCHandle destHandle = GCHandle.Alloc(destPInvoke, GCHandleType.Pinned);
            long pInvokeTime = 0;

            try
            {
                IntPtr sourcePtr = sourceHandle.AddrOfPinnedObject();
                IntPtr destPtr = destHandle.AddrOfPinnedObject();

                var sw2 = Stopwatch.StartNew();
                for (int i = 0; i < iterations; i++)
                {
                    memcpy(destPtr, sourcePtr, (UIntPtr)size);
                }
                sw2.Stop();
                pInvokeTime = sw2.ElapsedMilliseconds;
            }
            finally
            {
                sourceHandle.Free();
                destHandle.Free();
            }

            // Unsafe
            byte[] destUnsafe = new byte[size];
            var sw3 = Stopwatch.StartNew();
            for (int i = 0; i < iterations; i++)
            {
                UnsafeMemoryCopy(source, destUnsafe);
            }
            sw3.Stop();

            MemoryCopyResults.Text = $"COMPARISON - Size: {FormatBytes(size)}, Iterations: {iterations}\n\n" +
                                    $"Managed Array.Copy:    {sw1.ElapsedMilliseconds,6} ms  ({(size * iterations / 1024.0 / 1024.0 / sw1.Elapsed.TotalSeconds):F2} MB/s)\n" +
                                    $"P/Invoke memcpy:       {pInvokeTime,6} ms  ({(size * iterations / 1024.0 / 1024.0 / (pInvokeTime / 1000.0)):F2} MB/s)\n" +
                                    $"Unsafe Pointer Copy:   {sw3.ElapsedMilliseconds,6} ms  ({(size * iterations / 1024.0 / 1024.0 / sw3.Elapsed.TotalSeconds):F2} MB/s)\n\n" +
                                    $"Winner: {GetWinner(sw1.ElapsedMilliseconds, pInvokeTime, sw3.ElapsedMilliseconds)}";

            UpdatePerformanceSummary();
            UpdateStatus("Comparison completed");
        }

        private string GetWinner(long managed, long pinvoke, long unsafeTime)
        {
            long min = Math.Min(managed, Math.Min(pinvoke, unsafeTime));
            if (min == managed) return "Managed Array.Copy";
            if (min == pinvoke) return "P/Invoke memcpy";
            return "Unsafe Pointer Copy";
        }

        #endregion

        #region String Operations

        private void ReverseStringBtn_Click(object sender, RoutedEventArgs e)
        {
            string input = InputStringBox.Text;
            if (string.IsNullOrEmpty(input))
            {
                UpdateStatus("Enter text to reverse");
                return;
            }

            var sw = Stopwatch.StartNew();
            string result = UnsafeReverseString(input);
            sw.Stop();

            StringOperationResults.Text = $"Unsafe String Reverse:\n" +
                                         $"  Input length: {input.Length} characters\n" +
                                         $"  Time: {sw.Elapsed.TotalMicroseconds:F2} μs\n" +
                                         $"  Result: {result}";

            UpdateStatus("String reversed using unsafe code");
        }

        private unsafe string UnsafeReverseString(string input)
        {
            char[] chars = input.ToCharArray();
            fixed (char* ptr = chars)
            {
                char* left = ptr;
                char* right = ptr + chars.Length - 1;

                while (left < right)
                {
                    char temp = *left;
                    *left = *right;
                    *right = temp;
                    left++;
                    right--;
                }
            }
            return new string(chars);
        }

        private void ToUpperUnsafeBtn_Click(object sender, RoutedEventArgs e)
        {
            string input = InputStringBox.Text;
            if (string.IsNullOrEmpty(input))
            {
                UpdateStatus("Enter text to convert");
                return;
            }

            var sw = Stopwatch.StartNew();
            string result = UnsafeToUpper(input);
            sw.Stop();

            StringOperationResults.Text = $"Unsafe ToUpper:\n" +
                                         $"  Input length: {input.Length} characters\n" +
                                         $"  Time: {sw.Elapsed.TotalMicroseconds:F2} μs\n" +
                                         $"  Result: {result}";

            UpdateStatus("String converted to uppercase using unsafe code");
        }

        private unsafe string UnsafeToUpper(string input)
        {
            char[] chars = input.ToCharArray();
            fixed (char* ptr = chars)
            {
                for (int i = 0; i < chars.Length; i++)
                {
                    char c = ptr[i];
                    if (c >= 'a' && c <= 'z')
                    {
                        ptr[i] = (char)(c - 32);
                    }
                }
            }
            return new string(chars);
        }

        private void CountCharsBtn_Click(object sender, RoutedEventArgs e)
        {
            string input = InputStringBox.Text;
            if (string.IsNullOrEmpty(input))
            {
                UpdateStatus("Enter text to analyze");
                return;
            }

            var sw = Stopwatch.StartNew();
            var (letters, digits, spaces, others) = UnsafeCountCharTypes(input);
            sw.Stop();

            StringOperationResults.Text = $"Unsafe Character Analysis:\n" +
                                         $"  Total length: {input.Length} characters\n" +
                                         $"  Letters: {letters}\n" +
                                         $"  Digits: {digits}\n" +
                                         $"  Spaces: {spaces}\n" +
                                         $"  Others: {others}\n" +
                                         $"  Time: {sw.Elapsed.TotalMicroseconds:F2} μs";

            UpdateStatus("Character analysis completed using unsafe code");
        }

        private unsafe (int letters, int digits, int spaces, int others) UnsafeCountCharTypes(string input)
        {
            int letters = 0, digits = 0, spaces = 0, others = 0;

            fixed (char* ptr = input)
            {
                for (int i = 0; i < input.Length; i++)
                {
                    char c = ptr[i];
                    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                        letters++;
                    else if (c >= '0' && c <= '9')
                        digits++;
                    else if (c == ' ')
                        spaces++;
                    else
                        others++;
                }
            }

            return (letters, digits, spaces, others);
        }

        #endregion

        #region Native Windows API

        private void GetSystemInfoBtn_Click(object sender, RoutedEventArgs e)
        {
            GetSystemInfo(out SYSTEM_INFO sysInfo);

            StringBuilder sb = new StringBuilder();
            sb.AppendLine("System Information (via P/Invoke):");
            sb.AppendLine($"  Processor Architecture: {GetProcessorArchitecture(sysInfo.wProcessorArchitecture)}");
            sb.AppendLine($"  Number of Processors: {sysInfo.dwNumberOfProcessors}");
            sb.AppendLine($"  Page Size: {FormatBytes(sysInfo.dwPageSize)}");
            sb.AppendLine($"  Allocation Granularity: {FormatBytes(sysInfo.dwAllocationGranularity)}");
            sb.AppendLine($"  Processor Level: {sysInfo.wProcessorLevel}");
            sb.AppendLine($"  Processor Revision: {sysInfo.wProcessorRevision}");

            NativeApiResults.Text = sb.ToString();
            UpdateStatus("System information retrieved via P/Invoke");
        }

        private string GetProcessorArchitecture(ushort arch)
        {
            return arch switch
            {
                0 => "x86 (Intel)",
                5 => "ARM",
                6 => "Intel Itanium",
                9 => "x64 (AMD64)",
                12 => "ARM64",
                _ => $"Unknown ({arch})"
            };
        }

        private void GetMemoryInfoBtn_Click(object sender, RoutedEventArgs e)
        {
            MEMORYSTATUSEX memStatus = new MEMORYSTATUSEX
            {
                dwLength = (uint)Marshal.SizeOf(typeof(MEMORYSTATUSEX))
            };

            if (GlobalMemoryStatusEx(ref memStatus))
            {
                StringBuilder sb = new StringBuilder();
                sb.AppendLine("Memory Status (via P/Invoke):");
                sb.AppendLine($"  Memory Load: {memStatus.dwMemoryLoad}%");
                sb.AppendLine($"  Total Physical: {FormatBytes((long)memStatus.ullTotalPhys)}");
                sb.AppendLine($"  Available Physical: {FormatBytes((long)memStatus.ullAvailPhys)}");
                sb.AppendLine($"  Total Page File: {FormatBytes((long)memStatus.ullTotalPageFile)}");
                sb.AppendLine($"  Available Page File: {FormatBytes((long)memStatus.ullAvailPageFile)}");
                sb.AppendLine($"  Total Virtual: {FormatBytes((long)memStatus.ullTotalVirtual)}");
                sb.AppendLine($"  Available Virtual: {FormatBytes((long)memStatus.ullAvailVirtual)}");

                NativeApiResults.Text = sb.ToString();
                UpdateStatus("Memory information retrieved via P/Invoke");
            }
            else
            {
                NativeApiResults.Text = "Failed to retrieve memory information";
                UpdateStatus("Error retrieving memory information");
            }
        }

        private void FlashWindowBtn_Click(object sender, RoutedEventArgs e)
        {
            var hwnd = new System.Windows.Interop.WindowInteropHelper(this).Handle;

            FLASHWINFO fInfo = new FLASHWINFO
            {
                cbSize = (uint)Marshal.SizeOf(typeof(FLASHWINFO)),
                hwnd = hwnd,
                dwFlags = 0x0000000C, // FLASHW_ALL | FLASHW_TIMERNOFG
                uCount = 3,
                dwTimeout = 0
            };

            bool result = FlashWindowEx(ref fInfo);

            NativeApiResults.Text = $"Flash Window (via P/Invoke):\n" +
                                   $"  Window Handle: 0x{hwnd.ToInt64():X}\n" +
                                   $"  Flash Count: 3\n" +
                                   $"  Result: {(result ? "Success" : "Failed")}";

            UpdateStatus("Window flashed via P/Invoke");
        }

        private void GetPerformanceBtn_Click(object sender, RoutedEventArgs e)
        {
            PERFORMANCE_INFORMATION perfInfo = new PERFORMANCE_INFORMATION
            {
                cb = (uint)Marshal.SizeOf(typeof(PERFORMANCE_INFORMATION))
            };

            if (GetPerformanceInfo(out perfInfo, perfInfo.cb))
            {
                StringBuilder sb = new StringBuilder();
                sb.AppendLine("Performance Information (via P/Invoke):");
                sb.AppendLine($"  Commit Total: {FormatBytes((long)perfInfo.CommitTotal * (long)perfInfo.PageSize)}");
                sb.AppendLine($"  Commit Limit: {FormatBytes((long)perfInfo.CommitLimit * (long)perfInfo.PageSize)}");
                sb.AppendLine($"  Physical Total: {FormatBytes((long)perfInfo.PhysicalTotal * (long)perfInfo.PageSize)}");
                sb.AppendLine($"  Physical Available: {FormatBytes((long)perfInfo.PhysicalAvailable * (long)perfInfo.PageSize)}");
                sb.AppendLine($"  System Cache: {FormatBytes((long)perfInfo.SystemCache * (long)perfInfo.PageSize)}");
                sb.AppendLine($"  Handle Count: {perfInfo.HandleCount}");
                sb.AppendLine($"  Process Count: {perfInfo.ProcessCount}");
                sb.AppendLine($"  Thread Count: {perfInfo.ThreadCount}");

                NativeApiResults.Text = sb.ToString();
                UpdateStatus("Performance information retrieved via P/Invoke");
            }
            else
            {
                NativeApiResults.Text = "Failed to retrieve performance information";
                UpdateStatus("Error retrieving performance information");
            }
        }

        #endregion

        #region Bitmap Processing

        private void GenerateImageBtn_Click(object sender, RoutedEventArgs e)
        {
            string sizeStr = ((ComboBoxItem)ImageSizeCombo.SelectedItem).Content.ToString()!;
            string[] parts = sizeStr.Split('x');
            _imageWidth = int.Parse(parts[0]);
            _imageHeight = int.Parse(parts[1]);

            var sw = Stopwatch.StartNew();
            _testImageData = GenerateTestImage(_imageWidth, _imageHeight);
            sw.Stop();

            DisplayImage(_testImageData, _imageWidth, _imageHeight);

            ImageProcessingResults.Text = $"Test Image Generated:\n" +
                                         $"  Size: {_imageWidth}x{_imageHeight}\n" +
                                         $"  Data Size: {FormatBytes(_testImageData.Length)}\n" +
                                         $"  Generation Time: {sw.ElapsedMilliseconds} ms";

            UpdateStatus($"Test image {_imageWidth}x{_imageHeight} generated");
        }

        private byte[] GenerateTestImage(int width, int height)
        {
            byte[] data = new byte[width * height * 4]; // BGRA format
            Random rand = new Random();

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int index = (y * width + x) * 4;
                    data[index] = (byte)(x * 255 / width);         // B
                    data[index + 1] = (byte)(y * 255 / height);    // G
                    data[index + 2] = (byte)rand.Next(256);        // R
                    data[index + 3] = 255;                         // A
                }
            }

            return data;
        }

        private void GrayscaleManagedBtn_Click(object sender, RoutedEventArgs e)
        {
            if (_testImageData == null)
            {
                UpdateStatus("Generate test image first");
                return;
            }

            byte[] result = new byte[_testImageData.Length];
            Array.Copy(_testImageData, result, _testImageData.Length);

            var sw = Stopwatch.StartNew();
            ManagedGrayscale(result, _imageWidth, _imageHeight);
            sw.Stop();

            DisplayImage(result, _imageWidth, _imageHeight);

            ImageProcessingResults.Text = $"Grayscale (Managed):\n" +
                                         $"  Size: {_imageWidth}x{_imageHeight}\n" +
                                         $"  Pixels: {_imageWidth * _imageHeight:N0}\n" +
                                         $"  Time: {sw.ElapsedMilliseconds} ms\n" +
                                         $"  Throughput: {(_imageWidth * _imageHeight / sw.Elapsed.TotalSeconds / 1000000):F2} Mpixels/s";

            UpdateStatus("Grayscale conversion completed (managed)");
        }

        private void ManagedGrayscale(byte[] data, int width, int height)
        {
            for (int i = 0; i < data.Length; i += 4)
            {
                byte gray = (byte)(data[i] * 0.114 + data[i + 1] * 0.587 + data[i + 2] * 0.299);
                data[i] = gray;
                data[i + 1] = gray;
                data[i + 2] = gray;
            }
        }

        private void GrayscaleUnsafeBtn_Click(object sender, RoutedEventArgs e)
        {
            if (_testImageData == null)
            {
                UpdateStatus("Generate test image first");
                return;
            }

            byte[] result = new byte[_testImageData.Length];
            Array.Copy(_testImageData, result, _testImageData.Length);

            var sw = Stopwatch.StartNew();
            UnsafeGrayscale(result, _imageWidth, _imageHeight);
            sw.Stop();

            DisplayImage(result, _imageWidth, _imageHeight);

            ImageProcessingResults.Text = $"Grayscale (Unsafe):\n" +
                                         $"  Size: {_imageWidth}x{_imageHeight}\n" +
                                         $"  Pixels: {_imageWidth * _imageHeight:N0}\n" +
                                         $"  Time: {sw.ElapsedMilliseconds} ms\n" +
                                         $"  Throughput: {(_imageWidth * _imageHeight / sw.Elapsed.TotalSeconds / 1000000):F2} Mpixels/s\n" +
                                         $"  Optimized with pointer arithmetic";

            UpdateStatus("Grayscale conversion completed (unsafe)");
        }

        private unsafe void UnsafeGrayscale(byte[] data, int width, int height)
        {
            fixed (byte* ptr = data)
            {
                int pixelCount = width * height;
                byte* p = ptr;

                for (int i = 0; i < pixelCount; i++)
                {
                    byte gray = (byte)(p[0] * 0.114 + p[1] * 0.587 + p[2] * 0.299);
                    p[0] = gray;
                    p[1] = gray;
                    p[2] = gray;
                    p += 4;
                }
            }
        }

        private void InvertUnsafeBtn_Click(object sender, RoutedEventArgs e)
        {
            if (_testImageData == null)
            {
                UpdateStatus("Generate test image first");
                return;
            }

            byte[] result = new byte[_testImageData.Length];
            Array.Copy(_testImageData, result, _testImageData.Length);

            var sw = Stopwatch.StartNew();
            UnsafeInvert(result, _imageWidth, _imageHeight);
            sw.Stop();

            DisplayImage(result, _imageWidth, _imageHeight);

            ImageProcessingResults.Text = $"Invert (Unsafe):\n" +
                                         $"  Size: {_imageWidth}x{_imageHeight}\n" +
                                         $"  Pixels: {_imageWidth * _imageHeight:N0}\n" +
                                         $"  Time: {sw.ElapsedMilliseconds} ms\n" +
                                         $"  Throughput: {(_imageWidth * _imageHeight / sw.Elapsed.TotalSeconds / 1000000):F2} Mpixels/s";

            UpdateStatus("Image inverted using unsafe code");
        }

        private unsafe void UnsafeInvert(byte[] data, int width, int height)
        {
            fixed (byte* ptr = data)
            {
                int pixelCount = width * height;
                byte* p = ptr;

                for (int i = 0; i < pixelCount; i++)
                {
                    p[0] = (byte)(255 - p[0]);
                    p[1] = (byte)(255 - p[1]);
                    p[2] = (byte)(255 - p[2]);
                    p += 4;
                }
            }
        }

        private void BlurUnsafeBtn_Click(object sender, RoutedEventArgs e)
        {
            if (_testImageData == null)
            {
                UpdateStatus("Generate test image first");
                return;
            }

            byte[] result = new byte[_testImageData.Length];

            var sw = Stopwatch.StartNew();
            UnsafeBoxBlur(_testImageData, result, _imageWidth, _imageHeight, 3);
            sw.Stop();

            DisplayImage(result, _imageWidth, _imageHeight);

            ImageProcessingResults.Text = $"Box Blur (Unsafe):\n" +
                                         $"  Size: {_imageWidth}x{_imageHeight}\n" +
                                         $"  Pixels: {_imageWidth * _imageHeight:N0}\n" +
                                         $"  Kernel: 3x3\n" +
                                         $"  Time: {sw.ElapsedMilliseconds} ms\n" +
                                         $"  Throughput: {(_imageWidth * _imageHeight / sw.Elapsed.TotalSeconds / 1000000):F2} Mpixels/s";

            UpdateStatus("Box blur applied using unsafe code");
        }

        private unsafe void UnsafeBoxBlur(byte[] source, byte[] dest, int width, int height, int radius)
        {
            fixed (byte* srcPtr = source)
            fixed (byte* dstPtr = dest)
            {
                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        int sumB = 0, sumG = 0, sumR = 0, count = 0;

                        for (int ky = -radius; ky <= radius; ky++)
                        {
                            for (int kx = -radius; kx <= radius; kx++)
                            {
                                int px = Math.Clamp(x + kx, 0, width - 1);
                                int py = Math.Clamp(y + ky, 0, height - 1);
                                int idx = (py * width + px) * 4;

                                sumB += srcPtr[idx];
                                sumG += srcPtr[idx + 1];
                                sumR += srcPtr[idx + 2];
                                count++;
                            }
                        }

                        int destIdx = (y * width + x) * 4;
                        dstPtr[destIdx] = (byte)(sumB / count);
                        dstPtr[destIdx + 1] = (byte)(sumG / count);
                        dstPtr[destIdx + 2] = (byte)(sumR / count);
                        dstPtr[destIdx + 3] = 255;
                    }
                }
            }
        }

        private void DisplayImage(byte[] data, int width, int height)
        {
            var bitmap = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgra32, null);
            bitmap.WritePixels(new System.Windows.Int32Rect(0, 0, width, height), data, width * 4, 0);
            ProcessedImage.Source = bitmap;
        }

        #endregion

        #region Helper Methods

        private string FormatBytes(long bytes)
        {
            string[] sizes = { "B", "KB", "MB", "GB", "TB" };
            double len = bytes;
            int order = 0;

            while (len >= 1024 && order < sizes.Length - 1)
            {
                order++;
                len /= 1024;
            }

            return $"{len:F2} {sizes[order]}";
        }

        private void UpdateStatus(string message)
        {
            StatusText.Text = message;
        }

        private void UpdatePerformanceSummary()
        {
            PerformanceSummary.Text = "PERFORMANCE INSIGHTS:\n\n" +
                                     "• P/Invoke: Best for existing native libraries, has marshalling overhead\n" +
                                     "• Unsafe Code: Maximum performance for data processing, requires fixed/pinned memory\n" +
                                     "• Managed Code: Often optimized by JIT, safe and reliable for most scenarios\n\n" +
                                     "Key Optimization Tips:\n" +
                                     "1. Use unsafe code for tight loops and bulk data operations\n" +
                                     "2. Minimize P/Invoke calls, batch operations when possible\n" +
                                     "3. Pin managed memory once and reuse for multiple P/Invoke calls\n" +
                                     "4. Use appropriate struct layouts and marshalling attributes\n" +
                                     "5. Consider Buffer.BlockCopy for large managed array operations";
        }

        #endregion

        protected override void OnClosed(EventArgs e)
        {
            _gcTimer?.Stop();
            base.OnClosed(e);
        }
    }
}
