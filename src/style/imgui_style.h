#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

namespace StyleImGui{
extern    void SetStyle( const char* label, ImGuiStyle* dst = 0);
}