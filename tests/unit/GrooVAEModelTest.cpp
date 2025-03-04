#include <gtest/gtest.h>
#include "model_serving/GrooVAEModel.h"
#include "utils/MidiUtils.h"
#include <memory>
#include <vector>

using namespace lmms_magenta;

class GrooVAEModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create model instance
        m_model = std::make_shared<GrooVAEModel>();
        
        // Create test MIDI notes (a simple 4/4 beat)
        for (int i = 0; i < 16; ++i) {
            MidiNote note;
            note.pitch = 36; // Kick drum
            note.velocity = 100;
            note.startTime = i * 0.25f;
            note.endTime = note.startTime + 0.1f;
            
            m_testNotes.push_back(note);
            
            // Add snare on beats 2 and 4
            if (i % 4 == 2) {
                MidiNote snare;
                snare.pitch = 38; // Snare drum
                snare.velocity = 100;
                snare.startTime = i * 0.25f;
                snare.endTime = snare.startTime + 0.1f;
                
                m_testNotes.push_back(snare);
            }
            
            // Add hi-hat on every 8th note
            MidiNote hihat;
            hihat.pitch = 42; // Closed hi-hat
            hihat.velocity = 80;
            hihat.startTime = i * 0.25f;
            hihat.endTime = hihat.startTime + 0.1f;
            
            m_testNotes.push_back(hihat);
        }
    }
    
    std::shared_ptr<GrooVAEModel> m_model;
    std::vector<MidiNote> m_testNotes;
};

// Test model initialization
TEST_F(GrooVAEModelTest, Initialization) {
    // Check model type
    EXPECT_EQ(m_model->getType(), ModelType::GrooVAE);
    
    // Check initial state
    EXPECT_FALSE(m_model->isLoaded());
    EXPECT_EQ(m_model->getTemperature(), 1.0f);
    EXPECT_EQ(m_model->getHumanize(), 0.5f);
}

// Test model loading
TEST_F(GrooVAEModelTest, ModelLoading) {
    // Load model
    bool success = m_model->load("");
    
    // Since we don't have actual model files in the test,
    // this should return false
    EXPECT_FALSE(success);
    EXPECT_FALSE(m_model->isLoaded());
}

// Test groove application
TEST_F(GrooVAEModelTest, GrooveApplication) {
    // Try to apply groove
    std::vector<MidiNote> outputNotes;
    bool success = m_model->applyGroove(m_testNotes, outputNotes);
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
    EXPECT_TRUE(outputNotes.empty());
}

// Test groove extraction
TEST_F(GrooVAEModelTest, GrooveExtraction) {
    // Try to extract groove
    std::vector<float> groove;
    bool success = m_model->extractGroove(m_testNotes, groove);
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
    EXPECT_TRUE(groove.empty());
}

// Test groove vector application
TEST_F(GrooVAEModelTest, GrooveVectorApplication) {
    // Create a test groove vector
    std::vector<float> groove(256, 0.1f);
    
    // Try to apply groove vector
    std::vector<MidiNote> outputNotes;
    bool success = m_model->applyGrooveVector(m_testNotes, groove, outputNotes);
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
    EXPECT_TRUE(outputNotes.empty());
}

// Test temperature parameter
TEST_F(GrooVAEModelTest, TemperatureParameter) {
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

// Test humanize parameter
TEST_F(GrooVAEModelTest, HumanizeParameter) {
    // Set humanize
    float humanize = 0.75f;
    m_model->setHumanize(humanize);
    
    // Check humanize
    EXPECT_FLOAT_EQ(m_model->getHumanize(), humanize);
    
    // Test invalid humanize values
    m_model->setHumanize(-1.0f);
    EXPECT_FLOAT_EQ(m_model->getHumanize(), 0.0f);
    
    m_model->setHumanize(2.0f);
    EXPECT_FLOAT_EQ(m_model->getHumanize(), 1.0f);
}

// Test error handling
TEST_F(GrooVAEModelTest, ErrorHandling) {
    // Try to load non-existent model
    bool success = m_model->load("non_existent_model");
    EXPECT_FALSE(success);
    
    // Try operations with unloaded model
    std::vector<MidiNote> outputNotes;
    EXPECT_FALSE(m_model->applyGroove(m_testNotes, outputNotes));
    
    std::vector<float> groove;
    EXPECT_FALSE(m_model->extractGroove(m_testNotes, groove));
    
    std::vector<float> testGroove(256, 0.1f);
    EXPECT_FALSE(m_model->applyGrooveVector(m_testNotes, testGroove, outputNotes));
}

// Test model metadata
TEST_F(GrooVAEModelTest, ModelMetadata) {
    // Get model metadata
    ModelMetadata metadata = m_model->getMetadata();
    
    // Check metadata values
    EXPECT_EQ(metadata.type, ModelType::GrooVAE);
    EXPECT_TRUE(metadata.name.empty());
    EXPECT_EQ(metadata.memoryUsage, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
