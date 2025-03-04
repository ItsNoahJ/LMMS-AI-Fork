#ifndef LMMS_MAGENTA_MUSIC_VAE_MODEL_H
#define LMMS_MAGENTA_MUSIC_VAE_MODEL_H

#include "TensorFlowLiteModel.h"
#include "../utils/MidiUtils.h"
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
     * @brief Interpolate between two points in latent space
     * @param z1 First latent vector
     * @param z2 Second latent vector
     * @param numSteps Number of interpolation steps
     * @return Vector of interpolated latent vectors
     */
    std::vector<std::vector<float>> interpolate(const std::vector<float>& z1,
                                               const std::vector<float>& z2,
                                               int numSteps);
    
    /**
     * @brief Get the latent space dimension
     * @return Dimension of the latent space
     */
    int getLatentDimension() const;
    
private:
    // Latent space dimension
    int m_latentDimension;
    
    // Random number generator
    std::mt19937 m_rng;
    
    // Helper methods
    std::vector<float> sampleNormal(int dimension, float temperature);
    std::vector<float> applyTemperature(const std::vector<float>& logits, float temperature);
};

} // namespace lmms_magenta

#endif // LMMS_MAGENTA_MUSIC_VAE_MODEL_H
