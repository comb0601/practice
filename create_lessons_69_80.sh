#!/bin/bash
# Script to create comprehensive lessons 69-80

BASE_DIR="/home/user/practice/CPP-Tutorial-400Hours/Part3-3D-Rendering"

# List of remaining lessons
declare -a LESSONS=(
    "69:Cubemap-and-Skybox"
    "70:Environment-Mapping-Reflections"
    "71:Geometry-Shaders"
    "72:Tessellation-Shaders"
    "73:Compute-Shaders"
    "74:Instancing"
    "75:Level-of-Detail-LOD"
    "76:Frustum-Culling"
    "77:Occlusion-Culling"
    "78:Post-Processing-Effects"
    "79:Anti-Aliasing-Techniques"
    "80:Particle-Systems"
)

echo "Will create ${#LESSONS[@]} remaining lessons"
for lesson in "${LESSONS[@]}"; do
    NUM=$(echo $lesson | cut -d: -f1)
    NAME=$(echo $lesson | cut -d: -f2)
    echo "  Lesson $NUM: $NAME"
done
