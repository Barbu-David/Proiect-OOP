#include "display.h"
#include <raylib.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "sorter.h"
#include <fstream>
#include <cstring>


std::string to_string_with_precision(double value, int precision) {
	std::ostringstream out;
	out << std::fixed << std::setprecision(precision) << value;
	return out.str();
}


Display::Display(){

	Image img3 = LoadImage("assets/ok.jpg");
	Image img2 = LoadImage("assets/back.jpg");
	Image img = LoadImage("assets/plus.jpg");
	this->plus=LoadTextureFromImage(img);
	this->back=LoadTextureFromImage(img2);
	this->ok=LoadTextureFromImage(img3);
	this->display_workout=true;
	this->current_workout=nullptr;
	this->display_user=true;
	this->user_box=false;
	this->workout_box=false;
	this->fm=new FileManager("./data/exercises", "./data/workouts", "./data/users");
	this->scrollY=0;
	this->spacing=100;
	this->s1="";
	this->s2="";
	this->startX=0;
	this->startY=0;

}

void Display::DrawBackTexture(bool& flag) {
		
	float backWidth = 40; 
	float backHeight = 40; 
	float backPosX = 780 - backWidth;
	float backPosY = 20; 
	Rectangle backRect = {backPosX, backPosY, backWidth, backHeight};

	DrawTexturePro(
			back, // The "back" texture
			{0, 0, (float)back.width, (float)back.height}, // Source rectangle
			backRect, // Destination rectangle with scaling
			{0, 0}, // Origin (no rotation)
			0.0f, // Rotation (no rotation)
			WHITE // Tint color
		      );

	if(CheckCollisionPointRec(GetMousePosition(), backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		flag = true;


}

void Display::DrawAddButton(bool& flag, std::string text, float currentY ) {

	float boxPadding=10, boxHeight=60;
        Rectangle addRect = {startX, currentY, GetScreenWidth() - startX * 2, boxHeight};

        DrawRectangleRec(addRect, LIGHTGRAY);
        DrawRectangleLinesEx(addRect, 2, DARKGRAY);

        float addTextStartX = startX + boxPadding;
        float addTextStartY = currentY + boxPadding;
	DrawTextEx(GetFontDefault(), text.c_str(), {addTextStartX, addTextStartY}, 24, 2, BLACK);

	if (CheckCollisionPointRec(GetMousePosition(), addRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
		flag=true;  

}

void Display::RenderWorkoutList() {

	double kg_param=current_user->get_kg();	
	std::string username=current_user->get_name();
	std::vector<Workout*> workouts=current_user->workout_vector;

	float currentY = startY - scrollY; 
	float boxPadding = 10; 
	float lineSpacing = 20; 
	float boxSpacing = 30; 

	DrawBackTexture(display_user);

	// Draw the username and title at the top
	std::string title = username + "'s workouts  " +  to_string_with_precision(kg_param, 1) + " kgs";
	DrawTextEx(GetFontDefault(), title.c_str(), {startX, currentY}, 28, 2, BLACK);
	currentY += 50; 

	float boxHeight = 2 * lineSpacing + boxPadding * 2; // Box height based on text and padding

	for (const auto& workout : workouts) {
		if (!workout) continue; // Skip if workout is a nullptr

		// Define the rectangle for this workout
		Rectangle workoutRect = {startX, currentY, GetScreenWidth() - startX * 2, boxHeight};

		// Draw the rectangle
		DrawRectangleRec(workoutRect, LIGHTGRAY);
		DrawRectangleLinesEx(workoutRect, 2, DARKGRAY);

		// Draw the workout name
		float textStartX = startX + boxPadding;
		float textStartY = currentY + boxPadding;
		DrawTextEx(GetFontDefault(), workout->name.c_str(), {textStartX, textStartY}, 24, 2, BLACK);

		// Draw the workout description
		DrawTextEx(GetFontDefault(), workout->description.c_str(), {textStartX, textStartY + lineSpacing}, 20, 1, DARKGRAY);

		if (CheckCollisionPointRec(GetMousePosition(), workoutRect)) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				display_workout = false; 
				current_workout = workout;
			}
		}

		currentY += boxHeight + boxSpacing;
	}

	DrawAddButton(workout_box, "Add workout", currentY);
	if(workout_box) DrawWorkoutInputBox(s1);

}

void Display::RenderExerciseList() {

	std::string workout_name=current_workout->name;
	std::vector<Exercise*> exercises = current_workout->exercises;
	float currentY = startY - scrollY; 
	float textureWidth = 50; 
	float textureHeight = 50; 
	float boxPadding = 10; 
	float lineSpacing = 20; 


	// Draw the workout name at the top
	DrawTextEx(GetFontDefault(), workout_name.c_str(), {startX, currentY}, 28, 2, BLACK);
	currentY += 40; // Add some space below the workout name before the first exercise


	DrawBackTexture(display_workout);

	for (size_t i = 0; i < exercises.size(); ++i) {
		const auto& exercise = exercises[i];

		float texturePosX = startX;
		float texturePosY = currentY;

		// Draw the texture in the scaled rectangle next to the box
		Rectangle textureRect = {texturePosX, texturePosY, textureWidth, textureHeight};

		DrawTexturePro(
				exercise->get_photo(), // The texture
				{0, 0, (float)exercise->get_photo().width, (float)exercise->get_photo().height}, // Source rectangle
				textureRect, // Destination rectangle with scaling
				{0, 0}, // Origin (no rotation)
				0.0f, // Rotation (no rotation)
				WHITE // Tint color
			      );

		float textStartX = texturePosX + textureWidth + boxPadding;

		// Draw exercise name
		DrawTextEx(GetFontDefault(), exercise->get_name().c_str(), {textStartX, currentY}, 24, 2, BLACK);

		// Draw exercise description
		DrawTextEx(GetFontDefault(), exercise->get_description().c_str(), {textStartX, currentY + lineSpacing}, 16, 1, DARKGRAY);

		// Draw progress details based on the type of exercise
		ExerciseType type = exercise->type;
		std::string progressDetails;

		if (type == ExerciseType::Calisthenics || type == ExerciseType::Weight) {
			auto* calisthenics = dynamic_cast<Calisthenics*>(exercise);
			if (calisthenics) {
				progressDetails = "Reps: " + std::to_string(calisthenics->get_repetitons()) + "/" + std::to_string(calisthenics->get_max_repetitions()) + 
					", Sets: " + std::to_string(calisthenics->get_sets()) + "/" + std::to_string(calisthenics->get_max_sets()) +
					", Muscle Group: " + calisthenics->get_muscle_group();


				if(type==ExerciseType::Weight) {
					progressDetails=progressDetails + "Selected weight: KG: " + to_string_with_precision(GetWeightByExerciseName(calisthenics->get_name())  ,1);
				}
			} else progressDetails="Error";	

		}

		else if(type==ExerciseType::Running) {
			auto* running = dynamic_cast<Running*>(exercise);
			if (running) {
				progressDetails = "Distance:" + to_string_with_precision(running->get_distance(), 2) + "/" + to_string_with_precision(running->get_max_distance(), 2) + " km" +
					", Time: " + to_string_with_precision(running->get_time(), 2) + " m" +
					", Intensity Factor: " + to_string_with_precision(running->get_intensity_factor(), 1);
			} else progressDetails="Error";	

		}
		
		// Draw the progress details
		DrawTextEx(GetFontDefault(), progressDetails.c_str(), {textStartX, currentY + 2 * lineSpacing}, 16, 1, BLACK);
		// Draw Calories burned info
		std::string caloriesDetails = "Calories burned: " + to_string_with_precision(exercise->calculate_calories(90),1) + "/" + to_string_with_precision(exercise->calculate_calories_max(90), 1);
		DrawTextEx(GetFontDefault(), caloriesDetails.c_str(), {textStartX, currentY + 3 * lineSpacing}, 16, 1, DARKGRAY);

		// Draw progress bar
		double progress = exercise->get_progress();
		float barWidth = 200;
		float barHeight = 20;
		float filledWidth = static_cast<int>(barWidth * progress);

		DrawRectangle(textStartX, currentY + 4 * lineSpacing, barWidth, barHeight, LIGHTGRAY);
		DrawRectangle(textStartX, currentY + 4 * lineSpacing, filledWidth, barHeight, GREEN);
		DrawRectangleLines(textStartX, currentY + 4 * lineSpacing, barWidth, barHeight, BLACK);


		float plusPosX = textStartX + barWidth + 10; 
		float plusPosY = currentY + 4 * lineSpacing;  

		Rectangle plusRect = {plusPosX, plusPosY, textureWidth * 0.5f, textureHeight * 0.5f};
		DrawTexturePro(
				plus, 
				{0, 0, (float)plus.width, (float)plus.height},
				plusRect, 
				{0, 0}, 
				0.0f, 
				WHITE 
			      );

		if (CheckCollisionPointRec(GetMousePosition(), plusRect)) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				exercise->update_progress(); 
			}
		}

		currentY += textureHeight + spacing + lineSpacing -20;
	}
}

