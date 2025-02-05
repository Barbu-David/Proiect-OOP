#ifndef WORKOUTS_H
#define WORKOUTS_H

#include "exercises.h"
#include <vector>

class Workout {

	public:
		Workout(std::string Name, std::string Description, std::vector<Exercise*> Exercises);
		Workout(const Workout& other);
		
		std::string get_name();
		std::string get_description();
		std::vector<Exercise*>* get_exercises();
	private:	
		std::string name, description;
		std::vector<Exercise*> exercises;	
};

#endif
