#ifndef USER_H
#define USER_H

#include"workouts.h"

class User {
	public:
		User(std::string n, double kg);

		std::string get_name();
		double get_kg();
		std::vector<Workout*>* get_workout_vector();

	private:
		std::string name;	
		double user_kg;
		std::vector<Workout*> workout_vector;
};

#endif
