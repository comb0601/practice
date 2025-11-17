# ArcFace - Additive Angular Margin Loss

<div align="center">

![PyTorch](https://img.shields.io/badge/PyTorch-EE4C2C?style=flat-square&logo=pytorch&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=flat-square&logo=python&logoColor=white)
![CUDA](https://img.shields.io/badge/CUDA-11.1-brightgreen?style=flat-square&logo=nvidia)
![cuDNN](https://img.shields.io/badge/cuDNN-8.1.0-blue?style=flat-square)
![PyTorch](https://img.shields.io/badge/PyTorch-1.9.0-orange?style=flat-square)

*Deep face recognition with angular margin loss*

</div>

---

## Overview

This directory contains a PyTorch implementation of **ArcFace (Additive Angular Margin Loss)** for deep face recognition. ArcFace improves the discriminative power of face recognition models by adding an additive angular margin to the softmax loss, enhancing both feature separability and intra-class compactness.

## Main Idea

![ArcFace Concept](https://user-images.githubusercontent.com/87002037/128664968-c7c7973d-af17-4c10-9daa-4e6b0f55a103.PNG)

**Key Innovation**: ArcFace introduces an additive angular margin penalty between the feature vector and the ground truth weight, which enhances the discriminative power of the learned features by maximizing the decision boundary in the angular (geodesic) space.

## Details

**Implementation Scope:**
- Implemented **Section 2.1, Figure 3** (Toy Example Experiments) from the ArcFace paper
- Used **MNIST dataset** as a toy example to demonstrate the concept
- Did **not apply scale** on ArcFace loss for simplicity

### Understanding ArcFace Loss

**Common Confusion Addressed:**
> "Where do I have to put ArcFace loss function in the network architecture?"

**Key Understanding:**
- Loss is calculated with **logit's output** and **ground-truth label**
- ArcFace is a **technique for changing logit's input with margin**
- The term "ArcFace loss" is somewhat misleading - it's still **Cross-Entropy loss**, but with modified logits
- ArcFace adds margin to the **ground truth's weight angle** before computing cross-entropy

![ArcFace Architecture](https://user-images.githubusercontent.com/87002037/128664977-b0b250c1-d23f-4e1d-841f-ec85869c5250.PNG)

### Mathematical Formulation

**Softmax Loss:**
```
L = -log(e^(s·cos(θ_yi)) / Σ e^(s·cos(θ_j)))
```

**ArcFace Loss:**
```
L = -log(e^(s·cos(θ_yi + m)) / (e^(s·cos(θ_yi + m)) + Σ_{j≠yi} e^(s·cos(θ_j))))
```

Where:
- `θ_yi`: Angle between feature and ground truth weight
- `m`: Additive angular margin (hyperparameter)
- `s`: Scale factor (hyperparameter, not used in this implementation)

## Performance

### Softmax Baseline

![Softmax Results](https://user-images.githubusercontent.com/87002037/128678740-424c2325-221b-4895-a8ed-258ed42d9231.PNG)

Features learned with standard softmax loss show **less separation** between classes in the embedding space.

### ArcFace Results

![ArcFace Results](https://user-images.githubusercontent.com/87002037/128678762-48a60c7b-696d-440a-87fb-44664a327a56.PNG)

Features learned with ArcFace loss show **better separation** and **tighter clusters** for each class.

### 3D Visualization

![3D Visualization](https://user-images.githubusercontent.com/87002037/128678829-ce71e0ff-744d-4faf-b9da-be27c92c2d02.PNG)

The 3D visualization clearly demonstrates how ArcFace creates more discriminative embeddings with:
- **Better inter-class separation**: Classes are further apart
- **Tighter intra-class variance**: Samples from the same class cluster more tightly
- **Clearer decision boundaries**: Easier to separate different classes

## Key Features

- **Angular Margin**: Enhances feature discriminability
- **Geodesic Distance**: Operates in angular space
- **Better Generalization**: Improved performance on unseen data
- **Toy Example**: MNIST demonstration for educational purposes
- **Visualization**: 3D embedding space visualization

## File Structure

```
ArcFace/
├── ArcFace.ipynb              # ArcFace implementation notebook
├── Softmax.ipynb              # Baseline softmax implementation
├── 3D Visualization.ipynb     # 3D embedding visualization
├── mnist/                     # MNIST dataset directory
├── .ipynb_checkpoints/        # Jupyter checkpoints
└── README.md                  # This file
```

## Dataset

**MNIST Dataset:**
- 60,000 training images
- 10,000 test images
- Image size: 28×28 pixels
- Grayscale images
- 10 classes: digits 0-9

**Why MNIST?**
This is a **toy example** to demonstrate the ArcFace concept. The clear performance difference between Softmax and ArcFace on MNIST helps understand the benefits of angular margin loss. In practice, ArcFace is primarily used for face recognition on datasets like LFW, CFP-FP, and AgeDB.

## Usage

### Requirements

```bash
torch>=1.9.0
torchvision
numpy
matplotlib
jupyter
```

### Running the Notebooks

```bash
cd ArcFace

# Start Jupyter
jupyter notebook

# Open and run:
# 1. Softmax.ipynb (baseline)
# 2. ArcFace.ipynb (improved method)
# 3. 3D Visualization.ipynb (compare results)
```

## Key Concepts

### Angular Margin

**Geometric Interpretation:**
- Traditional softmax: Maximizes linear separability
- ArcFace: Maximizes **angular separability**
- Adds margin in angular space (more discriminative)

### Feature Normalization

ArcFace requires:
1. **L2 normalization** of features
2. **L2 normalization** of weights
3. Computing **cosine similarity** instead of inner product

```python
# Normalize features
features = F.normalize(features, dim=1)

# Normalize weights
weights = F.normalize(weights, dim=1)

# Compute cosine similarity
cosine = F.linear(features, weights)

# Add angular margin to ground truth
theta = torch.acos(cosine)
theta_m = theta + margin  # Add margin
cosine_m = torch.cos(theta_m)
```

### Why Angular Margin Works

**Intuition:**
- Forces network to learn more discriminative features
- Increases decision boundary margin
- Penalizes easy examples, focuses on hard ones
- Geodesic distance is more suitable for face recognition

## Implementation Insights

### Where Does ArcFace Fit?

```
Network Architecture:
    Input Image
        ↓
    Feature Extractor (CNN)
        ↓
    Feature Vector (embedding)
        ↓
    L2 Normalization
        ↓
    Fully Connected Layer (normalized weights)
        ↓
    ArcFace Margin Addition  ← HERE!
        ↓
    Cross-Entropy Loss
```

**Important**: ArcFace modifies the **logits** before the loss, not the loss function itself.

### Training Process

1. Extract features from input
2. Normalize features to unit hypersphere
3. Compute cosine similarity with weight vectors
4. Add angular margin to ground truth class
5. Apply softmax and compute cross-entropy
6. Backpropagate and update

## Advantages of ArcFace

| Aspect | Softmax | ArcFace |
|--------|---------|---------|
| **Margin Type** | None | Additive Angular |
| **Decision Boundary** | Linear | Angular (Geodesic) |
| **Feature Distribution** | Scattered | Compact clusters |
| **Inter-class Separation** | Lower | Higher |
| **Intra-class Variance** | Higher | Lower |
| **Face Verification** | Good | Excellent |

## Comparison with Other Losses

### Evolution of Face Recognition Losses

1. **Softmax**: Basic classification loss
2. **Center Loss**: Minimizes intra-class variance
3. **SphereFace**: Multiplicative angular margin
4. **CosFace**: Additive cosine margin
5. **ArcFace**: Additive angular margin (most effective)

### Why ArcFace Over Others?

- **Clear geometric interpretation**: Directly corresponds to geodesic distance
- **Better gradient behavior**: Easier to optimize than multiplicative margin
- **State-of-the-art performance**: Outperforms previous methods
- **Simple implementation**: Easy to integrate into existing networks

## Hyperparameters

### Key Parameters

```python
margin (m): Angular margin penalty
    - Typical values: 0.3 - 0.5
    - Higher margin = more discriminative but harder to optimize
    - This implementation: No scale applied

scale (s): Feature scale
    - Typical values: 30 - 64
    - Controls softmax temperature
    - This implementation: Not used (simplified version)
```

## Visualization Insights

The 3D visualization reveals:

**Softmax:**
- Classes overlap significantly
- No clear margin between clusters
- High intra-class variance

**ArcFace:**
- Clear separation between classes
- Tight intra-class clustering
- Visible margin between clusters
- Features distributed on hypersphere surface

## Extensions & Applications

### Real-world Applications

1. **Face Recognition**: Primary application
2. **Person Re-identification**: Track people across cameras
3. **Speaker Verification**: Voice-based authentication
4. **Image Retrieval**: Find similar images
5. **Fine-grained Classification**: Distinguish subtle differences

### Advanced Implementations

For production face recognition:
- Use larger datasets (MS-Celeb-1M, VGGFace2)
- Apply both margin (m) and scale (s)
- Use deeper backbones (ResNet-50, ResNet-100)
- Implement hard sample mining
- Add data augmentation

## Limitations

### Current Implementation

- Simplified toy example on MNIST
- No scale factor applied
- Single margin value (not optimized)
- No hard sample mining

### For Production Use

Consider:
- Proper hyperparameter tuning
- Large-scale face datasets
- State-of-the-art backbones
- Distributed training
- Hard sample mining strategies

## Reference

**ArcFace: Additive Angular Margin Loss for Deep Face Recognition**
- Authors: Jiankang Deng, Jia Guo, Niannan Xue, Stefanos Zafeiriou
- Conference: CVPR 2019
- Paper: https://arxiv.org/abs/1801.07698
- Code: https://github.com/deepinsight/insightface

### Key Contributions

1. Proposed additive angular margin loss
2. Achieved state-of-the-art face recognition
3. Clear geometric interpretation
4. Easy to implement and stable to train
5. Extensive experimental validation

### Real-world Performance

**On Face Recognition Benchmarks:**
- LFW: 99.83% accuracy
- CFP-FP: 98.27% accuracy
- AgeDB-30: 98.28% accuracy
- MegaFace: Rank-1 accuracy 98.35%

## Related Work

- **SphereFace**: Multiplicative angular margin
- **CosFace**: Additive cosine margin
- **CurricularFace**: Adaptive margin based on difficulty
- **MagFace**: Adaptive margin based on feature magnitude

## Future Improvements

### Potential Enhancements

```python
# Add scale factor
logits = s * cosine_with_margin

# Implement sub-center ArcFace
# Use multiple class centers

# Add hard sample mining
# Focus on difficult examples

# Implement partial FC
# For very large number of classes
```

---

<div align="center">

**ArcFace Implementation** | **Face Recognition** | **PyTorch**

[← Back to Main](../README.md)

</div>
