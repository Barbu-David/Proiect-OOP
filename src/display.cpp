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
	FONT=GetFontDefault();
}

void Display::DrawBackTexture(bool& flag) {
		
	Rectangle backRect = {BACK_TEXTURE_X, BACK_TEXTURE_Y, TEXTURE_LENGHT, TEXTURE_LENGHT};

	DrawTexturePro(
			back, 
			{0, 0, (float)back.width, (float)back.height}, 
			backRect, 
			{0, 0}, 
			0.0f, 
			WHITE 
		      );

	if(CheckCollisionPointRec(MousePosition, backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) flag = true;


}

void Display::DrawAddButton(bool& flag, std::string text, float currentY ) {

        Rectangle addRect = {OFFSET_X, currentY, float(GetScreenWidth()), BOX_HEIGHT};

        DrawRectangleRec(addRect, LIGHTGRAY);
        DrawRectangleLinesEx(addRect, LINES_W, DARKGRAY);

	DrawTextEx(FONT, text.c_str(), {BOX_PADDING, BOX_PADDING + currentY}, MEDIUM_FONT, TEXT_SPACING, BLACK);

	if (CheckCollisionPointRec(MousePosition, addRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
		flag=true;  

}

void Display::RenderWorkoutList() {

	const std::string username=current_user->get_name();
	const double kg_param=current_user->get_kg();	
	
	//Variables	
	float currentY = - scrollY; 
	std::vector<Workout*>* workouts=current_user->get_workout_vector();

	//Back button texture
	DrawBackTexture(display_user);

	//Title
	std::string title = username + "'s workouts  " +  helper.to_string_with_precision(kg_param, PRECISION) + " kgs";
	DrawTextEx(FONT, title.c_str(), {OFFSET_X, currentY}, BIG_FONT, TEXT_SPACING, BLACK);
	currentY += SMALL_INCREMENT; 

	//Draw each workout box
	for (const auto& workout : *workouts) {
		Rectangle workoutRect = {OFFSET_X, currentY, float(GetScreenWidth()), BOX_HEIGHT};
		DrawRectangleRec(workoutRect, LIGHTGRAY);
		
		DrawRectangleLinesEx(workoutRect, LINES_W, DARKGRAY);
		DrawTextEx(FONT, workout->get_name().c_str(), {BOX_PADDING, currentY + LINE_SPACING}, BIG_FONT, TEXT_SPACING, BLACK);
		DrawTextEx(FONT, workout->get_description().c_str(), {BOX_PADDING, currentY + LINE_SPACING*2}, MEDIUM_FONT, TEXT_SPACING, DARKGRAY);

		if (CheckCollisionPointRec(MousePosition, workoutRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { 
				display_workout = false; 
				current_workout = workout;
		}
		currentY += INCREMENT;
	}

	DrawAddButton(workout_box, "Add workout", currentY);
	if(workout_box) DrawWorkoutInputBox(s1);

}

void Display::RenderExerciseList() {

	std::string workout_name=current_workout->get_name();
	std::vector<Exercise*> exercises = *(current_workout->get_exercises());
	float currentY = - scrollY; 

	DrawTextEx(FONT, workout_name.c_str(), {OFFSET_X, currentY}, BIG_FONT, TEXT_SPACING, BLACK);
	currentY += SMALL_INCREMENT; 


	DrawBackTexture(display_workout);

	for (size_t i = 0; i < exercises.size(); ++i) {
		const auto& exercise = exercises[i];

		Rectangle textureRect = {OFFSET_X, currentY, TEXTURE_LENGHT, TEXTURE_LENGHT};

		DrawTexturePro(
				exercise->get_photo(), 
				{0, 0, (float)exercise->get_photo().width, (float)exercise->get_photo().height}, 
				textureRect, 
				{0, 0}, 
				0.0f, 
				WHITE 
			      );


		DrawTextEx(FONT, exercise->get_name().c_str(), {BIG_OFFSET_X, currentY}, MEDIUM_FONT, TEXT_SPACING, BLACK);

		DrawTextEx(FONT, exercise->get_description().c_str(), {BIG_OFFSET_X, currentY + LINE_SPACING*2}, SMALL_FONT, TEXT_SPACING, DARKGRAY);

		std::string progressDetails=helper.getProgressDetails(exercise);

		DrawTextEx(FONT, progressDetails.c_str(), {BIG_OFFSET_X, currentY + LINE_SPACING*3}, SMALL_FONT, TEXT_SPACING, BLACK);
		std::string caloriesDetails = "Calories burned: " + helper.to_string_with_precision(exercise->calculate_calories(current_user->get_kg()), PRECISION) + "/" + helper.to_string_with_precision(exercise->calculate_calories_max(current_user->get_kg()), PRECISION);
		DrawTextEx(FONT, caloriesDetails.c_str(), {BIG_OFFSET_X, currentY + LINE_SPACING}, SMALL_FONT, TEXT_SPACING, DARKGRAY);

		double progress = exercise->get_progress();
		float filledWidth = PROGRESS_BAR_W * progress;

		DrawRectangle(BIG_OFFSET_X, currentY + RECTANGLE_HEIGHT, PROGRESS_BAR_W, PROGRESS_BAR_H, LIGHTGRAY);
		DrawRectangle(BIG_OFFSET_X, currentY + RECTANGLE_HEIGHT, filledWidth, PROGRESS_BAR_H, GREEN);
		DrawRectangleLines(BIG_OFFSET_X, currentY + RECTANGLE_HEIGHT, PROGRESS_BAR_W, PROGRESS_BAR_H, BLACK);


		Rectangle plusRect = {PLUS_X, PLUS_Y + currentY, SMALL_TEXTURE_LENGHT, SMALL_TEXTURE_LENGHT};
		DrawTexturePro(
				plus, 
				{0, 0, (float)plus.width, (float)plus.height},
				plusRect, 
				{0, 0}, 
				0.0f, 
				WHITE 
			      );

		if (CheckCollisionPointRec(MousePosition, plusRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) exercise->update_progress(); 

	currentY+=INCREMENT;


	}
}



void Display::RenderUserList() {

	std::vector<User*> users=fm->get_user_vector();

	float currentY =  - scrollY; 

	std::string title = "Select user:";
	DrawTextEx(FONT, title.c_str(), {OFFSET_X, currentY}, BIG_FONT, TEXT_SPACING, BLACK);
	currentY += SMALL_INCREMENT; 

	for (const auto& user : users) {
		if (!user) continue; 

		Rectangle userRect = {OFFSET_X, currentY, float(GetScreenWidth()), BOX_HEIGHT};

		DrawRectangleRec(userRect, LIGHTGRAY);
		DrawRectangleLinesEx(userRect, LINES_W, DARKGRAY);

		DrawTextEx(FONT, user->get_name().c_str(), {BOX_PADDING, currentY + LINE_SPACING}, MEDIUM_FONT, TEXT_SPACING, BLACK);

		std::string weightText = helper.to_string_with_precision(user->get_kg(), PRECISION) + " kg";
		DrawTextEx(FONT, weightText.c_str(), {BOX_PADDING, currentY + LINE_SPACING*2}, MEDIUM_FONT, TEXT_SPACING, DARKGRAY);

		if (CheckCollisionPointRec(MousePosition, userRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				display_user=false;
				current_user=user;
		}

		currentY += INCREMENT;
	}

	DrawAddButton(user_box, "Add user", currentY);
	if(user_box) DrawUserInputBox(s1, s2);
}


void Display::Render() {

	MousePosition=GetMousePosition();	

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

	Rectangle outerBackgroundRect = {startX - 10, startY - 10, boxWidth + 20, (BOX_HEIGHT_SMALL + BOX_PADDING * 2) * 3 + 20};
	DrawRectangleRec(outerBackgroundRect, LIGHTGRAY);  // Fill the background

	Rectangle usernameRect = {startX, startY, boxWidth, BOX_HEIGHT_SMALL};
	DrawRectangleRec(usernameRect, usernameActive ? DARKGRAY : RAYWHITE);  // Highlight if active
	DrawRectangleLinesEx(usernameRect, LINES_W, DARKGRAY);
	DrawTextEx(FONT, ("Username: " + username).c_str(), {startX + BOX_PADDING, startY + BOX_PADDING}, MEDIUM_FONT, TEXT_SPACING, BLACK);

	if (CheckCollisionPointRec(MousePosition, usernameRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		usernameActive = true;
		weightActive = false;
	}

	Rectangle weightRect = {startX, startY + BOX_HEIGHT, boxWidth, BOX_HEIGHT};
	DrawRectangleRec(weightRect, weightActive ? DARKGRAY : RAYWHITE);  // Highlight if active
	DrawRectangleLinesEx(weightRect, LINES_W, DARKGRAY);
	DrawTextEx(FONT, ("Weight (kg): " + weightKg).c_str(), {startX + BOX_PADDING, startY + BOX_HEIGHT}, MEDIUM_FONT, 2, BLACK);

	if (CheckCollisionPointRec(MousePosition, weightRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		weightActive = true;
		usernameActive = false;
	}

	if (usernameActive) {
		helper.GetTextInput(username, 64);
	}
	if (weightActive) {
		helper.GetTextInput(weightKg, 64);
	}

	Rectangle backRect = {startX, startY + BOX_HEIGHT * 2 + BOX_PADDING_BIG, 100, 40};
	DrawTexturePro(back, {0, 0, (float)back.width, (float)back.height}, backRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(MousePosition, backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {	
		username="";
		weightKg="";
		user_box=false;
		return;
	}

	Rectangle okRect = {(float)GetScreenWidth() - 150, startY + (BOX_HEIGHT_SMALL + BOX_PADDING) * 2 + BOX_PADDING_BIG, 100, 40};
	DrawTexturePro(ok, {0, 0, (float)ok.width, (float)ok.height}, okRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(MousePosition, okRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		fm->add_user(username, std::stod(weightKg, nullptr));

		username="";
		weightKg="";
		user_box=false;

	}

	DrawRectangleLinesEx(outerBackgroundRect, LINES_W, DARKGRAY); 
}

void Display::DrawWorkoutInputBox(std::string &workoutname) {

	float startx = 50;
	float starty = 100;
	float boxWidth = float(GetScreenWidth());

	Rectangle outerBackgroundRect = {startx - 10, starty - 10, boxWidth + 20, (BOX_HEIGHT_SMALL + BOX_PADDING_BIG) * 3 + 20};
	DrawRectangleRec(outerBackgroundRect, LIGHTGRAY);  

	Rectangle usernameRect = {startx, starty, boxWidth, BOX_HEIGHT_SMALL};
	DrawRectangleRec(usernameRect, RAYWHITE); 
	DrawRectangleLinesEx(usernameRect, LINES_W, DARKGRAY);
	DrawTextEx(FONT, ("Workout: " + workoutname).c_str(), {startx, starty}, MEDIUM_FONT, TEXT_SPACING, BLACK);


	helper.GetTextInput(workoutname, 64);

	Rectangle backRect = {startx, starty + (BOX_HEIGHT_SMALL + BOX_PADDING) * 2 + BOX_PADDING_BIG, 100, 40};
	DrawTexturePro(back, {0, 0, (float)back.width, (float)back.height}, backRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(MousePosition, backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {	
		workoutname="";
		workout_box=false;
		return;
	}

	Rectangle okRect = {(float)GetScreenWidth() - 150, starty + (BOX_HEIGHT_SMALL + BOX_PADDING) * 2 + BOX_PADDING_BIG, 100, 40};
	DrawTexturePro(ok, {0, 0, (float)ok.width, (float)ok.height}, okRect, {0, 0}, 0.0f, WHITE);

	if (CheckCollisionPointRec(MousePosition, okRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

		try {
			fm->add_workout_to_user(current_user->get_name(), workoutname); }

		catch (std::runtime_error& e) {
			std::cout<<"Error adding user ";
		}
		workoutname="";
		workout_box=false;

	}

	DrawRectangleLinesEx(outerBackgroundRect, LINES_W, DARKGRAY);  
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

