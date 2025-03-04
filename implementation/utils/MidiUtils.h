#ifndef LMMS_MAGENTA_MIDI_UTILS_H
#define LMMS_MAGENTA_MIDI_UTILS_H

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
     * @brief Convert a tensor representation back to a MIDI sequence
     * @param tensor Vector of float values representing the sequence
     * @param ticksPerQuarter Ticks per quarter note
     * @return MIDI sequence
     */
    static MidiSequence tensorToSequence(const std::vector<float>& tensor, int ticksPerQuarter = 480);
    
    /**
     * @brief Quantize a MIDI sequence to a grid
     * @param sequence MIDI sequence to quantize
     * @param gridSize Grid size in ticks
     * @return Quantized MIDI sequence
     */
    static MidiSequence quantizeSequence(const MidiSequence& sequence, int gridSize);
    
    /**
     * @brief Apply groove to a MIDI sequence
     * @param sequence MIDI sequence to groove
     * @param groove Vector of timing and velocity adjustments
     * @return Grooved MIDI sequence
     */
    static MidiSequence applyGroove(const MidiSequence& sequence, const std::vector<float>& groove);
    
    /**
     * @brief Convert MIDI sequence to LMMS pattern
     * @param sequence MIDI sequence to convert
     * @return Serialized LMMS pattern data
     */
    static std::string sequenceToLMMSPattern(const MidiSequence& sequence);
    
    /**
     * @brief Convert LMMS pattern to MIDI sequence
     * @param pattern Serialized LMMS pattern data
     * @return MIDI sequence
     */
    static MidiSequence lmmsPatternToSequence(const std::string& pattern);
    
    /**
     * @brief Generate a random MIDI sequence
     * @param numNotes Number of notes to generate
     * @param totalTicks Total length in ticks
     * @param minPitch Minimum pitch
     * @param maxPitch Maximum pitch
     * @param isPercussion Whether to generate percussion notes
     * @return Random MIDI sequence
     */
    static MidiSequence generateRandomSequence(int numNotes, int totalTicks, 
                                              int minPitch = 36, int maxPitch = 84,
                                              bool isPercussion = false);
    
    /**
     * @brief Calculate the similarity between two MIDI sequences
     * @param sequence1 First MIDI sequence
     * @param sequence2 Second MIDI sequence
     * @return Similarity score between 0.0 and 1.0
     */
    static float calculateSequenceSimilarity(const MidiSequence& sequence1, 
                                           const MidiSequence& sequence2);
    
    /**
     * @brief Merge two MIDI sequences
     * @param sequence1 First MIDI sequence
     * @param sequence2 Second MIDI sequence
     * @param weight1 Weight of the first sequence (0.0-1.0)
     * @return Merged MIDI sequence
     */
    static MidiSequence mergeSequences(const MidiSequence& sequence1, 
                                      const MidiSequence& sequence2,
                                      float weight1 = 0.5f);
    
    /**
     * @brief Extract features from a MIDI sequence
     * @param sequence MIDI sequence to analyze
     * @return Vector of feature values
     */
    static std::vector<float> extractSequenceFeatures(const MidiSequence& sequence);
    
private:
    // Private helper methods
    static int findClosestGridPoint(int time, int gridSize);
    static float normalizeValue(float value, float min, float max);
    static float denormalizeValue(float value, float min, float max);
};

} // namespace lmms_magenta

#endif // LMMS_MAGENTA_MIDI_UTILS_H
