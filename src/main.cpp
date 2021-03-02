#include <string>

/* Extra Global ;-) */
bool IN_DEBUG_MODE = false;
std::string MODEL_NAME = "FOO";

#include <Engine.h>

int main(int argc, char** argv) {

    std::vector args(argv + 1, argv + argc);
    for (char* arg : args) {
        if (std::strcmp(arg, "-d") == 0) IN_DEBUG_MODE = true;
        else MODEL_NAME = std::string(arg);
    }

    Engine engine(1600, 900);
    while(engine.running) {
        engine.startFrame();
        engine.events();
        engine.update();
        engine.render();
    }
    return 0;
}