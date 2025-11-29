#pragma once
#include "Includes.h"

class Texture
{
private:
	GLuint mTextureID = 0;
	GLenum mTextureUnit = 0;
	GLenum mTarget = GL_TEXTURE_2D; // GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP

	static GLuint sNextUnit;
public:
	Texture(const string& PATH);
	Texture(const vector<string>& PATHS);
	~Texture();

	void bind() const;
	void unbind() const;

	GLuint getID() const;
	GLenum getUnit() const;

	int getUnitIndex() const;
};