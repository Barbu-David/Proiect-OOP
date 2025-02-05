#ifndef DISPLAY_HELPER_H
#define DISPLAY_HELPER_H

#include "exercises.h"
#include <vector>
#include <functional>


	class DisplayHelper {
		public:
			static std::vector<Exercise*> bubbleSort(std::vector<Exercise*>& exercises, std::function<bool(Exercise*, Exercise*)> comparator);
			static void handleKeyPressAndSort(std::vector<Exercise*>& exercises);
			static std::string to_string_with_precision(double value, int precision);
			static void GetTextInput(std::string &buffer, int maxLength);
			static std::string getProgressDetails (Exercise* exercise);
};

#endif
