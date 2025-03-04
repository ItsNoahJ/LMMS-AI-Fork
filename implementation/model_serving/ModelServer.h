#ifndef LMMS_MAGENTA_MODEL_SERVER_H
#define LMMS_MAGENTA_MODEL_SERVER_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <functional>
#include <mutex>

namespace lmms_magenta {

/**
 * @brief Enum representing the different types of AI models supported
 */
enum class ModelType {
    MusicVAE,      // For pattern generation
    GrooVAE,       // For groove modeling
    MelodyRNN,     // For melodic completion
    CycleGAN,      // For style transfer
    SmartGain,     // For gain staging
    EmotionMapper  // For emotion-to-music translation
};

/**
 * @brief Struct representing model metadata
 */
struct ModelMetadata {
    std::string name;
    ModelType type;
    std::string version;
    size_t memorySize;
    std::string description;
    bool isQuantized;
    bool supportsGPU;
};

/**
 * @brief Abstract base class for all model implementations
 */
class Model {
public:
    virtual ~Model() = default;
    
    /**
     * @brief Initialize the model
     * @return True if initialization was successful
     */
    virtual bool initialize() = 0;
    
    /**
     * @brief Check if the model is initialized
     * @return True if the model is initialized
     */
    virtual bool isInitialized() const = 0;
    
    /**
     * @brief Get the model metadata
     * @return ModelMetadata struct containing model information
     */
    virtual ModelMetadata getMetadata() const = 0;
    
    /**
     * @brief Get the model's memory usage in bytes
     * @return Memory usage in bytes
     */
    virtual size_t getMemoryUsage() const = 0;
    
    /**
     * @brief Unload the model from memory
     */
    virtual void unload() = 0;
};

/**
 * @brief Singleton class for managing AI models
 * 
 * This class is responsible for loading, managing, and serving AI models
 * for the LMMS-Magenta integration. It handles model lifecycle, caching,
 * and provides a unified interface for all AI features.
 */
class ModelServer {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the singleton instance
     */
    static ModelServer& getInstance();
    
    /**
     * @brief Initialize the model server
     * @param modelsDirectory Directory containing the model files
     * @param maxMemoryUsage Maximum memory usage in bytes (0 for unlimited)
     * @param enableGPU Whether to enable GPU acceleration if available
     * @return True if initialization was successful
     */
    bool initialize(const std::string& modelsDirectory, 
                   size_t maxMemoryUsage = 0, 
                   bool enableGPU = false);
    
    /**
     * @brief Load a model into memory
     * @param type Type of model to load
     * @param modelName Name of the model (if multiple models of same type exist)
     * @return True if loading was successful
     */
    bool loadModel(ModelType type, const std::string& modelName = "");
    
    /**
     * @brief Get a loaded model
     * @param type Type of model to get
     * @param modelName Name of the model (if multiple models of same type exist)
     * @return Shared pointer to the model, or nullptr if not loaded
     */
    std::shared_ptr<Model> getModel(ModelType type, const std::string& modelName = "");
    
    /**
     * @brief Unload a model from memory
     * @param type Type of model to unload
     * @param modelName Name of the model (if multiple models of same type exist)
     * @return True if unloading was successful
     */
    bool unloadModel(ModelType type, const std::string& modelName = "");
    
    /**
     * @brief Get available models
     * @return Vector of available model metadata
     */
    std::vector<ModelMetadata> getAvailableModels() const;
    
    /**
     * @brief Get loaded models
     * @return Vector of loaded model metadata
     */
    std::vector<ModelMetadata> getLoadedModels() const;
    
    /**
     * @brief Get total memory usage of all loaded models
     * @return Memory usage in bytes
     */
    size_t getTotalMemoryUsage() const;
    
    /**
     * @brief Set maximum memory usage
     * @param maxMemoryUsage Maximum memory usage in bytes (0 for unlimited)
     */
    void setMaxMemoryUsage(size_t maxMemoryUsage);
    
    /**
     * @brief Enable or disable GPU acceleration
     * @param enable Whether to enable GPU acceleration
     */
    void enableGPU(bool enable);
    
    /**
     * @brief Check if GPU acceleration is enabled
     * @return True if GPU acceleration is enabled
     */
    bool isGPUEnabled() const;
    
    /**
     * @brief Check if GPU acceleration is available
     * @return True if GPU acceleration is available
     */
    bool isGPUAvailable() const;
    
    /**
     * @brief Register a callback for model loading events
     * @param callback Function to call when a model is loaded or unloaded
     * @return ID of the registered callback
     */
    int registerModelCallback(std::function<void(ModelType, const std::string&, bool)> callback);
    
    /**
     * @brief Unregister a model callback
     * @param callbackId ID of the callback to unregister
     */
    void unregisterModelCallback(int callbackId);
    
private:
    // Private constructor for singleton
    ModelServer();
    
    // Prevent copying and assignment
    ModelServer(const ModelServer&) = delete;
    ModelServer& operator=(const ModelServer&) = delete;
    
    // Private implementation details
    std::string m_modelsDirectory;
    size_t m_maxMemoryUsage;
    bool m_enableGPU;
    bool m_isInitialized;
    
    // Map of loaded models
    std::map<std::pair<ModelType, std::string>, std::shared_ptr<Model>> m_loadedModels;
    
    // Map of available models
    std::map<std::pair<ModelType, std::string>, ModelMetadata> m_availableModels;
    
    // Callbacks for model loading events
    std::map<int, std::function<void(ModelType, const std::string&, bool)>> m_callbacks;
    int m_nextCallbackId;
    
    // Mutex for thread safety
    mutable std::mutex m_mutex;
    
    // Scan for available models in the models directory
    void scanForModels();
    
    // Unload models to free memory if necessary
    void unloadModelsIfNeeded(size_t requiredMemory);
};

} // namespace lmms_magenta

#endif // LMMS_MAGENTA_MODEL_SERVER_H
