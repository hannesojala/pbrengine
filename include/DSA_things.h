#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <Texture.h>

using namespace glm;

struct vertex_t {
    vec3 position;
    vec3 normal;
    vec2 texture_coord;
};

struct Mesh {
    GLuint vbo, ibo, vao, texture;
    GLuint num_indices;
    //mesh* parent;
};

struct Model {
    std::vector<Mesh> meshes;
};

Mesh upload_indexed_mesh(std::vector<vertex_t> vertices, std::vector<GLuint> indices, GLuint texture) {
    Mesh mesh;

    glCreateBuffers(1, &mesh.vbo);	
    glNamedBufferStorage(mesh.vbo, sizeof(vertex_t)*vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &mesh.ibo);
    glNamedBufferStorage(mesh.ibo, sizeof(GLuint)*indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &mesh.vao);

    glVertexArrayVertexBuffer(mesh.vao, 0, mesh.vbo, 0, sizeof(vertex_t));
    glVertexArrayElementBuffer(mesh.vao, mesh.ibo);

    glEnableVertexArrayAttrib(mesh.vao, 0);
    glEnableVertexArrayAttrib(mesh.vao, 1);
    glEnableVertexArrayAttrib(mesh.vao, 2);

    glVertexArrayAttribFormat(mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex_t, position));
    glVertexArrayAttribFormat(mesh.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(vertex_t, normal));
    glVertexArrayAttribFormat(mesh.vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(vertex_t, texture_coord));

    glVertexArrayAttribBinding(mesh.vao, 0, 0);
    glVertexArrayAttribBinding(mesh.vao, 1, 0);
    glVertexArrayAttribBinding(mesh.vao, 2, 0);

    mesh.texture = texture;
    mesh.num_indices = indices.size();

    return mesh;
}

Model import_obj(const std::string& fname) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( fname,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

    Model model;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        auto ai_mesh = scene->mMeshes[i];
        std::vector<vertex_t> vertices;
        std::vector<GLuint> indices;
        for (unsigned int f = 0; f < ai_mesh->mNumFaces; f++) {
            aiFace face = ai_mesh->mFaces[f];
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }
        for (unsigned int n = 0; n < ai_mesh->mNumVertices; n++) {
            aiVector3D pos = ai_mesh->mVertices[n];
            aiVector3D nrm = ai_mesh->mNormals[n];
            aiVector3D txc = ai_mesh->mTextureCoords[0][n];
            vertex_t vert = {
                vec3{pos.x, pos.y, pos.z},
                vec3{nrm.x, nrm.y, nrm.z},
                vec2{txc.x, txc.y} // consider if flipped (ogl vs dx)
            };
            vertices.push_back(vert);
        }
        GLuint texture = texFromImg(TEXTURE_FILENAME);
        Mesh mesh = upload_indexed_mesh(vertices, indices, texture);
        model.meshes.push_back(mesh);
    }
    return model;
}