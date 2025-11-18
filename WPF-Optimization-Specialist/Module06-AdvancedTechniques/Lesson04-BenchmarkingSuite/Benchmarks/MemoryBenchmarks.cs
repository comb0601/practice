using System;
using BenchmarkDotNet.Attributes;

namespace BenchmarkingSuiteApp.Benchmarks
{
    [MemoryDiagnoser]
    [SimpleJob(warmupCount: 3, iterationCount: 5)]
    public class MemoryBenchmarks
    {
        private const int ArraySize = 10000;

        [Benchmark(Baseline = true)]
        public byte[] ArrayAllocation()
        {
            return new byte[ArraySize];
        }

        [Benchmark]
        public byte[] ArrayPoolRent()
        {
            var pool = System.Buffers.ArrayPool<byte>.Shared;
            return pool.Rent(ArraySize);
        }

        [Benchmark]
        public void ArrayCopy()
        {
            var source = new byte[ArraySize];
            var dest = new byte[ArraySize];
            Array.Copy(source, dest, ArraySize);
        }

        [Benchmark]
        public void BufferBlockCopy()
        {
            var source = new byte[ArraySize];
            var dest = new byte[ArraySize];
            Buffer.BlockCopy(source, 0, dest, 0, ArraySize);
        }

        [Benchmark]
        public void SpanCopy()
        {
            var source = new byte[ArraySize];
            var dest = new byte[ArraySize];
            source.AsSpan().CopyTo(dest.AsSpan());
        }
    }
}
