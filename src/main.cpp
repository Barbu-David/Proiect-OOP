#include"file_manager.h"
#include"display.h"

int main() {

	const int screenWidth = 800;
	const int screenHeight = 600;
	InitWindow(screenWidth, screenHeight, "David Fitness");
	
	SetTargetFPS(60);

	Display d=Display();

	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(RAYWHITE);
		d.Render(d.fm->user_vector, 0, 0, 100);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
