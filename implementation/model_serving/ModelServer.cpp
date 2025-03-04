#include "ModelServer.h"
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace lmms_magenta {

// Singleton instance
ModelServer& ModelServer::getInstance() {
    static ModelServer instance;
    return instance;
}

// Constructor
ModelServer::ModelServer() 
    : m_modelsDirectory("")
    , m_maxMemoryUsage(0)
    , m_enableGPU(false)
    , m_isInitialized(false)
    , m_nextCallbackId(0) {
}

// Initialize the model server
bool ModelServer::initialize(const std::string& modelsDirectory, 
                           size_t maxMemoryUsage, 
                           bool enableGPU) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_isInitialized) {
        std::cerr << "ModelServer is already initialized" << std::endl;
        return false;
    }
    
    m_modelsDirectory = modelsDirectory;
    m_maxMemoryUsage = maxMemoryUsage;
    m_enableGPU = enableGPU;
    
    // Create models directory if it doesn't exist
    if (!std::filesystem::exists(m_modelsDirectory)) {
        try {
            std::filesystem::create_directories(m_modelsDirectory);
        } catch (const std::exception& e) {
            std::cerr << "Failed to create models directory: " << e.what() << std::endl;
            return false;
        }
    }
    
    // Scan for available models
    scanForModels();
    
    m_isInitialized = true;
    return true;
}

// Load a model into memory
bool ModelServer::loadModel(ModelType type, const std::string& modelName) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_isInitialized) {
        std::cerr << "ModelServer is not initialized" << std::endl;
        return false;
    }
    
    // Check if model is already loaded
    auto key = std::make_pair(type, modelName);
    if (m_loadedModels.find(key) != m_loadedModels.end()) {
        // Model already loaded
        return true;
    }
    
    // Check if model is available
    if (m_availableModels.find(key) == m_availableModels.end()) {
        std::cerr << "Model not found: " << static_cast<int>(type) << " " << modelName << std::endl;
        return false;
    }
    
    // Get model metadata
    const ModelMetadata& metadata = m_availableModels[key];
    
    // Check if we need to unload other models to free memory
    if (m_maxMemoryUsage > 0) {
        size_t requiredMemory = metadata.memorySize;
        unloadModelsIfNeeded(requiredMemory);
    }
    
    // Create model instance based on type
    std::shared_ptr<Model> model;
    
    // TODO: Implement model creation based on type
    // This will be implemented when we add specific model implementations
    
    // For now, just return false as we don't have model implementations yet
    std::cerr << "Model implementation not available yet" << std::endl;
    return false;
    
    // Initialize model
    /*
    if (!model->initialize()) {
        std::cerr << "Failed to initialize model: " << static_cast<int>(type) << " " << modelName << std::endl;
        return false;
    }
    
    // Add model to loaded models
    m_loadedModels[key] = model;
    
    // Notify callbacks
    for (const auto& callback : m_callbacks) {
        callback.second(type, modelName, true);
    }
    
    return true;
    */
}

// Get a loaded model
std::shared_ptr<Model> ModelServer::getModel(ModelType type, const std::string& modelName) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_isInitialized) {
        std::cerr << "ModelServer is not initialized" << std::endl;
        return nullptr;
    }
    
    // Check if model is loaded
    auto key = std::make_pair(type, modelName);
    if (m_loadedModels.find(key) == m_loadedModels.end()) {
        // Try to load the model
        if (!loadModel(type, modelName)) {
            return nullptr;
        }
    }
    
    return m_loadedModels[key];
}

// Unload a model from memory
bool ModelServer::unloadModel(ModelType type, const std::string& modelName) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_isInitialized) {
        std::cerr << "ModelServer is not initialized" << std::endl;
        return false;
    }
    
    // Check if model is loaded
    auto key = std::make_pair(type, modelName);
    if (m_loadedModels.find(key) == m_loadedModels.end()) {
        // Model not loaded
        return true;
    }
    
    // Unload model
    m_loadedModels[key]->unload();
    
    // Remove from loaded models
    m_loadedModels.erase(key);
    
    // Notify callbacks
    for (const auto& callback : m_callbacks) {
        callback.second(type, modelName, false);
    }
    
    return true;
}

// Get available models
std::vector<ModelMetadata> ModelServer::getAvailableModels() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::vector<ModelMetadata> models;
    for (const auto& pair : m_availableModels) {
        models.push_back(pair.second);
    }
    
    return models;
}

// Get loaded models
std::vector<ModelMetadata> ModelServer::getLoadedModels() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::vector<ModelMetadata> models;
    for (const auto& pair : m_loadedModels) {
        models.push_back(pair.second->getMetadata());
    }
    
    return models;
}

// Get total memory usage
size_t ModelServer::getTotalMemoryUsage() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    size_t totalMemory = 0;
    for (const auto& pair : m_loadedModels) {
        totalMemory += pair.second->getMemoryUsage();
    }
    
    return totalMemory;
}

// Set maximum memory usage
void ModelServer::setMaxMemoryUsage(size_t maxMemoryUsage) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_maxMemoryUsage = maxMemoryUsage;
    
    // Check if we need to unload models
    if (m_maxMemoryUsage > 0) {
        size_t currentMemory = getTotalMemoryUsage();
        if (currentMemory > m_maxMemoryUsage) {
            unloadModelsIfNeeded(0);
        }
    }
}

