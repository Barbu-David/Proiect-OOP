#include <iostream>
#include "file_manager.h"
#include <fstream>

using json = nlohmann::json;

//Constructor
FileManager::FileManager(const char* exercises_directory, const char* w_directory, const char* users_directory) 
{
	exercise_files=LoadDirectoryFiles(exercises_directory);

	this->workouts_directory=w_directory;
	workout_files=LoadDirectoryFiles(w_directory);

	this->users_directory=users_directory;
	user_files = LoadDirectoryFiles(users_directory);

	helper.add_files_to_vector(
			exercise_vector,
			std::function<Exercise*(json)>([this](json j) { return this->exercise_from_json(j);}),
			exercise_files
			);

	helper.add_files_to_vector(
			workout_vector,
			std::function<Workout*(json)>([this](json j) { return this->workout_from_json(j);}),
			workout_files
			);

	helper.add_files_to_vector(
			user_vector,
			std::function<User*(json)>([this](json j) { return this->user_from_json(j);}),
			user_files
			);

}


//Reading
Exercise* FileManager::exercise_from_json(json j) 
{
	std::string type_string = j["type"].get<std::string>();  
	ExerciseType type=helper.exercise_type_from_string(type_string);

	Exercise* exercise = nullptr;
	std::string name = j["name"].get<std::string>();    
	std::string description = j["description"].get<std::string>();

	std::string image_path = j["photo"].get<std::string>();
	Image photo_image = LoadImage(image_path.c_str());
	Texture2D photo = LoadTextureFromImage(photo_image);


	if(type==ExerciseType::Running) {
		double intensity_factor = j["intensity_factor"].get<double>();
		exercise = new Running(name, description, photo, intensity_factor);        

	}
	else if(type==ExerciseType::Calisthenics || type==ExerciseType::Weight) {
		std::string muscle_group = j["muscle_group"].get<std::string>();
		double calories_per_rep = j["calories_per_rep"].get<double>();
		if(type==ExerciseType::Calisthenics)  exercise = new Calisthenics(name, description, photo, calories_per_rep, muscle_group);    
		else if(type==ExerciseType::Weight) exercise = new Weight(name, description, photo, calories_per_rep, muscle_group);    
	}

	exercise->set_type(type);

	return exercise;
}



Workout* FileManager::workout_from_json(json workout_json) {

	std::string workout_name = workout_json["workout_name"].get<std::string>();
	std::string workout_description = workout_json["workout_description"].get<std::string>();
	std::vector<Exercise*> new_exercises;

	for (const auto& j : workout_json["exercises"]) {

		std::string name = j["name"].get<std::string>();

		Exercise* new_exercise = nullptr;

		for (Exercise* exercise : exercise_vector) {
			if (exercise->get_name() == name) {
				ExerciseType type=exercise->get_type();
				switch(type) {
					case ExerciseType::Calisthenics: {
										 int max_repetitions=j["max_repetitions"].get<int>();
										 int max_sets=j["max_sets"].get<int>();
										 new_exercise = new Calisthenics(*dynamic_cast<Calisthenics*>(exercise), max_repetitions, max_sets);	
										 break;
									 }

					case ExerciseType::Weight: {
									   int max_repetitions=j["max_repetitions"].get<int>();
									   int max_sets=j["max_sets"].get<int>();
									   double weight=j["weight"].get<double>();
									   new_exercise = new Weight(*dynamic_cast<Weight*>(exercise),max_repetitions, max_sets, weight);

									   break;
								   }

					case ExerciseType::Running: {
									    double max_distance=j["max_distance"].get<double>();
									    double time=j["time"].get<double>();
									    new_exercise = new Running(*dynamic_cast<Running*>(exercise), max_distance, time);
									    break;
								    }
				}
				if(new_exercise) new_exercises.push_back(new_exercise);
				break;
			}
		}
	}
	return new Workout(workout_name, workout_description, new_exercises);
}

User* FileManager::user_from_json(json user_json) {
	std::string user_name = user_json["name"].get<std::string>();
	double user_kg = user_json["kg"].get<double>();

	User* new_user = new User(user_name, user_kg);

	for (const auto& workout_name : user_json["workouts"]) {
		std::string workout_name_str = workout_name.get<std::string>();

		Workout* matched_workout = nullptr;
		for (Workout* workout : workout_vector) {
			if (workout->get_name() == workout_name_str) {
				matched_workout = workout;
				break;
			}
		}

		if (matched_workout) {
			new_user->get_workout_vector()->push_back(new Workout(*matched_workout));
		} else {
			std::cerr << "Workout not found: " << workout_name_str << " for user: " << user_name << std::endl;
		}
	}

	return new_user;
}

