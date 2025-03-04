#pragma once

#include "AIPlugin.h"
#include "InstrumentTrack.h"

namespace lmms_magenta {

/**
 * @brief Base class for AI-powered instruments
 * 
 * This class extends the AIPlugin class and provides common functionality
 * for AI-powered instruments, such as note handling and pattern generation.
 */
class AIInstrument : public AIPlugin {
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
    bool initialize() override;
    
    /**
     * @brief Play a note
     * @param n Note play handle
     */
    void playNote(lmms::NotePlayHandle* n) override;
    
    /**
     * @brief Generate a pattern
     * @param length Length of the pattern in ticks
     * @return Generated pattern
     */
    virtual lmms::Pattern* generatePattern(int length);
    
    /**
     * @brief Get the instrument track
     * @return Pointer to the instrument track
     */
    lmms::InstrumentTrack* instrumentTrack();
    
protected:
    /**
     * @brief Handle note on event
     * @param note Note number
     * @param velocity Velocity
     * @return True if the event was handled
     */
    virtual bool handleNoteOn(int note, int velocity);
    
    /**
     * @brief Handle note off event
     * @param note Note number
     * @return True if the event was handled
     */
    virtual bool handleNoteOff(int note);
    
    /**
     * @brief Handle MIDI event
     * @param event MIDI event
     * @return True if the event was handled
     */
    virtual bool handleMidiEvent(const lmms::MidiEvent& event);
};

} // namespace lmms_magenta
