#include "Material.h"

Material::Material(const vec3 AMBIENT, const vec3 DIFFUSE, const vec3 SPECULAR, const int SHININESS)
	: ambientFactor(AMBIENT), diffuseFactor(DIFFUSE), specularFactor(SPECULAR), shininess(SHININESS)
{
}

vec3 Material::getAmbientFactor()
{
	return ambientFactor;
}
vec3 Material::getDiffuseFactor()
{
	return diffuseFactor;
}
vec3 Material::getSpecularFactor()
{
	return specularFactor;
}
int Material::getShininess()
{
	return shininess;
}

void Material::setAmbientFactor(const vec3& AMBIENT)
{
	ambientFactor = AMBIENT;
}
void Material::setDiffuseFactor(const vec3& DIFFUSE)
{
	diffuseFactor = DIFFUSE;
}
void Material::setSpecularFactor(const vec3& SPECULAR)
{
	specularFactor = SPECULAR;
}
void Material::setShininess(const int SHININESS)
{
	this->shininess = SHININESS;
}