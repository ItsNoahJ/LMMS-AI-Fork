#include <gtest/gtest.h>
#include "model_serving/ModelServer.h"
#include <chrono>
#include <iostream>
#include <filesystem>

using namespace lmms_magenta;

class ModelLoadingBenchmark : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize model server
        std::string modelsDir = "../models";
        bool success = ModelServer::getInstance().initialize(modelsDir, 1024 * 1024 * 1024, false);
        
        // If initialization fails, create the directory and try again
        if (!success) {
            std::filesystem::create_directories(modelsDir);
            success = ModelServer::getInstance().initialize(modelsDir, 1024 * 1024 * 1024, false);
            ASSERT_TRUE(success) << "Failed to initialize ModelServer";
        }
    }
    
    void TearDown() override {
        // Clean up any loaded models
        auto loadedModels = ModelServer::getInstance().getLoadedModels();
        for (const auto& model : loadedModels) {
            ModelServer::getInstance().unloadModel(model.type, model.name);
        }
    }
    
    // Helper method to measure execution time
    template <typename Func>
    double measureExecutionTime(Func func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> duration = end - start;
        return duration.count();
    }
};

// Benchmark model loading time
TEST_F(ModelLoadingBenchmark, MusicVAELoadingTime) {
    // Measure time to load MusicVAE model
    double loadTime = measureExecutionTime([&]() {
        ModelServer::getInstance().loadModel(ModelType::MusicVAE, "");
    });
    
    std::cout << "MusicVAE model loading time: " << loadTime << " ms" << std::endl;
    
    // Get the model
    auto model = ModelServer::getInstance().getModel(ModelType::MusicVAE, "");
    ASSERT_NE(model, nullptr);
    
    // Measure time to unload model
    double unloadTime = measureExecutionTime([&]() {
        ModelServer::getInstance().unloadModel(ModelType::MusicVAE, "");
    });
    
    std::cout << "MusicVAE model unloading time: " << unloadTime << " ms" << std::endl;
}

// Benchmark model loading time
TEST_F(ModelLoadingBenchmark, GrooVAELoadingTime) {
    // Measure time to load GrooVAE model
    double loadTime = measureExecutionTime([&]() {
        ModelServer::getInstance().loadModel(ModelType::GrooVAE, "");
    });
    
    std::cout << "GrooVAE model loading time: " << loadTime << " ms" << std::endl;
    
    // Get the model
    auto model = ModelServer::getInstance().getModel(ModelType::GrooVAE, "");
    ASSERT_NE(model, nullptr);
    
    // Measure time to unload model
    double unloadTime = measureExecutionTime([&]() {
        ModelServer::getInstance().unloadModel(ModelType::GrooVAE, "");
    });
    
    std::cout << "GrooVAE model unloading time: " << unloadTime << " ms" << std::endl;
}

// Benchmark model inference time
TEST_F(ModelLoadingBenchmark, MusicVAEInferenceTime) {
    // Load MusicVAE model
    ASSERT_TRUE(ModelServer::getInstance().loadModel(ModelType::MusicVAE, ""));
    
    // Get the model
    auto model = ModelServer::getInstance().getModel(ModelType::MusicVAE, "");
    ASSERT_NE(model, nullptr);
    
    // Cast to MusicVAEModel
    auto musicVAEModel = std::dynamic_pointer_cast<MusicVAEModel>(model);
    ASSERT_NE(musicVAEModel, nullptr);
    
    // Measure time to generate a pattern
    std::vector<MidiNote> notes;
    double inferenceTime = measureExecutionTime([&]() {
        musicVAEModel->sample(notes);
    });
    
    std::cout << "MusicVAE pattern generation time: " << inferenceTime << " ms" << std::endl;
}

// Benchmark model inference time
TEST_F(ModelLoadingBenchmark, GrooVAEInferenceTime) {
    // Load GrooVAE model
    ASSERT_TRUE(ModelServer::getInstance().loadModel(ModelType::GrooVAE, ""));
    
    // Get the model
    auto model = ModelServer::getInstance().getModel(ModelType::GrooVAE, "");
    ASSERT_NE(model, nullptr);
    
    // Cast to GrooVAEModel
    auto grooVAEModel = std::dynamic_pointer_cast<GrooVAEModel>(model);
    ASSERT_NE(grooVAEModel, nullptr);
    
    // Create a MIDI sequence
    std::vector<MidiNote> inputNotes;
    
    // Add some notes (a simple 4/4 beat)
    for (int i = 0; i < 16; ++i) {
        MidiNote note;
        note.pitch = 36; // Kick drum
        note.velocity = 100;
        note.startTime = i * 0.25f;
        note.endTime = note.startTime + 0.1f;
        
        inputNotes.push_back(note);
        
        // Add snare on beats 2 and 4
        if (i % 4 == 2) {
            MidiNote snare;
            snare.pitch = 38; // Snare drum
            snare.velocity = 100;
            snare.startTime = i * 0.25f;
            snare.endTime = snare.startTime + 0.1f;
            
            inputNotes.push_back(snare);
        }
        
        // Add hi-hat on every 8th note
        MidiNote hihat;
        hihat.pitch = 42; // Closed hi-hat
        hihat.velocity = 80;
        hihat.startTime = i * 0.25f;
        hihat.endTime = hihat.startTime + 0.1f;
        
        inputNotes.push_back(hihat);
    }
    
    // Measure time to apply groove
    std::vector<MidiNote> outputNotes;
    double inferenceTime = measureExecutionTime([&]() {
        grooVAEModel->applyGroove(inputNotes, outputNotes);
    });
    
    std::cout << "GrooVAE groove application time: " << inferenceTime << " ms" << std::endl;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
