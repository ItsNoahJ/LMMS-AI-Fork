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
MidiSequence MidiUtils::tensorToSequence(const std::vector<float>& tensor, int ticksPerQuarter) {
    MidiSequence sequence(ticksPerQuarter);
    
    // Calculate total ticks based on the maximum end time in the tensor
    float maxEndTime = 0.0f;
    
    // Process tensor in groups of 5 values (pitch, velocity, start time, duration, is percussion)
    for (size_t i = 0; i < tensor.size(); i += 5) {
        if (i + 4 >= tensor.size()) {
            // Incomplete note data, skip
            break;
        }
        
        // Denormalize pitch (0-1 to 0-127)
        int pitch = static_cast<int>(std::round(denormalizeValue(tensor[i], 0, 127)));
        
        // Denormalize velocity (0-1 to 0-127)
        int velocity = static_cast<int>(std::round(denormalizeValue(tensor[i + 1], 0, 127)));
        
        // Start time and duration are normalized to 0-1, will be denormalized after total ticks is determined
        float normalizedStartTime = tensor[i + 2];
        float normalizedDuration = tensor[i + 3];
        
        // Is percussion (0 or 1)
        bool isPercussion = tensor[i + 4] > 0.5f;
        
        // Track maximum end time
        float endTime = normalizedStartTime + normalizedDuration;
        maxEndTime = std::max(maxEndTime, endTime);
        
        // Add note to sequence (temporarily with normalized times)
        MidiNote note(pitch, velocity, 0, 0, isPercussion);
        sequence.notes.push_back(note);
    }
    
    // Set total ticks based on maximum end time
    sequence.totalTicks = static_cast<int>(std::ceil(maxEndTime * 4 * ticksPerQuarter));
    
    // Denormalize start times and durations
    for (size_t i = 0; i < sequence.notes.size(); ++i) {
        size_t tensorIndex = i * 5;
        
        // Denormalize start time
        sequence.notes[i].startTime = static_cast<int>(std::round(tensor[tensorIndex + 2] * sequence.totalTicks));
        
        // Denormalize duration
        sequence.notes[i].duration = static_cast<int>(std::round(tensor[tensorIndex + 3] * sequence.totalTicks));
        
        // Ensure minimum duration
        sequence.notes[i].duration = std::max(sequence.notes[i].duration, 1);
    }
    
    return sequence;
}

// Quantize a MIDI sequence to a grid
MidiSequence MidiUtils::quantizeSequence(const MidiSequence& sequence, int gridSize) {
    MidiSequence quantized = sequence;
    
    for (auto& note : quantized.notes) {
        // Quantize start time
        note.startTime = findClosestGridPoint(note.startTime, gridSize);
        
        // Quantize duration
        int endTime = note.startTime + note.duration;
        endTime = findClosestGridPoint(endTime, gridSize);
        note.duration = endTime - note.startTime;
        
        // Ensure minimum duration
        note.duration = std::max(note.duration, gridSize);
    }
    
    return quantized;
}

// Apply groove to a MIDI sequence
MidiSequence MidiUtils::applyGroove(const MidiSequence& sequence, const std::vector<float>& groove) {
    MidiSequence grooved = sequence;
    
    // Groove vector contains pairs of timing and velocity adjustments
    if (groove.empty() || groove.size() % 2 != 0) {
        // Invalid groove vector
        return grooved;
    }
    
    int grooveLength = static_cast<int>(groove.size()) / 2;
    int gridSize = sequence.totalTicks / grooveLength;
    
    for (auto& note : grooved.notes) {
        // Determine which groove step applies to this note
        int grooveIndex = (note.startTime / gridSize) % grooveLength;
        
        // Apply timing adjustment (normalized to -1.0 to 1.0)
        float timingAdjustment = groove[grooveIndex * 2] * gridSize * 0.5f;
        note.startTime += static_cast<int>(timingAdjustment);
        
        // Apply velocity adjustment (normalized to -1.0 to 1.0)
        float velocityAdjustment = groove[grooveIndex * 2 + 1] * 64.0f;
        note.velocity += static_cast<int>(velocityAdjustment);
        
        // Clamp velocity to valid range
        note.velocity = std::max(1, std::min(127, note.velocity));
        
        // Ensure note doesn't start before 0
        note.startTime = std::max(0, note.startTime);
    }
    
    return grooved;
}

// Convert MIDI sequence to LMMS pattern
std::string MidiUtils::sequenceToLMMSPattern(const MidiSequence& sequence) {
    // TODO: Implement conversion to LMMS pattern format
    // This will require understanding the LMMS pattern serialization format
    
    // For now, return a placeholder
    return "LMMS_PATTERN_PLACEHOLDER";
}

// Convert LMMS pattern to MIDI sequence
MidiSequence MidiUtils::lmmsPatternToSequence(const std::string& pattern) {
    // TODO: Implement conversion from LMMS pattern format
    // This will require understanding the LMMS pattern serialization format
    
    // For now, return an empty sequence
    return MidiSequence();
}

