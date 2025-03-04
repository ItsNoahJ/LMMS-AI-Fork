#pragma once

#include "ModelServer.h"
#include <string>
#include <memory>
#include <vector>

// Forward declarations for TensorFlow Lite
namespace tflite {
class FlatBufferModel;
class Interpreter;
}

namespace lmms_magenta {

/**
 * @brief Base class for TensorFlow Lite models
 * 
 * This class provides a common implementation for TensorFlow Lite models,
 * handling model loading, inference, and memory management.
 */
class TensorFlowLiteModel : public Model {
public:
    /**
     * @brief Constructor
     * @param modelPath Path to the TensorFlow Lite model file
     * @param metadata Model metadata
     */
    TensorFlowLiteModel(const std::string& modelPath, const ModelMetadata& metadata);
    
    /**
     * @brief Destructor
     */
    ~TensorFlowLiteModel() override;
    
    /**
     * @brief Initialize the model
     * @return True if initialization was successful
     */
    bool initialize() override;
    
    /**
     * @brief Check if the model is initialized
     * @return True if the model is initialized
     */
    bool isInitialized() const override;
    
    /**
     * @brief Get the model metadata
     * @return ModelMetadata struct containing model information
     */
    ModelMetadata getMetadata() const override;
    
    /**
     * @brief Get the memory usage of the model
     * @return Memory usage in bytes
     */
    size_t getMemoryUsage() const override;
    
    /**
     * @brief Run inference on the model
     * @param inputTensor Input tensor data
     * @return Output tensor data
     */
    virtual std::vector<float> runInference(const std::vector<float>& inputTensor);
    
    /**
     * @brief Get the input tensor shape
     * @return Vector containing the dimensions of the input tensor
     */
    std::vector<int> getInputShape() const;
    
    /**
     * @brief Get the output tensor shape
     * @return Vector containing the dimensions of the output tensor
     */
    std::vector<int> getOutputShape() const;
    
    /**
     * @brief Set the number of threads to use for inference
     * @param numThreads Number of threads (0 for auto)
     */
    void setNumThreads(int numThreads);
    
    /**
     * @brief Set whether to use GPU acceleration
     * @param useGPU Whether to use GPU acceleration
     * @return True if GPU acceleration was successfully enabled
     */
    bool setUseGPU(bool useGPU);
    
    /**
     * @brief Check if GPU acceleration is being used
     * @return True if GPU acceleration is being used
     */
    bool isUsingGPU() const;
    
protected:
    /**
     * @brief Prepare input tensor for inference
     * @param inputData Input data to prepare
     * @return True if preparation was successful
     */
    virtual bool prepareInputTensor(const std::vector<float>& inputData);
    
    /**
     * @brief Extract output tensor after inference
     * @return Output data from the model
     */
    virtual std::vector<float> extractOutputTensor();
    
private:
    // Model path
    std::string m_modelPath;
    
    // Model metadata
    ModelMetadata m_metadata;
    
    // TensorFlow Lite model and interpreter
    std::unique_ptr<tflite::FlatBufferModel> m_model;
    std::unique_ptr<tflite::Interpreter> m_interpreter;
    
    // Model state
    bool m_isInitialized;
    bool m_isUsingGPU;
    int m_numThreads;
    
    // Memory usage tracking
    size_t m_memoryUsage;
};

} // namespace lmms_magenta
