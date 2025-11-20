#include "Material.h"

Material::Material(const vec4 AMBIENT, const vec4 DIFFUSE, const vec4 SPECULAR, const int SHININESS)
	: ambientFactor(AMBIENT), diffuseFactor(DIFFUSE), specularFactor(SPECULAR), shininess(SHININESS)
{
}

vec4 Material::getAmbientFactor() const
{
	return ambientFactor;
}
vec4 Material::getDiffuseFactor() const
{
	return diffuseFactor;
}
vec4 Material::getSpecularFactor() const
{
	return specularFactor;
}
int Material::getShininess() const
{
	return shininess;
}

void Material::setAmbientFactor(const vec4& AMBIENT)
{
	ambientFactor = AMBIENT;
}
void Material::setDiffuseFactor(const vec4& DIFFUSE)
{
	diffuseFactor = DIFFUSE;
}
void Material::setSpecularFactor(const vec4& SPECULAR)
{
	specularFactor = SPECULAR;
}
void Material::setShininess(const int SHININESS)
{
	this->shininess = SHININESS;
}