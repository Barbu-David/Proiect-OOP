#include "display.h"
#include <raylib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstring>
#include "gui_constants.h"

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
	this->s1="";
	this->s2="";
}

void Display::DrawBackTexture(bool& flag) {
		
	float backWidth = 40; 
	float backHeight = 40; 
	float backPosX = 780 - backWidth;
	float backPosY = 20; 
	Rectangle backRect = {backPosX, backPosY, backWidth, backHeight};

	DrawTexturePro(
			back, 
			{0, 0, (float)back.width, (float)back.height}, 
			backRect, 
			{0, 0}, 
			0.0f, 
			WHITE 
		      );

	if(CheckCollisionPointRec(GetMousePosition(), backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		flag = true;


}

void Display::DrawAddButton(bool& flag, std::string text, float currentY ) {

	float boxHeight=60;
        Rectangle addRect = {0, currentY, float(GetScreenWidth()), boxHeight};

        DrawRectangleRec(addRect, LIGHTGRAY);
        DrawRectangleLinesEx(addRect, 2, DARKGRAY);

	DrawTextEx(GetFontDefault(), text.c_str(), {BOX_PADDING, BOX_PADDING + currentY}, MEDIUM_FONT, TEXT_SPACING, BLACK);

	if (CheckCollisionPointRec(GetMousePosition(), addRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
		flag=true;  

}

void Display::RenderWorkoutList() {

	//Constants
	const float boxSpacing = 30; 
	const float boxHeight = 60; 
	const std::string username=current_user->get_name();
	const double kg_param=current_user->get_kg();	
	const int precision =1;

	
	//Variables	
	float currentY = - scrollY; 
	std::vector<Workout*>* workouts=current_user->get_workout_vector();

	//Back button texture
	DrawBackTexture(display_user);

	//Title
	std::string title = username + "'s workouts  " +  helper.to_string_with_precision(kg_param, precision) + " kgs";
	DrawTextEx(GetFontDefault(), title.c_str(), {0, currentY}, BIG_FONT, TEXT_SPACING, BLACK);
	currentY += 50; 

	//Draw each workout box
	for (const auto& workout : *workouts) {
		Rectangle workoutRect = {0, currentY, float(GetScreenWidth()), boxHeight};
		DrawRectangleRec(workoutRect, LIGHTGRAY);
		
		DrawRectangleLinesEx(workoutRect, 2, DARKGRAY);
		DrawTextEx(GetFontDefault(), workout->get_name().c_str(), {BOX_PADDING, currentY + BOX_PADDING}, BIG_FONT, TEXT_SPACING, BLACK);
		DrawTextEx(GetFontDefault(), workout->get_description().c_str(), {BOX_PADDING, currentY + BOX_PADDING + LINE_SPACING}, MEDIUM_FONT, TEXT_SPACING, DARKGRAY);

		if (CheckCollisionPointRec(GetMousePosition(), workoutRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { 
				display_workout = false; 
				current_workout = workout;
		}
		currentY += boxHeight + boxSpacing;
	}

	DrawAddButton(workout_box, "Add workout", currentY);
	if(workout_box) DrawWorkoutInputBox(s1);

}

void Display::RenderExerciseList() {

	std::string workout_name=current_workout->get_name();
	std::vector<Exercise*> exercises = *(current_workout->get_exercises());
	float currentY = - scrollY; 
	float textureWidth = 50; 
	float textureHeight = 50; 


	DrawTextEx(GetFontDefault(), workout_name.c_str(), {0, currentY}, BIG_FONT, TEXT_SPACING, BLACK);
	currentY += 40; 


	DrawBackTexture(display_workout);

	for (size_t i = 0; i < exercises.size(); ++i) {
		const auto& exercise = exercises[i];

		float texturePosX = 0;
		float texturePosY = currentY;

		Rectangle textureRect = {texturePosX, texturePosY, textureWidth, textureHeight};

		DrawTexturePro(
				exercise->get_photo(), 
				{0, 0, (float)exercise->get_photo().width, (float)exercise->get_photo().height}, 
				textureRect, 
				{0, 0}, 
				0.0f, 
				WHITE 
			      );

		float textStartX = texturePosX + textureWidth + BOX_PADDING;

		DrawTextEx(GetFontDefault(), exercise->get_name().c_str(), {textStartX, currentY}, MEDIUM_FONT, TEXT_SPACING, BLACK);

		DrawTextEx(GetFontDefault(), exercise->get_description().c_str(), {textStartX, currentY + LINE_SPACING}, SMALL_FONT, TEXT_SPACING, DARKGRAY);

		std::string progressDetails=helper.getProgressDetails(exercise);

		DrawTextEx(GetFontDefault(), progressDetails.c_str(), {textStartX, currentY + 2 * LINE_SPACING}, SMALL_FONT, TEXT_SPACING, BLACK);
		std::string caloriesDetails = "Calories burned: " + helper.to_string_with_precision(exercise->calculate_calories(current_user->get_kg()),1) + "/" + helper.to_string_with_precision(exercise->calculate_calories_max(current_user->get_kg()), 1);
		DrawTextEx(GetFontDefault(), caloriesDetails.c_str(), {textStartX, currentY + 3 * LINE_SPACING}, SMALL_FONT, TEXT_SPACING, DARKGRAY);

		double progress = exercise->get_progress();
		float barWidth = 200;
		float barHeight = 20;
		float filledWidth = static_cast<int>(barWidth * progress);

		DrawRectangle(textStartX, currentY + 4 * LINE_SPACING, barWidth, barHeight, LIGHTGRAY);
		DrawRectangle(textStartX, currentY + 4 * LINE_SPACING, filledWidth, barHeight, GREEN);
		DrawRectangleLines(textStartX, currentY + 4 * LINE_SPACING, barWidth, barHeight, BLACK);


		float plusPosX = textStartX + barWidth + 10; 
		float plusPosY = currentY + 4 * LINE_SPACING;  

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

		currentY += textureHeight + LINE_SPACING + 80;
	}
}



void Display::RenderUserList() {

	std::vector<User*> users=fm->get_user_vector();

	float currentY =  - scrollY; 
	float boxHeight = 60.0f; 

	std::string title = "Select user:";
	DrawTextEx(GetFontDefault(), title.c_str(), {0, currentY}, BIG_FONT, TEXT_SPACING, BLACK);
	currentY += 50; 

	for (const auto& user : users) {
		if (!user) continue; 

		Rectangle userRect = {0, currentY, float(GetScreenWidth()), boxHeight};

		DrawRectangleRec(userRect, LIGHTGRAY);
		DrawRectangleLinesEx(userRect, 2, DARKGRAY);

		DrawTextEx(GetFontDefault(), user->get_name().c_str(), {BOX_PADDING, currentY + BOX_PADDING}, MEDIUM_FONT, TEXT_SPACING, BLACK);

		std::string weightText = helper.to_string_with_precision(user->get_kg(), 1) + " kg";
		DrawTextEx(GetFontDefault(), weightText.c_str(), {BOX_PADDING, currentY + LINE_SPACING + BOX_PADDING}, MEDIUM_FONT, TEXT_SPACING, DARKGRAY);

		if (CheckCollisionPointRec(GetMousePosition(), userRect)) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				display_user=false;
				current_user=user;
			}
		}

		currentY += boxHeight + 50;
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
		std::vector<Exercise*>* current_vector=current_workout->get_exercises();
		helper.handleKeyPressAndSort(*current_vector);
	}

	UpdateScroll();
}

void Display::DrawUserInputBox(std::string &username, std::string &weightKg) {
	static bool usernameActive = false;
	static bool weightActive = false;

	float startX = 50;
	float startY = 100;
	float boxWidth = GetScreenWidth() - startX * 2;
	float boxHeight = 40;

	Rectangle outerBackgroundRect = {startX - 10, startY - 10, boxWidth + 20, (boxHeight + BOX_PADDING * 2) * 3 + 20};
	DrawRectangleRec(outerBackgroundRect, LIGHTGRAY);  // Fill the background

	Rectangle usernameRect = {startX, startY, boxWidth, boxHeight};
	DrawRectangleRec(usernameRect, usernameActive ? DARKGRAY : RAYWHITE);  // Highlight if active
	DrawRectangleLinesEx(usernameRect, 2, DARKGRAY);
	DrawTextEx(GetFontDefault(), ("Username: " + username).c_str(), {startX + BOX_PADDING, startY + BOX_PADDING}, MEDIUM_FONT, TEXT_SPACING, BLACK);

	if (CheckCollisionPointRec(GetMousePosition(), usernameRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		usernameActive = true;
		weightActive = false;
	}

	Rectangle weightRect = {startX, startY + boxHeight + BOX_PADDING, boxWidth, boxHeight};
	DrawRectangleRec(weightRect, weightActive ? DARKGRAY : RAYWHITE);  // Highlight if active
	DrawRectangleLinesEx(weightRect, 2, DARKGRAY);
	DrawTextEx(GetFontDefault(), ("Weight (kg): " + weightKg).c_str(), {startX + BOX_PADDING, startY + boxHeight + BOX_PADDING_BIG}, MEDIUM_FONT, 2, BLACK);

	if (CheckCollisionPointRec(GetMousePosition(), weightRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		weightActive = true;
		usernameActive = false;
	}

	if (usernameActive) {
		helper.GetTextInput(username, 64);
	}
	if (weightActive) {
		helper.GetTextInput(weightKg, 64);
	}

	Rectangle backRect = {startX, startY + (boxHeight + BOX_PADDING) * 2 + BOX_PADDING_BIG, 100, 40};
	DrawTexturePro(back, {0, 0, (float)back.width, (float)back.height}, backRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {	
		username="";
		weightKg="";
		user_box=false;
		return;
	}

	Rectangle okRect = {(float)GetScreenWidth() - 150, startY + (boxHeight + BOX_PADDING) * 2 + BOX_PADDING_BIG, 100, 40};
	DrawTexturePro(ok, {0, 0, (float)ok.width, (float)ok.height}, okRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), okRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		fm->add_user(username, std::stod(weightKg, nullptr));

		username="";
		weightKg="";
		user_box=false;

	}

	DrawRectangleLinesEx(outerBackgroundRect, 4, DARKGRAY); 
}

void Display::DrawWorkoutInputBox(std::string &workoutname) {

	float startx = 50;
	float starty = 100;
	float boxWidth = float(GetScreenWidth());
	float boxHeight = 40;

	Rectangle outerBackgroundRect = {startx - 10, starty - 10, boxWidth + 20, (boxHeight + BOX_PADDING_BIG) * 3 + 20};
	DrawRectangleRec(outerBackgroundRect, LIGHTGRAY);  

	Rectangle usernameRect = {startx, starty, boxWidth, boxHeight};
	DrawRectangleRec(usernameRect, RAYWHITE); 
	DrawRectangleLinesEx(usernameRect, 2, DARKGRAY);
	DrawTextEx(GetFontDefault(), ("Workout: " + workoutname).c_str(), {BOX_PADDING, BOX_PADDING}, MEDIUM_FONT, TEXT_SPACING, BLACK);


	helper.GetTextInput(workoutname, 64);

	Rectangle backRect = {startx, starty + (boxHeight + BOX_PADDING) * 2 + BOX_PADDING_BIG, 100, 40};
	DrawTexturePro(back, {0, 0, (float)back.width, (float)back.height}, backRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {	
		workoutname="";
		workout_box=false;
		return;
	}

	Rectangle okRect = {(float)GetScreenWidth() - 150, starty + (boxHeight + BOX_PADDING) * 2 + BOX_PADDING_BIG, 100, 40};
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

	DrawRectangleLinesEx(outerBackgroundRect, 4, DARKGRAY);  
}

void Display::UpdateScroll() 
{
	float wheelMove = GetMouseWheelMove();

	if (wheelMove != 0) {
		scrollY -= wheelMove * 40; 
	}

	if (IsKeyPressed(KEY_DOWN)) {
		scrollY += 10; 
	}
	if (IsKeyPressed(KEY_UP)) {
		scrollY -= 10; 
	}
}

