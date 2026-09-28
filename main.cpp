#include <iostream>
#include "webview/webview.h"
#include <chrono>
#include <numeric>
#include <vector>
#include <filesystem>
#include <string>
#include <gtk/gtk.h>

using namespace std;

float bpm(auto start, auto end, int min = 60){
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
		   end - start
    );
    float bpm_ = min / (float(time.count()) / 1000);
    return bpm_;
}

int main(){
    webview::webview w(true, nullptr);
#ifdef __linux__
    auto native_window_result = w.window();
    native_window_result.ensure_ok();

    GtkWindow* native_window =
        GTK_WINDOW(native_window_result.value());

    gtk_window_set_type_hint(
        native_window,
        GDK_WINDOW_TYPE_HINT_UTILITY
    );
#endif
    w.set_title("BPM calculater");
    w.set_size(247, 442, WEBVIEW_HINT_NONE);
    std::vector<int> bpms;
    bpms.reserve(5);
    int bpm_intermediate;
    bool first_click = true; // это первый клик
    std::chrono::steady_clock::time_point start;


	w.bind("action", [&](const std::string&) -> std::string {
        //нажали кнопку

        if (first_click == false){ // если второй клик то считаем
            auto end = std::chrono::steady_clock::now();
            bpm_intermediate = bpm(start, end);
            bpms.push_back(bpm_intermediate);
            if (bpms.size() > 4){
                std::cout << "youre bpm is: " << std::accumulate(bpms.begin(), bpms.end(), 0) / bpms.size() << '\n';
                w.eval(
                    "document.getElementById('bpm-value').textContent = '" + std::to_string(std::accumulate(bpms.begin(), bpms.end(), 0) / bpms.size()) +"';"
                );
            }
            first_click = true; // после второго первый
            
        }
        else{
            // старт
            start = std::chrono::steady_clock::now();
            first_click = false;
            
        }
        return "null";
	});
	     
    filesystem::path html = filesystem::absolute("ui/index.html");
    string url = "file://" + html.string();
    w.navigate(url);
    w.run();
    
    return 0;

    
}
