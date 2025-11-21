#include "ParametricLineMovement.h"
/*
ParametricLineMovement::ParametricLineMovement(const vector<vec3>& pts, float speed) : points(pts), speedT(speed), direction(1)
{
    // Start at a random point along the path
    currentT = static_cast<float>(rand()) / RAND_MAX;
}


mat4 ParametricLineMovement::getModelMatrix()
{
    if (points.size() < 2)
        return mat4(1.0f);

    currentT += speedT;
    if (currentT > 1.0f) currentT = 0.0f;

    int i0 = floor(currentT * (points.size() - 1));
    int i1 = std::min(i0 + 1, (int)points.size() - 1);

    float localT = (currentT * (points.size() - 1)) - i0;

    vec3 pos = mix(points[i0], points[i1], localT);

    return glm::translate(mat4(1.0f), pos);
}
*/

