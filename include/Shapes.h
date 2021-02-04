#include <glad/glad.h>
#include <DSA_things.h>

using namespace glm;

namespace CubeFlat {
    GLfloat size[] = {1.0, 1.0, 1.0};
    vertex vertices[] = {
        vertex{    vec3{-size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 1.0}    },
        vertex{    vec3{-size[0],-size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },
        vertex{    vec3{ size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },
        vertex{    vec3{ size[0],-size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 0.0}    },
        vertex{    vec3{ size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },
        vertex{    vec3{-size[0],-size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },
        vertex{    vec3{-size[0], size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 1.0}    },  
        vertex{    vec3{-size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },  
        vertex{    vec3{-size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },  
        vertex{    vec3{-size[0],-size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 0.0}    },  
        vertex{    vec3{-size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },  
        vertex{    vec3{-size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },  
        vertex{    vec3{ size[0], size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 1.0}    },  
        vertex{    vec3{ size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },  
        vertex{    vec3{-size[0], size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },  
        vertex{    vec3{-size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 0.0}    },  
        vertex{    vec3{-size[0], size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },  
        vertex{    vec3{ size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },  
        vertex{    vec3{ size[0],-size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },  
        vertex{    vec3{ size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 0.0}    },  
        vertex{    vec3{ size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 1.0}    },  
        vertex{    vec3{ size[0], size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },  
        vertex{    vec3{ size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 1.0}    },  
        vertex{    vec3{ size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 0.0}    },  
        vertex{    vec3{-size[0], size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 1.0}    },   
        vertex{    vec3{-size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },   
        vertex{    vec3{ size[0], size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },   
        vertex{    vec3{ size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 0.0}    },   
        vertex{    vec3{ size[0], size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },   
        vertex{    vec3{-size[0], size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },   
        vertex{    vec3{ size[0],-size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 1.0}    },  
        vertex{    vec3{-size[0],-size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    },  
        vertex{    vec3{ size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },  
        vertex{    vec3{-size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 0.0}    },  
        vertex{    vec3{ size[0],-size[1],-size[2]},    vec3{0.0, 0.0, 0.0},    vec2{0.0, 0.0}    },  
        vertex{    vec3{-size[0],-size[1], size[2]},    vec3{0.0, 0.0, 0.0},    vec2{1.0, 1.0}    }
    };  
    uint32_t indices[] = {
        0, 1, 2,
        3, 4, 5, 
        6, 7, 8, 
        9, 10, 11,
        12, 13, 14, 
        15, 16, 17,
        18, 19, 20, 
        21, 22, 23,
        24, 25, 26, 
        27, 28, 29,
        30, 31, 32, 
        33, 34, 35
    };
}