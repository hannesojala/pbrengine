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
    vec3 position, normal;
    vec2 tex_coord;
};

struct Mesh {
    GLuint vbo, ibo, vao, texture;
    GLuint num_indices;
};

struct Model {
    std::vector<Mesh> meshes;
    std::vector<Model> children;
};

Mesh upload_indexed_mesh(std::vector<vertex_t> vertices, std::vector<GLuint> indices, GLuint texture) {
    Mesh mesh;

    glCreateBuffers(1, &mesh.vbo);	
    glNamedBufferStorage(mesh.vbo, sizeof(vertex_t) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &mesh.ibo);
    glNamedBufferStorage(mesh.ibo, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &mesh.vao);

    glVertexArrayVertexBuffer(mesh.vao, 0, mesh.vbo, 0, sizeof(vertex_t));
    glVertexArrayElementBuffer(mesh.vao, mesh.ibo);

    glEnableVertexArrayAttrib(mesh.vao, 0);
    glEnableVertexArrayAttrib(mesh.vao, 1);
    glEnableVertexArrayAttrib(mesh.vao, 2);

    glVertexArrayAttribFormat(mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex_t, position));
    glVertexArrayAttribFormat(mesh.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(vertex_t, normal));
    glVertexArrayAttribFormat(mesh.vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(vertex_t, tex_coord));

    glVertexArrayAttribBinding(mesh.vao, 0, 0);
    glVertexArrayAttribBinding(mesh.vao, 1, 0);
    glVertexArrayAttribBinding(mesh.vao, 2, 0);

    mesh.texture = texture;
    mesh.num_indices = indices.size();

    return mesh;
}

Model node_trav(aiNode* node, const aiScene* scene) {
    Model model;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        auto ai_mesh = scene->mMeshes[i];
        std::vector<vertex_t> vertices;
        std::vector<GLuint> indices;
        indices.reserve(3 * ai_mesh->mNumFaces);

        for (unsigned int f = 0; f < ai_mesh->mNumFaces; f++) {
            auto fidxs = ai_mesh->mFaces[f].mIndices;
            indices.insert(indices.end(), {fidxs[0], fidxs[1], fidxs[2]});
        }

        for (unsigned int n = 0; n < ai_mesh->mNumVertices; n++) {
            auto pos = ai_mesh->mVertices[n],
                 nrm = ai_mesh->mNormals[n],
                 txc = ai_mesh->mTextureCoords[0][n];
            vertices.push_back({
                {pos.x, pos.y, pos.z},
                {nrm.x, nrm.y, nrm.z},
                {txc.x, txc.y} // consider if flipped (ogl vs dx)
            });
        }

        std::string texname = std::string(scene->mMaterials[ai_mesh->mMaterialIndex]->GetName().C_Str());
        auto texture = texFromImg(texname + ".png");
        auto mesh = upload_indexed_mesh(vertices, indices, texture);
        model.meshes.push_back(mesh);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) model.children.push_back(node_trav(node->mChildren[i], scene));
    return model;
}

Model import_obj(const std::string& filename) {
    Model model;
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(filename,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

    return node_trav(scene->mRootNode, scene);
}