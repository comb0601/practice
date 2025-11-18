using System.Collections.Generic;
using System.Collections.ObjectModel;
using BenchmarkDotNet.Attributes;

namespace BenchmarkingSuiteApp.Benchmarks
{
    [MemoryDiagnoser]
    [SimpleJob(warmupCount: 3, iterationCount: 5)]
    public class CollectionBenchmarks
    {
        private const int ItemCount = 1000;

        [Benchmark(Baseline = true)]
        public List<int> ListAdd()
        {
            var list = new List<int>();
            for (int i = 0; i < ItemCount; i++)
            {
                list.Add(i);
            }
            return list;
        }

        [Benchmark]
        public ObservableCollection<int> ObservableCollectionAdd()
        {
            var collection = new ObservableCollection<int>();
            for (int i = 0; i < ItemCount; i++)
            {
                collection.Add(i);
            }
            return collection;
        }

        [Benchmark]
        public ObservableCollection<int> ObservableCollectionFromList()
        {
            var list = new List<int>();
            for (int i = 0; i < ItemCount; i++)
            {
                list.Add(i);
            }
            return new ObservableCollection<int>(list);
        }

        [Benchmark]
        public List<int> ListPreallocated()
        {
            var list = new List<int>(ItemCount);
            for (int i = 0; i < ItemCount; i++)
            {
                list.Add(i);
            }
            return list;
        }
    }
}
