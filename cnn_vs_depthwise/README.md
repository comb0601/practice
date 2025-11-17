# CNN vs Depthwise Separable Convolution

<div align="center">

![PyTorch](https://img.shields.io/badge/PyTorch-EE4C2C?style=flat-square&logo=pytorch&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=flat-square&logo=python&logoColor=white)
![CIFAR-10](https://img.shields.io/badge/Dataset-CIFAR--10-orange?style=flat-square)

*Comparative analysis of standard CNN and depthwise separable convolutions*

</div>

---

## Overview

This directory contains a **comparative study** between standard Convolutional Neural Networks (CNN) and Depthwise Separable Convolutions. The experiment demonstrates how depthwise separable convolutions can achieve similar accuracy to standard CNNs while using significantly fewer parameters and computations.

## Main Concept

### Standard Convolution
Applies filters across all input channels simultaneously, computing spatial and channel-wise features together.

### Depthwise Separable Convolution
Splits the convolution into two separate operations:
1. **Depthwise Convolution (DW)**: Applies a single filter per input channel
2. **Pointwise Convolution (PW)**: Applies 1×1 convolution to combine channel information

## Comparison

| Aspect | Standard CNN | Depthwise Separable CNN |
|--------|--------------|-------------------------|
| Parameter Count | Higher | Lower (~8-9x reduction) |
| Computation (FLOPs) | Higher | Lower (~8-9x reduction) |
| Accuracy | Baseline | Comparable or slightly lower |
| Speed | Slower | Faster |
| Memory Usage | Higher | Lower |

## Architecture Details

### Standard CNN Architecture

```
Input (32×32×3)
    ↓
Conv(3→32) → BN → ReLU
    ↓
Conv(32→64) → BN → ReLU
    ↓
Conv(64→128) → BN → ReLU
    ↓
Conv(128→128) → ReLU → MaxPool → Dropout
    ↓
Conv(128→256) → BN → ReLU
    ↓
Conv(256→256) → ReLU → MaxPool
    ↓
FC(16384→1024) → ReLU
    ↓
FC(1024→512) → ReLU → Dropout
    ↓
FC(512→10)
```

### Depthwise Separable CNN Architecture

```
Input (32×32×3)
    ↓
DW-Sep Conv(3→32)
    ↓
DW-Sep Conv(32→64)
    ↓
DW-Sep Conv(64→128)
    ↓
Conv(128→128) → ReLU → MaxPool → Dropout
    ↓
DW-Sep Conv(128→256)
    ↓
Conv(256→256) → ReLU → MaxPool
    ↓
FC(16384→1024) → ReLU
    ↓
FC(1024→512) → ReLU → Dropout
    ↓
FC(512→10)
```

**Note**: DW-Sep Conv = Depthwise Conv → Pointwise Conv

## File Structure

```
cnn_vs_depthwise/
├── cnn.py              # Standard CNN implementation
├── depthwise.py        # Depthwise separable CNN implementation
├── CNN.PNG             # CNN architecture visualization
├── Depthwise.PNG       # Depthwise architecture visualization
└── README.md           # This file
```

## Implementation Details

### Standard Convolution

```python
nn.Conv2d(in_channels=3, out_channels=32, kernel_size=3, padding=1)
```

**Parameters**: `3 × 32 × 3 × 3 = 864`

### Depthwise Separable Convolution

```python
def conv_dw(inp, oup):
    return nn.Sequential(
        # Depthwise: one filter per input channel
        nn.Conv2d(inp, inp, 3, 1, 1, groups=inp, bias=False),
        nn.BatchNorm2d(inp),
        nn.ReLU(inplace=True),

        # Pointwise: 1×1 convolution to mix channels
        nn.Conv2d(inp, oup, 1, 1, 0, bias=False),
        nn.BatchNorm2d(oup),
        nn.ReLU(inplace=True),
    )
```

**Parameters**:
- Depthwise: `3 × 1 × 3 × 3 = 27`
- Pointwise: `3 × 32 × 1 × 1 = 96`
- Total: `123` (vs 864 for standard)

### Parameter Reduction Ratio

For a 3×3 convolution:
```
Reduction = (inp × k × k + inp × oup) / (inp × oup × k × k)
          ≈ 1/oup + 1/(k×k)
          ≈ 1/32 + 1/9 ≈ 0.14

Reduction Factor ≈ 7-9×
```

## Training Configuration

### Hyperparameters

```python
Batch Size: 128
Epochs: 10
Initial Learning Rate: 0.1
Momentum: 0.9
Weight Decay: 0.0001
Optimizer: SGD
Loss Function: CrossEntropyLoss
```

### Data Augmentation

```python
Training & Testing:
- Resize(32, 32)
- ToTensor()
- Normalize(mean=(0.5, 0.5, 0.5), std=(0.5, 0.5, 0.5))
```

## Dataset

**CIFAR-10 Dataset:**
- 50,000 training images
- 10,000 test images
- Image size: 32×32 pixels
- Color images (RGB)
- 10 classes: airplane, automobile, bird, cat, deer, dog, frog, horse, ship, truck

## Usage

### Requirements

```bash
torch>=1.9.0
torchvision
numpy
matplotlib
```

### Running Standard CNN

```bash
cd cnn_vs_depthwise
python cnn.py
```

### Running Depthwise CNN

```bash
cd cnn_vs_depthwise
python depthwise.py
```

### Comparing Results

```python
# After training both models
import torch

# Load models
cnn_checkpoint = torch.load('checkpoint/CNN.pt')
depthwise_checkpoint = torch.load('checkpoint/CNN_depthwise.pt')

# Count parameters
def count_parameters(model):
    return sum(p.numel() for p in model.parameters())

print(f"CNN Parameters: {count_parameters(cnn_model):,}")
print(f"Depthwise Parameters: {count_parameters(depthwise_model):,}")
```

## Key Concepts

### Depthwise Convolution

Applies a **single filter per input channel**:
- Input: H × W × C
- Filters: K × K × 1 × C (one K×K filter per channel)
- Output: H × W × C

```python
nn.Conv2d(inp, inp, kernel_size=3, groups=inp)
```

**Key Parameter**: `groups=inp` means each input channel gets its own filter

### Pointwise Convolution

A **1×1 convolution** to mix channel information:
- Input: H × W × C_in
- Filters: 1 × 1 × C_in × C_out
- Output: H × W × C_out

```python
nn.Conv2d(inp, oup, kernel_size=1)
```

**Purpose**: Combine features from different channels

## Computational Complexity

### Standard 3×3 Convolution

```
Cost = H × W × C_in × C_out × K × K
     = H × W × C_in × C_out × 9
```

### Depthwise Separable Convolution

```
DW Cost = H × W × C_in × K × K
PW Cost = H × W × C_in × C_out
Total   = H × W × C_in × (K × K + C_out)
```

### Example Calculation

For H=W=32, C_in=128, C_out=256, K=3:

**Standard Conv:**
```
32 × 32 × 128 × 256 × 9 = 301,989,888 operations
```

**Depthwise Sep Conv:**
```
DW: 32 × 32 × 128 × 9 = 1,179,648
PW: 32 × 32 × 128 × 256 = 33,554,432
Total: 34,734,080 operations

Speedup: 301M / 34.7M ≈ 8.7×
```

## Visualizations

### CNN Architecture
![CNN Architecture](CNN.PNG)

### Depthwise Architecture
![Depthwise Architecture](Depthwise.PNG)

## Performance Analysis

### Expected Results (10 epochs)

| Model | Test Accuracy | Parameters | Training Time |
|-------|---------------|------------|---------------|
| Standard CNN | ~70-75% | ~10-15M | Baseline |
| Depthwise CNN | ~68-73% | ~1-2M | 30-50% faster |

### Trade-offs

**Standard CNN:**
- **Pros**: Higher accuracy, better feature extraction
- **Cons**: More parameters, slower inference, higher memory

**Depthwise Separable CNN:**
- **Pros**: Fewer parameters, faster inference, lower memory
- **Cons**: Slightly lower accuracy, may need more layers

## Code Comparison

### Standard Convolution Layer

```python
self.conv_layer = nn.Sequential(
    nn.Conv2d(in_channels=3, out_channels=32, kernel_size=3, padding=1),
    nn.BatchNorm2d(32),
    nn.ReLU(inplace=True),
)
```

### Depthwise Separable Layer

```python
def conv_dw(inp, oup):
    return nn.Sequential(
        # Depthwise
        nn.Conv2d(inp, inp, 3, 1, 1, groups=inp, bias=False),
        nn.BatchNorm2d(inp),
        nn.ReLU(inplace=True),
        # Pointwise
        nn.Conv2d(inp, oup, 1, 1, 0, bias=False),
        nn.BatchNorm2d(oup),
        nn.ReLU(inplace=True),
    )

self.conv_layer = nn.Sequential(
    conv_dw(3, 32),
)
```

## Applications

### Where Depthwise Convolutions Excel

1. **Mobile Devices**: Limited computational resources
2. **Real-time Applications**: Need fast inference
3. **Edge Computing**: Memory constraints
4. **Large-scale Deployment**: Cost-effective inference

### Popular Architectures Using Depthwise Convolutions

- **MobileNet**: Mobile and embedded vision applications
- **MobileNetV2**: Inverted residuals and linear bottlenecks
- **EfficientNet**: Compound scaling with depthwise convolutions
- **Xception**: Extreme version of Inception

## Practical Tips

### When to Use Standard CNN

- Maximum accuracy is required
- Computational resources are abundant
- Working with small models (< 1M parameters)
- Research and experimentation

### When to Use Depthwise Separable

- Deploying to mobile/edge devices
- Real-time inference requirements
- Memory constraints
- Large-scale deployment
- Energy efficiency matters

## Extensions & Improvements

### Possible Enhancements

**For Better Accuracy:**
```python
# Add more layers
# Use residual connections
# Implement squeeze-and-excitation
# Apply advanced data augmentation
```

**For Better Efficiency:**
```python
# Quantization (INT8)
# Pruning
# Knowledge distillation
# Neural architecture search
```

### Experiment Ideas

1. **Hybrid Models**: Mix standard and depthwise convolutions
2. **Different Kernels**: Try 5×5 or 7×7 depthwise kernels
3. **Channel Multiplier**: Expand channels in depthwise layer
4. **Inverted Residuals**: Use MobileNetV2-style blocks

## Troubleshooting

### Common Issues

**Lower Accuracy with Depthwise:**
```python
# Solutions:
# 1. Add more layers
# 2. Use wider networks (more channels)
# 3. Add residual connections
# 4. Train for more epochs
```

**Grouped Convolution Not Accelerating:**
```python
# Some GPUs don't optimize grouped convolutions well
# Try on different hardware (e.g., mobile processors)
# Check PyTorch version (newer = better optimization)
```

## Benchmarking

### Parameter Count Comparison

```python
# Count parameters
def count_params(model):
    return sum(p.numel() for p in model.parameters() if p.requires_grad)

# Compare
cnn_params = count_params(CNN())
depthwise_params = count_params(CNN_depthwise())

print(f"Reduction: {cnn_params / depthwise_params:.1f}×")
```

### Inference Speed Comparison

```python
import time

# Benchmark
dummy_input = torch.randn(1, 3, 32, 32).cuda()

# CNN
start = time.time()
for _ in range(100):
    cnn(dummy_input)
cnn_time = time.time() - start

# Depthwise
start = time.time()
for _ in range(100):
    depthwise(dummy_input)
depthwise_time = time.time() - start

print(f"Speedup: {cnn_time / depthwise_time:.2f}×")
```

## Reference

### Papers

**MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications**
- Authors: Andrew G. Howard et al.
- Year: 2017
- arXiv: https://arxiv.org/abs/1704.04861

**Xception: Deep Learning with Depthwise Separable Convolutions**
- Authors: François Chollet
- Conference: CVPR 2017
- arXiv: https://arxiv.org/abs/1610.02357

### Key Insights

1. Depthwise separable convolutions decompose standard convolution
2. Achieve 8-9× reduction in computation and parameters
3. Slight accuracy trade-off (1-3%)
4. Essential for mobile and embedded vision

## Related Work

- **MobileNetV2**: Inverted residuals, linear bottlenecks
- **ShuffleNet**: Channel shuffle for group convolutions
- **EfficientNet**: Compound scaling method
- **GhostNet**: Generate more features from cheap operations

---

<div align="center">

**CNN vs Depthwise Comparison** | **Efficient Deep Learning** | **PyTorch**

[← Back to Main](../README.md)

</div>
