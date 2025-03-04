#include "MusicVAEInstrument.h"
#include <iostream>
#include <QDomDocument>

namespace lmms_magenta {

MusicVAEInstrument::MusicVAEInstrument(InstrumentTrack* track, const Plugin::Descriptor::SubPluginFeatures::Key* key)
    : AIInstrument(track, key)
    , m_temperature(1.0f)
    , m_patternLength(16)
    , m_currentPattern(0)
    , m_isGenerating(false) {
    
    // Load MusicVAE model
    loadModel(ModelType::MusicVAE, "");
    
    // Initialize patterns
    m_patterns.resize(4);
}

MusicVAEInstrument::~MusicVAEInstrument() {
}

void MusicVAEInstrument::playNote(NotePlayHandle* nph, sampleFrame* workingBuffer) {
    // Get the note
    const int note = nph->key();
    
    // Check if this is a pattern trigger note (C3-B3)
    if (note >= 48 && note <= 59) {
        // Calculate pattern index
        const int patternIndex = note - 48;
        
        // Check if pattern exists
        if (patternIndex < static_cast<int>(m_patterns.size())) {
            // Set current pattern
            m_currentPattern = patternIndex;
            
            // Play the pattern
            playPattern(m_patterns[m_currentPattern]);
        }
    }
}

void MusicVAEInstrument::deleteNotePluginData(NotePlayHandle* nph) {
    // Nothing to clean up
}

bool MusicVAEInstrument::handleMidiEvent(const MidiEvent& event, const MidiTime& time, f_cnt_t offset) {
    // Check if this is a note on event
    if (event.type() == MidiEvent::NoteOn) {
        // Get the note
        const int note = event.key();
        
        // Check if this is a pattern trigger note (C3-B3)
        if (note >= 48 && note <= 59) {
            // Calculate pattern index
            const int patternIndex = note - 48;
            
            // Check if pattern exists
            if (patternIndex < static_cast<int>(m_patterns.size())) {
                // Set current pattern
                m_currentPattern = patternIndex;
                
                // Play the pattern
                playPattern(m_patterns[m_currentPattern]);
                
                // Event handled
                return true;
            }
        }
    }
    
    // Event not handled
    return false;
}

void MusicVAEInstrument::generatePattern() {
    // Check if model is loaded
    if (!isModelLoaded()) {
        std::cerr << "Model not loaded" << std::endl;
        return;
    }
    
    // Check if already generating
    if (m_isGenerating) {
        std::cerr << "Already generating pattern" << std::endl;
        return;
    }
    
    // Set generating flag
    m_isGenerating = true;
    
    // Get the model
    auto model = std::dynamic_pointer_cast<MusicVAEModel>(getModel());
    if (!model) {
        std::cerr << "Failed to get MusicVAE model" << std::endl;
        m_isGenerating = false;
        return;
    }
    
    // Set temperature
    model->setTemperature(m_temperature);
    
    // Generate pattern
    std::vector<MidiNote> notes;
    if (!model->sample(notes)) {
        std::cerr << "Failed to generate pattern" << std::endl;
        m_isGenerating = false;
        return;
    }
    
    // Store pattern
    m_patterns[m_currentPattern] = notes;
    
    // Reset generating flag
    m_isGenerating = false;
    
    // Notify UI that pattern has been generated
    emit patternGenerated(m_currentPattern);
}

void MusicVAEInstrument::interpolatePatterns(int startPatternIndex, int endPatternIndex, int steps) {
    // Check if model is loaded
    if (!isModelLoaded()) {
        std::cerr << "Model not loaded" << std::endl;
        return;
    }
    
    // Check if already generating
    if (m_isGenerating) {
        std::cerr << "Already generating pattern" << std::endl;
        return;
    }
    
    // Check if pattern indices are valid
    if (startPatternIndex < 0 || startPatternIndex >= static_cast<int>(m_patterns.size()) ||
        endPatternIndex < 0 || endPatternIndex >= static_cast<int>(m_patterns.size())) {
        std::cerr << "Invalid pattern indices" << std::endl;
        return;
    }
    
    // Set generating flag
    m_isGenerating = true;
    
    // Get the model
    auto model = std::dynamic_pointer_cast<MusicVAEModel>(getModel());
    if (!model) {
        std::cerr << "Failed to get MusicVAE model" << std::endl;
        m_isGenerating = false;
        return;
    }
    
    // Set temperature
    model->setTemperature(m_temperature);
    
    // Get start and end patterns
    const auto& startPattern = m_patterns[startPatternIndex];
    const auto& endPattern = m_patterns[endPatternIndex];
    
    // Interpolate patterns
    std::vector<std::vector<MidiNote>> interpolatedPatterns;
    if (!model->interpolate(startPattern, endPattern, steps, interpolatedPatterns)) {
        std::cerr << "Failed to interpolate patterns" << std::endl;
        m_isGenerating = false;
        return;
    }
    
    // Store interpolated patterns
    // For now, just store the first and last interpolated patterns
    if (interpolatedPatterns.size() >= 2) {
        m_patterns[startPatternIndex] = interpolatedPatterns.front();
        m_patterns[endPatternIndex] = interpolatedPatterns.back();
    }
    
    // Reset generating flag
    m_isGenerating = false;
    
    // Notify UI that patterns have been interpolated
    emit patternsInterpolated(startPatternIndex, endPatternIndex);
}

void MusicVAEInstrument::setTemperature(float temperature) {
    m_temperature = temperature;
}

float MusicVAEInstrument::getTemperature() const {
    return m_temperature;
}

void MusicVAEInstrument::setPatternLength(int length) {
    m_patternLength = length;
}

int MusicVAEInstrument::getPatternLength() const {
    return m_patternLength;
}

void MusicVAEInstrument::setCurrentPattern(int index) {
    if (index >= 0 && index < static_cast<int>(m_patterns.size())) {
        m_currentPattern = index;
    }
}

int MusicVAEInstrument::getCurrentPattern() const {
    return m_currentPattern;
}

const std::vector<MidiNote>& MusicVAEInstrument::getPattern(int index) const {
    if (index >= 0 && index < static_cast<int>(m_patterns.size())) {
        return m_patterns[index];
    }
    
    // Return current pattern if index is invalid
    return m_patterns[m_currentPattern];
}

void MusicVAEInstrument::setPattern(int index, const std::vector<MidiNote>& pattern) {
    if (index >= 0 && index < static_cast<int>(m_patterns.size())) {
        m_patterns[index] = pattern;
    }
}

bool MusicVAEInstrument::isGenerating() const {
    return m_isGenerating;
}

void MusicVAEInstrument::saveInstrumentSpecificSettings(QDomDocument& doc, QDomElement& element) {
    // Save temperature
    element.setAttribute("temperature", m_temperature);
    
    // Save pattern length
    element.setAttribute("patternLength", m_patternLength);
    
    // Save current pattern
    element.setAttribute("currentPattern", m_currentPattern);
    
    // Save patterns
    QDomElement patternsElement = doc.createElement("patterns");
    element.appendChild(patternsElement);
    
    for (size_t i = 0; i < m_patterns.size(); ++i) {
        QDomElement patternElement = doc.createElement("pattern");
        patternsElement.appendChild(patternElement);
        
        patternElement.setAttribute("index", static_cast<int>(i));
        
        // Save notes
        for (const auto& note : m_patterns[i]) {
            QDomElement noteElement = doc.createElement("note");
            patternElement.appendChild(noteElement);
            
            noteElement.setAttribute("pitch", note.pitch);
            noteElement.setAttribute("velocity", note.velocity);
            noteElement.setAttribute("startTime", note.startTime);
            noteElement.setAttribute("endTime", note.endTime);
        }
    }
}

void MusicVAEInstrument::loadInstrumentSpecificSettings(const QDomElement& element) {
    // Load temperature
    m_temperature = element.attribute("temperature", "1.0").toFloat();
    
    // Load pattern length
    m_patternLength = element.attribute("patternLength", "16").toInt();
    
    // Load current pattern
    m_currentPattern = element.attribute("currentPattern", "0").toInt();
    
    // Load patterns
    QDomElement patternsElement = element.firstChildElement("patterns");
    if (!patternsElement.isNull()) {
        QDomElement patternElement = patternsElement.firstChildElement("pattern");
        while (!patternElement.isNull()) {
            int index = patternElement.attribute("index", "0").toInt();
            
            // Check if index is valid
            if (index >= 0 && index < static_cast<int>(m_patterns.size())) {
                // Clear pattern
                m_patterns[index].clear();
                
                // Load notes
                QDomElement noteElement = patternElement.firstChildElement("note");
                while (!noteElement.isNull()) {
                    MidiNote note;
                    note.pitch = noteElement.attribute("pitch", "60").toInt();
                    note.velocity = noteElement.attribute("velocity", "64").toInt();
                    note.startTime = noteElement.attribute("startTime", "0.0").toFloat();
                    note.endTime = noteElement.attribute("endTime", "0.5").toFloat();
                    
                    m_patterns[index].push_back(note);
                    
                    noteElement = noteElement.nextSiblingElement("note");
                }
            }
            
            patternElement = patternElement.nextSiblingElement("pattern");
        }
    }
}

void MusicVAEInstrument::playPattern(const std::vector<MidiNote>& pattern) {
    // This is a placeholder for actual pattern playback
    // In a real implementation, we would convert the pattern to LMMS notes
    // and add them to the track
    
    // For now, just log that we're playing the pattern
    std::cout << "Playing pattern with " << pattern.size() << " notes" << std::endl;
    
    // Notify UI that pattern is being played
    emit patternPlayed(m_currentPattern);
}

} // namespace lmms_magenta
