#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include "file_manager.h"

class Display {
	public:
		Display();
		void RenderExerciseList(const std::vector<Exercise*>& exercises, float startX, float startY, float spacing, float scrolly, const std::string& workout_name);
		void Render(std::vector<User*>& workouts, float startX, float startY, float spacing, float scrollY); 
		static void UpdateScroll(float& scrollY);
		void RenderWorkoutList(std::vector<Workout*>& workouts, float startX, float startY, float spacing, float scrollY, const std::string& username, double kg_param);
		void RenderUserList(std::vector<User*>& users, float startX, float startY, float spacing, float scrollY);

		Workout* current_workout;
		bool display_workout, display_user, user_box, workout_box;
		Texture2D plus, back, ok;
		User* current_user;

		std::string s1="", s2="";
		void DrawUserInputBox(std::string &username, std::string &weightKg);
		void DrawWorkoutInputBox(std::string &workoutname);
		FileManager* fm;

		double GetWeightByExerciseName(const std::string& exercise_name);



};


#endif
