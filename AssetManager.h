#pragma once
#include <unordered_map>
#include <string>
#include <stdint.h>

// specialized dictionary file

struct Texture {
    uint32_t* pixels;
    int width;
    int height;
};

class AssetManager {
private:
    std::unordered_map<std::string, Texture> m_textures;

public:
    // destructor to have no memory leak
    ~AssetManager() {
        for (auto& pair : m_textures) {
            delete[] pair.second.pixels; // Free the RAM
        }
        m_textures.clear();
    }

    // engine-side shape generation for placeholders
    void CreateColorSquare(const std::string& name, int size, uint32_t color) {
        if (m_textures.count(name)) return; // avoids duplicate allocations for the same asset name.

        uint32_t* pix = new uint32_t[size * size];
        for (int i = 0; i < size * size; i++) {
            pix[i] = color;
        }
        m_textures[name] = { pix, size, size };
    }

    // retrieve asset data by name
    Texture* GetTexture(const std::string& name) {
        if (m_textures.count(name)) {
            return &m_textures[name];
        }
        return nullptr;
    }
};