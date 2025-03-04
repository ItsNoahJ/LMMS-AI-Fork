#include "GrooVAEModel.h"
#include <iostream>
#include <algorithm>
#include <cmath>

namespace lmms_magenta {

GrooVAEModel::GrooVAEModel(const std::string& modelPath)
    : TensorFlowLiteModel(modelPath)
    , m_temperature(1.0f)
    , m_humanize(0.5f) {
}

GrooVAEModel::~GrooVAEModel() {
}

bool GrooVAEModel::applyGroove(const std::vector<MidiNote>& inputNotes, 
                             std::vector<MidiNote>& outputNotes) {
    // Check if model is loaded
    if (!isLoaded()) {
        if (!load()) {
            std::cerr << "Failed to load model" << std::endl;
            return false;
        }
    }
    
    try {
        // Convert MIDI notes to tensor representation
        std::vector<float> inputTensor = MidiUtils::notesToTensor(inputNotes);
        
        // Set input tensor
        if (!setInputTensor("input_sequence", inputTensor)) {
            std::cerr << "Failed to set input tensor" << std::endl;
            return false;
        }
        
        // Set the temperature
        if (!setInputTensor("temperature", {m_temperature})) {
            std::cerr << "Failed to set temperature" << std::endl;
            return false;
        }
        
        // Set the humanize parameter
        if (!setInputTensor("humanize", {m_humanize})) {
            std::cerr << "Failed to set humanize parameter" << std::endl;
            return false;
        }
        
        // Run the model
        if (!run()) {
            std::cerr << "Failed to run model" << std::endl;
            return false;
        }
        
        // Get the output tensor
        std::vector<float> outputTensor = getOutputTensor("output_sequence");
        
        // Convert tensor to MIDI notes
        outputNotes = MidiUtils::tensorToNotes(outputTensor);
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error applying groove: " << e.what() << std::endl;
        return false;
    }
}

bool GrooVAEModel::extractGroove(const std::vector<MidiNote>& notes, std::vector<float>& groove) {
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
        if (!setInputTensor("input_sequence", inputTensor)) {
            std::cerr << "Failed to set input tensor" << std::endl;
            return false;
        }
        
        // Run the model
        if (!run()) {
            std::cerr << "Failed to run model" << std::endl;
            return false;
        }
        
        // Get the groove vector from the output tensor
        groove = getOutputTensor("groove_embedding");
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error extracting groove: " << e.what() << std::endl;
        return false;
    }
}

bool GrooVAEModel::applyGrooveVector(const std::vector<MidiNote>& inputNotes, 
                                   const std::vector<float>& groove, 
                                   std::vector<MidiNote>& outputNotes) {
    // Check if model is loaded
    if (!isLoaded()) {
        if (!load()) {
            std::cerr << "Failed to load model" << std::endl;
            return false;
        }
    }
    
    try {
        // Convert MIDI notes to tensor representation
        std::vector<float> inputTensor = MidiUtils::notesToTensor(inputNotes);
        
        // Set input tensor
        if (!setInputTensor("input_sequence", inputTensor)) {
            std::cerr << "Failed to set input tensor" << std::endl;
            return false;
        }
        
        // Set the groove vector
        if (!setInputTensor("groove_embedding", groove)) {
            std::cerr << "Failed to set groove vector" << std::endl;
            return false;
        }
        
        // Set the temperature
        if (!setInputTensor("temperature", {m_temperature})) {
            std::cerr << "Failed to set temperature" << std::endl;
            return false;
        }
        
        // Set the humanize parameter
        if (!setInputTensor("humanize", {m_humanize})) {
            std::cerr << "Failed to set humanize parameter" << std::endl;
            return false;
        }
        
        // Run the model
        if (!run()) {
            std::cerr << "Failed to run model" << std::endl;
            return false;
        }
        
        // Get the output tensor
        std::vector<float> outputTensor = getOutputTensor("output_sequence");
        
        // Convert tensor to MIDI notes
        outputNotes = MidiUtils::tensorToNotes(outputTensor);
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error applying groove vector: " << e.what() << std::endl;
        return false;
    }
}

void GrooVAEModel::setTemperature(float temperature) {
    m_temperature = std::max(0.0001f, std::min(2.0f, temperature));
}

float GrooVAEModel::getTemperature() const {
    return m_temperature;
}

void GrooVAEModel::setHumanize(float humanize) {
    m_humanize = std::max(0.0f, std::min(1.0f, humanize));
}

float GrooVAEModel::getHumanize() const {
    return m_humanize;
}

} // namespace lmms_magenta
