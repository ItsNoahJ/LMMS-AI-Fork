#include <gtest/gtest.h>
#include "model_serving/ModelServer.h"
#include <filesystem>
#include <memory>

using namespace lmms_magenta;

class ModelServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test models directory
        m_modelsDir = "../test_models";
        std::filesystem::create_directories(m_modelsDir);
    }
    
    void TearDown() override {
        // Clean up any loaded models
        auto loadedModels = ModelServer::getInstance().getLoadedModels();
        for (const auto& model : loadedModels) {
            ModelServer::getInstance().unloadModel(model.type, model.name);
        }
        
        // Remove test models directory
        std::filesystem::remove_all(m_modelsDir);
    }
    
    std::string m_modelsDir;
};

// Test initialization
TEST_F(ModelServerTest, Initialization) {
    // Initialize with valid parameters
    bool success = ModelServer::getInstance().initialize(m_modelsDir, 1024 * 1024 * 1024, false);
    EXPECT_TRUE(success);
    
    // Check if models directory exists
    EXPECT_TRUE(std::filesystem::exists(m_modelsDir));
    
    // Check if initialized flag is set
    EXPECT_TRUE(ModelServer::getInstance().isInitialized());
}

// Test model loading
TEST_F(ModelServerTest, ModelLoading) {
    // Initialize server
    ASSERT_TRUE(ModelServer::getInstance().initialize(m_modelsDir, 1024 * 1024 * 1024, false));
    
    // Load MusicVAE model
    bool success = ModelServer::getInstance().loadModel(ModelType::MusicVAE, "");
    EXPECT_TRUE(success);
    
    // Check if model is loaded
    auto model = ModelServer::getInstance().getModel(ModelType::MusicVAE, "");
    EXPECT_NE(model, nullptr);
    
    // Check model type
    EXPECT_EQ(model->getType(), ModelType::MusicVAE);
}

// Test model unloading
TEST_F(ModelServerTest, ModelUnloading) {
    // Initialize server
    ASSERT_TRUE(ModelServer::getInstance().initialize(m_modelsDir, 1024 * 1024 * 1024, false));
    
    // Load model
    ASSERT_TRUE(ModelServer::getInstance().loadModel(ModelType::MusicVAE, ""));
    
    // Unload model
    bool success = ModelServer::getInstance().unloadModel(ModelType::MusicVAE, "");
    EXPECT_TRUE(success);
    
    // Check if model is unloaded
    auto model = ModelServer::getInstance().getModel(ModelType::MusicVAE, "");
    EXPECT_EQ(model, nullptr);
}

// Test loading multiple models
TEST_F(ModelServerTest, MultipleModels) {
    // Initialize server
    ASSERT_TRUE(ModelServer::getInstance().initialize(m_modelsDir, 1024 * 1024 * 1024, false));
    
    // Load MusicVAE model
    ASSERT_TRUE(ModelServer::getInstance().loadModel(ModelType::MusicVAE, ""));
    
    // Load GrooVAE model
    ASSERT_TRUE(ModelServer::getInstance().loadModel(ModelType::GrooVAE, ""));
    
    // Check if both models are loaded
    auto musicVAEModel = ModelServer::getInstance().getModel(ModelType::MusicVAE, "");
    EXPECT_NE(musicVAEModel, nullptr);
    
    auto grooVAEModel = ModelServer::getInstance().getModel(ModelType::GrooVAE, "");
    EXPECT_NE(grooVAEModel, nullptr);
    
    // Check model types
    EXPECT_EQ(musicVAEModel->getType(), ModelType::MusicVAE);
    EXPECT_EQ(grooVAEModel->getType(), ModelType::GrooVAE);
}

// Test memory management
TEST_F(ModelServerTest, MemoryManagement) {
    // Initialize server with small memory limit
    const size_t memoryLimit = 1024 * 1024; // 1MB
    ASSERT_TRUE(ModelServer::getInstance().initialize(m_modelsDir, memoryLimit, false));
    
    // Load MusicVAE model
    ASSERT_TRUE(ModelServer::getInstance().loadModel(ModelType::MusicVAE, ""));
    
    // Check memory usage
    size_t memoryUsage = ModelServer::getInstance().getMemoryUsage();
    EXPECT_LE(memoryUsage, memoryLimit);
    
    // Load GrooVAE model
    ASSERT_TRUE(ModelServer::getInstance().loadModel(ModelType::GrooVAE, ""));
    
    // Check memory usage again
    memoryUsage = ModelServer::getInstance().getMemoryUsage();
    EXPECT_LE(memoryUsage, memoryLimit);
}

// Test model callbacks
TEST_F(ModelServerTest, ModelCallbacks) {
    // Initialize server
    ASSERT_TRUE(ModelServer::getInstance().initialize(m_modelsDir, 1024 * 1024 * 1024, false));
    
    bool loadCallbackCalled = false;
    bool unloadCallbackCalled = false;
    
    // Register callbacks
    ModelServer::getInstance().registerModelCallback(
        ModelType::MusicVAE,
        [&loadCallbackCalled](const std::string& name) {
            loadCallbackCalled = true;
        },
        [&unloadCallbackCalled](const std::string& name) {
            unloadCallbackCalled = true;
        }
    );
    
    // Load model
    ASSERT_TRUE(ModelServer::getInstance().loadModel(ModelType::MusicVAE, ""));
    
    // Check if load callback was called
    EXPECT_TRUE(loadCallbackCalled);
    
    // Unload model
    ASSERT_TRUE(ModelServer::getInstance().unloadModel(ModelType::MusicVAE, ""));
    
    // Check if unload callback was called
    EXPECT_TRUE(unloadCallbackCalled);
}

// Test error handling
TEST_F(ModelServerTest, ErrorHandling) {
    // Initialize server
    ASSERT_TRUE(ModelServer::getInstance().initialize(m_modelsDir, 1024 * 1024 * 1024, false));
    
    // Try to load non-existent model
    bool success = ModelServer::getInstance().loadModel(ModelType::MusicVAE, "non_existent_model");
    EXPECT_FALSE(success);
    
    // Try to unload non-existent model
    success = ModelServer::getInstance().unloadModel(ModelType::MusicVAE, "non_existent_model");
    EXPECT_FALSE(success);
    
    // Try to get non-existent model
    auto model = ModelServer::getInstance().getModel(ModelType::MusicVAE, "non_existent_model");
    EXPECT_EQ(model, nullptr);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
