#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include "file_manager.h"
#include "display_helper.h"

class Display {
	public:
		Display();
		void Render(); 
	private:
		DisplayHelper helper;
		FileManager* fm;

		Workout* current_workout;
		User* current_user;
	
		std::string s1, s2;

		Texture2D plus, back, ok;

		bool display_workout, display_user, user_box, workout_box;
		float scrollY, spacing, startX, startY;

		void RenderExerciseList();
		void RenderWorkoutList();
		void RenderUserList();

		void UpdateScroll();

		void DrawUserInputBox(std::string &username, std::string &weightKg);
		void DrawWorkoutInputBox(std::string &workoutname);

		double GetWeightByExerciseName(const std::string& exercise_name);
		void DrawBackTexture(bool& flag);
		void DrawAddButton(bool& flag, std::string text, float currentY);
};


#endif