// Enable or disable GPU acceleration
void ModelServer::enableGPU(bool enable) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_enableGPU != enable) {
        m_enableGPU = enable;
        
        // TODO: Reload models with new GPU setting
    }
}

// Check if GPU acceleration is enabled
bool ModelServer::isGPUEnabled() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_enableGPU;
}

// Check if GPU acceleration is available
bool ModelServer::isGPUAvailable() const {
    // TODO: Implement GPU availability check
    return false;
}

// Register a callback for model loading events
int ModelServer::registerModelCallback(std::function<void(ModelType, const std::string&, bool)> callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    int id = m_nextCallbackId++;
    m_callbacks[id] = callback;
    
    return id;
}

// Unregister a model callback
void ModelServer::unregisterModelCallback(int callbackId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_callbacks.erase(callbackId);
}

// Scan for available models
void ModelServer::scanForModels() {
    m_availableModels.clear();
    
    // TODO: Implement model scanning
    // This will involve searching for model files in the models directory
    // and creating metadata for each model
    
    // For now, add some placeholder metadata for testing
    
    // MusicVAE
    ModelMetadata musicVAE;
    musicVAE.name = "MusicVAE";
    musicVAE.type = ModelType::MusicVAE;
    musicVAE.version = "1.0";
    musicVAE.memorySize = 22 * 1024 * 1024; // 22MB
    musicVAE.description = "Hierarchical VAE for pattern generation";
    musicVAE.isQuantized = true;
    musicVAE.supportsGPU = true;
    m_availableModels[std::make_pair(ModelType::MusicVAE, "")] = musicVAE;
    
    // GrooVAE
    ModelMetadata grooVAE;
    grooVAE.name = "GrooVAE";
    grooVAE.type = ModelType::GrooVAE;
    grooVAE.version = "1.0";
    grooVAE.memorySize = 18 * 1024 * 1024; // 18MB
    grooVAE.description = "VAE for groove modeling";
    grooVAE.isQuantized = true;
    grooVAE.supportsGPU = true;
    m_availableModels[std::make_pair(ModelType::GrooVAE, "")] = grooVAE;
    
    // MelodyRNN
    ModelMetadata melodyRNN;
    melodyRNN.name = "MelodyRNN";
    melodyRNN.type = ModelType::MelodyRNN;
    melodyRNN.version = "1.0";
    melodyRNN.memorySize = 16 * 1024 * 1024; // 16MB
    melodyRNN.description = "RNN for melodic completion";
    melodyRNN.isQuantized = true;
    melodyRNN.supportsGPU = true;
    m_availableModels[std::make_pair(ModelType::MelodyRNN, "")] = melodyRNN;
    
    // CycleGAN
    ModelMetadata cycleGAN;
    cycleGAN.name = "CycleGAN";
    cycleGAN.type = ModelType::CycleGAN;
    cycleGAN.version = "1.0";
    cycleGAN.memorySize = 35 * 1024 * 1024; // 35MB
    cycleGAN.description = "GAN for style transfer";
    cycleGAN.isQuantized = true;
    cycleGAN.supportsGPU = true;
    m_availableModels[std::make_pair(ModelType::CycleGAN, "")] = cycleGAN;
    
    // SmartGain
    ModelMetadata smartGain;
    smartGain.name = "SmartGain";
    smartGain.type = ModelType::SmartGain;
    smartGain.version = "1.0";
    smartGain.memorySize = 4 * 1024 * 1024; // 4MB
    smartGain.description = "ML model for gain staging";
    smartGain.isQuantized = true;
    smartGain.supportsGPU = false;
    m_availableModels[std::make_pair(ModelType::SmartGain, "")] = smartGain;
    
    // EmotionMapper
    ModelMetadata emotionMapper;
    emotionMapper.name = "EmotionMapper";
    emotionMapper.type = ModelType::EmotionMapper;
    emotionMapper.version = "1.0";
    emotionMapper.memorySize = 7 * 1024 * 1024; // 7MB
    emotionMapper.description = "ML model for emotion-to-music translation";
    emotionMapper.isQuantized = true;
    emotionMapper.supportsGPU = false;
    m_availableModels[std::make_pair(ModelType::EmotionMapper, "")] = emotionMapper;
}

// Unload models to free memory if necessary
void ModelServer::unloadModelsIfNeeded(size_t requiredMemory) {
    if (m_maxMemoryUsage == 0) {
        // No memory limit
        return;
    }
    
    size_t currentMemory = getTotalMemoryUsage();
    if (currentMemory + requiredMemory <= m_maxMemoryUsage) {
        // Enough memory available
        return;
    }
    
    // Need to unload some models
    size_t memoryToFree = currentMemory + requiredMemory - m_maxMemoryUsage;
    
    // Sort models by last access time (not implemented yet)
    // For now, just unload models in arbitrary order
    
    for (auto it = m_loadedModels.begin(); it != m_loadedModels.end(); ) {
        if (memoryToFree <= 0) {
            break;
        }
        
        size_t modelMemory = it->second->getMemoryUsage();
        
        // Unload model
        it->second->unload();
        
        // Notify callbacks
        for (const auto& callback : m_callbacks) {
            callback.second(it->first.first, it->first.second, false);
        }
        
        // Remove from loaded models
        it = m_loadedModels.erase(it);
        
        // Update memory to free
        memoryToFree -= std::min(memoryToFree, modelMemory);
    }
}

} // namespace lmms_magenta
