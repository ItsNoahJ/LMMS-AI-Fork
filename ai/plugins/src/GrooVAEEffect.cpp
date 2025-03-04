#include "GrooVAEEffect.h"
#include <iostream>
#include <QDomDocument>

namespace lmms_magenta {

GrooVAEEffect::GrooVAEEffect(Model* parent, const Plugin::Descriptor::SubPluginFeatures::Key* key)
    : AIEffect(parent, key)
    , m_temperature(1.0f)
    , m_humanize(0.5f)
    , m_swing(0.0f)
    , m_isProcessing(false) {
    
    // Load GrooVAE model
    loadModel(ModelType::GrooVAE, "");
    
    // Initialize groove presets
    m_groovePresets.resize(4);
}

GrooVAEEffect::~GrooVAEEffect() {
}

bool GrooVAEEffect::processAudioBuffer(sampleFrame* buffer, const fpp_t frames) {
    // This effect doesn't process audio directly
    // It processes MIDI data in the track
    return false;
}

bool GrooVAEEffect::handleMidiEvent(const MidiEvent& event, const MidiTime& time, f_cnt_t offset) {
    // Check if model is loaded
    if (!isModelLoaded()) {
        return false;
    }
    
    // Check if this is a note on event
    if (event.type() == MidiEvent::NoteOn) {
        // Get the note
        const int note = event.key();
        
        // Check if this is a groove preset trigger note (C2-B2)
        if (note >= 36 && note <= 47) {
            // Calculate preset index
            const int presetIndex = note - 36;
            
            // Check if preset exists
            if (presetIndex < static_cast<int>(m_groovePresets.size())) {
                // Set current preset
                m_currentPreset = presetIndex;
                
                // Apply the groove preset
                applyGroovePreset(m_groovePresets[m_currentPreset]);
                
                // Event handled
                return true;
            }
        }
    }
    
    // Event not handled
    return false;
}

void GrooVAEEffect::applyGroove() {
    // Check if model is loaded
    if (!isModelLoaded()) {
        std::cerr << "Model not loaded" << std::endl;
        return;
    }
    
    // Check if already processing
    if (m_isProcessing) {
        std::cerr << "Already processing" << std::endl;
        return;
    }
    
    // Set processing flag
    m_isProcessing = true;
    
    // Get the model
    auto model = std::dynamic_pointer_cast<GrooVAEModel>(getModel());
    if (!model) {
        std::cerr << "Failed to get GrooVAE model" << std::endl;
        m_isProcessing = false;
        return;
    }
    
    // Set model parameters
    model->setTemperature(m_temperature);
    model->setHumanize(m_humanize);
    
    // Get input notes from the track
    std::vector<MidiNote> inputNotes = getInputNotes();
    
    // Apply groove
    std::vector<MidiNote> outputNotes;
    if (!model->applyGroove(inputNotes, outputNotes)) {
        std::cerr << "Failed to apply groove" << std::endl;
        m_isProcessing = false;
        return;
    }
    
    // Apply swing
    if (m_swing != 0.0f) {
        applySwing(outputNotes);
    }
    
    // Update track with processed notes
    updateTrackNotes(outputNotes);
    
    // Reset processing flag
    m_isProcessing = false;
    
    // Notify UI that groove has been applied
    emit grooveApplied();
}

void GrooVAEEffect::extractGroove() {
    // Check if model is loaded
    if (!isModelLoaded()) {
        std::cerr << "Model not loaded" << std::endl;
        return;
    }
    
    // Check if already processing
    if (m_isProcessing) {
        std::cerr << "Already processing" << std::endl;
        return;
    }
    
    // Set processing flag
    m_isProcessing = true;
    
    // Get the model
    auto model = std::dynamic_pointer_cast<GrooVAEModel>(getModel());
    if (!model) {
        std::cerr << "Failed to get GrooVAE model" << std::endl;
        m_isProcessing = false;
        return;
    }
    
    // Get input notes from the track
    std::vector<MidiNote> inputNotes = getInputNotes();
    
    // Extract groove
    std::vector<float> groove;
    if (!model->extractGroove(inputNotes, groove)) {
        std::cerr << "Failed to extract groove" << std::endl;
        m_isProcessing = false;
        return;
    }
    
    // Store groove in current preset
    m_groovePresets[m_currentPreset] = groove;
    
    // Reset processing flag
    m_isProcessing = false;
    
    // Notify UI that groove has been extracted
    emit grooveExtracted(m_currentPreset);
}

void GrooVAEEffect::applyGroovePreset(const std::vector<float>& groove) {
    // Check if model is loaded
    if (!isModelLoaded()) {
        std::cerr << "Model not loaded" << std::endl;
        return;
    }
    
    // Check if already processing
    if (m_isProcessing) {
        std::cerr << "Already processing" << std::endl;
        return;
    }
    
    // Check if groove is valid
    if (groove.empty()) {
        std::cerr << "Empty groove preset" << std::endl;
        return;
    }
    
    // Set processing flag
    m_isProcessing = true;
    
    // Get the model
    auto model = std::dynamic_pointer_cast<GrooVAEModel>(getModel());
    if (!model) {
        std::cerr << "Failed to get GrooVAE model" << std::endl;
        m_isProcessing = false;
        return;
    }
    
    // Set model parameters
    model->setTemperature(m_temperature);
    model->setHumanize(m_humanize);
    
    // Get input notes from the track
    std::vector<MidiNote> inputNotes = getInputNotes();
    
    // Apply groove vector
    std::vector<MidiNote> outputNotes;
    if (!model->applyGrooveVector(inputNotes, groove, outputNotes)) {
        std::cerr << "Failed to apply groove vector" << std::endl;
        m_isProcessing = false;
        return;
    }
    
    // Apply swing
    if (m_swing != 0.0f) {
        applySwing(outputNotes);
    }
    
    // Update track with processed notes
    updateTrackNotes(outputNotes);
    
    // Reset processing flag
    m_isProcessing = false;
    
    // Notify UI that groove preset has been applied
    emit groovePresetApplied(m_currentPreset);
}

void GrooVAEEffect::setTemperature(float temperature) {
    m_temperature = temperature;
}

float GrooVAEEffect::getTemperature() const {
    return m_temperature;
}

void GrooVAEEffect::setHumanize(float humanize) {
    m_humanize = humanize;
}

float GrooVAEEffect::getHumanize() const {
    return m_humanize;
}

void GrooVAEEffect::setSwing(float swing) {
    m_swing = swing;
}

float GrooVAEEffect::getSwing() const {
    return m_swing;
}

void GrooVAEEffect::setCurrentPreset(int index) {
    if (index >= 0 && index < static_cast<int>(m_groovePresets.size())) {
        m_currentPreset = index;
    }
}

int GrooVAEEffect::getCurrentPreset() const {
    return m_currentPreset;
}

const std::vector<float>& GrooVAEEffect::getGroovePreset(int index) const {
    if (index >= 0 && index < static_cast<int>(m_groovePresets.size())) {
        return m_groovePresets[index];
    }
    
    // Return current preset if index is invalid
    return m_groovePresets[m_currentPreset];
}

void GrooVAEEffect::setGroovePreset(int index, const std::vector<float>& groove) {
    if (index >= 0 && index < static_cast<int>(m_groovePresets.size())) {
        m_groovePresets[index] = groove;
    }
}

bool GrooVAEEffect::isProcessing() const {
    return m_isProcessing;
}

void GrooVAEEffect::saveEffectSpecificSettings(QDomDocument& doc, QDomElement& element) {
    // Save parameters
    element.setAttribute("temperature", m_temperature);
    element.setAttribute("humanize", m_humanize);
    element.setAttribute("swing", m_swing);
    
    // Save current preset
    element.setAttribute("currentPreset", m_currentPreset);
    
    // Save groove presets
    QDomElement presetsElement = doc.createElement("groovePresets");
    element.appendChild(presetsElement);
    
    for (size_t i = 0; i < m_groovePresets.size(); ++i) {
        QDomElement presetElement = doc.createElement("preset");
        presetsElement.appendChild(presetElement);
        
        presetElement.setAttribute("index", static_cast<int>(i));
        
        // Save groove vector
        QString grooveStr;
        for (const auto& value : m_groovePresets[i]) {
            grooveStr += QString::number(value) + ",";
        }
        if (!grooveStr.isEmpty()) {
            grooveStr.chop(1); // Remove trailing comma
        }
        
        presetElement.setAttribute("groove", grooveStr);
    }
}

void GrooVAEEffect::loadEffectSpecificSettings(const QDomElement& element) {
    // Load parameters
    m_temperature = element.attribute("temperature", "1.0").toFloat();
    m_humanize = element.attribute("humanize", "0.5").toFloat();
    m_swing = element.attribute("swing", "0.0").toFloat();
    
    // Load current preset
    m_currentPreset = element.attribute("currentPreset", "0").toInt();
    
    // Load groove presets
    QDomElement presetsElement = element.firstChildElement("groovePresets");
    if (!presetsElement.isNull()) {
        QDomElement presetElement = presetsElement.firstChildElement("preset");
        while (!presetElement.isNull()) {
            int index = presetElement.attribute("index", "0").toInt();
            
            // Check if index is valid
            if (index >= 0 && index < static_cast<int>(m_groovePresets.size())) {
                // Load groove vector
                QString grooveStr = presetElement.attribute("groove", "");
                QStringList values = grooveStr.split(",", Qt::SkipEmptyParts);
                
                std::vector<float> groove;
                groove.reserve(values.size());
                
                for (const auto& value : values) {
                    groove.push_back(value.toFloat());
                }
                
                m_groovePresets[index] = groove;
            }
            
            presetElement = presetElement.nextSiblingElement("preset");
        }
    }
}

std::vector<MidiNote> GrooVAEEffect::getInputNotes() {
    // This is a placeholder for getting input notes from the track
    // In a real implementation, we would get the notes from the LMMS track
    
    // For now, just return some placeholder notes
    std::vector<MidiNote> notes;
    
    // Create a simple 4/4 beat
    for (int i = 0; i < 16; ++i) {
        MidiNote note;
        note.pitch = 36; // Kick drum
        note.velocity = 100;
        note.startTime = i * 0.25f;
        note.endTime = note.startTime + 0.1f;
        
        notes.push_back(note);
        
        // Add snare on beats 2 and 4
        if (i % 4 == 2) {
            MidiNote snare;
            snare.pitch = 38; // Snare drum
            snare.velocity = 100;
            snare.startTime = i * 0.25f;
            snare.endTime = snare.startTime + 0.1f;
            
            notes.push_back(snare);
        }
        
        // Add hi-hat on every 8th note
        MidiNote hihat;
        hihat.pitch = 42; // Closed hi-hat
        hihat.velocity = 80;
        hihat.startTime = i * 0.25f;
        hihat.endTime = hihat.startTime + 0.1f;
        
        notes.push_back(hihat);
    }
    
    return notes;
}

void GrooVAEEffect::updateTrackNotes(const std::vector<MidiNote>& notes) {
    // This is a placeholder for updating track notes
    // In a real implementation, we would update the LMMS track with the processed notes
    
    // For now, just log that we're updating the track
    std::cout << "Updating track with " << notes.size() << " processed notes" << std::endl;
}

void GrooVAEEffect::applySwing(std::vector<MidiNote>& notes) {
    // Apply swing to the notes
    // Swing affects the timing of even-numbered 8th notes
    
    // Calculate the swing amount in seconds (assuming 120 BPM)
    const float eighthNote = 0.25f; // Quarter note = 0.5s at 120 BPM
    const float swingAmount = eighthNote * m_swing;
    
    for (auto& note : notes) {
        // Calculate which 8th note this is
        const int eighthNoteIndex = static_cast<int>(note.startTime / eighthNote);
        
        // Apply swing to even-numbered 8th notes
        if (eighthNoteIndex % 2 == 1) {
            note.startTime += swingAmount;
            note.endTime += swingAmount;
        }
    }
}

} // namespace lmms_magenta
