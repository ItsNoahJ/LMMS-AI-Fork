#include "AIPlugin.h"
#include <iostream>

namespace lmms_magenta {

// Constructor
AIPlugin::AIPlugin(lmms::Model* parent, const lmms::Plugin::Descriptor* descriptor)
    : lmms::Plugin(parent, descriptor)
    , m_isInitialized(false)
    , m_modelCallbackId(-1)
    , m_modelCallback(nullptr) {
}

// Destructor
AIPlugin::~AIPlugin() {
    // Unregister model callback
    if (m_modelCallbackId >= 0) {
        ModelServer::getInstance().unregisterModelCallback(m_modelCallbackId);
        m_modelCallbackId = -1;
    }
    
    // Unload model
    unloadModel();
}

// Initialize the plugin
bool AIPlugin::initialize() {
    if (m_isInitialized) {
        return true;
    }
    
    // Register model callback
    m_modelCallbackId = ModelServer::getInstance().registerModelCallback(
        [this](ModelType type, const std::string& name, bool loaded) {
            this->handleModelEvent(type, name, loaded);
        }
    );
    
    m_isInitialized = true;
    return true;
}

// Check if the plugin is initialized
bool AIPlugin::isInitialized() const {
    return m_isInitialized;
}

// Load the AI model
bool AIPlugin::loadModel() {
    if (!m_isInitialized) {
        std::cerr << "AIPlugin is not initialized" << std::endl;
        return false;
    }
    
    // Get model type and name
    ModelType type = getModelType();
    std::string name = getModelName();
    
    // Load model
    return ModelServer::getInstance().loadModel(type, name);
}

// Unload the AI model
bool AIPlugin::unloadModel() {
    if (!m_isInitialized) {
        return false;
    }
    
    // Get model type and name
    ModelType type = getModelType();
    std::string name = getModelName();
    
    // Unload model
    return ModelServer::getInstance().unloadModel(type, name);
}

// Check if the model is loaded
bool AIPlugin::isModelLoaded() const {
    if (!m_isInitialized) {
        return false;
    }
    
    // Get model
    return getModel() != nullptr;
}

// Get the model
std::shared_ptr<Model> AIPlugin::getModel() {
    if (!m_isInitialized) {
        return nullptr;
    }
    
    // Get model type and name
    ModelType type = getModelType();
    std::string name = getModelName();
    
    // Get model
    return ModelServer::getInstance().getModel(type, name);
}

// Set a callback for model loading events
void AIPlugin::setModelCallback(std::function<void(bool)> callback) {
    m_modelCallback = callback;
}

// Handle model loading events
void AIPlugin::handleModelEvent(ModelType type, const std::string& name, bool loaded) {
    // Check if this event is for our model
    if (type == getModelType() && name == getModelName()) {
        // Call callback if set
        if (m_modelCallback) {
            m_modelCallback(loaded);
        }
    }
}

} // namespace lmms_magenta
