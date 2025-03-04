#ifndef LMMS_MAGENTA_TENSORFLOW_LITE_MODEL_H
#define LMMS_MAGENTA_TENSORFLOW_LITE_MODEL_H

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
     * @brief Get the model's memory usage in bytes
     * @return Memory usage in bytes
     */
    size_t getMemoryUsage() const override;
    
    /**
     * @brief Unload the model from memory
     */
    void unload() override;
    
protected:
    /**
     * @brief Set input tensor data
     * @param inputIndex Index of the input tensor
     * @param data Pointer to the input data
     * @param size Size of the input data in bytes
     * @return True if successful
     */
    bool setInputTensorData(int inputIndex, const void* data, size_t size);
    
    /**
     * @brief Get output tensor data
     * @param outputIndex Index of the output tensor
     * @param data Pointer to the output data buffer
     * @param size Size of the output data buffer in bytes
     * @return True if successful
     */
    bool getOutputTensorData(int outputIndex, void* data, size_t size) const;
    
    /**
     * @brief Run inference
     * @return True if inference was successful
     */
    bool runInference();
    
    /**
     * @brief Get input tensor shape
     * @param inputIndex Index of the input tensor
     * @return Vector containing the tensor shape
     */
    std::vector<int> getInputTensorShape(int inputIndex) const;
    
    /**
     * @brief Get output tensor shape
     * @param outputIndex Index of the output tensor
     * @return Vector containing the tensor shape
     */
    std::vector<int> getOutputTensorShape(int outputIndex) const;
    
    /**
     * @brief Get number of input tensors
     * @return Number of input tensors
     */
    int getNumInputTensors() const;
    
    /**
     * @brief Get number of output tensors
     * @return Number of output tensors
     */
    int getNumOutputTensors() const;
    
private:
    // Model path
    std::string m_modelPath;
    
    // Model metadata
    ModelMetadata m_metadata;
    
    // TensorFlow Lite model and interpreter
    std::unique_ptr<tflite::FlatBufferModel> m_model;
    std::unique_ptr<tflite::Interpreter> m_interpreter;
    
    // Initialization state
    bool m_isInitialized;
    
    // Memory usage
    size_t m_memoryUsage;
};

} // namespace lmms_magenta

#endif // LMMS_MAGENTA_TENSORFLOW_LITE_MODEL_H
