# Deep Learning Practice Repository

<div align="center">

![PyTorch](https://img.shields.io/badge/PyTorch-EE4C2C?style=for-the-badge&logo=pytorch&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![CUDA](https://img.shields.io/badge/CUDA-76B900?style=for-the-badge&logo=nvidia&logoColor=white)
![Deep Learning](https://img.shields.io/badge/Deep_Learning-FF6F00?style=for-the-badge&logo=tensorflow&logoColor=white)

*A comprehensive collection of deep learning implementations and experiments*

</div>

---

## Overview

This repository contains implementations and experiments with various deep learning architectures, focusing on computer vision tasks including image classification and face recognition. Each subdirectory contains working implementations with detailed documentation.

## Repository Structure

```
practice/
├── ArcFace/                    # Face recognition with ArcFace loss
├── ResNet/                     # ResNet-20 implementation
├── ResNeXt/                    # ResNeXt-29 implementation
├── cnn_vs_depthwise/          # CNN vs Depthwise Convolution comparison
└── basic_fr_demo-master/      # Face recognition demo application
```

## Projects

### ArcFace - Face Recognition
Implementation of ArcFace (Additive Angular Margin Loss) for face recognition tasks using MNIST dataset as a toy example.

**Key Features:**
- ArcFace loss implementation
- Softmax baseline comparison
- 3D visualization of embeddings
- Performance metrics and analysis

[View Documentation →](./ArcFace/README.md)

---

### ResNet - Residual Networks
ResNet-20/32/44/56 implementations trained on CIFAR-10 dataset.

**Key Features:**
- Multiple ResNet variants (20, 32, 44, 56 layers)
- BasicBlock implementation with skip connections
- Learning rate scheduling
- Model checkpointing

[View Documentation →](./ResNet/README.md)

---

### ResNeXt - Aggregated Residual Transformations
ResNeXt-29 implementation with grouped convolutions for improved performance.

**Key Features:**
- Cardinality and bottleneck width experiments
- Group convolution implementation
- TensorBoard integration
- CIFAR-10 classification

[View Documentation →](./ResNeXt/README.md)

---

### CNN vs Depthwise Convolution
Comparative study between standard CNN and depthwise separable convolutions.

**Key Features:**
- Standard CNN implementation
- Depthwise separable convolution implementation
- Parameter efficiency analysis
- Performance comparison on CIFAR-10

[View Documentation →](./cnn_vs_depthwise/README.md)

---

### Basic Face Recognition Demo
Production-ready face recognition demo application with PyQt5 GUI.

**Key Features:**
- Real-time face recognition
- Gallery management system
- Customizable backbone models
- CRUD operations for face database

[View Documentation →](./basic_fr_demo-master/README.md)

---

## Tech Stack

| Technology | Version | Purpose |
|------------|---------|---------|
| Python | 3.x | Primary programming language |
| PyTorch | 1.9.0+ | Deep learning framework |
| CUDA | 11.1+ | GPU acceleration |
| cuDNN | 8.1.0+ | CUDA Deep Neural Network library |
| TorchVision | Latest | Computer vision datasets and models |
| PyQt5 | Latest | GUI framework (for demo app) |

## Requirements

```bash
# Core dependencies
torch>=1.9.0
torchvision
numpy
matplotlib

# For face recognition demo
PyQt5
pandas
scikit-image
opencv-python
```

## Installation

1. Clone the repository:
```bash
git clone https://github.com/comb0703/practice.git
cd practice
```

2. Install dependencies:
```bash
pip install torch torchvision numpy matplotlib
```

3. For GPU support, ensure CUDA is properly installed:
```bash
python -c "import torch; print(torch.cuda.is_available())"
```

## Quick Start

Each project directory contains standalone Python scripts that can be run directly:

```bash
# ResNet-20 on CIFAR-10
cd ResNet
python ResNet20_fixed.py

# ResNeXt-29 on CIFAR-10
cd ResNeXt
python ResNeXt_29.py

# CNN comparison
cd cnn_vs_depthwise
python cnn.py
python depthwise.py
```

## Dataset Information

Most projects use the **CIFAR-10** dataset:
- 60,000 32x32 color images
- 10 classes
- 50,000 training images
- 10,000 test images

The dataset is automatically downloaded when running the scripts.

## Training Configuration

Common training parameters across projects:

| Parameter | Value |
|-----------|-------|
| Batch Size | 128 |
| Optimizer | SGD with Momentum (0.9) |
| Weight Decay | 0.0001 |
| Initial Learning Rate | 0.1 |
| LR Schedule | Divide by 10 at epochs 80 and 120 |

## Results & Benchmarks

Each project directory contains detailed results, including:
- Training/validation accuracy curves
- Loss progression
- Model checkpoints
- Visualization plots
- Performance metrics

## Project Highlights

### What Makes This Repository Useful?

- **Clean Implementations**: Well-structured, readable code following PyTorch best practices
- **Educational Value**: Each implementation includes detailed comments and documentation
- **Reproducible Results**: Consistent training configurations and random seeds
- **Comparative Analysis**: Direct comparisons between different architectures
- **Production-Ready Demo**: Functional face recognition application with GUI

## Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests
- Improve documentation

## Future Work

- [ ] Add more modern architectures (EfficientNet, Vision Transformers)
- [ ] Implement mixed precision training
- [ ] Add distributed training support
- [ ] Create Jupyter notebook tutorials
- [ ] Add pre-trained model weights

## References

- **ResNet**: Deep Residual Learning for Image Recognition (He et al., 2015)
- **ResNeXt**: Aggregated Residual Transformations for Deep Neural Networks (Xie et al., 2017)
- **ArcFace**: Additive Angular Margin Loss for Deep Face Recognition (Deng et al., 2019)
- **CIFAR-10**: Learning Multiple Layers of Features from Tiny Images (Krizhevsky, 2009)

## License

This project is open source and available for educational purposes.

## Contact

For questions or suggestions, please open an issue in the repository.

---

<div align="center">

**Made with PyTorch** | **Last Updated: November 2025**

</div>
