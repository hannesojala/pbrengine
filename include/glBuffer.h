#include <glad/glad.h>

class glBuffer {
public:
    glBuffer(GLfloat data[], unsigned long size, GLenum target, GLenum usage) :
        m_target(target), m_usage(usage), m_size(size)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(target, m_ID);
        glBufferData(target, size * sizeof(GLfloat), &data[0], usage);
    }
    ~glBuffer() { 
        glDeleteBuffers(1, &m_ID);
    }
private:
    GLenum m_target;
    GLenum m_usage;
    GLuint m_ID;
    unsigned long m_size;
public:
    void bind() {
        glBindBuffer(m_target, m_ID);
    }
    void unbind() {
        glBindBuffer(m_target, 0);
    }
    unsigned long size() {
        return m_size;
    }
};