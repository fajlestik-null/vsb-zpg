#pragma once
#include "Includes.h"

class Texture
{
private:
	GLuint mTextureID;
	GLenum mTextureUnit;

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