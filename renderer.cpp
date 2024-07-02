#include "renderer.h"

int Renderer::start_renderer(SDL_Window *winptr, int width, int height)
{
    target_rect = { 0, 0, width, height };
    SDL_Renderer *renderer_ptr = nullptr;
    SDL_Texture *frameBuffer_ptr = nullptr;

    renderer_ptr = SDL_CreateRenderer(winptr, -1, SDL_RENDERER_TARGETTEXTURE);
    if(renderer_ptr == nullptr)
        goto renderinit_error;
    
    renderer.reset(renderer_ptr);

    frameBuffer_ptr = SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA32, 
        SDL_TEXTUREACCESS_TARGET, width, height);
    if(frameBuffer_ptr == nullptr)
        goto renderinit_error;
    
    frameBuffer.reset(frameBuffer_ptr);

    // scaling setup to deal with high dpi
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer.get(), width, height);

    return 0;

renderinit_error:
    return -1;
}

void Renderer::do_render(uint32_t *pixels, int pitch)
{
    SDL_UpdateTexture(frameBuffer.get(), nullptr, pixels, pitch);
    SDL_RenderCopy(renderer.get(), frameBuffer.get(), nullptr, nullptr);
    SDL_RenderPresent(renderer.get());
}

void Renderer::kill_renderer()
{
    frameBuffer.release();
    renderer.release();
}