void Display::RenderUserList() {

	std::vector<User*> users=fm->get_user_vector();

	float currentY = startY - scrollY; 
	float boxPadding = 10.0f;
	float lineSpacing = 20.0f;
	float boxHeight = 60.0f; 

	std::string title = "Select user:";
	DrawTextEx(GetFontDefault(), title.c_str(), {startX, currentY}, 28, 2, BLACK);
	currentY += 50; 

	for (const auto& user : users) {
		if (!user) continue; 

		Rectangle userRect = {startX, currentY, GetScreenWidth() - startX * 2, boxHeight};

		DrawRectangleRec(userRect, LIGHTGRAY);
		DrawRectangleLinesEx(userRect, 2, DARKGRAY);

		float textStartX = startX + boxPadding;
		float textStartY = currentY + boxPadding;
		DrawTextEx(GetFontDefault(), user->get_name().c_str(), {textStartX, textStartY}, 24, 2, BLACK);

		std::string weightText = to_string_with_precision(user->get_kg(), 1) + " kg";
		DrawTextEx(GetFontDefault(), weightText.c_str(), {textStartX, textStartY + lineSpacing}, 20, 1, DARKGRAY);

		if (CheckCollisionPointRec(GetMousePosition(), userRect)) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				display_user=false;
				current_user=user;
			}
		}

		currentY += boxHeight + spacing/2;
	}

	DrawAddButton(user_box, "Add user", currentY);
	if(user_box) DrawUserInputBox(s1, s2);
}


