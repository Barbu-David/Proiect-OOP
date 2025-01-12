#include "display.h"
#include <raylib.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "sorter.h"
#include <fstream>
#include <cstring>


Display::Display() {

}

std::string to_string_with_precision(double value, int precision) {
	std::ostringstream out;
	out << std::fixed << std::setprecision(precision) << value;
	return out.str();
}

void Display::RenderWorkoutList(std::vector<Workout*>& workouts, float startX, float startY, float spacing, float scrollY, const std::string& username, double kg_param) {
	float currentY = startY - scrollY; // Adjust starting Y position based on scroll offset
	float boxPadding = 10; // Padding inside each workout's rectangle
	float lineSpacing = 20; // Space between lines of text
	float boxSpacing = 30; // Space between workout rectangles


	        // Draw the "back" texture in the top-right corner
        float backWidth = 40; // Width of the scaled "back" texture
        float backHeight = 40; // Height of the scaled "back" texture
        float backPosX = 780 - backWidth; // Right corner with 20px padding
        float backPosY = 20; // 20px padding from the top
        Rectangle backRect = {backPosX, backPosY, backWidth, backHeight};

        DrawTexturePro(
                        back, // The "back" texture
                        {0, 0, (float)back.width, (float)back.height}, // Source rectangle
                        backRect, // Destination rectangle with scaling
                        {0, 0}, // Origin (no rotation)
                        0.0f, // Rotation (no rotation)
                        WHITE // Tint color
                      );

    // Check if the mouse is inside the "back" rectangle
    if (CheckCollisionPointRec(GetMousePosition(), backRect)) {
        // If the mouse is inside the "back" rectangle and clicked
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            display_user = true;
            return; // Return early to prevent further rendering
        }
    }



	// Draw the username and title at the top
	std::string title = username + "'s workouts  " +  to_string_with_precision(kg_param, 1) + " kgs";
	DrawTextEx(GetFontDefault(), title.c_str(), {startX, currentY}, 28, 2, BLACK);
	currentY += 50; // Add space below the title

	// Calculate the maximum scrollable height
	float totalHeight = workouts.size() * (2 * lineSpacing + boxPadding * 2 + boxSpacing); // Height of all workout boxes
	float maxScrollY = totalHeight - (GetScreenHeight() - startY);

	// Ensure scrollY stays within bounds
	if (scrollY < 0) scrollY = 0;
	if (scrollY > maxScrollY) scrollY = maxScrollY;

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

		// Check if the mouse is inside the workout rectangle
		if (CheckCollisionPointRec(GetMousePosition(), workoutRect)) {
			// If the mouse is inside the rectangle and clicked
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				display_workout = false; // Set the display workout flag to true
				current_workout = workout; // Set the current workout to the clicked one
			}
		}

		// Move to the next workout's position
		currentY += boxHeight + boxSpacing;
	}

	         // ---- ADD "Add Workout" BUTTON ----
    // Define the rectangle for the "Add User" button
    Rectangle addWorkoutRect = {startX, currentY, GetScreenWidth() - startX * 2, boxHeight};

    // Draw the "Add User" rectangle
    DrawRectangleRec(addWorkoutRect, LIGHTGRAY);
    DrawRectangleLinesEx(addWorkoutRect, 2, DARKGRAY);

    // Draw the "Add User" text
    float addTextStartX = startX + boxPadding;
    float addTextStartY = currentY + boxPadding;
    DrawTextEx(GetFontDefault(), "Add workout", {addTextStartX, addTextStartY}, 24, 2, BLACK);

    if (CheckCollisionPointRec(GetMousePosition(), addWorkoutRect)) {
        // If the mouse is inside the rectangle and clicked
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        workout_box=true;
	}
    }   

	if(workout_box) DrawWorkoutInputBox(s1);


}

