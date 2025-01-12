#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include "file_manager.h"

class Display {
	public:
		Display();
		void Render(); 
	private:
		void RenderExerciseList();
		void RenderWorkoutList();
		void RenderUserList();

		void UpdateScroll();
		Workout* current_workout;
		bool display_workout, display_user, user_box, workout_box;
		Texture2D plus, back, ok;
		User* current_user;

		std::string s1, s2;
		void DrawUserInputBox(std::string &username, std::string &weightKg);
		void DrawWorkoutInputBox(std::string &workoutname);
		FileManager* fm;

		float scrollY, spacing, startX, startY;
		double GetWeightByExerciseName(const std::string& exercise_name);
		void DrawBackTexture(bool& flag);
		void DrawAddButton(bool& flag, std::string text, float currentY);
};


#endif
