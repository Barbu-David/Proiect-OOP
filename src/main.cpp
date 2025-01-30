#include"display.h"

int main() {

	const int screenWidth = 800;
	const int screenHeight = 600;
	InitWindow(screenWidth, screenHeight, "David Fitness");
	
	SetTargetFPS(60);

	Display display=Display();

	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(RAYWHITE);
		display.Render();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
