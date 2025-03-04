#include <gtest/gtest.h>
#include "model_serving/MusicVAEModel.h"
#include "utils/MidiUtils.h"
#include <memory>
#include <vector>

using namespace lmms_magenta;

class MusicVAEModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create model instance
        m_model = std::make_shared<MusicVAEModel>();
        
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
    
    std::shared_ptr<MusicVAEModel> m_model;
    std::vector<MidiNote> m_testNotes;
};

// Test model initialization
TEST_F(MusicVAEModelTest, Initialization) {
    // Check model type
    EXPECT_EQ(m_model->getType(), ModelType::MusicVAE);
    
    // Check initial state
    EXPECT_FALSE(m_model->isLoaded());
    EXPECT_EQ(m_model->getTemperature(), 1.0f);
}

// Test model loading
TEST_F(MusicVAEModelTest, ModelLoading) {
    // Load model
    bool success = m_model->load("");
    
    // Since we don't have actual model files in the test,
    // this should return false
    EXPECT_FALSE(success);
    EXPECT_FALSE(m_model->isLoaded());
}

// Test pattern encoding
TEST_F(MusicVAEModelTest, PatternEncoding) {
    // Try to encode pattern
    std::vector<float> latentVector;
    bool success = m_model->encode(m_testNotes, latentVector);
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
    EXPECT_TRUE(latentVector.empty());
}

// Test pattern decoding
TEST_F(MusicVAEModelTest, PatternDecoding) {
    // Create a test latent vector
    std::vector<float> latentVector(256, 0.0f);
    
    // Try to decode pattern
    std::vector<MidiNote> decodedNotes;
    bool success = m_model->decode(latentVector, decodedNotes);
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
    EXPECT_TRUE(decodedNotes.empty());
}

// Test pattern sampling
TEST_F(MusicVAEModelTest, PatternSampling) {
    // Try to sample pattern
    std::vector<MidiNote> sampledNotes;
    bool success = m_model->sample(sampledNotes);
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
    EXPECT_TRUE(sampledNotes.empty());
}

// Test pattern interpolation
TEST_F(MusicVAEModelTest, PatternInterpolation) {
    // Create two test patterns
    std::vector<MidiNote> pattern1 = m_testNotes;
    std::vector<MidiNote> pattern2 = m_testNotes;
    
    // Modify pattern2 to be different
    for (auto& note : pattern2) {
        note.pitch += 12; // Up one octave
    }
    
    // Try to interpolate between patterns
    std::vector<std::vector<MidiNote>> interpolatedPatterns;
    bool success = m_model->interpolate(pattern1, pattern2, 5, interpolatedPatterns);
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
    EXPECT_TRUE(interpolatedPatterns.empty());
}

// Test temperature parameter
TEST_F(MusicVAEModelTest, TemperatureParameter) {
    // Set temperature
    float temperature = 0.5f;
    m_model->setTemperature(temperature);
    
    // Check temperature
    EXPECT_FLOAT_EQ(m_model->getTemperature(), temperature);
    
    // Test invalid temperature values
    m_model->setTemperature(-1.0f);
    EXPECT_FLOAT_EQ(m_model->getTemperature(), 0.0f);
    
    m_model->setTemperature(2.0f);
    EXPECT_FLOAT_EQ(m_model->getTemperature(), 1.0f);
}

// Test error handling
TEST_F(MusicVAEModelTest, ErrorHandling) {
    // Try to load non-existent model
    bool success = m_model->load("non_existent_model");
    EXPECT_FALSE(success);
    
    // Try operations with unloaded model
    std::vector<float> latentVector;
    EXPECT_FALSE(m_model->encode(m_testNotes, latentVector));
    
    std::vector<MidiNote> decodedNotes;
    EXPECT_FALSE(m_model->decode(latentVector, decodedNotes));
    
    std::vector<MidiNote> sampledNotes;
    EXPECT_FALSE(m_model->sample(sampledNotes));
    
    std::vector<std::vector<MidiNote>> interpolatedPatterns;
    EXPECT_FALSE(m_model->interpolate(m_testNotes, m_testNotes, 5, interpolatedPatterns));
}

// Test model metadata
TEST_F(MusicVAEModelTest, ModelMetadata) {
    // Get model metadata
    ModelMetadata metadata = m_model->getMetadata();
    
    // Check metadata values
    EXPECT_EQ(metadata.type, ModelType::MusicVAE);
    EXPECT_TRUE(metadata.name.empty());
    EXPECT_EQ(metadata.memoryUsage, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
