#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "users.h"
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class FileManager {

	public:
		FileManager(const char* exercises_directory, const char* w_directory, const char* users_directory);	
		
		Workout* workout_from_json(json workout_json);	
		User* read_user_from_json(json user_json);
		
		std::vector<User*> get_user_vector();
		std::vector<Workout*> get_workout_vector();
		std::vector<Exercise*> get_exercise_vector();
		void add_user(const std::string& username, double user_kgs);
		void add_workout_to_user(const std::string& username, const std::string& workout_name);
	private:
		FilePathList exercise_files, workout_files, user_files;
		std::vector<Exercise*> exercise_vector;
		Exercise* exercise_from_json(json j);
		const char* workouts_directory;	
		const char* users_directory;
		std::vector<User*> user_vector;
		std::vector<Workout*> workout_vector;
};

ExerciseType exercise_type_from_string(const std::string& type_str);

#endif
