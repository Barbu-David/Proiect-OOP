#include"file_manager.h"
#include"display.h"

int main() {

	const int screenWidth = 800;
	const int screenHeight = 600;
	InitWindow(screenWidth, screenHeight, "David Fitness");

	SetTargetFPS(60);

	Display d;
	Image img3 = LoadImage("assets/ok.jpg");
	Image img2 = LoadImage("assets/back.jpg");
	Image img = LoadImage("assets/plus.jpg");
	d.plus=LoadTextureFromImage(img);
	d.back=LoadTextureFromImage(img2);
	d.ok=LoadTextureFromImage(img3);
	float scrolly=0;
	d.display_workout=true;
	d.current_workout=nullptr;
	d.display_user=true;
	d.user_box=false;
	d.workout_box=false;
	d.fm=new FileManager("./data/exercises", "./data/workouts", "./data/users");

	
	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(RAYWHITE);
		d.Render(d.fm->user_vector, 0, 0, 100, scrolly);
		d.UpdateScroll(scrolly);
		EndDrawing();
	}
	
	CloseWindow();

	return 0;
}
