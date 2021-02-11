#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include <iostream>
#include <Texture.h>

/* Assimp is ass, use a small gltf header or something */

using namespace glm;

struct Vertex {
    vec3 position, normal, tangent;
    vec2 tex_coord;
};

struct PBR_map_set {
    GLuint dif, mtl_rgh, nrm;
};

struct Mesh {
    GLuint vbo, ibo, vao;
    PBR_map_set maps;
    GLuint num_indices;
};

struct Model {
    std::vector<Mesh> meshes;
    std::vector<Model> children;
};

Mesh upload_indexed_mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, PBR_map_set maps) {
    Mesh mesh;

    glCreateBuffers(1, &mesh.vbo);	
    glNamedBufferStorage(mesh.vbo, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &mesh.ibo);
    glNamedBufferStorage(mesh.ibo, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &mesh.vao);

    glVertexArrayVertexBuffer(mesh.vao, 0, mesh.vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(mesh.vao, mesh.ibo);

    glEnableVertexArrayAttrib(mesh.vao, 0);
    glEnableVertexArrayAttrib(mesh.vao, 1);
    glEnableVertexArrayAttrib(mesh.vao, 2);
    glEnableVertexArrayAttrib(mesh.vao, 3);

    glVertexArrayAttribFormat(mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(mesh.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexArrayAttribFormat(mesh.vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tangent));
    glVertexArrayAttribFormat(mesh.vao, 3, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex_coord));

    glVertexArrayAttribBinding(mesh.vao, 0, 0);
    glVertexArrayAttribBinding(mesh.vao, 1, 0);
    glVertexArrayAttribBinding(mesh.vao, 2, 0);
    glVertexArrayAttribBinding(mesh.vao, 3, 0);

    mesh.maps = maps;
    mesh.num_indices = indices.size();

    return mesh;
}

Model node_trav(aiNode* node, const aiScene* scene, std::string dir) {
    Model model;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        auto ai_mesh = scene->mMeshes[i];
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        indices.reserve(3 * ai_mesh->mNumFaces);

        for (unsigned int f = 0; f < ai_mesh->mNumFaces; f++) {
            auto fidxs = ai_mesh->mFaces[f].mIndices;
            indices.insert(indices.end(), {fidxs[0], fidxs[1], fidxs[2]});
        }

        for (unsigned int n = 0; n < ai_mesh->mNumVertices; n++) {
            auto pos = ai_mesh->mVertices[n],
                 nrm = ai_mesh->mNormals[n],
                 tgt = ai_mesh->mTangents[n],
                 txc = ai_mesh->mTextureCoords[0][n];
            vertices.push_back({
                {pos.x, pos.y, pos.z},
                {nrm.x, nrm.y, nrm.z},
                {tgt.x, tgt.y, tgt.z},
                {txc.x, txc.y}
            });
        }

        auto mat = scene->mMaterials[ai_mesh->mMaterialIndex];
        aiString dif_map_name, mtl_rgh_map_name, nrm_map_name;

        mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0),   dif_map_name);
        mat->Get(AI_MATKEY_TEXTURE(aiTextureType_UNKNOWN, 0),   mtl_rgh_map_name);
        mat->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0),   nrm_map_name);

        auto mesh = upload_indexed_mesh(vertices, indices, {
            texFromImg(dir + "/" + std::string(dif_map_name.C_Str())),
            texFromImg(dir + "/" + std::string(mtl_rgh_map_name.C_Str())),
            texFromImg(dir + "/" + std::string(nrm_map_name.C_Str()))});

        model.meshes.push_back(mesh);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) model.children.push_back(node_trav(node->mChildren[i], scene, dir));
    return model;
}

Model import_model(const std::string& model_name) {
    Model model;
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(model_name + "/" + model_name + ".gltf",
        aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene)  {
        std::cerr << "Assimp error: " << importer.GetErrorString() << "\n";
        return model;
    }

    return node_trav(scene->mRootNode, scene, model_name);
}