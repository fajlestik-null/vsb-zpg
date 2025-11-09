#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Lib/stb_image/stb_image.h"

GLuint Texture::sNextUnit = 0;


Texture::Texture(const std::string& PATH)
{
    mTextureUnit = GL_TEXTURE0;

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(PATH.c_str(), &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "Error loading texture: " << PATH << std::endl;
        mTextureID = 0;
        return;
    }

    glGenTextures(1, &mTextureID);

    mTextureUnit = GL_TEXTURE0 + sNextUnit;   // assign next free unit
    sNextUnit++;                               // increment for next texture

    glActiveTexture(mTextureUnit);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &mTextureID);
	sNextUnit--;
}

void Texture::bind() const
{
    glActiveTexture(0 + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}


void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getID() const
{
    return mTextureID;
}

int Texture::getUnitIndex() const
{
    return (int)(mTextureUnit + GL_TEXTURE0);
}
