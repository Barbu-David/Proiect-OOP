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
				copiedExercise = new Weight(*weight, weight->get_max_repetitions(), weight->get_max_sets(), weight->get_weight());
				dynamic_cast<Weight*>(copiedExercise)->reset_reps();     // Reset repetitions
				dynamic_cast<Weight*>(copiedExercise)->reset_sets();     // Reset sets

			} else	if (auto* calisthenics = dynamic_cast<Calisthenics*>(exercise)) {
				copiedExercise = new Calisthenics(*calisthenics, calisthenics->get_max_repetitions(), calisthenics->get_max_sets());
				dynamic_cast<Calisthenics*>(copiedExercise)->reset_reps();  // Reset repetitions
				dynamic_cast<Calisthenics*>(copiedExercise)->reset_sets();  // Reset sets

			} else if (auto* running = dynamic_cast<Running*>(exercise)) {
				copiedExercise = new Running(*running, running->get_max_distance(), running->get_time());
				dynamic_cast<Running*>(copiedExercise)->reset_distance(); // Reset distance


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

