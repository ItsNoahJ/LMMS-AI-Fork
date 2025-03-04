#pragma once

#include "TensorFlowLiteModel.h"
#include "../../utils/include/MidiUtils.h"
#include <vector>
#include <string>
#include <memory>

namespace lmms_magenta {

/**
 * @brief MusicVAE model implementation
 * 
 * This class implements the MusicVAE model for pattern generation
 * using TensorFlow Lite.
 */
class MusicVAEModel : public TensorFlowLiteModel {
public:
    /**
     * @brief Constructor
     * @param modelPath Path to the TensorFlow Lite model file
     * @param metadata Model metadata
     */
    MusicVAEModel(const std::string& modelPath, const ModelMetadata& metadata);
    
    /**
     * @brief Destructor
     */
    ~MusicVAEModel() override;
    
    /**
     * @brief Encode a MIDI sequence to latent space
     * @param sequence MIDI sequence to encode
     * @return Vector of latent variables (z)
     */
    std::vector<float> encode(const MidiSequence& sequence);
    
    /**
     * @brief Decode latent variables to a MIDI sequence
     * @param z Vector of latent variables
     * @param temperature Temperature for sampling (randomness)
     * @return Generated MIDI sequence
     */
    MidiSequence decode(const std::vector<float>& z, float temperature = 1.0f);
    
    /**
     * @brief Sample from the prior distribution
     * @param temperature Temperature for sampling (randomness)
     * @return Vector of latent variables (z)
     */
    std::vector<float> samplePrior(float temperature = 1.0f);
    
    /**
     * @brief Interpolate between two MIDI sequences
     * @param sequence1 First MIDI sequence
     * @param sequence2 Second MIDI sequence
     * @param numSteps Number of interpolation steps
     * @param temperature Temperature for sampling (randomness)
     * @return Vector of interpolated MIDI sequences
     */
    std::vector<MidiSequence> interpolate(const MidiSequence& sequence1, 
                                         const MidiSequence& sequence2,
                                         int numSteps,
                                         float temperature = 1.0f);
    
    /**
     * @brief Get the latent space dimension
     * @return Dimension of the latent space
     */
    int getLatentDimension() const;
    
    /**
     * @brief Get the maximum sequence length
     * @return Maximum sequence length in ticks
     */
    int getMaxSequenceLength() const;
    
private:
    // Latent space dimension
    int m_latentDimension;
    
    // Maximum sequence length
    int m_maxSequenceLength;
    
    // Ticks per quarter note
    int m_ticksPerQuarter;
    
    // Apply temperature to latent vector
    std::vector<float> applyTemperature(const std::vector<float>& z, float temperature);
};

} // namespace lmms_magenta
