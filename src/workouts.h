#ifndef WORKOUTS_H
#define WORKOUTS_H

#include "exercises.h"
#include <vector>

class Workout {

	public:
	Workout(std::string Name, std::string Description, std::vector<Exercise*> Exercises);
	Workout(const Workout& other);
	std::vector<Exercise*> exercises;	

	std::string name, description;
};

#endif
