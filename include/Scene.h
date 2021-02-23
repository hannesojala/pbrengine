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

/* Struct representing a vertex with position, normal, tangent, and texture coordinate attributes */
struct Vertex {
    vec3 position, normal, tangent;
    vec2 tex_coord;
};

/* Struct representing a PBR material through GLuint names for textures previously uploaded. */
struct Material {
    GLuint albedo, metal_rough, normal;
};

/* Struct representing a mesh through GLuint names for vertex/index buffers and a vao representing it. */
/* Also stores material and num_indices for indexed drawing. */
struct Mesh {
    GLuint vbo, ibo, vao;
    Material material;
    GLuint num_indices;
};

/* Struct representing a scene/model composed of a number of meshes and child models. */
struct Model {
    std::vector<Mesh> meshes;
    std::vector<Model> children;
};

/* Upload an indexed mesh consisting of supplied vertices, indices, and material and return a mesh struct of names to the relevant objects. */
Mesh upload_indexed_mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material material) {
    Mesh mesh;

    /* Create vertex and index buffers, and vertex array object */
    glCreateBuffers(1, &mesh.vbo);	
    glNamedBufferStorage(mesh.vbo, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &mesh.ibo);
    glNamedBufferStorage(mesh.ibo, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &mesh.vao);

    /* Link VBO to binding index 0 of VAO */
    glVertexArrayVertexBuffer(mesh.vao, 0, mesh.vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(mesh.vao, mesh.ibo);

    /* TODO: Programmatically using attribute spec. struct. */
    /* Enable vertex attributes */
    glEnableVertexArrayAttrib(mesh.vao, 0);
    glEnableVertexArrayAttrib(mesh.vao, 1);
    glEnableVertexArrayAttrib(mesh.vao, 2);
    glEnableVertexArrayAttrib(mesh.vao, 3);

    /* Specify format */
    glVertexArrayAttribFormat(mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(mesh.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexArrayAttribFormat(mesh.vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, tangent));
    glVertexArrayAttribFormat(mesh.vao, 3, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex_coord));

    /* Bind VAO attributes to VBO using binding index 0*/
    glVertexArrayAttribBinding(mesh.vao, 0, 0);
    glVertexArrayAttribBinding(mesh.vao, 1, 0);
    glVertexArrayAttribBinding(mesh.vao, 2, 0);
    glVertexArrayAttribBinding(mesh.vao, 3, 0);

    /* Set mesh draw info */
    mesh.material = material;
    mesh.num_indices = indices.size();

    return mesh;
}

/* Recursively traverse an assimp scene and extract models, meshes, and materials. */
Model node_trav(aiNode* node, const aiScene* scene, std::string model_directory) {
    Model model;

    /* Process model meshes */
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        auto ai_mesh = scene->mMeshes[i];
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        /* Reserve space for 3 indices for each face */
        indices.reserve(3 * ai_mesh->mNumFaces);

        /* Extract indices from faces */
        for (unsigned int f = 0; f < ai_mesh->mNumFaces; f++) {
            auto fidxs = ai_mesh->mFaces[f].mIndices;
            indices.insert(indices.end(), {fidxs[0], fidxs[1], fidxs[2]});
        }

        /* Extract vertex information from mesh */
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

        /* Get material texture names */
        auto ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
        aiString albedo_map_name, metal_rough_map_name, normal_map_name;
        ai_material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0),   albedo_map_name);
        ai_material->Get(AI_MATKEY_TEXTURE(aiTextureType_UNKNOWN, 0),   metal_rough_map_name);
        ai_material->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0),   normal_map_name);

        /* Upload mesh with vertices, indices, and PBR map textures */
        auto mesh = upload_indexed_mesh(vertices, indices, {
            texFromImg(model_directory + "/" + std::string(albedo_map_name.C_Str())),
            texFromImg(model_directory + "/" + std::string(metal_rough_map_name.C_Str())),
            texFromImg(model_directory + "/" + std::string(normal_map_name.C_Str()))});
        
        /* Add mesh to model */
        model.meshes.push_back(mesh);
    }
    /* Recursively process child models */
    for (unsigned int i = 0; i < node->mNumChildren; i++) model.children.push_back(node_trav(node->mChildren[i], scene, model_directory));
    return model;
}

/* Import a model from directory ./model_name/ and return the processed version of it. */
/* Assumes a gltf model in ./model_name/model_name.gltf with non bundled textures */

/* NOTE: GOOD EXAMPLES FOR TINYGLTF LOADER IN ITS REPO, switch to it */

Model import_model(const std::string& model_name) {
    Model model;
    Assimp::Importer importer;
    
    /* Import and apply processing */
    const aiScene* scene = importer.ReadFile(model_name + "/" + model_name + ".gltf",
        aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    /* If successful return the traversed scene as a model */
    if (scene) return node_trav(scene->mRootNode, scene, model_name);

    std::cerr << "Assimp error: " << importer.GetErrorString() << "\n";
    return model;
}