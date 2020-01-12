#pragma once
#include <vector>
#include "../glm/gtc/type_ptr.hpp"

constexpr float PI = glm::pi<float>();

std::vector<float> linearSpace(float start, float end, int numPoints);