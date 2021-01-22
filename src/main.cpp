/** 
 * TODO:
 * Convert calls to DSA equiv
 * Actually everything here: https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions
*/

const char* texname;

#include <Engine.h>
int main(int argc, char** argv) {
    std::vector<char*> arguments;
    arguments.assign(argv, argv + argc);
    for (char* arg : arguments) {
        std::cout << arg << "\n";
    }
    if (argc == 2) texname = argv[1];
    else texname = "unspecified_image.png";
    Engine engine(1600, 900);
    while(engine.running) {
        engine.startFrame();
        engine.events();
        engine.update();
        engine.render();
    }
    return 0;
}