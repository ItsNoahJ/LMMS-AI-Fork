#include <gtest/gtest.h>
#include "model_serving/TensorFlowLiteModel.h"
#include <memory>
#include <vector>

using namespace lmms_magenta;

class TensorFlowLiteModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create model instance
        m_model = std::make_shared<TensorFlowLiteModel>();
    }
    
    std::shared_ptr<TensorFlowLiteModel> m_model;
};

// Test model initialization
TEST_F(TensorFlowLiteModelTest, Initialization) {
    // Check initial state
    EXPECT_FALSE(m_model->isLoaded());
}

// Test model loading
TEST_F(TensorFlowLiteModelTest, ModelLoading) {
    // Load model
    bool success = m_model->load("");
    
    // Since we don't have actual model files in the test,
    // this should return false
    EXPECT_FALSE(success);
    EXPECT_FALSE(m_model->isLoaded());
}

// Test input tensor setting
TEST_F(TensorFlowLiteModelTest, InputTensorSetting) {
    // Create test input data
    std::vector<float> inputData(128, 0.5f);
    
    // Try to set input tensor
    bool success = m_model->setInputTensor(0, inputData);
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
}

// Test output tensor getting
TEST_F(TensorFlowLiteModelTest, OutputTensorGetting) {
    // Create vector for output data
    std::vector<float> outputData;
    
    // Try to get output tensor
    bool success = m_model->getOutputTensor(0, outputData);
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
    EXPECT_TRUE(outputData.empty());
}

// Test model inference
TEST_F(TensorFlowLiteModelTest, ModelInference) {
    // Try to run inference
    bool success = m_model->runInference();
    
    // Since the model is not loaded, this should fail
    EXPECT_FALSE(success);
}

// Test error handling
TEST_F(TensorFlowLiteModelTest, ErrorHandling) {
    // Try to load non-existent model
    bool success = m_model->load("non_existent_model");
    EXPECT_FALSE(success);
    
    // Try operations with unloaded model
    std::vector<float> data(128, 0.5f);
    EXPECT_FALSE(m_model->setInputTensor(0, data));
    
    std::vector<float> outputData;
    EXPECT_FALSE(m_model->getOutputTensor(0, outputData));
    
    EXPECT_FALSE(m_model->runInference());
}

// Test model metadata
TEST_F(TensorFlowLiteModelTest, ModelMetadata) {
    // Get model metadata
    ModelMetadata metadata = m_model->getMetadata();
    
    // Check metadata values
    EXPECT_EQ(metadata.type, ModelType::TensorFlowLite);
    EXPECT_TRUE(metadata.name.empty());
    EXPECT_EQ(metadata.memoryUsage, 0);
}

// Test tensor shape validation
TEST_F(TensorFlowLiteModelTest, TensorShapeValidation) {
    // Try to set input tensor with invalid shape
    std::vector<float> invalidData(1); // Too small
    EXPECT_FALSE(m_model->setInputTensor(0, invalidData));
    
    // Try to set input tensor with invalid index
    std::vector<float> validData(128, 0.5f);
    EXPECT_FALSE(m_model->setInputTensor(-1, validData));
    EXPECT_FALSE(m_model->setInputTensor(100, validData));
    
    // Try to get output tensor with invalid index
    std::vector<float> outputData;
    EXPECT_FALSE(m_model->getOutputTensor(-1, outputData));
    EXPECT_FALSE(m_model->getOutputTensor(100, outputData));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
