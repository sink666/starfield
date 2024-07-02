#include "env.h"

int main()
{
    Environment env;
    if((env.startup()) == -1)
    {
        std::cout << "error during startup: " << env.getError() << std::endl;
        std::cout << "exiting ..." << std::endl;
        return 1;
    }

    env.run();

    env.shutdown();

    return 0;
}