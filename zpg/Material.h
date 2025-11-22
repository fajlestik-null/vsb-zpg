#pragma once

#include "Includes.h"

class Material
{
private:
	vec3 ambientFactor = vec3(0.0);
	vec3 diffuseFactor = vec3(0.0);
	vec3 specularFactor = vec3(0.0);
	int shininess = 32;

public:
	Material(const vec3 AMBIENT = vec3(0.1f), const vec3 DIFFUSE = vec3(1.0f), const vec3 SPECULAR = vec3(0.5f), const int SHININESS = 32);

	vec3 getAmbientFactor();
	vec3 getDiffuseFactor();
	vec3 getSpecularFactor();
	int getShininess();

	void setAmbientFactor(const vec3& ambient);
	void setDiffuseFactor(const vec3& diffuse);
	void setSpecularFactor(const vec3& specular);
	void setShininess(const int shininess);
};
