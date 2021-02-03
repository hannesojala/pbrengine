/** 
 * TODO:
 * Convert calls to DSA equiv
 * Actually everything here: https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions
*/

const char* TEXTURE_FILENAME = "FILE_NOT_SPECIFIED";
bool IN_DEBUG_MODE = false;

#include <Engine.h>
int main(int argc, char** argv) {
    std::vector<char*> arguments;
    arguments.assign(argv, argv + argc);
    for (char* arg : arguments) {
        if (std::strcmp(arg, "-d") == 0) IN_DEBUG_MODE = true;
        else TEXTURE_FILENAME = arg;
    }
    SDL_Init(SDL_INIT_VIDEO); SDL_Quit();
    Engine engine(1600, 900);
    while(engine.running) {
        engine.startFrame();
        engine.events();
        engine.update();
        engine.render();
    }
    return 0;
}