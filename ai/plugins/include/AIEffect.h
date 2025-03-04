#pragma once

#include "AIPlugin.h"
#include "Effect.h"

namespace lmms_magenta {

/**
 * @brief Base class for AI-powered effects
 * 
 * This class extends the AIPlugin class and provides common functionality
 * for AI-powered effects, such as audio processing and parameter binding.
 */
class AIEffect : public AIPlugin {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent Parent model
     * @param descriptor Plugin descriptor
     */
    AIEffect(lmms::Model* parent, const lmms::Plugin::Descriptor* descriptor);
    
    /**
     * @brief Destructor
     */
    ~AIEffect() override;
    
    /**
     * @brief Initialize the effect
     * @return True if initialization was successful
     */
    bool initialize() override;
    
    /**
     * @brief Process audio
     * @param buf Buffer to process
     */
    virtual void processAudio(lmms::SampleFrame* buf);
    
protected:
    /**
     * @brief Handle parameter change
     * @param param Parameter
     * @param value New value
     */
    virtual void handleParameterChange(const lmms::AutomatableModel* param, float value);
};

} // namespace lmms_magenta
