#include <emscripten.h>
#include <emscripten/html5.h>

#include <imgui.h>
#include <rlImGui.h>

#include <raylib.h>

EM_JS(int, canvas_get_width, (),
    { return document.getElementById("canvas").clientWidth; });
EM_JS(int, canvas_get_height, (),
    { return document.getElementById("canvas").clientHeight; });

extern "C" {

EM_BOOL resize_event_callback(int event_type, const EmscriptenUiEvent *event, void *ud) {
    if (event_type == EMSCRIPTEN_EVENT_RESIZE) {
        SetWindowSize(canvas_get_width(), canvas_get_height());
    }
    return false;
}

} // extern "C"

void ja_demo1_update(void *ud) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    rlImGuiBegin();

    bool open = true;
    ImGui::ShowDemoWindow(&open);

    open = true;
    if (ImGui::Begin("Test Window", &open)) {
        ImGui::Text("Test text in the Test Window.");
    }

    ImGui::End();

    rlImGuiEnd();
    EndDrawing();
}

int main() {
    InitWindow(800, 600, "jumpartifact demo 1");

    SetWindowSize(canvas_get_width(), canvas_get_height());

    rlImGuiSetup(false);

    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, false, resize_event_callback);

    emscripten_set_main_loop_arg(ja_demo1_update, nullptr, 0, 1);

    return 0;
}
