#include <iostream>
#include "file_manager.h"
#include <fstream>

ExerciseType exercise_type_from_string(const std::string& type_str) {
	if (type_str == "Calisthenics") {
		return ExerciseType::Calisthenics;
	} else if (type_str == "Running") {
		return ExerciseType::Running;
	} else if (type_str == "Weight") {
		return ExerciseType::Weight;
	}
	throw std::invalid_argument("Invalid ExerciseType string");
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
										 new_exercise = new Calisthenics(*dynamic_cast<Calisthenics*>(exercise));	
										 dynamic_cast<Calisthenics*>(new_exercise)->set_reps(max_repetitions); 
										 dynamic_cast<Calisthenics*>(new_exercise)->set_sets(max_sets); 
										 break;
									 }

					case ExerciseType::Weight: {
									   int max_repetitions=j["max_repetitions"].get<int>();
									   int max_sets=j["max_sets"].get<int>();
									   double weight=j["weight"].get<double>();
									   new_exercise = new Weight(*dynamic_cast<Weight*>(exercise));
									   dynamic_cast<Weight*>(new_exercise)->set_reps(max_repetitions);
									   dynamic_cast<Weight*>(new_exercise)->set_sets(max_sets); 
									   dynamic_cast<Weight*>(new_exercise)->set_weight(weight);

									   break;
								   }

					case ExerciseType::Running: {
									    double max_distance=j["max_distance"].get<double>();
									    double time=j["time"].get<double>();
									    new_exercise = new Running(*dynamic_cast<Running*>(exercise));
									    dynamic_cast<Running*>(new_exercise)->set_distance(max_distance);
									    dynamic_cast<Running*>(new_exercise)->set_time(time);      
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

Exercise* FileManager::exercise_from_json(json j) 
{
	try{
		std::string type_string = j["type"].get<std::string>();  
		ExerciseType type=exercise_type_from_string(type_string);

		Exercise* exercise = nullptr;

		std::string name = j["name"].get<std::string>();    
		std::string description = j["description"].get<std::string>();

		std::string image_path = j["photo"].get<std::string>();
		Image photo_image = LoadImage(image_path.c_str());
		Texture2D photo = LoadTextureFromImage(photo_image);

		switch(type) {
			case ExerciseType::Calisthenics: {
								 std::string muscle_group = j["muscle_group"].get<std::string>();
								 double calories_per_rep = j["calories_per_rep"].get<double>();
								 Calisthenics* calisthenics = new Calisthenics(name, description, photo, calories_per_rep, muscle_group);    
								 exercise=calisthenics;  

								 break;
							 }

			case ExerciseType::Weight: {
							   std::string muscle_group = j["muscle_group"].get<std::string>();
							   double calories_per_rep = j["calories_per_rep"].get<double>();
							   Weight* weight = new Weight(name, description, photo, calories_per_rep, muscle_group);    
							   exercise=weight;    
							   break;
						   }
			case ExerciseType::Running: {


							    double intensity_factor = j["intensity_factor"].get<double>();
							    Running* running = new Running(name, description, photo, intensity_factor);    
							    exercise=running;    
							    break;
						    }

		}
		exercise->type=type;
		return exercise;
	} catch(const json::exception& e) {
		std::cout<<"Error parsing json workout"<<e.what();
		return nullptr;
	}
}

User* FileManager::read_user_from_json(json user_json) {
	try {
		std::string user_name = user_json["name"].get<std::string>();
		double user_kg = user_json["kg"].get<double>();

		User* new_user = new User(user_name, user_kg);

		for (const auto& workout_name : user_json["workouts"]) {
			std::string workout_name_str = workout_name.get<std::string>();

			Workout* matched_workout = nullptr;
			for (Workout* workout : workout_vector) {
				if (workout->name == workout_name_str) {
					matched_workout = workout;
					break;
				}
			}

			if (matched_workout) {
				new_user->workout_vector.push_back(new Workout(*matched_workout));
			} else {
				std::cerr << "Workout not found: " << workout_name_str << " for user: " << user_name << std::endl;
			}
		}

		return new_user;
	} catch (const json::exception& e) {
		std::cerr << "Error parsing user JSON: " << e.what() << std::endl;
		return nullptr;
	}
}

FileManager::FileManager(const char* exercises_directory, const char* w_directory, const char* users_directory) 
{
	exercise_files=LoadDirectoryFiles(exercises_directory);
	unsigned int index;
	for(index=0; index<exercise_files.count; index++) {
		std::ifstream file(exercise_files.paths[index]);
		if (file.is_open()) {
			std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			try {
				nlohmann::json j = nlohmann::json::parse(file_content);

				exercise_vector.push_back(exercise_from_json(j));
			} catch (const nlohmann::json::exception& e) {
				std::cerr << "Error parsing JSON from file: " << exercise_files.paths[index] << "\n" << e.what() << std::endl;
			}
		} else {
			std::cerr << "Error opening file: " << exercise_files.paths[index] << std::endl;
		}
	}

	workouts_directory=w_directory;
	workout_files=LoadDirectoryFiles(w_directory);
	for(index=0; index<workout_files.count; index++) {
		std::ifstream file1(workout_files.paths[index]);
		if (file1.is_open()) {
			std::string file_content1((std::istreambuf_iterator<char>(file1)), std::istreambuf_iterator<char>());
			try {
				nlohmann::json j1 = nlohmann::json::parse(file_content1);

				workout_vector.push_back(workout_from_json(j1));
			} catch (const nlohmann::json::exception& e1) {
				std::cerr << "Error parsing JSON from workout file: " << workout_files.paths[index] << "\n" << e1.what() << std::endl;
			}
		} else {
			std::cerr << "Error opening file: " << workout_files.paths[index] << std::endl;
		}
	}


	user_files = LoadDirectoryFiles(users_directory);
	this->users_directory=users_directory;
	for (index = 0; index < user_files.count; index++) {
		std::ifstream file2(user_files.paths[index]);
		if (file2.is_open()) {
			std::string file_content2((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());
			try {
				nlohmann::json j2 = nlohmann::json::parse(file_content2);
				user_vector.push_back(read_user_from_json(j2));
			} catch (const nlohmann::json::exception& e2) {
				std::cerr << "Error parsing JSON from user file: " << user_files.paths[index] << "\n" << e2.what() << std::endl;
			}
		} else {
			std::cerr << "Error opening file: " << user_files.paths[index] << std::endl;
		}


	}	
}


void FileManager::add_user(const std::string& username, double user_kgs) {
	json user_json;
	user_json["name"] = username;
	user_json["kg"] = user_kgs;
	std::vector<std::string> strings;
	user_json["workouts"] = strings;

	std::string file_path = std::string(users_directory) + "/" + username + ".json";
	std::ofstream user_file(file_path);
	if (user_file.is_open()) {
		user_file << user_json.dump(4); // Write JSON with 4-space indentation
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

		User* new_user = read_user_from_json(read_user_json);
		if (new_user) {
			user_vector.push_back(new_user);
		} else {
			std::cerr << "Error reading user from JSON file: " << file_path << std::endl;
		}
	} else {
		std::cerr << "Error opening user file for reading: " << file_path << std::endl;
	}
}



std::vector<Exercise*> FileManager::get_exercise_vector() 
{
	return exercise_vector;
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
			return workout->name == workout_name;
			});

	if (workout_it == workout_vector.end()) {
		throw std::runtime_error("Workout not found: " + workout_name);
	}

	Workout* workout = *workout_it;

	user->workout_vector.push_back(new Workout(*workout));

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


std::vector<User*> FileManager::get_user_vector(){
	return user_vector;
}


std::vector<Workout*> FileManager::get_workout_vector(){
	return workout_vector;
}
