#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

class Renderer
{
    struct SdlTextureDestroyer
    {
        void operator()(SDL_Texture *texture) noexcept
        {
            SDL_DestroyTexture(texture);
        }
    };

    struct SdlRendererDestroyer
    {
        void operator()(SDL_Renderer *renderer) noexcept
        {
            SDL_DestroyRenderer(renderer);
        }
    };

    std::unique_ptr<SDL_Texture, SdlTextureDestroyer> frameBuffer;
    std::unique_ptr<SDL_Renderer, SdlRendererDestroyer> renderer;
    SDL_Rect target_rect;

public:
    int start_renderer(SDL_Window *winptr, int width, int height);
    void do_render(uint32_t *pixels, int pitch);
    void kill_renderer();
};