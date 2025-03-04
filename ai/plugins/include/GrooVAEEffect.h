#pragma once

#include "AIEffect.h"
#include "../../model_serving/include/GrooVAEModel.h"
#include <memory>

namespace lmms_magenta {

/**
 * @brief GrooVAE effect plugin
 * 
 * This effect uses GrooVAE to apply groove to MIDI patterns.
 */
class GrooVAEEffect : public AIEffect {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent Parent model
     * @param descriptor Plugin descriptor
     */
    GrooVAEEffect(lmms::Model* parent, const lmms::Plugin::Descriptor* descriptor);
    
    /**
     * @brief Destructor
     */
    ~GrooVAEEffect() override;
    
    /**
     * @brief Initialize the effect
     * @return True if initialization was successful
     */
    bool initialize() override;
    
    /**
     * @brief Get the model type used by this plugin
     * @return Model type
     */
    ModelType getModelType() const override;
    
    /**
     * @brief Get the model name used by this plugin
     * @return Model name
     */
    std::string getModelName() const override;
    
    /**
     * @brief Process a pattern
     * @param pattern Pattern to process
     * @return Processed pattern
     */
    lmms::Pattern* processPattern(lmms::Pattern* pattern);
    
    /**
     * @brief Apply groove to a pattern
     * @param pattern Pattern to apply groove to
     * @param grooveAmount Amount of groove to apply (0-1)
     * @param swingAmount Amount of swing to apply (0-1)
     * @return Pattern with groove applied
     */
    lmms::Pattern* applyGroove(lmms::Pattern* pattern, float grooveAmount, float swingAmount);
    
    /**
     * @brief Get the GrooVAE model
     * @return Shared pointer to the GrooVAE model
     */
    std::shared_ptr<GrooVAEModel> getGrooVAEModel();
    
protected:
    /**
     * @brief Handle parameter change
     * @param param Parameter
     * @param value New value
     */
    void handleParameterChange(const lmms::AutomatableModel* param, float value) override;
    
private:
    // Groove amount
    float m_grooveAmount;
    
    // Swing amount
    float m_swingAmount;
    
    // Groove style
    int m_grooveStyle;
    
    // Whether to quantize before applying groove
    bool m_quantizeBeforeGroove;
    
    // Convert LMMS pattern to MIDI sequence
    MidiSequence patternToSequence(lmms::Pattern* pattern);
    
    // Convert MIDI sequence to LMMS pattern
    lmms::Pattern* sequenceToPattern(const MidiSequence& sequence);
};

} // namespace lmms_magenta
