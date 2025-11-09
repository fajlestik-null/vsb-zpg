#pragma once
#include "Includes.h"

class Texture
{
private:
	GLuint mTextureID = 0;
	GLenum mTextureUnit = 0;

	static GLuint sNextUnit;
public:
	Texture(const string& PATH);
	~Texture();

	void bind() const;
	void unbind() const;

	GLuint getID() const;
	GLenum getUnit() const { return mTextureUnit; }

	int getUnitIndex() const;
};