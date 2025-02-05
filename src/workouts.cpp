#include "workouts.h"

Workout::Workout(std::string Name, std::string Description, std::vector<Exercise*> Exercises)
{
	this->name=Name;
	this->description=Description;
	this->exercises=Exercises;
}

Workout::Workout(const Workout& other)
	: name(other.name), description(other.description) {
		for (Exercise* exercise : other.exercises) {
			if (!exercise) continue;

			Exercise* copiedExercise = nullptr;


			//ORDINEA ESTE SUPER IMPORTANTA

			if (auto* weight = dynamic_cast<Weight*>(exercise)) {    
				copiedExercise = new Weight(*weight, weight->get_weight());
				dynamic_cast<Weight*>(copiedExercise)->reset_reps();     // Reset repetitions
				dynamic_cast<Weight*>(copiedExercise)->reset_sets();     // Reset sets
				dynamic_cast<Weight*>(copiedExercise)->set_reps(weight->get_max_repetitions());
				dynamic_cast<Weight*>(copiedExercise)->set_sets(weight->get_max_sets());

			} else	if (auto* calisthenics = dynamic_cast<Calisthenics*>(exercise)) {
				copiedExercise = new Calisthenics(*calisthenics);
				dynamic_cast<Calisthenics*>(copiedExercise)->reset_reps();  // Reset repetitions
				dynamic_cast<Calisthenics*>(copiedExercise)->reset_sets();  // Reset sets
				dynamic_cast<Calisthenics*>(copiedExercise)->set_reps(calisthenics->get_max_repetitions());
				dynamic_cast<Calisthenics*>(copiedExercise)->set_sets(calisthenics->get_max_sets());

			} else if (auto* running = dynamic_cast<Running*>(exercise)) {
				copiedExercise = new Running(*running);
				dynamic_cast<Running*>(copiedExercise)->set_distance(0); // Reset distance
				dynamic_cast<Running*>(copiedExercise)->set_distance(running->get_max_distance());
				dynamic_cast<Running*>(copiedExercise)->set_time(running->get_time());



			} else {
				// Unsupported type or base Exercise, skip
				continue;
			}

			if (copiedExercise) {
				exercises.push_back(copiedExercise);
			}
		}
	}


std::string Workout::get_name()
{
	return name;
}

std::string Workout::get_description()
{
	return description;
}

std::vector<Exercise*>* Workout::get_exercises()
{
	return &exercises;
}

