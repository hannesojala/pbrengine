#pragma once

#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <cmath>
#include <string>

// this is complete garbage delete it

/* Still a distaster */

/* AND NOW I FIND OUT RGBA IS BROKEN? */

/* TODO: LOAD ANY RGB/RGBA ANY PRECISION PNG */

GLuint texFromImg(std::string path) {
    IMG_Init(~0);
    auto img_surf = IMG_Load(path.c_str());
    if( img_surf == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return 0;
    }

    auto is_4_bpp = img_surf->format->BytesPerPixel == 4;
    // Why are both necessary? WHY DOES RGBA NOT WORK?
    auto format = (is_4_bpp) ? GL_RGBA : GL_RGB;
    auto sized_format = (is_4_bpp) ? GL_RGBA8 : GL_RGB8;

    GLuint name;
    glCreateTextures(GL_TEXTURE_2D, 1, &name);
    
    auto max_mips = std::log2(std::min(img_surf->w, img_surf->h)); // why min again?
    glTextureParameteri(name, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTextureParameteri(name, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTextureParameteri(name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(name, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(name, GL_TEXTURE_MAX_LEVEL, max_mips);

    glTextureStorage2D(name, max_mips, sized_format, img_surf->w, img_surf->h);
    glTextureSubImage2D(name, 0, 0, 0, img_surf->w, img_surf->h, format, GL_UNSIGNED_BYTE, img_surf->pixels);
    glGenerateTextureMipmap(name);

    SDL_FreeSurface(img_surf);
    IMG_Quit();
    return name;
}