//Adding

void FileManager::add_user(const std::string& username, double user_kgs) {
	json user_json;
	user_json["name"] = username;
	user_json["kg"] = user_kgs;
	std::vector<std::string> strings;
	user_json["workouts"] = strings;

	std::string file_path = std::string(users_directory) + "/" + username + ".json";
	std::ofstream user_file(file_path);
	if (user_file.is_open()) {
		user_file << user_json.dump(4); 
		user_file.close();
	} else {
		std::cerr << "Error creating user file: " << file_path << std::endl;
		return;
	}

	std::ifstream read_file(file_path);
	if (read_file.is_open()) {
		json read_user_json;
		read_file >> read_user_json;
		read_file.close();

		User* new_user = user_from_json(read_user_json);
		if (new_user) {
			user_vector.push_back(new_user);
		} else {
			std::cerr << "Error reading user from JSON file: " << file_path << std::endl;
		}
	} else {
		std::cerr << "Error opening user file for reading: " << file_path << std::endl;
	}
}


void FileManager::add_workout_to_user(const std::string& username, const std::string& workout_name) {

	std::string susername= "/"+username;
	std::string user_json_path=users_directory +  susername + ".json";
	auto user_it = std::find_if(user_vector.begin(), user_vector.end(), [&username](User* user) {
			return user->get_name() == username;
			});

	if (user_it == user_vector.end()) {
		throw std::runtime_error("User not found: " + username);
	}

	User* user = *user_it;

	auto workout_it = std::find_if(workout_vector.begin(), workout_vector.end(), [&workout_name](Workout* workout) {
			return workout->get_name() == workout_name;
			});

	if (workout_it == workout_vector.end()) {
		throw std::runtime_error("Workout not found: " + workout_name);
	}

	Workout* workout = *workout_it;

	user->get_workout_vector()->push_back(new Workout(*workout));

	std::ifstream input_file(user_json_path);
	if (!input_file.is_open()) {
		throw std::runtime_error("Failed to open JSON file: " + user_json_path);
	}

	json user_json;
	input_file >> user_json;
	input_file.close();

	if (!user_json.contains("workouts") || !user_json["workouts"].is_array()) {
		throw std::runtime_error("\"workouts\" field is missing or is not an array in the JSON file.");
	}

	if (std::find(user_json["workouts"].begin(), user_json["workouts"].end(), workout_name) == user_json["workouts"].end()) {
		user_json["workouts"].push_back(workout_name);
	}

	std::ofstream output_file(user_json_path);
	if (!output_file.is_open()) {
		throw std::runtime_error("Failed to write to JSON file: " + user_json_path);
	}

	output_file << user_json.dump(4);
	output_file.close();
}



//Getters
std::vector<User*> FileManager::get_user_vector(){
	return user_vector;
}

std::vector<Workout*> FileManager::get_workout_vector(){
	return workout_vector;
}

std::vector<Exercise*> FileManager::get_exercise_vector() 
{
	return exercise_vector;
}


//Helper

ExerciseType FileManagerHelper::exercise_type_from_string(const std::string& type_str) 
{
	if(type_str=="Calisthenics")
		return ExerciseType::Calisthenics;

	if(type_str=="Running")
		return ExerciseType::Running;

	if(type_str=="Weight")
		return ExerciseType::Weight;

	throw std::invalid_argument("Invalid ExerciseType string");
}


template <typename T> void FileManagerHelper::add_files_to_vector(std::vector<T>& vector, std::function<T(json)> from_json, FilePathList files)
{
	unsigned int index;
	for(index=0; index<files.count; index++) {
		std::ifstream file(files.paths[index]);
		if (file.is_open()) {
			std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			try {
				json j = json::parse(file_content);
				vector.push_back(from_json(j));
			}
			catch (const json::exception& e) {
				std::cerr << "Error parsing JSON from file: " << files.paths[index] << "\n" << e.what() << std::endl;
			}
		} 
		else std::cerr << "Error opening file: " << files.paths[index] << std::endl;
	}
}