void Display::RenderExerciseList(const std::vector<Exercise*>& exercises, float startX, float startY, float spacing, float scrollY, const std::string& workout_name ) {
	float currentY = startY - scrollY; // Adjust starting Y position based on scroll offset
	float textureWidth = 50; // Set a small width for the texture
	float textureHeight = 50; // Set a small height for the texture
	float boxPadding = 10; // Padding between the texture and other elements
	float lineSpacing = 20; // Line spacing for better readability


	// Draw the workout name at the top
	DrawTextEx(GetFontDefault(), workout_name.c_str(), {startX, currentY}, 28, 2, BLACK);
	currentY += 40; // Add some space below the workout name before the first exercise


	// Draw the "back" texture in the top-right corner
	float backWidth = 40; // Width of the scaled "back" texture
	float backHeight = 40; // Height of the scaled "back" texture
	float backPosX = 780 - backWidth; // Right corner with 20px padding
	float backPosY = 20; // 20px padding from the top
	Rectangle backRect = {backPosX, backPosY, backWidth, backHeight};

	DrawTexturePro(
			back, // The "back" texture
			{0, 0, (float)back.width, (float)back.height}, // Source rectangle
			backRect, // Destination rectangle with scaling
			{0, 0}, // Origin (no rotation)
			0.0f, // Rotation (no rotation)
			WHITE // Tint color
		      );

    // Check if the mouse is inside the "back" rectangle
    if (CheckCollisionPointRec(GetMousePosition(), backRect)) {
        // If the mouse is inside the "back" rectangle and clicked
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            display_workout = true; 
            return; // Return early to prevent further rendering
        }
    }

	// Calculate the maximum scrollable height
	float maxScrollY = (exercises.size() * (textureHeight + spacing)) - (GetScreenHeight() - startY);

	// Ensure scrollY stays within bounds
	if (scrollY < 0) scrollY = 0;
	if (scrollY > maxScrollY) scrollY = maxScrollY;

	for (size_t i = 0; i < exercises.size(); ++i) {
		const auto& exercise = exercises[i];

		// Calculate the position for the texture
		float texturePosX = startX;
		float texturePosY = currentY;

		// Draw the texture in the scaled rectangle next to the box
		Rectangle textureRect = {texturePosX, texturePosY, textureWidth, textureHeight};

		// Draw the texture with scaling
		DrawTexturePro(
				exercise->get_photo(), // The texture
				{0, 0, (float)exercise->get_photo().width, (float)exercise->get_photo().height}, // Source rectangle
				textureRect, // Destination rectangle with scaling
				{0, 0}, // Origin (no rotation)
				0.0f, // Rotation (no rotation)
				WHITE // Tint color
			      );

		// Adjust the starting position for text and progress bar (move right of the texture)
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
			}
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


		// Calculate position for the "plus" icon next to the progress bar
		float plusPosX = textStartX + barWidth + 10; // 10 is the padding from the progress bar
		float plusPosY = currentY + 4 * lineSpacing;  // Same Y position as the progress bar

		// Draw the "plus" icon, scaled similarly to the exercise photo
		Rectangle plusRect = {plusPosX, plusPosY, textureWidth * 0.5f, textureHeight * 0.5f}; // Scale the "plus" icon smaller (50%)
		DrawTexturePro(
				plus, // The "plus" texture
				{0, 0, (float)plus.width, (float)plus.height}, // Source rectangle
				plusRect, // Destination rectangle with scaling
				{0, 0}, // Origin (no rotation)
				0.0f, // Rotation (no rotation)
				WHITE // Tint color
			      );

		// Check if the mouse is inside the "plus" rectangle
		if (CheckCollisionPointRec(GetMousePosition(), plusRect)) {
			// If the mouse is inside the "plus" rectangle and clicked
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				exercise->update_progress(); // Call the update_progress() function for the current exercise
			}
		}

		// Increment Y position for the next exercise in the list
		currentY += textureHeight + spacing + lineSpacing -20;
	}
}

