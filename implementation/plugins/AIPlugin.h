#ifndef LMMS_MAGENTA_AI_PLUGIN_H
#define LMMS_MAGENTA_AI_PLUGIN_H

#include <memory>
#include <string>
#include <functional>

// Include LMMS Plugin base class
#include "Plugin.h"
#include "Model.h"

// Include our model server
#include "../model_serving/ModelServer.h"

namespace lmms_magenta {

/**
 * @brief Base class for all AI plugins
 * 
 * This class extends the LMMS Plugin class and provides common functionality
 * for AI-powered plugins, such as model loading and parameter binding.
 */
class AIPlugin : public lmms::Plugin {
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent Parent model
     * @param descriptor Plugin descriptor
     */
    AIPlugin(lmms::Model* parent, const lmms::Plugin::Descriptor* descriptor);
    
    /**
     * @brief Destructor
     */
    ~AIPlugin() override;
    
    /**
     * @brief Initialize the plugin
     * @return True if initialization was successful
     */
    virtual bool initialize();
    
    /**
     * @brief Check if the plugin is initialized
     * @return True if the plugin is initialized
     */
    virtual bool isInitialized() const;
    
    /**
     * @brief Get the model type used by this plugin
     * @return Model type
     */
    virtual ModelType getModelType() const = 0;
    
    /**
     * @brief Get the model name used by this plugin
     * @return Model name
     */
    virtual std::string getModelName() const = 0;
    
    /**
     * @brief Load the AI model
     * @return True if loading was successful
     */
    virtual bool loadModel();
    
    /**
     * @brief Unload the AI model
     * @return True if unloading was successful
     */
    virtual bool unloadModel();
    
    /**
     * @brief Check if the model is loaded
     * @return True if the model is loaded
     */
    virtual bool isModelLoaded() const;
    
    /**
     * @brief Get the model
     * @return Shared pointer to the model, or nullptr if not loaded
     */
    virtual std::shared_ptr<Model> getModel();
    
    /**
     * @brief Set a callback for model loading events
     * @param callback Function to call when the model is loaded or unloaded
     */
    virtual void setModelCallback(std::function<void(bool)> callback);
    
protected:
    /**
     * @brief Handle model loading events
     * @param type Model type
     * @param name Model name
     * @param loaded Whether the model was loaded or unloaded
     */
    virtual void handleModelEvent(ModelType type, const std::string& name, bool loaded);
    
private:
    // Initialization state
    bool m_isInitialized;
    
    // Model callback ID
    int m_modelCallbackId;
    
    // Model callback
    std::function<void(bool)> m_modelCallback;
};

} // namespace lmms_magenta

#endif // LMMS_MAGENTA_AI_PLUGIN_H
