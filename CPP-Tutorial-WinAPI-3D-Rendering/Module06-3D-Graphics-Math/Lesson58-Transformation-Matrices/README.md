# Lesson 58: Transformation Matrices

## Translation, Rotation, Scaling

### Translation Matrix
```
T = [ 1  0  0  tx ]
    [ 0  1  0  ty ]
    [ 0  0  1  tz ]
    [ 0  0  0   1 ]
```

### Rotation (Z-axis)
```
Rz(θ) = [ cos(θ) -sin(θ)  0  0 ]
        [ sin(θ)  cos(θ)  0  0 ]
        [   0       0     1  0 ]
        [   0       0     0  1 ]
```

### Scaling Matrix
```
S = [ sx  0   0  0 ]
    [  0 sy   0  0 ]
    [  0  0  sz  0 ]
    [  0  0   0  1 ]
```

## Code Examples
1-15: Translation, Rotation X/Y/Z, Scaling, Combined transforms, Euler angles, Local/World space, Transform hierarchies
