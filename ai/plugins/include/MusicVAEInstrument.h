#pragma once

#include "AIInstrument.h"
#include "../../model_serving/include/MusicVAEModel.h"
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
     */
    void playNote(lmms::NotePlayHandle* n) override;
    
    /**
     * @brief Generate a pattern
     * @param length Length of the pattern in ticks
     * @return Generated pattern
     */
    lmms::Pattern* generatePattern(int length) override;
    
    /**
     * @brief Generate a pattern from a seed
     * @param seed Seed pattern
     * @param temperature Temperature for sampling (randomness)
     * @return Generated pattern
     */
    lmms::Pattern* generatePatternFromSeed(lmms::Pattern* seed, float temperature = 1.0f);
    
    /**
     * @brief Interpolate between two patterns
     * @param pattern1 First pattern
     * @param pattern2 Second pattern
     * @param numSteps Number of interpolation steps
     * @param temperature Temperature for sampling (randomness)
     * @return Vector of interpolated patterns
     */
    std::vector<lmms::Pattern*> interpolatePatterns(lmms::Pattern* pattern1, 
                                                  lmms::Pattern* pattern2,
                                                  int numSteps,
                                                  float temperature = 1.0f);
    
    /**
     * @brief Get the MusicVAE model
     * @return Shared pointer to the MusicVAE model
     */
    std::shared_ptr<MusicVAEModel> getMusicVAEModel();
    
protected:
    /**
     * @brief Handle note on event
     * @param note Note number
     * @param velocity Velocity
     * @return True if the event was handled
     */
    bool handleNoteOn(int note, int velocity) override;
    
    /**
     * @brief Handle note off event
     * @param note Note number
     * @return True if the event was handled
     */
    bool handleNoteOff(int note) override;
    
    /**
     * @brief Handle MIDI event
     * @param event MIDI event
     * @return True if the event was handled
     */
    bool handleMidiEvent(const lmms::MidiEvent& event) override;
    
private:
    // Current latent vector
    std::vector<float> m_currentLatentVector;
    
    // Temperature for sampling
    float m_temperature;
    
    // Whether to use the seed pattern
    bool m_useSeedPattern;
    
    // Seed pattern
    lmms::Pattern* m_seedPattern;
    
    // Convert LMMS pattern to MIDI sequence
    MidiSequence patternToSequence(lmms::Pattern* pattern);
    
    // Convert MIDI sequence to LMMS pattern
    lmms::Pattern* sequenceToPattern(const MidiSequence& sequence);
};

} // namespace lmms_magenta
