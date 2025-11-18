using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Controls;

namespace RealWorldProjectsApp.CaseStudies
{
    /// <summary>
    /// AFTER: Optimized DataGrid with virtualization and proper INotifyPropertyChanged
    /// Improvements: Virtualization enabled, optimized bindings, selective updates
    /// </summary>
    public class Case1After : UserControl
    {
        private DataGrid _dataGrid;
        private ObservableCollection<OptimizedStockData> _data = new ObservableCollection<OptimizedStockData>();
        private Random _random = new Random();

        public Case1After()
        {
            InitializeControl();
        }

        private void InitializeControl()
        {
            _dataGrid = new DataGrid
            {
                ItemsSource = _data,
                AutoGenerateColumns = false,
                IsReadOnly = true,
                // GOOD: Virtualization enabled
                EnableRowVirtualization = true,
                EnableColumnVirtualization = true,
                VirtualizingPanel.IsVirtualizing = true,
                VirtualizingPanel.VirtualizationMode = VirtualizationMode.Recycling
            };

            // Set ScrollViewer for better performance
            ScrollViewer.SetCanContentScroll(_dataGrid, true);
            ScrollViewer.SetIsDeferredScrollingEnabled(_dataGrid, true);

            // Create columns with optimized bindings
            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Symbol",
                Binding = new System.Windows.Data.Binding("Symbol")
                {
                    Mode = System.Windows.Data.BindingMode.OneWay // GOOD: OneWay binding
                }
            });

            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Price",
                Binding = new System.Windows.Data.Binding("Price")
                {
                    StringFormat = "C2",
                    Mode = System.Windows.Data.BindingMode.OneWay
                }
            });

            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Change",
                Binding = new System.Windows.Data.Binding("Change")
                {
                    StringFormat = "P2",
                    Mode = System.Windows.Data.BindingMode.OneWay
                }
            });

            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Volume",
                Binding = new System.Windows.Data.Binding("Volume")
                {
                    StringFormat = "N0",
                    Mode = System.Windows.Data.BindingMode.OneWay
                }
            });

            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Last Updated",
                Binding = new System.Windows.Data.Binding("LastUpdated")
                {
                    StringFormat = "HH:mm:ss",
                    Mode = System.Windows.Data.BindingMode.OneWay
                }
            });

            Content = _dataGrid;
        }

        public void LoadData(int rowCount)
        {
            _data.Clear();

            // GOOD: Data is loaded but only visible rows are rendered due to virtualization
            for (int i = 0; i < rowCount; i++)
            {
                _data.Add(new OptimizedStockData
                {
                    Symbol = $"STK{i:D4}",
                    Price = 100 + _random.NextDouble() * 900,
                    Change = (_random.NextDouble() - 0.5) * 0.1,
                    Volume = _random.Next(1000000, 10000000),
                    LastUpdated = DateTime.Now
                });
            }
        }

        public void UpdateRandomRows(int count)
        {
            // GOOD: Updates only notify changed properties
            for (int i = 0; i < count && i < _data.Count; i++)
            {
                int index = _random.Next(_data.Count);
                var item = _data[index];

                // Only update changed properties - raises individual notifications
                item.Price += (_random.NextDouble() - 0.5) * 10;
                item.Change = (_random.NextDouble() - 0.5) * 0.1;
                item.LastUpdated = DateTime.Now;
            }
        }

        // GOOD: Implements INotifyPropertyChanged for efficient updates
        public class OptimizedStockData : INotifyPropertyChanged
        {
            private string _symbol = string.Empty;
            private double _price;
            private double _change;
            private long _volume;
            private DateTime _lastUpdated;

            public string Symbol
            {
                get => _symbol;
                set => SetProperty(ref _symbol, value);
            }

            public double Price
            {
                get => _price;
                set => SetProperty(ref _price, value);
            }

            public double Change
            {
                get => _change;
                set => SetProperty(ref _change, value);
            }

            public long Volume
            {
                get => _volume;
                set => SetProperty(ref _volume, value);
            }

            public DateTime LastUpdated
            {
                get => _lastUpdated;
                set => SetProperty(ref _lastUpdated, value);
            }

            public event PropertyChangedEventHandler? PropertyChanged;

            protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
            {
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }

            protected bool SetProperty<T>(ref T field, T value, [CallerMemberName] string? propertyName = null)
            {
                if (Equals(field, value)) return false;
                field = value;
                OnPropertyChanged(propertyName);
                return true;
            }
        }
    }
}
