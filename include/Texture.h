#pragma once

#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <cmath>
#include <string>

// this is complete garbage delete it

GLuint texFromImg(std::string path) {
    IMG_Init(~0);
    SDL_Surface* imgSurf = IMG_Load(path.c_str());
    if( imgSurf == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return 0;
    }

    auto is4BPP = imgSurf->format->BitsPerPixel == 4;
    auto format = (is4BPP) ? GL_RGBA : GL_RGB;
    auto sized_format = (is4BPP) ? GL_RGBA8 : GL_RGB8;

    GLuint ID;
    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    
    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(ID, GL_TEXTURE_BASE_LEVEL, 0);
    unsigned int max_mips = std::log2(std::min(imgSurf->w, imgSurf->h));
    glTextureParameteri(ID, GL_TEXTURE_MAX_LEVEL, max_mips);

    glTextureStorage2D(ID, max_mips, sized_format, imgSurf->w, imgSurf->h);
    glTextureSubImage2D(ID, 0, 0, 0, imgSurf->w, imgSurf->h, format, GL_UNSIGNED_BYTE, imgSurf->pixels);
    glGenerateTextureMipmap(ID);

    SDL_FreeSurface(imgSurf);
    IMG_Quit();
    return ID;
}