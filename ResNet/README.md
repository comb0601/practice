# ResNet - Deep Residual Learning

<div align="center">

![PyTorch](https://img.shields.io/badge/PyTorch-EE4C2C?style=flat-square&logo=pytorch&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=flat-square&logo=python&logoColor=white)
![CUDA](https://img.shields.io/badge/CUDA-11.1-brightgreen?style=flat-square&logo=nvidia)
![cuDNN](https://img.shields.io/badge/cuDNN-8.1.0-blue?style=flat-square)
![CIFAR-10](https://img.shields.io/badge/Dataset-CIFAR--10-orange?style=flat-square)

*Implementation of ResNet architectures for image classification*

</div>

---

## Overview

This directory contains PyTorch implementations of **ResNet (Residual Networks)** for CIFAR-10 image classification. ResNet introduced skip connections (residual connections) that allow training of very deep networks by addressing the vanishing gradient problem.

## Main Idea

![1311](https://user-images.githubusercontent.com/87002037/124681898-83db9080-df04-11eb-8a02-b77391df3ade.png)

**Residual Learning Framework:** Instead of hoping layers learn H(x), we let them learn the residual F(x) = H(x) - x. This reformulation makes optimization easier and enables training of very deep networks.

## Architecture Details

### ResNet Variants

The implementation supports multiple ResNet depths:

| Model | Layers | Blocks per Stage | Parameters | Test Error (Ours) | Paper Error |
|-------|--------|------------------|------------|-------------------|-------------|
| ResNet-20 | 20 | [3, 3, 3] | ~272K | 10.52% | 8.75% |
| ResNet-32 | 32 | [5, 5, 5] | ~464K | 9.42% | 7.51% |
| ResNet-44 | 44 | [7, 7, 7] | ~656K | 9.48% | 7.46% |
| ResNet-56 | 56 | [9, 9, 9] | ~848K | - | 6.97% |

### Network Structure

```
Input (32x32x3)
    ↓
Conv1 (3x3, 16 filters)
    ↓
Layer1 (16 filters, stride=1) × n blocks
    ↓
Layer2 (32 filters, stride=2) × n blocks
    ↓
Layer3 (64 filters, stride=2) × n blocks
    ↓
Global Average Pooling
    ↓
Fully Connected (10 classes)
```

### BasicBlock Architecture

Each BasicBlock consists of:
```
x → Conv(3x3) → BatchNorm → ReLU → Conv(3x3) → BatchNorm → + → ReLU
↓                                                              ↑
└─────────────────── Shortcut ────────────────────────────────┘
```

**Shortcut Connection:**
- Identity mapping when dimensions match
- 1x1 convolution + BatchNorm when dimensions change (stride=2)

## Details

This implementation follows **Section 4.2 (CIFAR-10)** from the paper "Deep Residual Learning for Image Recognition".

**Key Implementation Details:**
- Trained 3 ResNet variants with different depths
- Compared results with paper benchmarks
- Used standard CIFAR-10 preprocessing and augmentation
- Applied learning rate schedule as described in paper

## Key Features

- **Residual Learning**: Skip connections enable training of deeper networks
- **Batch Normalization**: Applied after each convolution layer
- **Learning Rate Scheduling**: Step decay at epochs 80 and 120
- **Data Augmentation**: Random crop and horizontal flip
- **Model Checkpointing**: Saves best model during training
- **Multiple Variants**: ResNet-20, 32, 44, and 56 implementations

## File Structure

```
ResNet/
├── ResNet20_fixed.py       # Main implementation
├── ResNet.ipynb            # Jupyter notebook version
├── Untitled.ipynb          # Experiments notebook
├── resnet20_cifar10.pt     # Pre-trained ResNet-20
├── resnet32_cifar10.pt     # Pre-trained ResNet-32
├── resnet44_cifar10.pt     # Pre-trained ResNet-44
└── README.md               # This file
```

## Training Configuration

### Hyperparameters

```python
Batch Size: 128
Epochs: 160
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
Epoch 120-159: lr = 0.001
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

The dataset is automatically downloaded when running the script.

## Usage

### Requirements

```bash
torch>=1.9.0
torchvision
numpy
matplotlib
```

### Running the Code

```bash
# Navigate to the directory
cd ResNet

# Run training (requires CUDA GPU)
python ResNet20_fixed.py

# Or use Jupyter notebook
jupyter notebook ResNet.ipynb
```

### Using Pre-trained Models

```python
import torch
from ResNet20_fixed import ResNet20

# Load model
model = ResNet20()
checkpoint = torch.load('resnet20_cifar10.pt')
model.load_state_dict(checkpoint['net'])
model.eval()
```

## Performance

### Results Comparison

||ResNet-20|ResNet-32|ResNet-44|
|------|---------|---------|---------|
|Test err (Ours)|10.52%| 9.42%|9.48%|
|Paper err|8.75%|7.51%|7.46%|
|Accuracy (Ours)|89.48%|90.58%|90.52%|

**Analysis:**
- Our implementation achieves competitive results
- Slight performance gap likely due to:
  - Different random initialization
  - Training duration variations
  - Hardware differences
  - Minor implementation details

## Key Concepts

### Residual Learning

**Problem**: Deep networks suffer from degradation
**Solution**: Learn residual function F(x) instead of H(x)

```
H(x) = F(x) + x
where F(x) = weight layers
```

### Skip Connections Benefits

- Allows gradients to flow directly through network
- Enables training of very deep networks (100+ layers)
- Reduces vanishing gradient problem
- Improves optimization landscape
- Makes identity mapping easy to learn

## Code Structure

### Main Components

**1. BasicBlock Class**
```python
class BasicBlock(nn.Module):
    - Two 3x3 convolutions
    - Batch normalization
    - Skip connection
    - ReLU activation
```

**2. ResNet Class**
```python
class ResNet(nn.Module):
    - Initial convolution
    - Three residual layers
    - Global average pooling
    - Fully connected layer
```

**3. Training Functions**
```python
def train(epoch):
    - Forward pass
    - Loss computation
    - Backpropagation
    - Metrics logging

def test(epoch):
    - Evaluation mode
    - Accuracy calculation
    - Model checkpointing
```

## Comparison with Plain Networks

| Metric | Plain-20 | ResNet-20 | Improvement |
|--------|----------|-----------|-------------|
| Training Error | Higher | Lower | Significant |
| Test Error | ~13% | ~10.5% | ~2.5% |
| Convergence | Slower | Faster | Notable |
| Depth Scalability | Poor | Excellent | Dramatic |

## Extensions & Improvements

### Possible Enhancements

- **Pre-activation ResNet**: Move BatchNorm before ReLU
- **Wide ResNet**: Increase width instead of depth
- **ResNeXt**: Add grouped convolutions (see ../ResNeXt)
- **Squeeze-and-Excitation**: Add attention mechanisms
- **Mixed Precision Training**: Use FP16 for faster training

### Advanced Techniques

```python
# Cutout augmentation
# Mixup training
# Label smoothing
# Cosine annealing scheduler
# Gradient clipping
```

## Troubleshooting

### Common Issues

**CUDA Out of Memory:**
```python
# Reduce batch size
batch_size = 64  # instead of 128
```

**Variable Name Errors:**
The fixed version uses consistent variable naming throughout.

## Visualization

### Training Curves

Monitor these metrics during training:
- Training loss (should decrease steadily)
- Training accuracy (should increase)
- Test accuracy (should plateau ~90%)
- Learning rate (step decay pattern)

### Model Analysis

```python
# Count parameters
total_params = sum(p.numel() for p in model.parameters())

# Visualize architecture
from torchviz import make_dot
make_dot(model(input), params=dict(model.named_parameters()))
```

## Reference

**Deep Residual Learning for Image Recognition**
- Authors: Kaiming He, Xiangyu Zhang, Shaoqing Ren, Jian Sun
- Conference: CVPR 2016, pp. 770-778
- Paper: https://arxiv.org/abs/1512.03385
- Citations: 100,000+

### Key Contributions

1. Introduced residual learning framework
2. Demonstrated networks with 100+ layers
3. Won ImageNet and COCO 2015 competitions
4. Influenced countless subsequent architectures

## Related Work

- **ResNeXt**: Aggregated residual transformations → `../ResNeXt/`
- **DenseNet**: Dense connections between layers
- **Wide ResNet**: Wider but shallower networks
- **ResNeSt**: Split-attention networks

---

<div align="center">

**ResNet Implementation** | **CIFAR-10 Classification** | **PyTorch**

[← Back to Main](../README.md)

</div>
