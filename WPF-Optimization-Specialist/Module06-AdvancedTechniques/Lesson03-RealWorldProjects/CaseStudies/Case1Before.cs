using System;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;

namespace RealWorldProjectsApp.CaseStudies
{
    /// <summary>
    /// BEFORE: Non-optimized DataGrid with no virtualization
    /// Issues: All rows loaded, high memory, slow scrolling
    /// </summary>
    public class Case1Before : UserControl
    {
        private DataGrid _dataGrid;
        private ObservableCollection<StockData> _data = new ObservableCollection<StockData>();
        private Random _random = new Random();

        public Case1Before()
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
                // BAD: Virtualization disabled
                EnableRowVirtualization = false,
                EnableColumnVirtualization = false
            };

            // Create columns with complex bindings
            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Symbol",
                Binding = new System.Windows.Data.Binding("Symbol") // No optimization
            });

            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Price",
                Binding = new System.Windows.Data.Binding("Price") { StringFormat = "C2" }
            });

            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Change",
                Binding = new System.Windows.Data.Binding("Change") { StringFormat = "P2" }
            });

            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Volume",
                Binding = new System.Windows.Data.Binding("Volume") { StringFormat = "N0" }
            });

            _dataGrid.Columns.Add(new DataGridTextColumn
            {
                Header = "Last Updated",
                Binding = new System.Windows.Data.Binding("LastUpdated") { StringFormat = "HH:mm:ss" }
            });

            Content = _dataGrid;
        }

        public void LoadData(int rowCount)
        {
            _data.Clear();

            // BAD: Load all rows at once - no lazy loading
            for (int i = 0; i < rowCount; i++)
            {
                _data.Add(new StockData
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
            // BAD: Updates trigger full property change notifications
            for (int i = 0; i < count && i < _data.Count; i++)
            {
                int index = _random.Next(_data.Count);
                var item = _data[index];
                item.Price += (_random.NextDouble() - 0.5) * 10;
                item.Change = (_random.NextDouble() - 0.5) * 0.1;
                item.LastUpdated = DateTime.Now;
                // This triggers multiple property notifications
            }
        }

        public class StockData
        {
            public string Symbol { get; set; } = string.Empty;
            public double Price { get; set; }
            public double Change { get; set; }
            public long Volume { get; set; }
            public DateTime LastUpdated { get; set; }
        }
    }
}
