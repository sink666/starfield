#include <SDL2/SDL.h>
#include "userview.h"
#include "renderer.h"
#include "starfield.h"

class Environment
{
    StarfieldEffect sf;
    UserView uv;
    Renderer rd;
    std::string errMsg;

public:
    Environment(){};
    ~Environment(){};

    int startup();
    void run();
    void shutdown();

    std::string getError();
};