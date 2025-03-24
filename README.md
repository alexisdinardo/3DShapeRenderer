# 3D Shape Renderer

An interactive 3D shape renderer built with C and OpenGL, demonstrating fundamental computer graphics principles and real-time 3D visualization.

## Features

- **Multiple 3D Shapes**:
  - Sphere: Generated using spherical coordinates
  - Torus: Created with parametric equations
  - Spring: Complex helical structure
  - Custom 3D model import support

- **Interactive Controls**:
  - Left-click and drag to rotate objects
  - Up/Down arrow keys to zoom in/out
  - Press 'r' to reset transformations
  - Press 'q' to quit

- **Custom Model Support**:
  - Import custom 3D models from text files
  - Automatic scaling and centering
  - Progress tracking for large models

## Prerequisites

- C compiler (gcc recommended)
- OpenGL
- GLUT (OpenGL Utility Toolkit)
- Make

### Installation on macOS
```bash
brew install freeglut
```

### Installation on Ubuntu/Debian
```bash
sudo apt-get install freeglut3-dev
```

### Installation on Windows
- Install MinGW or MSYS2
- Install GLUT binaries
- Add OpenGL and GLUT to your system PATH

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/yourusername/geo-mod.git
cd geo-mod
```

2. Build the project:
```bash
cd project1
make
```

3. Run the program:
```bash
./project1
```

## Usage

1. Launch the program
2. Choose a shape by pressing:
   - `1`: Sphere
   - `2`: Torus
   - `3`: Spring
   - `4`: Load custom model (will prompt for filename)

3. Interact with the shape:
   - Left-click and drag to rotate
   - Use arrow keys to zoom
   - Press 'r' to reset view
   - Press 'q' to quit

## Custom Model Format

To import your own 3D model, create a text file with the following format:
```
<number_of_vertices>
x1,y1,z1,w1
x2,y2,z2,w2
...
```

Example:
```
36
-0.5,-0.5,-0.5,1.0
-0.5,0.5,-0.5,1.0
...
```

## Technical Details

### Shape Generation
- **Sphere**: Created using spherical coordinates with configurable resolution
- **Torus**: Generated using parametric equations with major and minor radii
- **Spring**: Complex helical structure with configurable coils and spacing

### Rendering Pipeline
- Vertex buffer objects for efficient rendering
- Custom vertex and fragment shaders
- Depth buffer for proper 3D visualization
- Matrix transformations for rotation and scaling

### Performance Optimizations
- Efficient memory management
- Optimized vertex buffer usage
- Batch processing for large models
