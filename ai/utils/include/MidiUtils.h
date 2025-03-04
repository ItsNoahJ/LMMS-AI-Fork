#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace lmms_magenta {

/**
 * @brief Structure representing a MIDI note
 */
struct MidiNote {
    int pitch;          // MIDI pitch (0-127)
    int velocity;       // MIDI velocity (0-127)
    int startTime;      // Start time in ticks
    int duration;       // Duration in ticks
    bool isPercussion;  // Whether this is a percussion note
    
    // Constructor
    MidiNote(int p = 60, int v = 64, int s = 0, int d = 480, bool perc = false)
        : pitch(p), velocity(v), startTime(s), duration(d), isPercussion(perc) {}
};

/**
 * @brief Structure representing a MIDI sequence
 */
struct MidiSequence {
    std::vector<MidiNote> notes;  // Notes in the sequence
    int ticksPerQuarter;          // Ticks per quarter note
    int totalTicks;               // Total length in ticks
    int timeSignatureNumerator;   // Time signature numerator
    int timeSignatureDenominator; // Time signature denominator
    
    // Constructor
    MidiSequence(int tpq = 480, int tt = 1920, int tsn = 4, int tsd = 4)
        : ticksPerQuarter(tpq), totalTicks(tt), 
          timeSignatureNumerator(tsn), timeSignatureDenominator(tsd) {}
};

/**
 * @brief Utility class for MIDI operations
 */
class MidiUtils {
public:
    /**
     * @brief Convert a MIDI sequence to a tensor representation for AI models
     * @param sequence MIDI sequence to convert
     * @return Vector of float values representing the sequence
     */
    static std::vector<float> sequenceToTensor(const MidiSequence& sequence);
    
    /**
     * @brief Convert a tensor representation to a MIDI sequence
     * @param tensor Vector of float values representing the sequence
     * @param ticksPerQuarter Ticks per quarter note
     * @param totalTicks Total length in ticks
     * @return Converted MIDI sequence
     */
    static MidiSequence tensorToSequence(const std::vector<float>& tensor, 
                                        int ticksPerQuarter = 480,
                                        int totalTicks = 1920);
    
    /**
     * @brief Load a MIDI file into a MidiSequence
     * @param filePath Path to the MIDI file
     * @return Loaded MIDI sequence
     */
    static MidiSequence loadMidiFile(const std::string& filePath);
    
    /**
     * @brief Save a MidiSequence to a MIDI file
     * @param sequence MIDI sequence to save
     * @param filePath Path to save the MIDI file
     * @return True if saving was successful
     */
    static bool saveMidiFile(const MidiSequence& sequence, const std::string& filePath);
    
    /**
     * @brief Quantize a MIDI sequence to a grid
     * @param sequence MIDI sequence to quantize
     * @param gridSize Grid size in ticks
     * @return Quantized MIDI sequence
     */
    static MidiSequence quantizeSequence(const MidiSequence& sequence, int gridSize);
    
    /**
     * @brief Transpose a MIDI sequence
     * @param sequence MIDI sequence to transpose
     * @param semitones Number of semitones to transpose by
     * @return Transposed MIDI sequence
     */
    static MidiSequence transposeSequence(const MidiSequence& sequence, int semitones);
    
    /**
     * @brief Extract a subsequence from a MIDI sequence
     * @param sequence Source MIDI sequence
     * @param startTick Start tick of the subsequence
     * @param endTick End tick of the subsequence
     * @return Extracted subsequence
     */
    static MidiSequence extractSubsequence(const MidiSequence& sequence, 
                                          int startTick, 
                                          int endTick);
};

} // namespace lmms_magenta
