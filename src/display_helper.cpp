#include <algorithm>
#include "display_helper.h"

std::vector<Exercise*> bubbleSort(std::vector<Exercise*>& exercises, std::function<bool(Exercise*, Exercise*)> comparator) {
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

void handleKeyPressAndSort(std::vector<Exercise*>& exercises) {
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
			std::cout<<"\nsorting\n";
		}

	if (IsKeyPressed(KEY_P)) {
		auto comparator = [](Exercise* a, Exercise* b) {
			return a->get_progress() > b->get_progress();  // Sorting by progress
		};
		bubbleSort(exercises, comparator);
	}
}

