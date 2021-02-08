/* Extra Global ;-) */
bool IN_DEBUG_MODE = false;

#include <Engine.h>

int main(int argc, char** argv) {
    if (argc >= 2 && std::strcmp(argv[1], "-d") == 0) IN_DEBUG_MODE = true;
    Engine engine(1600, 900);
    while(engine.running) {
        engine.startFrame();
        engine.events();
        engine.update();
        engine.render();
    }
    return 0;
}