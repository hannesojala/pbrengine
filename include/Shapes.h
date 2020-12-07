#include <glad/glad.h>

namespace CubeFlat {
    GLfloat size[] = {1.0, 1.0, 1.0};
    GLfloat vertices[] = {
        -size[0],  size[1],  size[2], 0.0, 1.0,
        -size[0], -size[1],  size[2], 0.0, 0.0,
        size[0],  size[1],  size[2], 1.0, 1.0,
        size[0], -size[1],  size[2], 1.0, 0.0,
        size[0],  size[1],  size[2], 1.0, 1.0,
        -size[0], -size[1],  size[2], 0.0, 0.0,

        -size[0],  size[1], -size[2], 0.0, 1.0,  
        -size[0], -size[1], -size[2], 0.0, 0.0,  
        -size[0],  size[1],  size[2], 1.0, 1.0,  
        -size[0], -size[1],  size[2], 1.0, 0.0,  
        -size[0],  size[1],  size[2], 1.0, 1.0,  
        -size[0], -size[1], -size[2], 0.0, 0.0,  
        
        size[0],  size[1], -size[2], 0.0, 1.0,  
        size[0], -size[1], -size[2], 0.0, 0.0,  
        -size[0],  size[1], -size[2], 1.0, 1.0,  
        -size[0], -size[1], -size[2], 1.0, 0.0,  
        -size[0],  size[1], -size[2], 1.0, 1.0,  
        size[0], -size[1], -size[2], 0.0, 0.0,  
        
        size[0], -size[1],  size[2], 0.0, 0.0,  
        size[0], -size[1], -size[2], 1.0, 0.0,  
        size[0],  size[1],  size[2], 0.0, 1.0,  
        size[0],  size[1], -size[2], 1.0, 1.0,  
        size[0],  size[1],  size[2], 0.0, 1.0,  
        size[0], -size[1], -size[2], 1.0, 0.0,  
        
        -size[0], size[1], -size[2], 0.0, 1.0,   
        -size[0], size[1],  size[2], 0.0, 0.0,   
        size[0], size[1], -size[2], 1.0, 1.0,   
        size[0], size[1],  size[2], 1.0, 0.0,   
        size[0], size[1], -size[2], 1.0, 1.0,   
        -size[0], size[1],  size[2], 0.0, 0.0,   
        
        size[0], -size[1],  size[2], 0.0, 1.0,  
        -size[0], -size[1],  size[2], 1.0, 1.0,  
        size[0], -size[1], -size[2], 0.0, 0.0,  
        -size[0], -size[1], -size[2], 1.0, 0.0,  
        size[0], -size[1], -size[2], 0.0, 0.0,  
        -size[0], -size[1],  size[2], 1.0, 1.0
    };

    GLuint indices[] = {
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