# LMMS-Magenta Integration

This directory contains the implementation of the LMMS-Magenta integration project, which aims to add AI-powered music generation features to LMMS.

## Directory Structure

- `model_serving/`: Framework for loading and serving AI models
- `plugins/`: AI-powered LMMS plugins
- `ui_components/`: Reusable UI components for AI features
- `utils/`: Utility functions and helpers

## Building the Project

### Prerequisites

- CMake 3.10 or higher
- Qt 5.12 or higher
- TensorFlow Lite
- LMMS source code

### Build Instructions

1. Clone the LMMS repository:
   ```
   git clone https://github.com/LMMS/lmms.git
   ```

2. Clone the Magenta repository:
   ```
   git clone https://github.com/magenta/magenta.git
   ```

3. Clone the TensorFlow repository:
   ```
   git clone https://github.com/tensorflow/tensorflow.git
   ```

4. Build TensorFlow Lite:
   ```
   cd tensorflow
   mkdir build
   cd build
   cmake ../tensorflow/lite
   cmake --build .
   ```

5. Build the LMMS-Magenta integration:
   ```
   cd implementation
   mkdir build
   cd build
   cmake .. -DTensorFlowLite_DIR=/path/to/tensorflow/build
   cmake --build .
   ```

6. Install the plugins:
   ```
   cmake --install .
   ```

## Usage

After building and installing the plugins, they will be available in LMMS under the "AI" category.

### MusicVAE Instrument

The MusicVAE instrument allows you to generate musical patterns based on latent space interpolation. It provides the following features:

- Generate new patterns with adjustable temperature
- Interpolate between patterns
- Adjust style parameters such as complexity, density, rhythmic variation, and harmonic complexity

### GrooVAE Effect

The GrooVAE effect allows you to apply groove modeling to your patterns. It provides the following features:

- Adjust swing amount
- Control humanization
- Apply different groove styles

## Development

### Adding a New AI Plugin

To add a new AI plugin, follow these steps:

1. Create a new class that inherits from `AIInstrument` or `AIEffect`
2. Implement the required virtual methods
3. Add the plugin to the CMakeLists.txt file
4. Register the plugin in the LMMS plugin system

### Adding a New AI Model

To add a new AI model, follow these steps:

1. Create a new class that inherits from `TensorFlowLiteModel`
2. Implement the required virtual methods
3. Add the model to the ModelServer
4. Add the model to the CMakeLists.txt file

## License

This project is licensed under the GNU General Public License v2.0 - see the LICENSE file for details.
