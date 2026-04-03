#pragma once
#include <map>
#include <string>
#include <stdint.h>
#include <memory>
#include <cstdlib>

// specialized dictionary file

struct Texture {
    std::unique_ptr<uint32_t[], void(*)(void*)> pixels{ nullptr, std::free };
    int width;
    int height;
};

class AssetManager {
private:
    // map keeps things ordered, which is the norm for engines / tools.
    // unordered_map is faster but not in order.
    std::map<std::string, Texture> m_textures;

public:
    // no destructors are needed since unique_ptr and map free themselves when needed.

    // engine-side shape generation for placeholders
    void CreateColorSquare(const std::string& name, int size, uint32_t color) {
        if (m_textures.count(name)) return; // avoids duplicate allocations for the same asset name.

        // static_cast is used for converting to the type wanted <type>
        // malloc is used in the way "(type*)std::malloc)(amount of space in bytes)
        // (size) * size instead of (size * size) because we cast it to one first in case it overflows.
        size_t sizeOfPixels = static_cast<size_t>(size) * size;
        uint32_t* allocatedPixelMemory = (uint32_t*)std::malloc(sizeOfPixels * sizeof(uint32_t));

        if (!allocatedPixelMemory) return; // if we don't find the amount of RAM needed, cancel.

        for (size_t i = 0; i < sizeOfPixels; i++)
        {
            allocatedPixelMemory[i] = color;
        }

        Texture tex;
        // resets whatever value tex.pixels had, with allocatedPixelMemory
        tex.pixels.reset(allocatedPixelMemory);     
        tex.width = size;
        tex.height = size;

        m_textures[name] = std::move(tex); // we move the texture into the map
    }

    // retrieve asset data by name
    // "it" is the naming convention for iterators, iterating through something.
    Texture* GetTexture(const std::string& name) {

        // we use find to search and store the position of our iterator once.
        auto it = m_textures.find(name);
        if (it != m_textures.end())
        {
            // if we don't reach the end, it means we found the iterator.
            // we then take the value, (first -> key, second -> value)
            return &it->second;
        }
        return nullptr;
    }
};