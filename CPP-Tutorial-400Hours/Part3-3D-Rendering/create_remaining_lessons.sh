#!/bin/bash
# This script creates the lesson files as a placeholder to track completion
# The actual content will be added individually

lessons=(
"Lesson30_Texturing_Basics"
"Lesson31_Texture_Filtering_and_Mipmapping"
"Lesson32_Multiple_Textures_and_Texture_Units"
"Lesson33_Transformations_in_OpenGL"
"Lesson34_Camera_Implementation_in_OpenGL"
"Lesson35_Lighting_Phong_Lighting_Model"
"Lesson36_Materials_and_Light_Properties"
"Lesson37_Multiple_Lights"
"Lesson38_Model_Loading_with_Assimp"
"Lesson39_Depth_Testing_and_Z_Buffer"
"Lesson40_Blending_and_Transparency"
)

for lesson in "${lessons[@]}"; do
    echo "Creating $lesson.md"
done
