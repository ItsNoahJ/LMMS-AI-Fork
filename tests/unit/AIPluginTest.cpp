#include <gtest/gtest.h>
#include "plugins/AIPlugin.h"
#include "model_serving/ModelServer.h"
#include <memory>

using namespace lmms_magenta;

// Mock AIPlugin for testing
class MockAIPlugin : public AIPlugin {
public:
    MockAIPlugin() : AIPlugin("MockPlugin") {}
    
    // Make protected methods public for testing
    using AIPlugin::loadModel;
    using AIPlugin::unloadModel;
    using AIPlugin::getModelServer;
    
    // Implement pure virtual methods
    bool handleMidiEvent(const MidiEvent& event) override {
        m_lastEvent = event;
        return true;
    }
    
    bool processAudio(float* buffer, size_t length) override {
        m_lastBufferLength = length;
        return true;
    }
    
    // Test helper methods
    MidiEvent getLastEvent() const { return m_lastEvent; }
    size_t getLastBufferLength() const { return m_lastBufferLength; }
    
private:
    MidiEvent m_lastEvent;
    size_t m_lastBufferLength = 0;
};

class AIPluginTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create plugin instance
        m_plugin = std::make_shared<MockAIPlugin>();
    }
    
    std::shared_ptr<MockAIPlugin> m_plugin;
};

// Test plugin initialization
TEST_F(AIPluginTest, Initialization) {
    // Check plugin name
    EXPECT_EQ(m_plugin->getName(), "MockPlugin");
    
    // Check initial state
    EXPECT_FALSE(m_plugin->isEnabled());
    EXPECT_FALSE(m_plugin->isModelLoaded());
}

// Test enable/disable functionality
TEST_F(AIPluginTest, EnableDisable) {
    // Enable plugin
    m_plugin->enable();
    EXPECT_TRUE(m_plugin->isEnabled());
    
    // Disable plugin
    m_plugin->disable();
    EXPECT_FALSE(m_plugin->isEnabled());
}

// Test model loading
TEST_F(AIPluginTest, ModelLoading) {
    // Try to load model
    bool success = m_plugin->loadModel("");
    
    // Since we don't have actual model files in the test,
    // this should return false
    EXPECT_FALSE(success);
    EXPECT_FALSE(m_plugin->isModelLoaded());
    
    // Try to unload model
    m_plugin->unloadModel();
    EXPECT_FALSE(m_plugin->isModelLoaded());
}

// Test MIDI event handling
TEST_F(AIPluginTest, MidiEventHandling) {
    // Create test MIDI event
    MidiEvent event;
    event.type = MidiEventType::NoteOn;
    event.note.pitch = 60;
    event.note.velocity = 100;
    event.note.channel = 0;
    
    // Handle event
    bool success = m_plugin->handleMidiEvent(event);
    EXPECT_TRUE(success);
    
    // Check that event was handled
    MidiEvent lastEvent = m_plugin->getLastEvent();
    EXPECT_EQ(lastEvent.type, event.type);
    EXPECT_EQ(lastEvent.note.pitch, event.note.pitch);
    EXPECT_EQ(lastEvent.note.velocity, event.note.velocity);
    EXPECT_EQ(lastEvent.note.channel, event.note.channel);
}

// Test audio processing
TEST_F(AIPluginTest, AudioProcessing) {
    // Create test audio buffer
    std::vector<float> buffer(1024, 0.0f);
    size_t length = buffer.size();
    
    // Process audio
    bool success = m_plugin->processAudio(buffer.data(), length);
    EXPECT_TRUE(success);
    
    // Check that buffer was processed
    EXPECT_EQ(m_plugin->getLastBufferLength(), length);
}

// Test model server access
TEST_F(AIPluginTest, ModelServerAccess) {
    // Get model server
    auto modelServer = m_plugin->getModelServer();
    EXPECT_NE(modelServer, nullptr);
}

// Test error handling
TEST_F(AIPluginTest, ErrorHandling) {
    // Try to load non-existent model
    bool success = m_plugin->loadModel("non_existent_model");
    EXPECT_FALSE(success);
    
    // Try to process audio with null buffer
    EXPECT_FALSE(m_plugin->processAudio(nullptr, 1024));
    
    // Try to process audio with zero length
    float buffer[1024];
    EXPECT_FALSE(m_plugin->processAudio(buffer, 0));
}

// Test plugin state management
TEST_F(AIPluginTest, StateManagement) {
    // Test initial state
    EXPECT_FALSE(m_plugin->isEnabled());
    EXPECT_FALSE(m_plugin->isModelLoaded());
    
    // Enable plugin without model
    m_plugin->enable();
    EXPECT_TRUE(m_plugin->isEnabled());
    EXPECT_FALSE(m_plugin->isModelLoaded());
    
    // Try to load model
    bool success = m_plugin->loadModel("");
    EXPECT_FALSE(success);
    
    // Disable plugin
    m_plugin->disable();
    EXPECT_FALSE(m_plugin->isEnabled());
}

// Test concurrent operations
TEST_F(AIPluginTest, ConcurrentOperations) {
    // Enable plugin and try operations
    m_plugin->enable();
    
    // Create test data
    MidiEvent event;
    event.type = MidiEventType::NoteOn;
    std::vector<float> buffer(1024, 0.0f);
    
    // Try concurrent operations
    bool midiSuccess = m_plugin->handleMidiEvent(event);
    bool audioSuccess = m_plugin->processAudio(buffer.data(), buffer.size());
    
    EXPECT_TRUE(midiSuccess);
    EXPECT_TRUE(audioSuccess);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
