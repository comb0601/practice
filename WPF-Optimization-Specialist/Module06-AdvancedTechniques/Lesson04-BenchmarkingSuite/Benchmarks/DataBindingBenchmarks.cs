using System.ComponentModel;
using System.Runtime.CompilerServices;
using BenchmarkDotNet.Attributes;

namespace BenchmarkingSuiteApp.Benchmarks
{
    [MemoryDiagnoser]
    [SimpleJob(warmupCount: 3, iterationCount: 5)]
    public class DataBindingBenchmarks
    {
        private TestObject _obj = null!;

        [GlobalSetup]
        public void Setup()
        {
            _obj = new TestObject();
        }

        [Benchmark(Baseline = true)]
        public void DirectPropertySet()
        {
            _obj.DirectValue = 42;
        }

        [Benchmark]
        public void PropertyWithNotify()
        {
            _obj.NotifyValue = 42;
        }

        [Benchmark]
        public void PropertyWithValidation()
        {
            _obj.ValidatedValue = 42;
        }

        private class TestObject : INotifyPropertyChanged
        {
            private int _directValue;
            private int _notifyValue;
            private int _validatedValue;

            public int DirectValue
            {
                get => _directValue;
                set => _directValue = value;
            }

            public int NotifyValue
            {
                get => _notifyValue;
                set => SetProperty(ref _notifyValue, value);
            }

            public int ValidatedValue
            {
                get => _validatedValue;
                set
                {
                    if (value >= 0 && value <= 100)
                    {
                        SetProperty(ref _validatedValue, value);
                    }
                }
            }

            public event PropertyChangedEventHandler? PropertyChanged;

            protected void SetProperty<T>(ref T field, T value, [CallerMemberName] string? propertyName = null)
            {
                if (!Equals(field, value))
                {
                    field = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
                }
            }
        }
    }
}
