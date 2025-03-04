#include "MusicVAEModel.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace lmms_magenta {

// Constructor
MusicVAEModel::MusicVAEModel(const std::string& modelPath, const ModelMetadata& metadata)
    : TensorFlowLiteModel(modelPath, metadata)
    , m_latentDimension(256)  // Default latent dimension, will be updated during initialization
    , m_rng(std::random_device{}()) {
}

// Destructor
MusicVAEModel::~MusicVAEModel() {
}

// Encode a MIDI sequence to latent space
std::vector<float> MusicVAEModel::encode(const MidiSequence& sequence) {
    if (!isInitialized()) {
        std::cerr << "Model not initialized" << std::endl;
        return std::vector<float>();
    }
    
    // Convert MIDI sequence to tensor
    std::vector<float> inputTensor = MidiUtils::sequenceToTensor(sequence);
    
    // Get input tensor shape
    std::vector<int> inputShape = getInputTensorShape(0);
    
    // Check if input tensor matches expected shape
    if (inputTensor.size() != static_cast<size_t>(inputShape[0] * inputShape[1])) {
        std::cerr << "Input tensor size mismatch" << std::endl;
        return std::vector<float>();
    }
    
    // Set input tensor data
    if (!setInputTensorData(0, inputTensor.data(), inputTensor.size() * sizeof(float))) {
        std::cerr << "Failed to set input tensor data" << std::endl;
        return std::vector<float>();
    }
    
    // Run inference
    if (!runInference()) {
        std::cerr << "Failed to run inference" << std::endl;
        return std::vector<float>();
    }
    
    // Get output tensor shape
    std::vector<int> outputShape = getOutputTensorShape(0);
    
    // Allocate output tensor
    std::vector<float> outputTensor(outputShape[0] * outputShape[1]);
    
    // Get output tensor data
    if (!getOutputTensorData(0, outputTensor.data(), outputTensor.size() * sizeof(float))) {
        std::cerr << "Failed to get output tensor data" << std::endl;
        return std::vector<float>();
    }
    
    // Update latent dimension
    m_latentDimension = outputShape[1];
    
    return outputTensor;
}

// Decode latent variables to a MIDI sequence
MidiSequence MusicVAEModel::decode(const std::vector<float>& z, float temperature) {
    if (!isInitialized()) {
        std::cerr << "Model not initialized" << std::endl;
        return MidiSequence();
    }
    
    // Check if latent vector matches expected dimension
    if (z.size() != static_cast<size_t>(m_latentDimension)) {
        std::cerr << "Latent vector dimension mismatch" << std::endl;
        return MidiSequence();
    }
    
    // Set input tensor data
    if (!setInputTensorData(1, z.data(), z.size() * sizeof(float))) {
        std::cerr << "Failed to set input tensor data" << std::endl;
        return MidiSequence();
    }
    
    // Set temperature
    std::vector<float> tempTensor = {temperature};
    if (!setInputTensorData(2, tempTensor.data(), tempTensor.size() * sizeof(float))) {
        std::cerr << "Failed to set temperature tensor data" << std::endl;
        return MidiSequence();
    }
    
    // Run inference
    if (!runInference()) {
        std::cerr << "Failed to run inference" << std::endl;
        return MidiSequence();
    }
    
    // Get output tensor shape
    std::vector<int> outputShape = getOutputTensorShape(1);
    
    // Allocate output tensor
    std::vector<float> outputTensor(outputShape[0] * outputShape[1]);
    
    // Get output tensor data
    if (!getOutputTensorData(1, outputTensor.data(), outputTensor.size() * sizeof(float))) {
        std::cerr << "Failed to get output tensor data" << std::endl;
        return MidiSequence();
    }
    
    // Convert tensor to MIDI sequence
    return MidiUtils::tensorToSequence(outputTensor);
}

// Sample from the prior distribution
std::vector<float> MusicVAEModel::samplePrior(float temperature) {
    // Sample from standard normal distribution
    std::vector<float> z = sampleNormal(m_latentDimension, temperature);
    
    return z;
}

// Interpolate between two points in latent space
std::vector<std::vector<float>> MusicVAEModel::interpolate(const std::vector<float>& z1,
                                                         const std::vector<float>& z2,
                                                         int numSteps) {
    std::vector<std::vector<float>> results;
    
    // Check if latent vectors match expected dimension
    if (z1.size() != static_cast<size_t>(m_latentDimension) || 
        z2.size() != static_cast<size_t>(m_latentDimension)) {
        std::cerr << "Latent vector dimension mismatch" << std::endl;
        return results;
    }
    
    // Ensure at least 2 steps
    numSteps = std::max(2, numSteps);
    
    // Allocate result vectors
    results.resize(numSteps);
    for (auto& z : results) {
        z.resize(m_latentDimension);
    }
    
    // Perform linear interpolation
    for (int step = 0; step < numSteps; ++step) {
        float t = static_cast<float>(step) / (numSteps - 1);
        
        for (int i = 0; i < m_latentDimension; ++i) {
            results[step][i] = (1.0f - t) * z1[i] + t * z2[i];
        }
    }
    
    return results;
}

// Get the latent space dimension
int MusicVAEModel::getLatentDimension() const {
    return m_latentDimension;
}

// Sample from normal distribution
std::vector<float> MusicVAEModel::sampleNormal(int dimension, float temperature) {
    std::vector<float> samples(dimension);
    
    // Standard normal distribution
    std::normal_distribution<float> dist(0.0f, 1.0f);
    
    // Generate samples
    for (int i = 0; i < dimension; ++i) {
        samples[i] = dist(m_rng) * temperature;
    }
    
    return samples;
}

// Apply temperature to logits
std::vector<float> MusicVAEModel::applyTemperature(const std::vector<float>& logits, float temperature) {
    std::vector<float> scaled = logits;
    
    // Apply temperature scaling
    if (temperature != 1.0f) {
        for (auto& logit : scaled) {
            logit /= temperature;
        }
    }
    
    // Apply softmax
    float maxLogit = *std::max_element(scaled.begin(), scaled.end());
    float sum = 0.0f;
    
    for (auto& logit : scaled) {
        logit = std::exp(logit - maxLogit);
        sum += logit;
    }
    
    for (auto& logit : scaled) {
        logit /= sum;
    }
    
    return scaled;
}

} // namespace lmms_magenta
