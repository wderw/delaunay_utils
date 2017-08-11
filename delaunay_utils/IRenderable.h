#pragma once

#include "common.h"
#include <mutex>

/* interfejs obiektow widocznych */
class IRenderable
{
public:
	static std::mutex mutex;
	static std::vector<IRenderable*> renderables;

	IRenderable()
	{
		mutex.lock();
		IRenderable::renderables.push_back(this);
		mutex.unlock();
	}
};

