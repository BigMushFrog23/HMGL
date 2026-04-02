#pragma once
#include "Registry.h"
#include "GraphicsLib.h"

// work-system file

namespace EngineSystems
{
    // instead of using a draw function for each individual entity, we render everything at once to speed up the process.
    inline void Render(Registry& registry)
    {
        // loop through the slots in the vectors
        for (Entity i = 0; i < (Entity)registry.GetEntityCount(); i++)
        {
            // only draw if the entity at index 'i' has a position AND an image.
            if (registry.hasTransform[i] && registry.hasSprite[i])
            {
                // we return a referance to the struct
                TransformComponent& transform = registry.transforms[i];
                SpriteComponent& sprite = registry.sprites[i];

                for (int y = 0; y < sprite.height; y++)
                {
                    for (int x = 0; x < sprite.width; x++)
                    {
                        uint32_t color = sprite.pixels[y * sprite.width + x];

                        // check for Alpha Transparency (Bits 24-31)
                        if ((color >> 24) != 0x00)
                        {
                            // convert sprite date into screen coordinates.
                            PutPixel((int)transform.x + x, (int)transform.y + y, color);
                        }
                    }
                }
            }
        }
    }
}