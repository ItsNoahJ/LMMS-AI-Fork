#include "MidiUtils.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>

namespace lmms_magenta {

// Convert a MIDI sequence to a tensor representation for AI models
std::vector<float> MidiUtils::sequenceToTensor(const MidiSequence& sequence) {
    std::vector<float> tensor;
    
    // Reserve space for efficiency
    tensor.reserve(sequence.notes.size() * 5);  // 5 values per note
    
    // Normalize time values to 0-1 range
    float timeNormalizer = 1.0f / sequence.totalTicks;
    
    // For each note, add pitch, velocity, start time, duration, and is percussion
    for (const auto& note : sequence.notes) {
        // Normalize pitch to 0-1 range (MIDI pitch is 0-127)
        tensor.push_back(normalizeValue(note.pitch, 0, 127));
        
        // Normalize velocity to 0-1 range (MIDI velocity is 0-127)
        tensor.push_back(normalizeValue(note.velocity, 0, 127));
        
        // Normalize start time to 0-1 range
        tensor.push_back(note.startTime * timeNormalizer);
        
        // Normalize duration to 0-1 range
        tensor.push_back(note.duration * timeNormalizer);
        
        // Add is percussion as 0 or 1
        tensor.push_back(note.isPercussion ? 1.0f : 0.0f);
    }
    
    return tensor;
}

// Convert a tensor representation back to a MIDI sequence
MidiSequence MidiUtils::tensorToSequence(const std::vector<float>& tensor, 
                                        int ticksPerQuarter,
                                        int totalTicks) {
    MidiSequence sequence(ticksPerQuarter, totalTicks);
    
    // Calculate total ticks based on the maximum end time in the tensor
    float maxEndTime = 0.0f;
    
    // Process tensor in groups of 5 values (pitch, velocity, start time, duration, is percussion)
    for (size_t i = 0; i < tensor.size(); i += 5) {
        if (i + 4 >= tensor.size()) {
            // Incomplete note data, skip
            break;
        }
        
        // Extract note data from tensor
        float normalizedPitch = tensor[i];
        float normalizedVelocity = tensor[i + 1];
        float normalizedStartTime = tensor[i + 2];
        float normalizedDuration = tensor[i + 3];
        float isPercussion = tensor[i + 4];
        
        // Denormalize values
        int pitch = static_cast<int>(denormalizeValue(normalizedPitch, 0, 127));
        int velocity = static_cast<int>(denormalizeValue(normalizedVelocity, 0, 127));
        int startTime = static_cast<int>(normalizedStartTime * totalTicks);
        int duration = static_cast<int>(normalizedDuration * totalTicks);
        
        // Create and add note
        MidiNote note(
            pitch,
            velocity,
            startTime,
            duration,
            isPercussion > 0.5f
        );
        
        sequence.notes.push_back(note);
        
        // Update max end time
        float endTime = normalizedStartTime + normalizedDuration;
        maxEndTime = std::max(maxEndTime, endTime);
    }
    
    // Update total ticks if necessary
    if (maxEndTime > 1.0f) {
        sequence.totalTicks = static_cast<int>(maxEndTime * totalTicks);
    }
    
    return sequence;
}

// Quantize a MIDI sequence to a grid
MidiSequence MidiUtils::quantizeSequence(const MidiSequence& sequence, int gridSize) {
    MidiSequence result = sequence;
    
    for (auto& note : result.notes) {
        // Quantize start time
        note.startTime = findClosestGridPoint(note.startTime, gridSize);
        
        // Quantize duration, ensuring it's at least one grid unit
        note.duration = std::max(gridSize, findClosestGridPoint(note.duration, gridSize));
    }
    
    return result;
}

// Apply groove to a MIDI sequence
MidiSequence MidiUtils::applyGroove(const MidiSequence& sequence, const std::vector<float>& groove) {
    if (groove.empty()) {
        return sequence;
    }
    
    MidiSequence result = sequence;
    int grooveLength = static_cast<int>(groove.size());
    
    for (auto& note : result.notes) {
        // Calculate which groove step this note falls on
        int grooveStep = (note.startTime / (sequence.totalTicks / grooveLength)) % grooveLength;
        
        // Apply timing offset
        float offset = groove[grooveStep];
        note.startTime = static_cast<int>(note.startTime + offset * sequence.ticksPerQuarter);
        
        // Ensure start time is not negative
        note.startTime = std::max(0, note.startTime);
    }
    
    return result;
}

// Convert MIDI sequence to LMMS pattern
std::string MidiUtils::sequenceToLMMSPattern(const MidiSequence& sequence) {
    // Placeholder for actual implementation
    // This would convert the sequence to LMMS pattern XML format
    return "LMMS Pattern XML";
}

// Convert LMMS pattern to MIDI sequence
MidiSequence MidiUtils::lmmsPatternToSequence(const std::string& pattern) {
    // Placeholder for actual implementation
    // This would parse LMMS pattern XML and convert to MidiSequence
    return MidiSequence();
}

// Generate a random MIDI sequence
MidiSequence MidiUtils::generateRandomSequence(int numNotes, int totalTicks, 
                                             int minPitch, int maxPitch,
                                             bool isPercussion) {
    MidiSequence sequence(480, totalTicks);
    
    // Random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pitchDist(minPitch, maxPitch);
    std::uniform_int_distribution<> velocityDist(60, 100);
    std::uniform_int_distribution<> startTimeDist(0, totalTicks - 1);
    std::uniform_int_distribution<> durationDist(120, 480);
    
    // Generate random notes
    for (int i = 0; i < numNotes; ++i) {
        int pitch = pitchDist(gen);
        int velocity = velocityDist(gen);
        int startTime = startTimeDist(gen);
        int duration = durationDist(gen);
        
        // Ensure note doesn't extend beyond sequence length
        if (startTime + duration > totalTicks) {
            duration = totalTicks - startTime;
        }
        
        // Create and add note
        MidiNote note(
            pitch,
            velocity,
            startTime,
            duration,
            isPercussion
        );
        
        sequence.notes.push_back(note);
    }
    
    // Sort notes by start time
    std::sort(sequence.notes.begin(), sequence.notes.end(), 
              [](const MidiNote& a, const MidiNote& b) {
                  return a.startTime < b.startTime;
              });
    
    return sequence;
}

// Calculate the similarity between two MIDI sequences
float MidiUtils::calculateSequenceSimilarity(const MidiSequence& sequence1, 
                                           const MidiSequence& sequence2) {
    // Extract features from both sequences
    std::vector<float> features1 = extractSequenceFeatures(sequence1);
    std::vector<float> features2 = extractSequenceFeatures(sequence2);
    
    // Calculate cosine similarity
    float dotProduct = 0.0f;
    float norm1 = 0.0f;
    float norm2 = 0.0f;
    
    for (size_t i = 0; i < features1.size(); ++i) {
        dotProduct += features1[i] * features2[i];
        norm1 += features1[i] * features1[i];
        norm2 += features2[i] * features2[i];
    }
    
    return dotProduct / (std::sqrt(norm1) * std::sqrt(norm2));
}

// Merge two MIDI sequences
MidiSequence MidiUtils::mergeSequences(const MidiSequence& sequence1, 
                                      const MidiSequence& sequence2,
                                      float weight1) {
    // Ensure weight is between 0 and 1
    weight1 = std::max(0.0f, std::min(1.0f, weight1));
    float weight2 = 1.0f - weight1;
    
    // Create result sequence
    MidiSequence result;
    
    // Set properties based on weighted average
    result.ticksPerQuarter = sequence1.ticksPerQuarter; // Use first sequence's ticks per quarter
    result.totalTicks = std::max(sequence1.totalTicks, sequence2.totalTicks);
    
    // Add notes from first sequence
    for (const auto& note : sequence1.notes) {
        // Scale velocity by weight
        MidiNote weightedNote = note;
        weightedNote.velocity = static_cast<int>(note.velocity * weight1);
        
        // Only add if velocity is above threshold
        if (weightedNote.velocity > 10) {
            result.notes.push_back(weightedNote);
        }
    }
    
    // Add notes from second sequence
    for (const auto& note : sequence2.notes) {
        // Scale velocity by weight
        MidiNote weightedNote = note;
        weightedNote.velocity = static_cast<int>(note.velocity * weight2);
        
        // Only add if velocity is above threshold
        if (weightedNote.velocity > 10) {
            result.notes.push_back(weightedNote);
        }
    }
    
    // Sort notes by start time
    std::sort(result.notes.begin(), result.notes.end(), 
              [](const MidiNote& a, const MidiNote& b) {
                  return a.startTime < b.startTime;
              });
    
    return result;
}

// Extract features from a MIDI sequence
std::vector<float> MidiUtils::extractSequenceFeatures(const MidiSequence& sequence) {
    std::vector<float> features;
    
    // Reserve space for features
    features.reserve(128 + 16 + 16 + 1); // Pitch histogram + rhythm histogram + velocity histogram + note density
    
    // Initialize histograms
    std::vector<int> pitchHistogram(128, 0);
    std::vector<int> rhythmHistogram(16, 0);
    std::vector<int> velocityHistogram(16, 0);
    
    // Calculate histograms
    for (const auto& note : sequence.notes) {
        // Pitch histogram
        pitchHistogram[note.pitch]++;
        
        // Rhythm histogram (quantize to 16 bins)
        int rhythmBin = (note.startTime * 16) / sequence.totalTicks;
        rhythmBin = std::min(15, std::max(0, rhythmBin));
        rhythmHistogram[rhythmBin]++;
        
        // Velocity histogram (quantize to 16 bins)
        int velocityBin = (note.velocity * 16) / 128;
        velocityBin = std::min(15, std::max(0, velocityBin));
        velocityHistogram[velocityBin]++;
    }
    
    // Normalize histograms
    float totalNotes = static_cast<float>(sequence.notes.size());
    
    // Add pitch histogram to features
    for (int count : pitchHistogram) {
        features.push_back(count / totalNotes);
    }
    
    // Add rhythm histogram to features
    for (int count : rhythmHistogram) {
        features.push_back(count / totalNotes);
    }
    
    // Add velocity histogram to features
    for (int count : velocityHistogram) {
        features.push_back(count / totalNotes);
    }
    
    // Add note density (notes per tick)
    features.push_back(totalNotes / sequence.totalTicks);
    
    return features;
}

// Find closest grid point
int MidiUtils::findClosestGridPoint(int time, int gridSize) {
    int remainder = time % gridSize;
    
    if (remainder < gridSize / 2) {
        // Round down
        return time - remainder;
    } else {
        // Round up
        return time + (gridSize - remainder);
    }
}

// Normalize value to 0-1 range
float MidiUtils::normalizeValue(float value, float min, float max) {
    if (max == min) return 0.5f;
    return (value - min) / (max - min);
}

// Denormalize value from 0-1 range
float MidiUtils::denormalizeValue(float value, float min, float max) {
    return min + value * (max - min);
}

} // namespace lmms_magenta
