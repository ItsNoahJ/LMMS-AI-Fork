#include "AIInstrument.h"
#include <iostream>

namespace lmms_magenta {

// Constructor
AIInstrument::AIInstrument(lmms::InstrumentTrack* track, const lmms::Plugin::Descriptor* descriptor)
    : lmms::Instrument(track, descriptor)
    , AIPlugin(track, descriptor) {
}

// Destructor
AIInstrument::~AIInstrument() {
    // Clean up note data
    for (auto& pair : m_noteData) {
        delete pair.second;
    }
    m_noteData.clear();
}

// Initialize the instrument
bool AIInstrument::initialize() {
    // Initialize AIPlugin
    if (!AIPlugin::initialize()) {
        return false;
    }
    
    // Load model
    if (!loadModel()) {
        std::cerr << "Failed to load model for " << nodeName().toStdString() << std::endl;
        // Continue anyway, as the model might be loaded later
    }
    
    return true;
}

// Play a note
void AIInstrument::playNote(lmms::NotePlayHandle* n, lmms::SampleFrame* workingBuffer) {
    // Check if model is loaded
    if (!isModelLoaded()) {
        // Model not loaded, output silence
        return;
    }
    
    // Check if we have note data for this note
    auto it = m_noteData.find(n);
    if (it == m_noteData.end()) {
        // Create new note data
        NoteData* data = new NoteData();
        m_noteData[n] = data;
    }
    
    // Get note data
    NoteData* data = m_noteData[n];
    
    // TODO: Implement note playback using AI model
    // This will be implemented in derived classes
}

// Delete note plugin data
void AIInstrument::deleteNotePluginData(lmms::NotePlayHandle* n) {
    // Check if we have note data for this note
    auto it = m_noteData.find(n);
    if (it != m_noteData.end()) {
        // Delete note data
        delete it->second;
        m_noteData.erase(it);
    }
}

// Save settings to XML
void AIInstrument::saveSettings(QDomDocument& doc, QDomElement& element) {
    // Save AIPlugin settings
    element.setAttribute("model_type", static_cast<int>(getModelType()));
    element.setAttribute("model_name", QString::fromStdString(getModelName()));
    
    // Save instrument-specific settings
    // This will be implemented in derived classes
}

// Load settings from XML
void AIInstrument::loadSettings(const QDomElement& element) {
    // Load instrument-specific settings
    // This will be implemented in derived classes
}

// Get node name
QString AIInstrument::nodeName() const {
    // This will be overridden in derived classes
    return "ai_instrument";
}

// Create a view for the instrument
lmms::gui::PluginView* AIInstrument::instantiateView(QWidget* parent) {
    // This will be implemented in derived classes
    return nullptr;
}

} // namespace lmms_magenta
