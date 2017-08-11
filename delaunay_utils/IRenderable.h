#pragma once

#include "common.h"
#include <mutex>

/* interfejs obiektow widocznych */
class IRenderable
{
public:
	static std::mutex mutex;
	static std::vector<Triangle*> renderables;
};

