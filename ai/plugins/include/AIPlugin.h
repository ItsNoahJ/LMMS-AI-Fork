#pragma once

#include <memory>
#include <string>
#include <functional>

// Include LMMS Plugin base class
#include "Plugin.h"
#include "Model.h"

// Include our model server
#include "../../model_serving/include/ModelServer.h"

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
     * @brief Get the model used by this plugin
     * @return Shared pointer to the model
     */
    std::shared_ptr<Model> getModel();
    
    /**
     * @brief Set the model used by this plugin
     * @param model Shared pointer to the model
     */
    void setModel(std::shared_ptr<Model> model);
    
    /**
     * @brief Load the model used by this plugin
     * @return True if loading was successful
     */
    bool loadModel();
    
    /**
     * @brief Unload the model used by this plugin
     * @return True if unloading was successful
     */
    bool unloadModel();
    
    /**
     * @brief Check if the model is loaded
     * @return True if the model is loaded
     */
    bool isModelLoaded() const;
    
    /**
     * @brief Register a callback for model loading events
     * @param callback Function to call when the model is loaded or unloaded
     * @return ID of the registered callback
     */
    int registerModelCallback(std::function<void(bool)> callback);
    
    /**
     * @brief Unregister a model callback
     * @param callbackId ID of the callback to unregister
     */
    void unregisterModelCallback(int callbackId);
    
protected:
    /**
     * @brief Handle model loading events
     * @param type Model type
     * @param name Model name
     * @param loaded Whether the model was loaded or unloaded
     */
    void handleModelEvent(ModelType type, const std::string& name, bool loaded);
    
private:
    // Model used by this plugin
    std::shared_ptr<Model> m_model;
    
    // Whether the plugin is initialized
    bool m_isInitialized;
    
    // Callback ID for model server events
    int m_modelCallbackId;
    
    // Callbacks for model loading events
    std::map<int, std::function<void(bool)>> m_callbacks;
    int m_nextCallbackId;
};

} // namespace lmms_magenta
