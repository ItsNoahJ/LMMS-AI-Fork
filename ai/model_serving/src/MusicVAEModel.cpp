#include "MusicVAEModel.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

namespace lmms_magenta {

MusicVAEModel::MusicVAEModel(const std::string& modelPath)
    : TensorFlowLiteModel(modelPath)
    , m_temperature(1.0f)
    , m_zDimension(256) {
}

MusicVAEModel::~MusicVAEModel() {
}

bool MusicVAEModel::encode(const std::vector<MidiNote>& notes, std::vector<float>& latentVector) {
    // Check if model is loaded
    if (!isLoaded()) {
        if (!load()) {
            std::cerr << "Failed to load model" << std::endl;
            return false;
        }
    }
    
    try {
        // Convert MIDI notes to tensor representation
        std::vector<float> inputTensor = MidiUtils::notesToTensor(notes);
        
        // Set input tensor
        if (!setInputTensor("encoder_input", inputTensor)) {
            std::cerr << "Failed to set input tensor" << std::endl;
            return false;
        }
        
        // Run the model
        if (!run()) {
            std::cerr << "Failed to run model" << std::endl;
            return false;
        }
        
        // Get the latent vector from the output tensor
        latentVector = getOutputTensor("z");
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error encoding MIDI: " << e.what() << std::endl;
        return false;
    }
}

bool MusicVAEModel::decode(const std::vector<float>& latentVector, std::vector<MidiNote>& notes) {
    // Check if model is loaded
    if (!isLoaded()) {
        if (!load()) {
            std::cerr << "Failed to load model" << std::endl;
            return false;
        }
    }
    
    try {
        // Set the latent vector as input tensor
        if (!setInputTensor("z", latentVector)) {
            std::cerr << "Failed to set input tensor" << std::endl;
            return false;
        }
        
        // Set the temperature
        if (!setInputTensor("temperature", {m_temperature})) {
            std::cerr << "Failed to set temperature" << std::endl;
            return false;
        }
        
        // Run the model
        if (!run()) {
            std::cerr << "Failed to run model" << std::endl;
            return false;
        }
        
        // Get the output tensor
        std::vector<float> outputTensor = getOutputTensor("decoder_output");
        
        // Convert tensor to MIDI notes
        notes = MidiUtils::tensorToNotes(outputTensor);
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error decoding latent vector: " << e.what() << std::endl;
        return false;
    }
}

bool MusicVAEModel::sample(std::vector<MidiNote>& notes) {
    // Check if model is loaded
    if (!isLoaded()) {
        if (!load()) {
            std::cerr << "Failed to load model" << std::endl;
            return false;
        }
    }
    
    try {
        // Generate random latent vector
        std::vector<float> latentVector = generateRandomLatentVector();
        
        // Decode the latent vector
        return decode(latentVector, notes);
    }
    catch (const std::exception& e) {
        std::cerr << "Error sampling from model: " << e.what() << std::endl;
        return false;
    }
}

bool MusicVAEModel::interpolate(const std::vector<MidiNote>& startNotes, 
                              const std::vector<MidiNote>& endNotes, 
                              int steps, 
                              std::vector<std::vector<MidiNote>>& interpolatedSequences) {
    // Check if model is loaded
    if (!isLoaded()) {
        if (!load()) {
            std::cerr << "Failed to load model" << std::endl;
            return false;
        }
    }
    
    try {
        // Encode start and end notes
        std::vector<float> startLatent, endLatent;
        
        if (!encode(startNotes, startLatent)) {
            std::cerr << "Failed to encode start notes" << std::endl;
            return false;
        }
        
        if (!encode(endNotes, endLatent)) {
            std::cerr << "Failed to encode end notes" << std::endl;
            return false;
        }
        
        // Interpolate between latent vectors
        interpolatedSequences.clear();
        interpolatedSequences.resize(steps);
        
        for (int i = 0; i < steps; ++i) {
            // Calculate interpolation factor
            float t = static_cast<float>(i) / (steps - 1);
            
            // Interpolate latent vectors
            std::vector<float> interpolatedLatent(m_zDimension);
            for (size_t j = 0; j < m_zDimension; ++j) {
                interpolatedLatent[j] = (1.0f - t) * startLatent[j] + t * endLatent[j];
            }
            
            // Decode interpolated latent vector
            std::vector<MidiNote> notes;
            if (!decode(interpolatedLatent, notes)) {
                std::cerr << "Failed to decode interpolated latent vector at step " << i << std::endl;
                return false;
            }
            
            interpolatedSequences[i] = notes;
        }
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error interpolating: " << e.what() << std::endl;
        return false;
    }
}

void MusicVAEModel::setTemperature(float temperature) {
    m_temperature = std::max(0.0001f, std::min(2.0f, temperature));
}

float MusicVAEModel::getTemperature() const {
    return m_temperature;
}

std::vector<float> MusicVAEModel::generateRandomLatentVector() const {
    // Create random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(0.0f, 1.0f);
    
    // Generate random latent vector
    std::vector<float> latentVector(m_zDimension);
    for (size_t i = 0; i < m_zDimension; ++i) {
        latentVector[i] = dist(gen);
    }
    
    return latentVector;
}

} // namespace lmms_magenta
