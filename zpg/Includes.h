#pragma once
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/vec3.hpp>                 // vec3
#include <glm/vec4.hpp>                 // vec4
#include <glm/mat4x4.hpp>               // mat4
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>         // value_ptr
// Include Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//Include c++
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
//#include <algorithm>
//Namespaces
using namespace std;
using namespace glm;