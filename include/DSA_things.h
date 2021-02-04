#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

using namespace glm;

struct vertex {
    vec3 position;
    vec3 normal;
    vec2 texture;
};

struct indexed_model {
    GLuint vbo, ibo, vao, texture;
};

/* TODO */

// Merge ibo and vbo into one buffer
// Generalize for different attrib layouts (as in deleted code, go to earlier commits, supply struct)

indexed_model upload_indexed_model(vertex* vertices, int vertex_count, uint32_t* indices, int index_count, GLuint texture) {
    indexed_model model;

    glCreateBuffers(1, &model.vbo);	
    glNamedBufferStorage(model.vbo, sizeof(vertex)*vertex_count, vertices, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &model.ibo);
    glNamedBufferStorage(model.ibo, sizeof(uint32_t)*index_count, indices, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &model.vao);

    glVertexArrayVertexBuffer(model.vao, 0, model.vbo, 0, sizeof(vertex));
    glVertexArrayElementBuffer(model.vao, model.ibo);

    glEnableVertexArrayAttrib(model.vao, 0);
    glEnableVertexArrayAttrib(model.vao, 1);
    glEnableVertexArrayAttrib(model.vao, 2);

    glVertexArrayAttribFormat(model.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, position));
    glVertexArrayAttribFormat(model.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, normal));
    glVertexArrayAttribFormat(model.vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(vertex, texture));

    glVertexArrayAttribBinding(model.vao, 0, 0);
    glVertexArrayAttribBinding(model.vao, 1, 0);
    glVertexArrayAttribBinding(model.vao, 2, 0);

    model.texture = texture;

    return model;
}