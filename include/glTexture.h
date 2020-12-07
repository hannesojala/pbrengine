#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <string>

GLuint texFromImg(std::string path) {
    IMG_Init(~0);
    SDL_Surface* imgSurf = IMG_Load(path.c_str());
    if( imgSurf == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return 0;
    }

    GLuint ID;
    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    
    int Mode = GL_RGB;
    
    if(imgSurf->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, imgSurf->w, imgSurf->h, 0, Mode, GL_UNSIGNED_BYTE, imgSurf->pixels);

    glGenerateTextureMipmap(ID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_FreeSurface(imgSurf);
    IMG_Quit();
    return ID;
}