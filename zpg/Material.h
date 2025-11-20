#pragma once

#include "Includes.h"

class Material
{
private:
	vec4 ambientFactor;
	vec4 diffuseFactor;
	vec4 specularFactor;
	int shininess;

public:
	Material(const vec4 AMBIENT = vec4(1.0f), const vec4 DIFFUSE = vec4(1.0f), const vec4 SPECULAR = vec4(1.0f), const int SHININESS = 32);

	vec4 getAmbientFactor() const;
	vec4 getDiffuseFactor() const;
	vec4 getSpecularFactor() const;
	int getShininess() const;

	void setAmbientFactor(const vec4& ambient);
	void setDiffuseFactor(const vec4& diffuse);
	void setSpecularFactor(const vec4& specular);
	void setShininess(const int shininess);
};
