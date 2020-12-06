#include <glad/glad.h>

class glBuffer {
public:
    glBuffer(GLfloat data[], unsigned long size, GLenum target, GLenum usage) :
        target(target), usage(usage), size(size)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(target, ID);
        glBufferData(target, size * sizeof(GLfloat), &data[0], usage);
    }
    ~glBuffer() { 
        glDeleteBuffers(1, &ID);
    }
private:
    GLenum target;
    GLenum usage;
    GLuint ID;
    unsigned long size;
public:
    void bind() {
        glBindBuffer(target, ID);
    }
    void unbind() {
        glBindBuffer(target, 0);
    }
    unsigned long getSize() {
        return size;
    }
};