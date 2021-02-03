#include <glad/glad.h>

class glBuffer {
public:
    glBuffer() = default;
    glBuffer(GLfloat data[], GLuint size, GLenum target, GLenum usage) :
        target(target), usage(usage), size(size)
    {
        GLuint ID;
        glGenBuffers(1, &ID);
        gl_name = glName(ID);
        glBindBuffer(target, gl_name.get());
        glBufferData(target, size, data, usage);
        //GLuint ID;
        //glCreateBuffers(1, &ID);
        //gl_name = glName(ID);
        //glNamedBufferData(gl_name.get(), size, data, usage);
    }
    glBuffer(GLuint data[], GLuint size, GLenum target, GLenum usage) :
        target(target), usage(usage), size(size)
    {
        GLuint ID;
        glGenBuffers(1, &ID);
        gl_name = glName(ID);
        glBindBuffer(target, gl_name.get());
        glBufferData(target, size, data, usage);
        //GLuint ID;
        //glCreateBuffers(1, &ID);
        //gl_name = glName(ID);
        //glNamedBufferData(gl_name.get(), size, data, usage);
    }

    // Default move ctor and operator
    glBuffer(glBuffer &&other) = default;
    glBuffer &operator=(glBuffer &&other) = default;

    ~glBuffer() { 
        GLuint ID = gl_name.get();
        glDeleteBuffers(1, &ID);
    }
private:
    GLenum target;
    GLenum usage;
    glName gl_name;
    GLuint size; // NOT length. size in bytes
public:
    void bind() {
        glBindBuffer(target, gl_name.get());
    }
    void unbind() {
        glBindBuffer(target, 0);
    }
    GLuint getSize() {
        return size;
    }
};