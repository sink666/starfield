#include <SDL2/SDL.h>
#include <iostream>

class UserView
{   
    struct SdlWindowDestroyer
    {
        void operator()(SDL_Window *window) noexcept
        {
            SDL_DestroyWindow(window);
        }
    };
    
    std::unique_ptr<SDL_Window, SdlWindowDestroyer> window;
    
    int view_width;
    int view_height;

public:
    UserView(){};
    ~UserView(){};
    int start_userview(int width, int height);
    void kill_userview();

    SDL_Window *get_windowptr();
    int get_vw();
    int get_vh();
};