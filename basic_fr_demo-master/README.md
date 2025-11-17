# Face Recognition Demo Application

<div align="center">

![PyQt5](https://img.shields.io/badge/PyQt5-41CD52?style=flat-square&logo=qt&logoColor=white)
![PyTorch](https://img.shields.io/badge/PyTorch-EE4C2C?style=flat-square&logo=pytorch&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=flat-square&logo=python&logoColor=white)
![OpenCV](https://img.shields.io/badge/OpenCV-5C3EE8?style=flat-square&logo=opencv&logoColor=white)

*Production-ready face recognition demo with GUI*

**연구용 Face Recognition Demo Application**

</div>

---

## Overview

This is a **research-oriented face recognition demo application** with a PyQt5 graphical user interface. The application provides real-time face recognition with gallery management capabilities, designed for testing and demonstrating face recognition algorithms.

## Key Features

- **Real-time Face Recognition**: Live face detection and recognition
- **Gallery Management**: CRUD operations for face database
- **Customizable Backbone**: Easy model replacement and configuration
- **Persistent Storage**: Gallery data saved as CSV and embeddings as NPY
- **PyQt5 GUI**: User-friendly interface for demo purposes
- **Research-friendly**: Modular SDK design for easy experimentation

## File Structure

```
basic_fr_demo-master/
├── demo.py                      # Main GUI application
├── CURDsdk.py                   # SDK for face recognition operations
├── CURD_css.py                  # GUI styling (CSS)
├── models/
│   ├── arcface_torch/
│   │   └── iresnet.py          # iResNet backbone implementation
│   └── Glint360k_r18.pth       # Pre-trained model weights
├── tutorial/
│   ├── RetinaFace/             # Face detection module
│   └── pth/                    # Additional model weights
├── imgs/                        # Sample images
├── pth/                         # Model checkpoints
└── README.md                    # This file
```

## Requirements

### Core Dependencies

```bash
PyQt5          # GUI framework
pandas         # Data management
torch          # Deep learning framework
scikit-image   # Image processing
opencv-python  # Computer vision
numpy          # Numerical operations
```

### Installation

```bash
pip install PyQt5 pandas torch scikit-image opencv-python numpy
```

## Gallery Storage Format

The application stores gallery data in two formats:

### 1. File List (CSV)
Gallery file list is stored as a **pandas DataFrame** in CSV format:
```
gallery_folder/Glint360k_r18.csv
```

**Contents:**
- Image filenames
- Person IDs
- Metadata

### 2. Embedding Vectors (NPY)
Face embeddings are stored as **numpy arrays** in NPY format:
```
gallery_folder/Glint360k_r18.npy
```

**Contents:**
- Feature vectors (embeddings)
- Shape: [num_faces, embedding_dim]

## Customization

### Basic Configuration

Modify the SDK initialization in `CURDsdk.py`:

```python
class CURD_sdk:
    def __init__(self):
        self.init_done = False
        self.detector = None

        self.reid_thres = 35              # Re-identification threshold
        self.model_path = 'models/Glint360k_r18.pth'  # Model path

        self.gal_root = 'gal/'            # Gallery root directory
        self.gal_file_name = 'Glint360k_r18'  # Gallery filename prefix
```

### Configuration Parameters

| Parameter | Description | Default Value |
|-----------|-------------|---------------|
| `reid_thres` | Re-identification threshold | 35 |
| `model_path` | Path to model weights | 'models/Glint360k_r18.pth' |
| `gal_root` | Gallery directory | 'gal/' |
| `gal_file_name` | Gallery file prefix | 'Glint360k_r18' |

## Backbone Replacement

To replace the face recognition backbone, modify these **3 functions** in `CURDsdk.py`:

### 1. Set Extractor

```python
def set_extractor(self):
    """Initialize the feature extractor"""
    self.extractor = self.get_extractor(self.model_path)
    self.extractor.to(self.device)
    self.extractor.eval()
```

### 2. Get Extractor

```python
def get_extractor(self, model_path):
    """Load the feature extraction model"""
    sys.path.append('./models/arcface_torch')
    from iresnet import iresnet18

    net = iresnet18(fp16=False)
    net.load_state_dict(torch.load(model_path))
    sys.stdout.write('\t[Network] %s\n\n' % model_path)
    sys.stdout.flush()
    return net
```

**To use a different backbone:**
```python
# Example: Using ResNet-50
from torchvision.models import resnet50
net = resnet50(pretrained=True)
# Modify final layer for embedding extraction
net.fc = nn.Identity()
```

### 3. Extract Embedding

```python
def extract_emb(self, face):
    """Extract embedding from face image"""
    face = cv2.cvtColor(face, cv2.COLOR_BGR2RGB)
    face = np.transpose(face, (2, 0, 1))
    face = torch.from_numpy(face).unsqueeze(0).float()
    face.div_(255).sub_(0.5).div_(0.5)  # Normalize to [-1, 1]

    self.extractor.eval()
    return self.extractor(face.to(self.device)).cpu().detach().numpy()
```

**Preprocessing steps:**
1. Convert BGR to RGB
2. Transpose to CHW format
3. Convert to PyTorch tensor
4. Normalize to [-1, 1]
5. Extract features

## SDK Design

### CURD_sdk Class

The SDK provides comprehensive face recognition operations:

```python
class CURD_sdk:
    def __init__(self)
        # Initialize models and configurations

    def set_detector(self)
        # Initialize face detector

    def set_extractor(self)
        # Initialize feature extractor

    def extract_emb(self, face)
        # Extract face embedding

    def get_frame(self, frame)
        # Process video frame

    def add_to_gallery(self, face, person_id)
        # Add face to gallery

    def search_gallery(self, embedding)
        # Search for matching face

    def remove_from_gallery(self, index)
        # Remove face from gallery

    def update_gallery(self, index, new_data)
        # Update gallery entry
```

## Using Without GUI

The SDK can be used independently from the PyQt5 GUI:

```python
from CURDsdk import CURD_sdk

# Initialize SDK
sdk = CURD_sdk()

# Process a frame
result_frame = sdk.get_frame(input_frame)

# Add to gallery
sdk.add_to_gallery(face_image, person_id="John_Doe")

# Search gallery
matches = sdk.search_gallery(face_embedding)
```

This allows integration into:
- Custom applications
- Web services
- Streaming pipelines
- Batch processing scripts

## Application Features

### Gallery Operations (CRUD)

1. **Create**: Add new faces to gallery
   - Upload image
   - Extract embedding
   - Store with person ID

2. **Read**: View gallery contents
   - Display all registered faces
   - Show metadata
   - Preview embeddings

3. **Update**: Modify gallery entries
   - Update person ID
   - Replace image
   - Re-extract features

4. **Delete**: Remove faces from gallery
   - Select entry
   - Confirm deletion
   - Update storage files

### Face Recognition Pipeline

```
Input Frame
    ↓
Face Detection (RetinaFace)
    ↓
Face Alignment
    ↓
Preprocessing
    ↓
Feature Extraction (iResNet-18)
    ↓
Gallery Matching (Cosine Similarity)
    ↓
Threshold Filtering
    ↓
Output (Person ID / Unknown)
```

## Model Information

### Default Backbone: iResNet-18

**Architecture**: Improved ResNet-18 for face recognition
**Training Data**: Glint360K dataset
**Embedding Dimension**: 512
**Input Size**: 112×112
**Format**: FP32 (FP16 optional)

### Pre-trained Weights

```
models/Glint360k_r18.pth
```

**Training Details:**
- Dataset: Glint360K (360,000 identities, 17M images)
- Loss: ArcFace
- Accuracy: State-of-the-art on major benchmarks

## Performance Tuning

### Threshold Adjustment

```python
# Lower threshold = More lenient matching (higher false positives)
self.reid_thres = 25

# Higher threshold = Stricter matching (higher false negatives)
self.reid_thres = 45

# Default: Balanced
self.reid_thres = 35
```

### Speed Optimization

```python
# Use FP16 for faster inference
net = iresnet18(fp16=True)

# Reduce input resolution
# (Trade accuracy for speed)

# Batch processing
# Process multiple faces at once
```

## Technical Details

### Face Detection

Uses **RetinaFace** for robust face detection:
- Multi-scale detection
- Face landmarks (5 points)
- High accuracy on difficult cases
- Located in `tutorial/RetinaFace/`

### Feature Extraction

Uses **iResNet-18** with ArcFace training:
- Normalized embeddings (L2 norm = 1)
- Cosine similarity for matching
- 512-dimensional feature vectors

### Similarity Computation

```python
# Cosine similarity
similarity = np.dot(embedding1, embedding2)

# Convert to distance
distance = 1 - similarity

# Threshold comparison
is_match = distance < (threshold / 100)
```

## GUI Overview

The PyQt5 interface provides:

### Main Components

1. **Video Display**: Real-time camera feed
2. **Gallery Panel**: View registered faces
3. **Control Buttons**: Add/Remove/Update operations
4. **Status Bar**: System messages and feedback
5. **Settings Panel**: Threshold and configuration

### Styling

Custom CSS styling in `CURD_css.py`:
- Modern flat design
- Color-coded status indicators
- Responsive layout
- Customizable themes

## Use Cases

### Research Applications

- **Algorithm Testing**: Evaluate different backbones
- **Dataset Collection**: Build custom galleries
- **Performance Analysis**: Compare recognition methods
- **Visualization**: Demonstrate face recognition concepts

### Demo Scenarios

- **Access Control**: Simulate entry systems
- **Attendance**: Track person presence
- **Customer Recognition**: Identify returning customers
- **Security**: Monitor authorized personnel

## Troubleshooting

### Common Issues

**Model Loading Error:**
```python
# Ensure model path is correct
self.model_path = 'models/Glint360k_r18.pth'

# Check file exists
import os
assert os.path.exists(self.model_path)
```

**Gallery Not Found:**
```python
# Create gallery directory
os.makedirs(self.gal_root, exist_ok=True)
```

**Low Recognition Accuracy:**
```python
# Adjust threshold
self.reid_thres = 30  # Lower for more matches

# Ensure good lighting
# Check face alignment
# Verify model quality
```

## Extensions & Improvements

### Possible Enhancements

**Advanced Features:**
- Multi-face tracking
- Age/gender estimation
- Emotion recognition
- Mask detection
- Live video recording

**Technical Improvements:**
- Database backend (SQLite/PostgreSQL)
- REST API server
- Cloud storage integration
- Real-time performance metrics
- Logging and analytics

**UI Improvements:**
- Dark mode
- Multi-language support
- Customizable layouts
- Advanced search filters
- Batch operations

## Security Considerations

For production deployment:

1. **Data Privacy**: Encrypt stored embeddings
2. **Access Control**: Implement user authentication
3. **Audit Logging**: Track all operations
4. **Secure Communication**: Use HTTPS for API
5. **GDPR Compliance**: Handle personal data properly

## Related Projects

- **InsightFace**: Comprehensive face recognition toolkit
- **FaceNet**: Google's face recognition system
- **DeepFace**: Facebook's deep learning face recognition
- **OpenFace**: Open-source face recognition library

## Reference

### Papers

**ArcFace**: Face recognition backbone training
**RetinaFace**: Face detection
**iResNet**: Improved ResNet for face recognition

### Datasets

**Glint360K**: Large-scale face recognition dataset
**MS-Celeb-1M**: Million-scale celebrity dataset
**VGGFace2**: Large-scale face recognition dataset

## Contributing

This is a research demo project. Contributions welcome:

- Bug fixes
- New features
- Documentation improvements
- Performance optimizations
- UI enhancements

## License

Research and educational use.

---

<div align="center">

**Face Recognition Demo** | **PyQt5 + PyTorch** | **Production Ready**

[← Back to Main](../README.md)

</div>
