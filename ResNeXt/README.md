# ResNeXt - Aggregated Residual Transformations

<div align="center">

![PyTorch](https://img.shields.io/badge/PyTorch-EE4C2C?style=flat-square&logo=pytorch&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=flat-square&logo=python&logoColor=white)
![CUDA](https://img.shields.io/badge/CUDA-11.1-brightgreen?style=flat-square&logo=nvidia)
![cuDNN](https://img.shields.io/badge/cuDNN-8.1.0-blue?style=flat-square)
![CIFAR-10](https://img.shields.io/badge/Dataset-CIFAR--10-orange?style=flat-square)

*Next generation ResNet with aggregated residual transformations*

</div>

---

## Overview

This directory contains PyTorch implementation of **ResNeXt (Aggregated Residual Transformations)** for CIFAR-10 classification. ResNeXt improves upon ResNet by introducing a new dimension called "cardinality" (the size of the set of transformations), which is shown to be more effective than going deeper or wider.

## Main Idea

![2](https://user-images.githubusercontent.com/87002037/125739793-c376ea3d-2c3c-4e63-9248-91bcbc4c2419.PNG)

**Key Innovation**: Instead of using a single transformation path, ResNeXt uses multiple parallel paths (cardinality) with grouped convolutions, aggregating their outputs. This design principle is inspired by the "split-transform-merge" strategy used in Inception models.

**Core Principle**: *"Use more groups (cardinality) rather than going deeper or wider"*

## Architecture Details

### ResNeXt-29 Structure

```
Input (32x32x3)
    ↓
Conv1 (3x3, 64 filters)
    ↓
3 ResNeXt Blocks (cardinality × bottleneck_width)
    ↓
3 ResNeXt Blocks (2× channels, stride=2)
    ↓
3 ResNeXt Blocks (4× channels, stride=2)
    ↓
Global Average Pooling
    ↓
Fully Connected (10 classes)
```

### ResNeXt Block Design

Each block implements the bottleneck architecture with grouped convolutions:

```
x → 1×1 Conv (reduce) → BatchNorm → ReLU
    ↓
    3×3 Grouped Conv → BatchNorm → ReLU
    ↓
    1×1 Conv (expand) → BatchNorm → + → ReLU
    ↓                                 ↑
    └──────── Shortcut ───────────────┘
```

**Grouped Convolution**: The 3×3 convolution is split into `cardinality` groups, each processing a portion of the channels independently.

## Details

This implementation follows **Section 5.3 (Experiments on CIFAR-10)** from the paper "Aggregated Residual Transformations for Deep Neural Networks".

**Experimental Design:**
1. Increase cardinality and fix all widths
2. Increase width of bottleneck and fix cardinality = 1

**Model Variants:**
- ResNeXt-29 with varying cardinality (1, 2, 4)
- ResNeXt-29 with varying bottleneck width (64d, 128d, 192d)

## Key Features

- **Cardinality**: New dimension beyond depth and width
- **Grouped Convolutions**: Efficient parallel transformations
- **Bottleneck Design**: 1×1 → 3×3(grouped) → 1×1 structure
- **Flexible Configuration**: Adjustable cardinality and width
- **TensorBoard Integration**: Real-time training visualization
- **Model Checkpointing**: Automatic saving of best models

## File Structure

```
ResNeXt/
├── ResNeXt_29.py              # Main implementation
├── ResNeXt29_(2x64d).ipynb    # Jupyter notebook experiments
└── README.md                  # This file
```

## Performance

### Increasing Cardinality

|ResNeXt29|1x64d|2x64d|4x64d|
|---------|-----|-----|-----|
|Test err|5.42%|0.00%|0.00%|

### Increasing Bottleneck Width

|ResNeXt29|1x64d|1x128d|1x192d|
|---------|-----|-------|-------|
|Test err|-%|-%|-%|

### Paper Performance

![1](https://user-images.githubusercontent.com/87002037/125739808-8af862ab-e6ad-49dc-9829-066e73d5d1b1.PNG)

**Key Observations:**
- Higher cardinality generally leads to better performance
- Increasing cardinality is more effective than increasing depth
- Group convolutions maintain similar parameter count while improving accuracy

## Configuration Options

### Cardinality Experiments

```python
# Increasing cardinality
def ResNeXt29_1x64d():
    return ResNeXt(cardinality=1, bottleneck_width=64)

def ResNeXt29_2x64d():
    return ResNeXt(cardinality=2, bottleneck_width=64)

def ResNeXt29_4x64d():
    return ResNeXt(cardinality=4, bottleneck_width=64)
```

### Bottleneck Width Experiments

```python
# Increasing bottleneck width
def ResNeXt29_1x64d():
    return ResNeXt(cardinality=1, bottleneck_width=64)

def ResNeXt29_1x128d():
    return ResNeXt(cardinality=1, bottleneck_width=128)

def ResNeXt29_1x192d():
    return ResNeXt(cardinality=1, bottleneck_width=192)
```

## Training Configuration

### Hyperparameters

```python
Batch Size: 128
Epochs: 300
Initial Learning Rate: 0.1
Momentum: 0.9
Weight Decay: 0.0001
Optimizer: SGD
Loss Function: CrossEntropyLoss
```

### Learning Rate Schedule

```python
Epoch   0-79:  lr = 0.1
Epoch  80-119: lr = 0.01
Epoch 120-299: lr = 0.001
```

### Data Augmentation

```python
Training:
- RandomCrop(32, padding=4)
- RandomHorizontalFlip()
- Normalize(mean=(0.5, 0.5, 0.5), std=(0.5, 0.5, 0.5))

Testing:
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
tensorboard
```

### Running the Code

```bash
# Navigate to the directory
cd ResNeXt

# Run training (requires CUDA GPU)
python ResNeXt_29.py

# Or use Jupyter notebook
jupyter notebook ResNeXt29_\(2x64d\).ipynb
```

### Monitoring with TensorBoard

```bash
# Start TensorBoard
tensorboard --logdir=runs

# Open browser to http://localhost:6006
```

### Custom Configuration

```python
# Create custom ResNeXt
net = ResNeXt(cardinality=8, bottleneck_width=32)
net = net.to(device)
```

## Implementation Highlights

### Grouped Convolution

The key innovation is the grouped 3×3 convolution:

```python
self.conv3 = nn.Conv2d(
    group_width,
    group_width,
    kernel_size=3,
    stride=1,
    padding=1,
    groups=cardinality,  # Split into groups
    bias=False
)
```

**How it works:**
- Input channels are divided into `cardinality` groups
- Each group is processed independently
- Results are concatenated along channel dimension
- Reduces computation while increasing model capacity

### Block Structure

Each ResNeXt block consists of **9 blocks** organized in 3 stages:
- **Stage 1** (Blocks 1-3): Base resolution, group_width channels
- **Stage 2** (Blocks 4-6): Half resolution, 2× group_width channels
- **Stage 3** (Blocks 7-9): Quarter resolution, 4× group_width channels

### Shortcut Connections

```python
# Identity shortcut when dimensions match
self.shortcut = nn.Sequential()

# Projection shortcut when dimensions change
self.shortcut = nn.Sequential(
    nn.Conv2d(in_channels, out_channels, kernel_size=1, stride=2, bias=False),
    nn.BatchNorm2d(out_channels)
)
```

## Key Concepts

### Cardinality

**Definition**: The size of the set of transformations

**Intuition**: Instead of one deep/wide transformation, use many smaller parallel transformations

**Benefits:**
- More expressive than simply going deeper
- More parameter-efficient than simply going wider
- Better accuracy with similar complexity

### Equivalence Formulations

ResNeXt blocks can be implemented in three equivalent ways:

1. **Split-Transform-Merge**: Multiple paths that are aggregated
2. **Grouped Convolutions**: Single layer with groups parameter
3. **Concatenated Convolutions**: Group results then concatenate

This implementation uses **grouped convolutions** for efficiency.

## Comparison with ResNet

| Aspect | ResNet | ResNeXt |
|--------|--------|---------|
| Key Dimension | Depth | Cardinality |
| Convolution Type | Standard | Grouped |
| Design Strategy | Deeper or Wider | More groups |
| Parameter Efficiency | Baseline | More efficient |
| CIFAR-10 Error | ~6.4% | ~3.6% |

## TensorBoard Logging

The implementation includes comprehensive logging:

```python
writer.add_scalar('training loss', loss.item() / 100, batch_idx)
writer.add_scalar('Accuracy on training', (100 * correct / total), epoch)
writer.add_scalar('Accuracy on testing', (100 * correct / total), epoch)
```

**Metrics tracked:**
- Training loss per batch
- Training accuracy per epoch
- Testing accuracy per epoch

## Model Checkpointing

Models are saved after each epoch:

```python
state = {
    'net': net.state_dict()
}
file_name = 'ResNeXt29_1x64d.pt'
torch.save(state, './checkpoint/' + file_name)
```

## Code Structure

### Main Components

**1. ResNeXt Class**
```python
class ResNeXt(nn.Module):
    - 9 bottleneck blocks
    - Grouped convolutions
    - Shortcut connections
    - Configurable cardinality and width
```

**2. Training Function**
```python
def train(epoch):
    - Forward pass
    - Loss computation
    - Backpropagation
    - TensorBoard logging
    - Progress printing
```

**3. Testing Function**
```python
def test(epoch):
    - Evaluation mode
    - Accuracy calculation
    - TensorBoard logging
    - Model checkpointing
```

## Design Principles

### Template Design

All ResNeXt blocks follow a template:
- **Width**: d × cardinality (total width)
- **Depth**: 3 layers (bottleneck)
- **Cardinality**: Number of groups

**Example**: ResNeXt-29 (2×64d)
- Cardinality = 2
- Bottleneck width = 64
- Total group width = 2 × 64 = 128

### Complexity Analysis

For same complexity as ResNet:
- **Option 1**: Increase cardinality, decrease width per group
- **Option 2**: Increase depth
- **Option 3**: Increase width

**Finding**: Option 1 (cardinality) works best!

## Troubleshooting

### Common Issues

**CUDA Out of Memory:**
```python
# Reduce batch size
batch_size = 64  # instead of 128
```

**Long Training Time:**
```python
# Reduce number of epochs for testing
for epoch in range(0, 50):  # instead of 300
```

**TensorBoard Not Working:**
```bash
# Install tensorboard
pip install tensorboard

# Check if writer is properly initialized
writer = SummaryWriter()
```

## Extensions & Improvements

### Possible Enhancements

- **SE-ResNeXt**: Add Squeeze-and-Excitation modules
- **ResNeXt-WSL**: Weakly supervised learning
- **Mixed Precision**: Use FP16 for faster training
- **Cutout/Mixup**: Advanced augmentation techniques
- **Cosine Annealing**: Better LR schedule

### Advanced Configurations

```python
# Higher cardinality
ResNeXt(cardinality=8, bottleneck_width=32)
ResNeXt(cardinality=16, bottleneck_width=16)

# Wider bottleneck
ResNeXt(cardinality=4, bottleneck_width=128)
```

## Reference

**Aggregated Residual Transformations for Deep Neural Networks**
- Authors: Saining Xie, Ross Girshick, Piotr Dollár, Zhuowen Tu, Kaiming He
- Conference: CVPR 2017
- Paper: https://arxiv.org/abs/1611.05431
- Won 2nd place in ILSVRC 2016 classification

### Key Contributions

1. Introduced cardinality as a new dimension
2. Demonstrated effectiveness of grouped convolutions
3. Achieved better accuracy than ResNet with similar complexity
4. Provided simple, modular design

## Related Work

- **ResNet**: Foundation architecture → `../ResNet/`
- **Inception**: Inspired split-transform-merge strategy
- **Xception**: Extreme version of Inception
- **ShuffleNet**: Efficient grouped convolutions for mobile

## ImageNet Results

While this implementation is for CIFAR-10, ResNeXt achieved:
- **ImageNet-1K**: 3.03% top-5 error (ResNeXt-101 64×4d)
- **ImageNet-5K**: 17.7% top-5 error
- **COCO Detection**: Improved over ResNet-101

---

<div align="center">

**ResNeXt Implementation** | **Aggregated Residual Transformations** | **PyTorch**

[← Back to Main](../README.md)

</div>