void Display::Render() {
	if (display_user) {
		RenderUserList();
	} else if (display_workout && current_user != nullptr) {
		RenderWorkoutList();
	} else if (current_workout != nullptr) {
		RenderExerciseList();
		handleKeyPressAndSort(current_workout->exercises);
	}

	UpdateScroll();
}


void GetTextInput(std::string &buffer, int maxLength) {
	int key;
	// GetCharPressed reads the next pressed character in the input queue
	while ((key = GetCharPressed()) != 0) {
		if (key >= 32 && key <= 125 && buffer.size() <(unsigned int) maxLength - 1) {
			buffer += (char)key;
		}
	}

	// Handle backspace
	if (IsKeyPressed(KEY_BACKSPACE) && !buffer.empty()) {
		buffer.pop_back();
	}
}


void Display::DrawUserInputBox(std::string &username, std::string &weightKg) {
	static bool usernameActive = false;
	static bool weightActive = false;

	// --- Overall Background ---
	float startX = 50;
	float startY = 100;
	float boxWidth = GetScreenWidth() - startX * 2;
	float boxHeight = 40;
	float padding = 10;

	Rectangle outerBackgroundRect = {startX - 10, startY - 10, boxWidth + 20, (boxHeight + padding * 2) * 3 + 20};
	DrawRectangleRec(outerBackgroundRect, LIGHTGRAY);  // Fill the background

	// --- USERNAME FIELD ---
	Rectangle usernameRect = {startX, startY, boxWidth, boxHeight};
	DrawRectangleRec(usernameRect, usernameActive ? DARKGRAY : RAYWHITE);  // Highlight if active
	DrawRectangleLinesEx(usernameRect, 2, DARKGRAY);
	DrawTextEx(GetFontDefault(), ("Username: " + username).c_str(), {startX + padding, startY + padding}, 24, 2, BLACK);

	if (CheckCollisionPointRec(GetMousePosition(), usernameRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		usernameActive = true;
		weightActive = false;
	}

	// --- WEIGHT FIELD ---
	Rectangle weightRect = {startX, startY + boxHeight + padding * 2, boxWidth, boxHeight};
	DrawRectangleRec(weightRect, weightActive ? DARKGRAY : RAYWHITE);  // Highlight if active
	DrawRectangleLinesEx(weightRect, 2, DARKGRAY);
	DrawTextEx(GetFontDefault(), ("Weight (kg): " + weightKg).c_str(), {startX + padding, startY + boxHeight + padding * 3}, 24, 2, BLACK);

	if (CheckCollisionPointRec(GetMousePosition(), weightRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		weightActive = true;
		usernameActive = false;
	}

	// Handle text input for the active field
	if (usernameActive) {
		GetTextInput(username, 64);
	}
	if (weightActive) {
		GetTextInput(weightKg, 64);
	}

	// --- BACK BUTTON ---
	Rectangle backRect = {startX, startY + (boxHeight + padding) * 2 + padding * 2, 100, 40};
	DrawTexturePro(back, {0, 0, (float)back.width, (float)back.height}, backRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {	
		username="";
		weightKg="";
		user_box=false;
		return;
	}

	// --- OK BUTTON ---
	Rectangle okRect = {(float)GetScreenWidth() - 150, startY + (boxHeight + padding) * 2 + padding * 2, 100, 40};
	DrawTexturePro(ok, {0, 0, (float)ok.width, (float)ok.height}, okRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), okRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		fm->add_user(username, std::stod(weightKg, nullptr));

		username="";
		weightKg="";
		user_box=false;

	}

	// --- Outer Rectangle Border ---
	DrawRectangleLinesEx(outerBackgroundRect, 4, DARKGRAY);  // Draw border around the UI
}

void Display::DrawWorkoutInputBox(std::string &workoutname) {

	float startX = 50;
	float startY = 100;
	float boxWidth = GetScreenWidth() - startX * 2;
	float boxHeight = 40;
	float padding = 10;

	Rectangle outerBackgroundRect = {startX - 10, startY - 10, boxWidth + 20, (boxHeight + padding * 2) * 3 + 20};
	DrawRectangleRec(outerBackgroundRect, LIGHTGRAY);  // Fill the background

	Rectangle usernameRect = {startX, startY, boxWidth, boxHeight};
	DrawRectangleRec(usernameRect, RAYWHITE);  // Highlight if active
	DrawRectangleLinesEx(usernameRect, 2, DARKGRAY);
	DrawTextEx(GetFontDefault(), ("Workout: " + workoutname).c_str(), {startX + padding, startY + padding}, 24, 2, BLACK);


	GetTextInput(workoutname, 64);

	// --- BACK BUTTON ---
	Rectangle backRect = {startX, startY + (boxHeight + padding) * 2 + padding * 2, 100, 40};
	DrawTexturePro(back, {0, 0, (float)back.width, (float)back.height}, backRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {	
		workoutname="";
		workout_box=false;
		return;
	}

	// --- OK BUTTON ---
	Rectangle okRect = {(float)GetScreenWidth() - 150, startY + (boxHeight + padding) * 2 + padding * 2, 100, 40};
	DrawTexturePro(ok, {0, 0, (float)ok.width, (float)ok.height}, okRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), okRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

		try {
		fm->add_workout_to_user(current_user->get_name(), workoutname); }

		catch (std::runtime_error& e) {
			std::cout<<"Error adding user ";
		}
		workoutname="";
		workout_box=false;

	}

	DrawRectangleLinesEx(outerBackgroundRect, 4, DARKGRAY);  // Draw border around the UI
}


double Display::GetWeightByExerciseName(const std::string& exercise_name) {
	for (Workout* workout : fm->get_workout_vector()) {
		for (Exercise* exercise : workout->exercises) {
			// Dynamic cast to Weight type
			Weight* weight_exercise = dynamic_cast<Weight*>(exercise);
			if (weight_exercise && exercise->get_name() == exercise_name) {
				return weight_exercise->get_weight();
			}
		}
	}
	return 0.0;
}


void Display::UpdateScroll() {
	float wheelMove = GetMouseWheelMove();

	if (wheelMove != 0) {
		scrollY -= wheelMove * 40; 
	}

	if (IsKeyPressed(KEY_DOWN)) {
		scrollY += 10; // Scroll down
	}
	if (IsKeyPressed(KEY_UP)) {
		scrollY -= 10; // Scroll up
	}
}