// Generate a random MIDI sequence
MidiSequence MidiUtils::generateRandomSequence(int numNotes, int totalTicks, 
                                             int minPitch, int maxPitch,
                                             bool isPercussion) {
    MidiSequence sequence;
    sequence.totalTicks = totalTicks;
    
    // Create random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pitchDist(minPitch, maxPitch);
    std::uniform_int_distribution<> velocityDist(40, 100);
    std::uniform_int_distribution<> timeDist(0, totalTicks - 1);
    std::uniform_int_distribution<> durationDist(totalTicks / 16, totalTicks / 4);
    
    // Generate random notes
    for (int i = 0; i < numNotes; ++i) {
        MidiNote note;
        note.pitch = pitchDist(gen);
        note.velocity = velocityDist(gen);
        note.startTime = timeDist(gen);
        note.duration = durationDist(gen);
        note.isPercussion = isPercussion;
        
        // Ensure note doesn't extend beyond sequence
        if (note.startTime + note.duration > totalTicks) {
            note.duration = totalTicks - note.startTime;
        }
        
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
    
    // Calculate Euclidean distance between feature vectors
    float sumSquaredDiff = 0.0f;
    for (size_t i = 0; i < features1.size() && i < features2.size(); ++i) {
        float diff = features1[i] - features2[i];
        sumSquaredDiff += diff * diff;
    }
    
    // Convert distance to similarity (1.0 for identical, 0.0 for maximally different)
    float distance = std::sqrt(sumSquaredDiff);
    float similarity = 1.0f / (1.0f + distance);
    
    return similarity;
}

// Merge two MIDI sequences
MidiSequence MidiUtils::mergeSequences(const MidiSequence& sequence1, 
                                      const MidiSequence& sequence2,
                                      float weight1) {
    // Clamp weight to 0-1 range
    weight1 = std::max(0.0f, std::min(1.0f, weight1));
    float weight2 = 1.0f - weight1;
    
    // Create result sequence
    MidiSequence result;
    
    // Set total ticks to the maximum of both sequences
    result.totalTicks = std::max(sequence1.totalTicks, sequence2.totalTicks);
    
    // Add notes from first sequence with probability based on weight1
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    
    for (const auto& note : sequence1.notes) {
        if (dist(gen) < weight1) {
            result.notes.push_back(note);
        }
    }
    
    // Add notes from second sequence with probability based on weight2
    for (const auto& note : sequence2.notes) {
        if (dist(gen) < weight2) {
            result.notes.push_back(note);
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
    
    // Skip empty sequences
    if (sequence.notes.empty()) {
        // Return empty feature vector
        return features;
    }
    
    // Calculate basic statistics
    int minPitch = 127;
    int maxPitch = 0;
    int totalPitch = 0;
    int totalVelocity = 0;
    int totalDuration = 0;
    
    for (const auto& note : sequence.notes) {
        minPitch = std::min(minPitch, note.pitch);
        maxPitch = std::max(maxPitch, note.pitch);
        totalPitch += note.pitch;
        totalVelocity += note.velocity;
        totalDuration += note.duration;
    }
    
    // Calculate averages
    float avgPitch = static_cast<float>(totalPitch) / sequence.notes.size();
    float avgVelocity = static_cast<float>(totalVelocity) / sequence.notes.size();
    float avgDuration = static_cast<float>(totalDuration) / sequence.notes.size();
    
    // Calculate pitch range
    float pitchRange = static_cast<float>(maxPitch - minPitch);
    
    // Calculate note density (notes per tick)
    float noteDensity = static_cast<float>(sequence.notes.size()) / sequence.totalTicks;
    
    // Calculate average interval between consecutive notes
    float avgInterval = 0.0f;
    if (sequence.notes.size() > 1) {
        int totalInterval = 0;
        for (size_t i = 1; i < sequence.notes.size(); ++i) {
            totalInterval += sequence.notes[i].startTime - sequence.notes[i-1].startTime;
        }
        avgInterval = static_cast<float>(totalInterval) / (sequence.notes.size() - 1);
    }
    
    // Add features to vector
    features.push_back(normalizeValue(avgPitch, 0, 127));
    features.push_back(normalizeValue(avgVelocity, 0, 127));
    features.push_back(normalizeValue(avgDuration, 0, sequence.totalTicks));
    features.push_back(normalizeValue(pitchRange, 0, 127));
    features.push_back(normalizeValue(noteDensity, 0, 1.0f));
    features.push_back(normalizeValue(avgInterval, 0, sequence.totalTicks));
    
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
    if (max == min) {
        return 0.5f;  // Avoid division by zero
    }
    return (value - min) / (max - min);
}

// Denormalize value from 0-1 range
float MidiUtils::denormalizeValue(float value, float min, float max) {
    return min + value * (max - min);
}

} // namespace lmms_magenta
