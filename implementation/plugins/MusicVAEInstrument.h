#ifndef LMMS_MAGENTA_MUSIC_VAE_INSTRUMENT_H
#define LMMS_MAGENTA_MUSIC_VAE_INSTRUMENT_H

#include "AIInstrument.h"
#include <memory>
#include <vector>

namespace lmms_magenta {

/**
 * @brief MusicVAE instrument plugin
 * 
 * This instrument uses MusicVAE to generate musical patterns
 * based on latent space interpolation.
 */
class MusicVAEInstrument : public AIInstrument {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param track Parent instrument track
     * @param descriptor Plugin descriptor
     */
    MusicVAEInstrument(lmms::InstrumentTrack* track, const lmms::Plugin::Descriptor* descriptor);
    
    /**
     * @brief Destructor
     */
    ~MusicVAEInstrument() override;
    
    /**
     * @brief Initialize the instrument
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
     * @brief Play a note
     * @param n Note play handle
     * @param workingBuffer Working buffer for audio output
     */
    void playNote(lmms::NotePlayHandle* n, lmms::SampleFrame* workingBuffer) override;
    
    /**
     * @brief Delete note plugin data
     * @param n Note play handle
     */
    void deleteNotePluginData(lmms::NotePlayHandle* n) override;
    
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
    
    /**
     * @brief Generate a new pattern
     * @param temperature Temperature for generation (randomness)
     * @return True if generation was successful
     */
    bool generatePattern(float temperature = 1.0f);
    
    /**
     * @brief Interpolate between two patterns
     * @param startPattern Start pattern
     * @param endPattern End pattern
     * @param steps Number of interpolation steps
     * @param temperature Temperature for generation (randomness)
     * @return True if interpolation was successful
     */
    bool interpolatePatterns(const std::vector<float>& startPattern, 
                            const std::vector<float>& endPattern,
                            int steps,
                            float temperature = 1.0f);
    
    /**
     * @brief Get the current pattern
     * @return Current pattern
     */
    const std::vector<float>& getCurrentPattern() const;
    
    /**
     * @brief Set the current pattern
     * @param pattern Pattern to set
     */
    void setCurrentPattern(const std::vector<float>& pattern);
    
protected:
    /**
     * @brief Create a view for the instrument
     * @param parent Parent widget
     * @return Plugin view
     */
    lmms::gui::PluginView* instantiateView(QWidget* parent) override;
    
private:
    // Note data structure
    struct MusicVAENoteData : public NoteData {
        // Pattern position
        int patternPosition;
        
        // Note velocity
        float velocity;
        
        // Note active
        bool active;
        
        // Constructor
        MusicVAENoteData()
            : patternPosition(0)
            , velocity(1.0f)
            , active(true) {
        }
    };
    
    // Current pattern
    std::vector<float> m_currentPattern;
    
    // Interpolation patterns
    std::vector<std::vector<float>> m_interpolationPatterns;
    
    // Interpolation position
    int m_interpolationPosition;
    
    // Generation temperature
    float m_temperature;
    
    // Style parameters
    float m_complexity;
    float m_density;
    float m_rhythmicVariation;
    float m_harmonicComplexity;
    
    // Convert pattern to MIDI notes
    void patternToNotes();
    
    // Convert MIDI notes to pattern
    void notesToPattern();
};

} // namespace lmms_magenta

#endif // LMMS_MAGENTA_MUSIC_VAE_INSTRUMENT_H
