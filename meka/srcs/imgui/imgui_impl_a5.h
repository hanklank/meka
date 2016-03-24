// ImGui Allegro 5 bindings
// In this binding, ImTextureID is used to store a 'ALLEGRO_BITMAP*' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui
// by @birthggd

#pragma once

struct ALLEGRO_DISPLAY;
union ALLEGRO_EVENT;

bool    ImGui_ImplA5_Init(ALLEGRO_DISPLAY* display);
void    ImGui_ImplA5_Shutdown();
void    ImGui_ImplA5_NewFrame();
void    ImGui_ImplA5_SetDisplay(ALLEGRO_DISPLAY* display);
bool    ImGui_ImplA5_ProcessEvent(ALLEGRO_EVENT* event);

// Use if you want to reset your rendering device without losing ImGui state.
bool    Imgui_ImplA5_CreateDeviceObjects();
void    ImGui_ImplA5_InvalidateDeviceObjects();
