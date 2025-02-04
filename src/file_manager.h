#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "users.h"
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class FileManagerHelper {
	
	public:
		static ExerciseType exercise_type_from_string(const std::string& type_str);
		template <typename T> static void add_files_to_vector(std::vector<T>& vector, std::function<T(json)> from_json, FilePathList files);

};

class FileManager {

	public:

	//Constructor
		FileManager(const char* exercises_directory, const char* w_directory, const char* users_directory);	
	//Getters	
		std::vector<User*> get_user_vector();
		std::vector<Workout*> get_workout_vector();
		std::vector<Exercise*> get_exercise_vector();
	//Adding
		void add_user(const std::string& username, double user_kgs);
		void add_workout_to_user(const std::string& username, const std::string& workout_name);

	private:

	//Files
		FilePathList exercise_files, workout_files, user_files;
	
		const char* workouts_directory;	
		const char* users_directory;

	//Data
		std::vector<Exercise*> exercise_vector;
		std::vector<User*> user_vector;
		std::vector<Workout*> workout_vector;

	//Reading
		Exercise* exercise_from_json(json j);
		Workout* workout_from_json(json workout_json);
		User* user_from_json(json user_json);
	
		static FileManagerHelper helper;

};
#endif
