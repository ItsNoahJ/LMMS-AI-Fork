#ifndef LMMS_MAGENTA_AI_EFFECT_H
#define LMMS_MAGENTA_AI_EFFECT_H

#include "AIPlugin.h"
#include "Effect.h"

namespace lmms_magenta {

/**
 * @brief Base class for AI effect plugins
 * 
 * This class extends the LMMS Effect class and the AIPlugin class
 * to provide a base for AI-powered effect plugins.
 */
class AIEffect : public lmms::Effect, public AIPlugin {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param track Parent effect chain
     * @param descriptor Plugin descriptor
     */
    AIEffect(lmms::EffectChain* parent, const lmms::Plugin::Descriptor* descriptor);
    
    /**
     * @brief Destructor
     */
    ~AIEffect() override;
    
    /**
     * @brief Initialize the effect
     * @return True if initialization was successful
     */
    bool initialize();
    
    /**
     * @brief Process audio buffer
     * @param buf Buffer to process
     */
    bool processAudioBuffer(lmms::SampleFrame* buf, const lmms::fpp_t frames) override;
    
    /**
     * @brief Save settings to XML
     * @param doc XML document
     * @param element XML element
     */
    void saveSettings(QDomDocument& doc, QDomElement& element) override;
    
    /**
     * @brief Load settings from XML
     * @param element XML element
     */
    void loadSettings(const QDomElement& element) override;
    
    /**
     * @brief Get node name
     * @return Node name
     */
    QString nodeName() const override;
    
protected:
    /**
     * @brief Create a view for the effect
     * @param parent Parent widget
     * @return Plugin view
     */
    lmms::gui::PluginView* instantiateView(QWidget* parent) override;
    
private:
    // Processing state
    bool m_processingEnabled;
};

} // namespace lmms_magenta

#endif // LMMS_MAGENTA_AI_EFFECT_H
