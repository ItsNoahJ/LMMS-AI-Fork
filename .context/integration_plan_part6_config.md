# LMMS-Magenta Comprehensive Integration Plan - Part 6: Configuration System

## Unified Configuration System

A centralized configuration system is essential for managing application settings, user preferences, and environment-specific configurations. Based on research into configuration management best practices for cross-platform C++ applications, we'll implement a comprehensive configuration system that addresses the unique needs of the LMMS-Magenta project.

### Configuration System Requirements

The configuration system must support:

1. **Hierarchical Configuration**: Settings organized in a hierarchical structure
2. **Type Safety**: Strong typing for configuration values
3. **Default Values**: Sensible defaults for all settings
4. **Validation**: Validation of configuration values
5. **Persistence**: Saving and loading configurations
6. **Schema Evolution**: Handling changes to the configuration schema
7. **Cross-Platform Compatibility**: Working consistently across all supported platforms
8. **User Interface Integration**: Easy integration with UI components

### Configuration Format

After evaluating various configuration formats, we've selected **JSON** as the primary format for the following reasons:

1. Human-readable and editable
2. Wide support across programming languages
3. Schema validation support
4. Good performance characteristics
5. Native support in many libraries

We'll use the nlohmann/json library for C++ JSON handling:

```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;
```

### Configuration Structure

The configuration will be organized hierarchically:

```json
{
  "general": {
    "language": "en",
    "theme": "dark",
    "autoSave": true,
    "autoSaveInterval": 300
  },
  "audio": {
    "sampleRate": 44100,
    "bufferSize": 512,
    "driver": "asio"
  },
  "ai": {
    "modelPath": "models/",
    "enableGPU": true,
    "maxThreads": 4,
    "cacheSize": 1024
  },
  "plugins": {
    "musicVAE": {
      "temperature": 0.8,
      "batchSize": 4
    },
    "grooveVAE": {
      "temperature": 0.5,
      "strength": 0.8
    }
  }
}
```

### Configuration Manager Implementation

The configuration system will be implemented as a singleton class:

```cpp
// In include/lmms-magenta/config/config_manager.h
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

namespace lmms_magenta {
namespace config {

class ConfigManager {
public:
    static ConfigManager& getInstance();
    
    // Delete copy and move constructors and assign operators
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;
    
    // Load configuration from file
    bool loadFromFile(const std::string& filePath);
    
    // Save configuration to file
    bool saveToFile(const std::string& filePath);
    
    // Get value with default
    template<typename T>
    T getValue(const std::string& path, const T& defaultValue) const;
    
    // Set value
    template<typename T>
    void setValue(const std::string& path, const T& value);
    
    // Check if path exists
    bool hasPath(const std::string& path) const;
    
    // Reset to defaults
    void resetToDefaults();
    
    // Get entire configuration
    const nlohmann::json& getConfig() const;
    
private:
    ConfigManager();
    ~ConfigManager() = default;
    
    // Parse path (e.g., "general.theme" -> ["general", "theme"])
    std::vector<std::string> parsePath(const std::string& path) const;
    
    // Get JSON value at path
    nlohmann::json getValueAtPath(const std::string& path) const;
    
    // Set JSON value at path
    void setValueAtPath(const std::string& path, const nlohmann::json& value);
    
    // Load default configuration
    void loadDefaults();
    
    nlohmann::json config_;
    nlohmann::json defaults_;
    mutable std::mutex mutex_;
};

} // namespace config
} // namespace lmms_magenta
```

### Configuration Access

To ensure type safety and ease of use, we'll provide strongly-typed accessors for configuration values:

```cpp
// In src/config/config_manager.cpp
template<typename T>
T ConfigManager::getValue(const std::string& path, const T& defaultValue) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    try {
        nlohmann::json value = getValueAtPath(path);
        return value.get<T>();
    } catch (const std::exception& e) {
        return defaultValue;
    }
}

template<typename T>
void ConfigManager::setValue(const std::string& path, const T& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    setValueAtPath(path, value);
}
```

### Configuration Schema Validation

To ensure configuration validity, we'll implement schema validation using JSON Schema:

