#include "ModelServer.h"
#include <filesystem>
#include <algorithm>
#include <iostream>

namespace lmms_magenta {

// Initialize static instance
ModelServer& ModelServer::getInstance() {
    static ModelServer instance;
    return instance;
}

ModelServer::ModelServer()
    : m_modelsDirectory("")
    , m_maxMemoryUsage(0)
    , m_enableGPU(false)
    , m_isInitialized(false)
    , m_nextCallbackId(0) {
}

bool ModelServer::initialize(const std::string& modelsDirectory, 
                           size_t maxMemoryUsage, 
                           bool enableGPU) {
    // Check if already initialized
    if (m_isInitialized) {
        std::cerr << "ModelServer already initialized" << std::endl;
        return false;
    }
    
    // Check if directory exists
    if (!std::filesystem::exists(modelsDirectory) || 
        !std::filesystem::is_directory(modelsDirectory)) {
        std::cerr << "Models directory does not exist: " << modelsDirectory << std::endl;
        return false;
    }
    
    // Set member variables
    m_modelsDirectory = modelsDirectory;
    m_maxMemoryUsage = maxMemoryUsage;
    m_enableGPU = enableGPU;
    
    // Scan for available models
    scanForModels();
    
    m_isInitialized = true;
    return true;
}

bool ModelServer::loadModel(ModelType type, const std::string& modelName) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // Check if initialized
    if (!m_isInitialized) {
        std::cerr << "ModelServer not initialized" << std::endl;
        return false;
    }
    
    // Create key for model
    auto key = std::make_pair(type, modelName);
    
    // Check if model is already loaded
    if (m_loadedModels.find(key) != m_loadedModels.end()) {
        // Model already loaded
        return true;
    }
    
    // Check if model is available
    if (m_availableModels.find(key) == m_availableModels.end()) {
        std::cerr << "Model not available: " << static_cast<int>(type) << " " << modelName << std::endl;
        return false;
    }
    
    // Get model metadata
    const auto& metadata = m_availableModels[key];
    
    // Check if we need to unload other models to free memory
    if (m_maxMemoryUsage > 0) {
        unloadModelsIfNeeded(metadata.memorySize);
    }
    
    // Create model instance based on type
    std::shared_ptr<Model> model;
    
    try {
        // Create model instance based on type
        // This is a placeholder for actual model creation
        // In a real implementation, we would create the appropriate model type
        
        // For now, just log that we're loading the model
        std::cout << "Loading model: " << metadata.name << " (" << metadata.description << ")" << std::endl;
        
        // Model loading would happen here
        
        // Add model to loaded models
        m_loadedModels[key] = model;
        
        // Notify callbacks
        for (const auto& callback : m_callbacks) {
            callback.second(type, modelName, true);
        }
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading model: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<Model> ModelServer::getModel(ModelType type, const std::string& modelName) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // Check if initialized
    if (!m_isInitialized) {
        std::cerr << "ModelServer not initialized" << std::endl;
        return nullptr;
    }
    
    // Create key for model
    auto key = std::make_pair(type, modelName);
    
    // Check if model is loaded
    if (m_loadedModels.find(key) == m_loadedModels.end()) {
        // Try to load the model
        if (!loadModel(type, modelName)) {
            return nullptr;
        }
    }
    
    return m_loadedModels[key];
}

bool ModelServer::unloadModel(ModelType type, const std::string& modelName) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // Check if initialized
    if (!m_isInitialized) {
        std::cerr << "ModelServer not initialized" << std::endl;
        return false;
    }
    
    // Create key for model
    auto key = std::make_pair(type, modelName);
    
    // Check if model is loaded
    if (m_loadedModels.find(key) == m_loadedModels.end()) {
        // Model not loaded
        return true;
    }
    
    // Remove model from loaded models
    m_loadedModels.erase(key);
    
    // Notify callbacks
    for (const auto& callback : m_callbacks) {
        callback.second(type, modelName, false);
    }
    
    return true;
}

std::vector<ModelMetadata> ModelServer::getAvailableModels() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::vector<ModelMetadata> models;
    models.reserve(m_availableModels.size());
    
    for (const auto& pair : m_availableModels) {
        models.push_back(pair.second);
    }
    
    return models;
}

std::vector<ModelMetadata> ModelServer::getLoadedModels() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::vector<ModelMetadata> models;
    models.reserve(m_loadedModels.size());
    
    for (const auto& pair : m_loadedModels) {
        const auto& key = pair.first;
        const auto& metadata = m_availableModels.at(key);
        models.push_back(metadata);
    }
    
    return models;
}

