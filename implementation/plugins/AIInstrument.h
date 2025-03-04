#ifndef LMMS_MAGENTA_AI_INSTRUMENT_H
#define LMMS_MAGENTA_AI_INSTRUMENT_H

#include "AIPlugin.h"
#include "Instrument.h"

namespace lmms_magenta {

/**
 * @brief Base class for AI instrument plugins
 * 
 * This class extends the LMMS Instrument class and the AIPlugin class
 * to provide a base for AI-powered instrument plugins.
 */
class AIInstrument : public lmms::Instrument, public AIPlugin {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param track Parent instrument track
     * @param descriptor Plugin descriptor
     */
    AIInstrument(lmms::InstrumentTrack* track, const lmms::Plugin::Descriptor* descriptor);
    
    /**
     * @brief Destructor
     */
    ~AIInstrument() override;
    
    /**
     * @brief Initialize the instrument
     * @return True if initialization was successful
     */
    bool initialize();
    
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
    
protected:
    /**
     * @brief Create a view for the instrument
     * @param parent Parent widget
     * @return Plugin view
     */
    lmms::gui::PluginView* instantiateView(QWidget* parent) override;
    
private:
    // Note data structure
    struct NoteData {
        // Add note-specific data here
    };
    
    // Map of note data
    std::map<lmms::NotePlayHandle*, NoteData*> m_noteData;
};

} // namespace lmms_magenta

#endif // LMMS_MAGENTA_AI_INSTRUMENT_H
