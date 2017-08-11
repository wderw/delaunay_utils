#include "IRenderable.h"

std::vector<Triangle*> IRenderable::triangles;
std::mutex IRenderable::mutex;