size_t ModelServer::getTotalMemoryUsage() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    size_t total = 0;
    for (const auto& pair : m_loadedModels) {
        total += pair.second->getMemoryUsage();
    }
    
    return total;
}

void ModelServer::setMaxMemoryUsage(size_t maxMemoryUsage) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_maxMemoryUsage = maxMemoryUsage;
    
    // If max memory usage is reduced, unload models if necessary
    if (m_maxMemoryUsage > 0) {
        size_t currentUsage = getTotalMemoryUsage();
        if (currentUsage > m_maxMemoryUsage) {
            unloadModelsIfNeeded(0);
        }
    }
}

void ModelServer::enableGPU(bool enable) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_enableGPU = enable;
}

bool ModelServer::isGPUEnabled() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    return m_enableGPU;
}

bool ModelServer::isGPUAvailable() const {
    // Placeholder for actual GPU availability check
    // In a real implementation, we would check if TensorFlow can use GPU
    return false;
}

int ModelServer::registerModelCallback(std::function<void(ModelType, const std::string&, bool)> callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    int id = m_nextCallbackId++;
    m_callbacks[id] = callback;
    return id;
}

void ModelServer::unregisterModelCallback(int callbackId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_callbacks.erase(callbackId);
}

void ModelServer::scanForModels() {
    // Placeholder for actual model scanning
    // In a real implementation, we would scan the models directory for model files
    // and create metadata for each model
    
    // For now, just add some placeholder models
    
    // MusicVAE model
    {
        ModelMetadata metadata;
        metadata.name = "MusicVAE";
        metadata.type = ModelType::MusicVAE;
        metadata.version = "1.0.0";
        metadata.memorySize = 100 * 1024 * 1024; // 100 MB
        metadata.description = "MusicVAE model for pattern generation";
        metadata.isQuantized = true;
        metadata.supportsGPU = true;
        
        m_availableModels[std::make_pair(ModelType::MusicVAE, "")] = metadata;
    }
    
    // GrooVAE model
    {
        ModelMetadata metadata;
        metadata.name = "GrooVAE";
        metadata.type = ModelType::GrooVAE;
        metadata.version = "1.0.0";
        metadata.memorySize = 50 * 1024 * 1024; // 50 MB
        metadata.description = "GrooVAE model for groove modeling";
        metadata.isQuantized = true;
        metadata.supportsGPU = true;
        
        m_availableModels[std::make_pair(ModelType::GrooVAE, "")] = metadata;
    }
    
    // MelodyRNN model
    {
        ModelMetadata metadata;
        metadata.name = "MelodyRNN";
        metadata.type = ModelType::MelodyRNN;
        metadata.version = "1.0.0";
        metadata.memorySize = 30 * 1024 * 1024; // 30 MB
        metadata.description = "MelodyRNN model for melodic completion";
        metadata.isQuantized = true;
        metadata.supportsGPU = true;
        
        m_availableModels[std::make_pair(ModelType::MelodyRNN, "")] = metadata;
    }
}

void ModelServer::unloadModelsIfNeeded(size_t requiredMemory) {
    // Calculate current memory usage
    size_t currentUsage = getTotalMemoryUsage();
    
    // Check if we need to unload models
    if (currentUsage + requiredMemory <= m_maxMemoryUsage) {
        // No need to unload models
        return;
    }
    
    // Calculate how much memory we need to free
    size_t memoryToFree = currentUsage + requiredMemory - m_maxMemoryUsage;
    
    // Get loaded models sorted by last access time (not implemented yet)
    // For now, just unload models until we have enough memory
    
    // Create a copy of the loaded models
    auto loadedModels = m_loadedModels;
    
    // Unload models until we have enough memory
    size_t freedMemory = 0;
    for (const auto& pair : loadedModels) {
        const auto& key = pair.first;
        const auto& model = pair.second;
        
        // Skip if this is the only model
        if (loadedModels.size() == 1) {
            break;
        }
        
        // Unload model
        size_t modelMemory = model->getMemoryUsage();
        unloadModel(key.first, key.second);
        
        // Update freed memory
        freedMemory += modelMemory;
        
        // Check if we have freed enough memory
        if (freedMemory >= memoryToFree) {
            break;
        }
    }
}

} // namespace lmms_magenta
