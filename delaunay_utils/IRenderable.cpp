#include "IRenderable.h"

std::vector<Triangle*> IRenderable::renderables;
std::mutex IRenderable::mutex;