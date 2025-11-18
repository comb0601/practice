using System;
using BenchmarkDotNet.Attributes;

namespace BenchmarkingSuiteApp.Benchmarks
{
    [MemoryDiagnoser]
    [SimpleJob(warmupCount: 3, iterationCount: 5)]
    public class UnsafeBenchmarks
    {
        private const int ArraySize = 10000;
        private byte[] _source = null!;
        private byte[] _dest = null!;

        [GlobalSetup]
        public void Setup()
        {
            _source = new byte[ArraySize];
            _dest = new byte[ArraySize];
            new Random().NextBytes(_source);
        }

        [Benchmark(Baseline = true)]
        public void ManagedArrayCopy()
        {
            Array.Copy(_source, _dest, ArraySize);
        }

        [Benchmark]
        public unsafe void UnsafePointerCopy()
        {
            fixed (byte* srcPtr = _source)
            fixed (byte* dstPtr = _dest)
            {
                byte* src = srcPtr;
                byte* dst = dstPtr;
                int length = ArraySize;

                // Copy in 8-byte chunks
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

        [Benchmark]
        public void SpanCopy()
        {
            _source.AsSpan().CopyTo(_dest.AsSpan());
        }

        [Benchmark]
        public unsafe void UnsafeStringReverse()
        {
            string input = "The quick brown fox jumps over the lazy dog";
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

            _ = new string(chars);
        }
    }
}
