#include "env.h"

int Environment::startup()
{
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)) == -1)
    {
        errMsg.assign("failed to initialize SDL2");
        goto init_error;
    }

    if((uv.start_userview(800, 600)) == -1)
    {
        errMsg.assign("failure to initialize userview");
        goto init_error;
    }
    
    if((rd.start_renderer(uv.get_windowptr(), uv.get_vw(), uv.get_vh())) == -1)
    {
        errMsg.assign("failure to initialize renderer");
        goto init_error;
    }

    sf.start_effect(uv.get_vw(), uv.get_vh());

    return 0;

init_error:
    return -1;
}

void Environment::run()
{
    SDL_Event ev;

    while(1) 
    {
        if(SDL_PollEvent(&ev)) 
        {
            if((ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE) || 
            (ev.key.keysym.scancode == SDL_SCANCODE_Q) ||
            (ev.type == SDL_QUIT)) {
                break;
            }
        }

        sf.run_effect();
        rd.do_render(sf.get_data(), sf.get_pitch());
        SDL_Delay(30);
    }
}

std::string Environment::getError()
{
    return errMsg;
}

void Environment::shutdown()
{
    uv.kill_userview();
    rd.kill_renderer();
}