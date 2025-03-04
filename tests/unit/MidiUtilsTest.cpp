#include <gtest/gtest.h>
#include "utils/MidiUtils.h"
#include <vector>

using namespace lmms_magenta;

class MidiUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test MIDI notes
        MidiNote note1;
        note1.pitch = 60; // C4
        note1.velocity = 100;
        note1.startTime = 0.0f;
        note1.endTime = 0.5f;
        
        MidiNote note2;
        note2.pitch = 64; // E4
        note2.velocity = 80;
        note2.startTime = 0.5f;
        note2.endTime = 1.0f;
        
        MidiNote note3;
        note3.pitch = 67; // G4
        note3.velocity = 90;
        note3.startTime = 1.0f;
        note3.endTime = 1.5f;
        
        m_testNotes.push_back(note1);
        m_testNotes.push_back(note2);
        m_testNotes.push_back(note3);
    }
    
    std::vector<MidiNote> m_testNotes;
};

// Test conversion from notes to tensor
TEST_F(MidiUtilsTest, NotesToTensor) {
    // Convert notes to tensor
    std::vector<float> tensor = MidiUtils::notesToTensor(m_testNotes);
    
    // Check tensor size
    // Each note should have 3 values (pitch, velocity, time)
    EXPECT_EQ(tensor.size(), m_testNotes.size() * 3);
    
    // Check tensor values
    // Note 1
    EXPECT_EQ(tensor[0], 60.0f);
    EXPECT_EQ(tensor[1], 100.0f);
    EXPECT_EQ(tensor[2], 0.0f);
    
    // Note 2
    EXPECT_EQ(tensor[3], 64.0f);
    EXPECT_EQ(tensor[4], 80.0f);
    EXPECT_EQ(tensor[5], 0.5f);
    
    // Note 3
    EXPECT_EQ(tensor[6], 67.0f);
    EXPECT_EQ(tensor[7], 90.0f);
    EXPECT_EQ(tensor[8], 1.0f);
}

// Test conversion from tensor to notes
TEST_F(MidiUtilsTest, TensorToNotes) {
    // Create a tensor
    std::vector<float> tensor = {
        60.0f, 100.0f, 0.0f,   // Note 1
        64.0f, 80.0f, 0.5f,    // Note 2
        67.0f, 90.0f, 1.0f     // Note 3
    };
    
    // Convert tensor to notes
    std::vector<MidiNote> notes = MidiUtils::tensorToNotes(tensor);
    
    // Check number of notes
    EXPECT_EQ(notes.size(), 3);
    
    // Check note values
    // Note 1
    EXPECT_EQ(notes[0].pitch, 60);
    EXPECT_EQ(notes[0].velocity, 100);
    EXPECT_FLOAT_EQ(notes[0].startTime, 0.0f);
    
    // Note 2
    EXPECT_EQ(notes[1].pitch, 64);
    EXPECT_EQ(notes[1].velocity, 80);
    EXPECT_FLOAT_EQ(notes[1].startTime, 0.5f);
    
    // Note 3
    EXPECT_EQ(notes[2].pitch, 67);
    EXPECT_EQ(notes[2].velocity, 90);
    EXPECT_FLOAT_EQ(notes[2].startTime, 1.0f);
}

// Test quantization
TEST_F(MidiUtilsTest, QuantizeNotes) {
    // Create notes with non-quantized times
    std::vector<MidiNote> notes;
    
    MidiNote note1;
    note1.pitch = 60;
    note1.velocity = 100;
    note1.startTime = 0.05f; // Slightly off-grid
    note1.endTime = 0.45f;
    
    MidiNote note2;
    note2.pitch = 64;
    note2.velocity = 80;
    note2.startTime = 0.53f; // Slightly off-grid
    note2.endTime = 0.97f;
    
    notes.push_back(note1);
    notes.push_back(note2);
    
    // Quantize notes to 16th notes (0.25 beats)
    float quantizeGrid = 0.25f;
    std::vector<MidiNote> quantizedNotes = MidiUtils::quantizeNotes(notes, quantizeGrid);
    
    // Check quantized times
    EXPECT_FLOAT_EQ(quantizedNotes[0].startTime, 0.0f);
    EXPECT_FLOAT_EQ(quantizedNotes[0].endTime, 0.5f);
    
    EXPECT_FLOAT_EQ(quantizedNotes[1].startTime, 0.5f);
    EXPECT_FLOAT_EQ(quantizedNotes[1].endTime, 1.0f);
}

// Test MIDI file loading
TEST_F(MidiUtilsTest, LoadMidiFile) {
    // Create a test MIDI file path
    std::string midiFilePath = "test.mid";
    
    // Load MIDI file
    std::vector<MidiNote> notes = MidiUtils::loadMidiFile(midiFilePath);
    
    // Since we don't have an actual file, this should return an empty vector
    EXPECT_TRUE(notes.empty());
}

// Test MIDI file saving
TEST_F(MidiUtilsTest, SaveMidiFile) {
    // Create a test MIDI file path
    std::string midiFilePath = "test.mid";
    
    // Save MIDI file
    bool success = MidiUtils::saveMidiFile(m_testNotes, midiFilePath);
    
    // Since we're not implementing actual file I/O in this test,
    // this should return false
    EXPECT_FALSE(success);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
