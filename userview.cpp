#include "userview.h"

int UserView::start_userview(int width, int height)
{
    view_width = width;
    view_height = height;

    auto *window_ptr = SDL_CreateWindow("starfield", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    if(window_ptr == nullptr)
        return -1;

    window.reset(window_ptr);

    return 0;
}

void UserView::kill_userview()
{
    window.release();
}

SDL_Window *UserView::get_windowptr()
{
    return window.get();
}

int UserView::get_vw()
{
    return view_width;
}

int UserView::get_vh()
{
    return view_height;
}
