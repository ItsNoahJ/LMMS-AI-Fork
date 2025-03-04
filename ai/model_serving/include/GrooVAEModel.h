#pragma once

#include "TensorFlowLiteModel.h"
#include "../utils/MidiUtils.h"
#include <vector>
#include <string>
#include <memory>

namespace lmms_magenta {

/**
 * @brief GrooVAE model implementation
 * 
 * This class implements the GrooVAE model for groove modeling
 * using TensorFlow Lite.
 */
class GrooVAEModel : public TensorFlowLiteModel {
public:
    /**
     * @brief Constructor
     * @param modelPath Path to the TensorFlow Lite model file
     * @param metadata Model metadata
     */
    GrooVAEModel(const std::string& modelPath, const ModelMetadata& metadata);
    
    /**
     * @brief Destructor
     */
    ~GrooVAEModel() override;
    
    /**
     * @brief Apply groove to a MIDI sequence
     * @param sequence MIDI sequence to apply groove to
     * @param grooveAmount Amount of groove to apply (0-1)
     * @param swingAmount Amount of swing to apply (0-1)
     * @return MIDI sequence with groove applied
     */
    MidiSequence applyGroove(const MidiSequence& sequence, 
                            float grooveAmount, 
                            float swingAmount);
    
    /**
     * @brief Extract groove from a MIDI sequence
     * @param sequence MIDI sequence to extract groove from
     * @return Vector of groove values
     */
    std::vector<float> extractGroove(const MidiSequence& sequence);
    
    /**
     * @brief Apply extracted groove to a MIDI sequence
     * @param sequence MIDI sequence to apply groove to
     * @param groove Vector of groove values
     * @param amount Amount of groove to apply (0-1)
     * @return MIDI sequence with groove applied
     */
    MidiSequence applyExtractedGroove(const MidiSequence& sequence, 
                                     const std::vector<float>& groove,
                                     float amount);
    
    /**
     * @brief Get available groove styles
     * @return Vector of available groove style names
     */
    std::vector<std::string> getAvailableGrooveStyles() const;
    
    /**
     * @brief Set groove style
     * @param styleIndex Index of the groove style to use
     */
    void setGrooveStyle(int styleIndex);
    
    /**
     * @brief Get current groove style
     * @return Index of the current groove style
     */
    int getGrooveStyle() const;
    
private:
    // Available groove styles
    std::vector<std::string> m_grooveStyles;
    
    // Current groove style
    int m_currentGrooveStyle;
    
    // Precomputed groove patterns
    std::vector<std::vector<float>> m_groovePatterns;
    
    // Load groove patterns
    void loadGroovePatterns();
};

} // namespace lmms_magenta
