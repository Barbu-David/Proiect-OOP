#ifndef DISPLAY_HELPER_H
#define DISPLAY_HELPER_H

#include "exercises.h"
#include <vector>
#include <functional>

	std::vector<Exercise*> bubbleSort(std::vector<Exercise*>& exercises, std::function<bool(Exercise*, Exercise*)> comparator);
	void handleKeyPressAndSort(std::vector<Exercise*>& exercises);

#endif
