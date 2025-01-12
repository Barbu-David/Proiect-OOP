#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include "file_manager.h"

class Display {
	public:
		Display();
		void RenderExerciseList(const std::vector<Exercise*>& exercises, float startX, float startY, float spacing, const std::string& workout_name);
		void Render(std::vector<User*>& workouts, float startX, float startY, float spacing); 
		void UpdateScroll();
		void RenderWorkoutList(std::vector<Workout*>& workouts, float startX, float startY, float spacing, const std::string& username, double kg_param);
		void RenderUserList(std::vector<User*>& users, float startX, float startY, float spacing);

		Workout* current_workout;
		bool display_workout, display_user, user_box, workout_box;
		Texture2D plus, back, ok;
		User* current_user;

		std::string s1, s2;
		void DrawUserInputBox(std::string &username, std::string &weightKg);
		void DrawWorkoutInputBox(std::string &workoutname);
		FileManager* fm;


		float scrollY;
		double GetWeightByExerciseName(const std::string& exercise_name);



};


#endif
