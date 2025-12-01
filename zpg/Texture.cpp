#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Lib/stb_image/stb_image.h"

GLuint Texture::sNextUnit = 0;


Texture::Texture(const string& PATH)
{
    mTextureUnit = GL_TEXTURE0;

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(PATH.c_str(), &width, &height, &channels, 4);

    if (!data) {
        cerr << "Error loading texture: " << PATH << endl;
        mTextureID = 0;
        return;
    }

    glGenTextures(1, &mTextureID);

	mTarget = GL_TEXTURE_2D;
    mTextureUnit = GL_TEXTURE0 + sNextUnit;
    sNextUnit++;                               

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

Texture::Texture(const vector<string>& PATHS)
{
    if (PATHS.size() != 6)
    {
        cerr << "Error: Skybox requires 6 images." << endl;
        return;
    }

    mTarget = GL_TEXTURE_CUBE_MAP;
    mTextureUnit = GL_TEXTURE0 + sNextUnit++;

    glGenTextures(1, &mTextureID);
    glActiveTexture(mTextureUnit);
    glBindTexture(mTarget, mTextureID);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char* data = stbi_load(PATHS[i].c_str(), &width, &height, &channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            cerr << "Failed to load cubemap face: " << PATHS[i] << endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


Texture::~Texture()
{
    glDeleteTextures(1, &mTextureID);
}

void Texture::bind() const
{
    glActiveTexture(mTextureUnit);
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
    return (int)(mTextureUnit - GL_TEXTURE0);
}

GLenum Texture::getUnit() const
{
    return mTextureUnit;
}
