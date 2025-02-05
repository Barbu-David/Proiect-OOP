#include <algorithm>
#include "display_helper.h"
#include <iomanip>

std::vector<Exercise*> DisplayHelper::bubbleSort(std::vector<Exercise*>& exercises, std::function<bool(Exercise*, Exercise*)> comparator) {
	size_t n = exercises.size();
	bool swapped;

	for (size_t i = 0; i < n - 1; ++i) {
		swapped = false;
		for (size_t j = 0; j < n - i - 1; ++j) {
			if (!comparator(exercises[j], exercises[j + 1])) {
				std::swap(exercises[j], exercises[j + 1]);
				swapped = true;
			}
		}

		if (!swapped) {
			break;
		}
	}

	return exercises;
}

void DisplayHelper::handleKeyPressAndSort(std::vector<Exercise*>& exercises) {
	if (IsKeyPressed(KEY_A)) {
		auto comparator = [](Exercise* a, Exercise* b) {
			return a->get_name() < b->get_name();  // Sorting alphabetically by name
		};
		bubbleSort(exercises, comparator);
	

	}

	if (IsKeyPressed(KEY_C)) {
		auto comparator = [](Exercise* a, Exercise* b) {
			return a->calculate_calories_max(90.0) > b->calculate_calories_max(90.0);  // Sorting by calories 
		};
		bubbleSort(exercises, comparator);
		}

	if (IsKeyPressed(KEY_P)) {
		auto comparator = [](Exercise* a, Exercise* b) {
			return a->get_progress() > b->get_progress();  // Sorting by progress
		};
		bubbleSort(exercises, comparator);
	}
}

std::string DisplayHelper::to_string_with_precision(double value, int precision) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(precision) << value;
        return out.str();
}

void DisplayHelper::GetTextInput(std::string &buffer, int maxLength) {
        int key;
        while ((key = GetCharPressed()) != 0) {
                if (key >= 32 && key <= 125 && buffer.size() <(unsigned int) maxLength - 1) {
                        buffer += (char)key;
                }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !buffer.empty()) {
                buffer.pop_back();
        }
}
