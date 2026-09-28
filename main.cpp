#include <iostream>
#include <raylib.h>
#include <chrono>
#include <numeric>
#include <vector>

using namespace std;

float bpm(auto start, auto end, int min = 60){
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
		   end - start
    );
    float bpm_ = min / (float(time.count()) / 1000);
    return bpm_;
}

int main(){
    InitWindow(800, 450, "Button");
    Rectangle button = {300, 180, 200, 60};
    std::vector<int> bpms;
    bpms.reserve(5);
    int bpm_intermediate;
    bool first_click = true; // это первый клик
    std::chrono::steady_clock::time_point start;
    
    while (!WindowShouldClose())

    {
        Vector2 mouse = GetMousePosition();
	
	
	if (CheckCollisionPointRec(mouse, button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        //нажали кнопку

        if (first_click == false){ // если второй клик то считаем
            auto end = std::chrono::steady_clock::now();
            bpm_intermediate = bpm(start, end);
            bpms.push_back(bpm_intermediate);
            if (bpms.size() > 4){
                std::cout << "youre bpm is: " << std::accumulate(bpms.begin(), bpms.end(), 0) / bpms.size() << '\n';
            }
            first_click = true; // после второго первый
            
        }
        else{
            // старт
            start = std::chrono::steady_clock::now();
            first_click = false;
            
        }
	}
	     
	BeginDrawing();
	ClearBackground(BLACK);
	DrawRectangleRec(button, PURPLE);
	EndDrawing();
    }
    return 0;
    CloseWindow();
}
