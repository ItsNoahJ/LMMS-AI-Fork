#include "AIPlugin.h"
#include <iostream>

namespace lmms_magenta {

AIPlugin::AIPlugin(Plugin::Model* parent, const Plugin::Descriptor::SubPluginFeatures::Key* key)
    : Plugin(parent, key)
    , m_isModelLoaded(false) {
    
    // Register callback for model loading
    m_callbackId = ModelServer::getInstance().registerModelCallback(
        [this](ModelType type, const std::string& modelName, bool loaded) {
            // Check if this is our model
            if (type == m_modelType && modelName == m_modelName) {
                m_isModelLoaded = loaded;
                
                // Notify UI that model status has changed
                emit modelStatusChanged(loaded);
            }
        }
    );
}

AIPlugin::~AIPlugin() {
    // Unregister callback
    ModelServer::getInstance().unregisterModelCallback(m_callbackId);
}

bool AIPlugin::loadModel(ModelType type, const std::string& modelName) {
    // Save model info
    m_modelType = type;
    m_modelName = modelName;
    
    // Try to load the model
    bool success = ModelServer::getInstance().loadModel(type, modelName);
    
    // Update model loaded flag
    m_isModelLoaded = success;
    
    // Notify UI that model status has changed
    emit modelStatusChanged(success);
    
    return success;
}

bool AIPlugin::isModelLoaded() const {
    return m_isModelLoaded;
}

void AIPlugin::unloadModel() {
    if (m_isModelLoaded) {
        // Unload the model
        ModelServer::getInstance().unloadModel(m_modelType, m_modelName);
        
        // Update model loaded flag
        m_isModelLoaded = false;
        
        // Notify UI that model status has changed
        emit modelStatusChanged(false);
    }
}

std::vector<ModelMetadata> AIPlugin::getAvailableModels() const {
    return ModelServer::getInstance().getAvailableModels();
}

std::shared_ptr<Model> AIPlugin::getModel() {
    if (!m_isModelLoaded) {
        return nullptr;
    }
    
    return ModelServer::getInstance().getModel(m_modelType, m_modelName);
}

void AIPlugin::saveSettings(QDomDocument& doc, QDomElement& element) {
    // Save model type and name
    element.setAttribute("modelType", static_cast<int>(m_modelType));
    element.setAttribute("modelName", QString::fromStdString(m_modelName));
    
    // Save additional plugin settings
    savePluginSettings(doc, element);
}

void AIPlugin::loadSettings(const QDomElement& element) {
    // Load model type and name
    m_modelType = static_cast<ModelType>(element.attribute("modelType").toInt());
    m_modelName = element.attribute("modelName").toStdString();
    
    // Try to load the model
    if (!m_modelName.empty()) {
        loadModel(m_modelType, m_modelName);
    }
    
    // Load additional plugin settings
    loadPluginSettings(element);
}

void AIPlugin::savePluginSettings(QDomDocument& doc, QDomElement& element) {
    // Base implementation does nothing
    // Derived classes should override this to save their settings
}

void AIPlugin::loadPluginSettings(const QDomElement& element) {
    // Base implementation does nothing
    // Derived classes should override this to load their settings
}

} // namespace lmms_magenta