```cpp
// In src/config/config_validator.cpp
#include "config/config_validator.h"
#include <nlohmann/json-schema.hpp>

namespace lmms_magenta {
namespace config {

bool ConfigValidator::validate(const nlohmann::json& config, std::string& error) {
    try {
        nlohmann::json_schema::json_validator validator;
        validator.set_root_schema(getSchema());
        validator.validate(config);
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

nlohmann::json ConfigValidator::getSchema() {
    return nlohmann::json::parse(R"({
        "$schema": "http://json-schema.org/draft-07/schema#",
        "type": "object",
        "properties": {
            "general": {
                "type": "object",
                "properties": {
                    "language": { "type": "string" },
                    "theme": { "type": "string" },
                    "autoSave": { "type": "boolean" },
                    "autoSaveInterval": { "type": "integer", "minimum": 10 }
                }
            },
            "audio": {
                "type": "object",
                "properties": {
                    "sampleRate": { "type": "integer", "enum": [44100, 48000, 96000] },
                    "bufferSize": { "type": "integer", "enum": [64, 128, 256, 512, 1024, 2048] },
                    "driver": { "type": "string" }
                }
            },
            "ai": {
                "type": "object",
                "properties": {
                    "modelPath": { "type": "string" },
                    "enableGPU": { "type": "boolean" },
                    "maxThreads": { "type": "integer", "minimum": 1 },
                    "cacheSize": { "type": "integer", "minimum": 0 }
                }
            },
            "plugins": {
                "type": "object",
                "properties": {
                    "musicVAE": {
                        "type": "object",
                        "properties": {
                            "temperature": { "type": "number", "minimum": 0.0, "maximum": 1.0 },
                            "batchSize": { "type": "integer", "minimum": 1 }
                        }
                    },
                    "grooveVAE": {
                        "type": "object",
                        "properties": {
                            "temperature": { "type": "number", "minimum": 0.0, "maximum": 1.0 },
                            "strength": { "type": "number", "minimum": 0.0, "maximum": 1.0 }
                        }
                    }
                }
            }
        }
    })");
}

} // namespace config
} // namespace lmms_magenta
```

### Configuration UI Integration

To integrate the configuration system with the UI, we'll implement a configuration dialog:

```cpp
// In include/lmms-magenta/ui/config_dialog.h
#pragma once

#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

namespace lmms_magenta {
namespace ui {

class ConfigDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit ConfigDialog(QWidget* parent = nullptr);
    ~ConfigDialog() override;
    
private slots:
    void saveConfig();
    void resetToDefaults();
    
private:
    void setupUi();
    void loadConfig();
    
    QTabWidget* tabWidget_;
    
    // General tab
    QComboBox* languageComboBox_;
    QComboBox* themeComboBox_;
    QCheckBox* autoSaveCheckBox_;
    QSpinBox* autoSaveIntervalSpinBox_;
    
    // Audio tab
    QComboBox* sampleRateComboBox_;
    QComboBox* bufferSizeComboBox_;
    QComboBox* driverComboBox_;
    
    // AI tab
    QLineEdit* modelPathLineEdit_;
    QCheckBox* enableGPUCheckBox_;
    QSpinBox* maxThreadsSpinBox_;
    QSpinBox* cacheSizeSpinBox_;
    
    // Plugin tabs
    QDoubleSpinBox* musicVAETemperatureSpinBox_;
    QSpinBox* musicVAEBatchSizeSpinBox_;
    QDoubleSpinBox* grooveVAETemperatureSpinBox_;
    QDoubleSpinBox* grooveVAEStrengthSpinBox_;
};

} // namespace ui
} // namespace lmms_magenta
```

### Configuration Migration

To handle changes to the configuration schema, we'll implement a migration system:

```cpp
// In include/lmms-magenta/config/config_migrator.h
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <functional>

namespace lmms_magenta {
namespace config {

class ConfigMigrator {
public:
    using MigrationFunction = std::function<void(nlohmann::json&)>;
    
    ConfigMigrator();
    
    // Register a migration function for a specific version
    void registerMigration(int fromVersion, int toVersion, MigrationFunction migrationFn);
    
    // Migrate configuration from one version to another
    bool migrate(nlohmann::json& config, int fromVersion, int toVersion);
    
private:
    struct Migration {
        int fromVersion;
        int toVersion;
        MigrationFunction migrationFn;
    };
    
    std::vector<Migration> migrations_;
};

} // namespace config
} // namespace lmms_magenta
```

### Environment-Specific Configurations

To support different environments (development, production, etc.), we'll implement environment-specific configurations:

```cpp
// In src/config/config_manager.cpp
bool ConfigManager::loadFromFile(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    try {
        // Load base configuration
        std::ifstream baseFile(filePath);
        if (baseFile.is_open()) {
            baseFile >> config_;
        } else {
            return false;
        }
        
        // Load environment-specific configuration if available
        std::string env = getEnvironment();
        if (!env.empty()) {
            std::string envFilePath = filePath.substr(0, filePath.find_last_of('.')) + 
                                      "." + env + 
                                      filePath.substr(filePath.find_last_of('.'));
            
            std::ifstream envFile(envFilePath);
            if (envFile.is_open()) {
                nlohmann::json envConfig;
                envFile >> envConfig;
                
                // Merge environment-specific configuration
                mergeJson(config_, envConfig);
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::string ConfigManager::getEnvironment() {
    // Check environment variable
    const char* env = std::getenv("LMMS_MAGENTA_ENV");
    if (env) {
        return env;
    }
    
    // Default to development in debug builds, production in release builds
#ifdef NDEBUG
    return "production";
#else
    return "development";
#endif
}
```

This comprehensive configuration system will provide a solid foundation for managing application settings, user preferences, and environment-specific configurations in the LMMS-Magenta project.