void Display::RenderUserList(std::vector<User*>& users, float startX, float startY, float spacing, float scrollY) {
	float currentY = startY - scrollY; // Adjust starting Y position based on scroll offset
	float boxPadding = 10.0f;
	float lineSpacing = 20.0f;
	float boxHeight = 60.0f; // Height of each user box

	//Draw string
	std::string title = "Select user:";
	DrawTextEx(GetFontDefault(), title.c_str(), {startX, currentY}, 28, 2, BLACK);
	currentY += 50; // Add space below the title

	// Calculate the maximum scrollable height
	float totalHeight = users.size() * (boxHeight + spacing);
	float maxScrollY = totalHeight - (GetScreenHeight() - startY);

	// Ensure scrollY stays within bounds
	if (scrollY < 0) scrollY = 0;
	if (scrollY > maxScrollY) scrollY = maxScrollY;

	for (const auto& user : users) {
		if (!user) continue; // Skip null pointers

		// Define the rectangle for the current user
		Rectangle userRect = {startX, currentY, GetScreenWidth() - startX * 2, boxHeight};

		// Draw the user rectangle
		DrawRectangleRec(userRect, LIGHTGRAY);
		DrawRectangleLinesEx(userRect, 2, DARKGRAY);

		// Draw the user's name
		float textStartX = startX + boxPadding;
		float textStartY = currentY + boxPadding;
		DrawTextEx(GetFontDefault(), user->get_name().c_str(), {textStartX, textStartY}, 24, 2, BLACK);

		// Draw the user's weight (kg)
		std::string weightText = to_string_with_precision(user->get_kg(), 1) + " kg";
		DrawTextEx(GetFontDefault(), weightText.c_str(), {textStartX, textStartY + lineSpacing}, 20, 1, DARKGRAY);

		// Check if the mouse is inside the user rectangle
		if (CheckCollisionPointRec(GetMousePosition(), userRect)) {
			// If the mouse is inside the rectangle and clicked
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				display_user=false;
				current_user=user;
			}
		}

		// Move to the next user's position
		currentY += boxHeight + spacing/2;
	}

	// ---- ADD "Add User" BUTTON ----
	// Define the rectangle for the "Add User" button
	Rectangle addUserRect = {startX, currentY, GetScreenWidth() - startX * 2, boxHeight};

	// Draw the "Add User" rectangle
	DrawRectangleRec(addUserRect, LIGHTGRAY);
	DrawRectangleLinesEx(addUserRect, 2, DARKGRAY);

	// Draw the "Add User" text
	float addTextStartX = startX + boxPadding;
	float addTextStartY = currentY + boxPadding;
	DrawTextEx(GetFontDefault(), "Add User", {addTextStartX, addTextStartY}, 24, 2, BLACK);

	// Check if the mouse is inside the "Add User" rectangle
	if (CheckCollisionPointRec(GetMousePosition(), addUserRect)) {
		// If the mouse is inside the rectangle and clicked
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			user_box=true;	
		}
	}

	if(user_box) DrawUserInputBox(s1, s2);

}


void Display::Render(std::vector<User*>& users, float startX, float startY, float spacing, float scrollY) {
	if (display_user) {
		// Render the user list if display_user is true
		RenderUserList(users, startX, startY, spacing, scrollY);
	} else if (display_workout && current_user != nullptr) {
		// Render the list of workouts for the selected user
		RenderWorkoutList(current_user->workout_vector, startX, startY, spacing, scrollY, current_user->get_name(), current_user->get_kg());
	} else if (current_workout != nullptr) {
		// Render the exercises of the current workout
		RenderExerciseList(current_workout->exercises, startX, startY, spacing, scrollY, current_workout->name);
		handleKeyPressAndSort(current_workout->exercises);
	}
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
		fm->add_workout_to_user(current_user->get_name(), workoutname);
		workoutname="";
		workout_box=false;

	}

	// --- Outer Rectangle Border ---
	DrawRectangleLinesEx(outerBackgroundRect, 4, DARKGRAY);  // Draw border around the UI
}


double Display::GetWeightByExerciseName(const std::string& exercise_name) {
	for (Workout* workout : fm->workout_vector) {
		for (Exercise* exercise : workout->exercises) {
			// Dynamic cast to Weight type
			Weight* weight_exercise = dynamic_cast<Weight*>(exercise);
			if (weight_exercise && exercise->get_name() == exercise_name) {
				return weight_exercise->get_weight();
			}
		}
	}
	// Return a default value if not found
	return 0.0;
}


void Display::UpdateScroll(float& scrollY) {
	float wheelMove = GetMouseWheelMove();

	if (wheelMove != 0) {
		scrollY -= wheelMove * 40; // Adjust scroll speed by changing the multiplier
	}

	if (IsKeyPressed(KEY_DOWN)) {
		scrollY += 10; // Scroll down
	}
	if (IsKeyPressed(KEY_UP)) {
		scrollY -= 10; // Scroll up
	}
}
