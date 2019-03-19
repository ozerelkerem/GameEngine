#pragma once

#include<imgui.h>
#include <imgui_internal.h>
#include <string>
#include <vector>

namespace ImGui
{
	IMGUI_API bool BeginTimeline(const char* str_id, float max_value);
	IMGUI_API bool TimelineEvent(const char* str_id, uint16_t* values, unsigned int);
	IMGUI_API void EndTimeline();
	//IMGUI_API void DrawList(std::vector<std::pair<std::string, std::vector<float*>>> drawlist);


static float s_max_timeline_value = 1.f;
static int timelinechildcounter = 0;



	static bool BeginTimeline(const char* str_id, float max_value)
	{
		s_max_timeline_value = max_value;
		timelinechildcounter = 0;
		return BeginChild(str_id, { 0,0 },false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

	}

	static int LINE_COUNT = 1000;
	static const float TIMELINE_RADIUS = 6;

	
	static int lineheight = 17;
	static bool TimelineEvent(const char* str_id, uint16_t* value, unsigned int order)
	{
		ImGuiWindow* win = GetCurrentWindow();
		
		const ImU32 inactive_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Button]);
		const ImU32 active_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_ButtonHovered]);
		const ImU32 line_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Text]);
		bool changed = false;
		ImVec2 cursor_pos = win->DC.CursorPos;

		ImVec2 pos = cursor_pos;
		pos.x = GetWindowContentRegionMin().x + win->Pos.x + TIMELINE_RADIUS + (*value) * GetWindowContentRegionWidth() / s_max_timeline_value;
		pos.y += TIMELINE_RADIUS;
		pos.y += lineheight * order;
		SetCursorScreenPos({ pos.x - TIMELINE_RADIUS,pos.y - TIMELINE_RADIUS });
		timelinechildcounter++;
		InvisibleButton((std::string(str_id) + std::to_string(timelinechildcounter)).c_str(), ImVec2(2 * TIMELINE_RADIUS, 2 * TIMELINE_RADIUS));
		if (IsItemActive() || IsItemHovered())
		{
			ImGui::SetTooltip("%d %s", *value, str_id);
			ImVec2 a(pos.x, win->Pos.y +10);
			ImVec2 b(pos.x, GetWindowHeight() + 50);
			win->DrawList->AddLine(a, b, line_color);
		}
		if (IsItemActive() && IsMouseDragging(0))
		{
			//*value += GetIO().MouseDelta.x / win->Size.x * s_max_timeline_value;
			changed = true;
		}
		win->DrawList->AddCircleFilled(
			pos, TIMELINE_RADIUS, IsItemActive() || IsItemHovered() ? active_color : inactive_color);
		ImGui::SetCursorScreenPos(cursor_pos);
		return changed;
	}


	static void EndTimeline()
	{
		ImGuiWindow* win = GetCurrentWindow();

		ImU32 color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Button]);
		ImU32 line_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Border]);
		ImU32 text_color = ColorConvertFloat4ToU32(GImGui->Style.Colors[ImGuiCol_Text]);
		float rounding = GImGui->Style.ScrollbarRounding;
		ImVec2 start(GetWindowContentRegionMin().x + win->Pos.x,
			win->Pos.y);
		ImVec2 end = { GetWindowContentRegionMax().x + win->Pos.x, win->Pos.y + 15 };

		win->DrawList->AddRectFilled(start, end, color, rounding);

	
		const ImVec2 text_offset(0, GetTextLineHeightWithSpacing());
		for (int i = 0; i < LINE_COUNT; ++i)
		{
			ImVec2 a = { GetWindowContentRegionMin().x + win->Pos.x + TIMELINE_RADIUS,  GetWindowContentRegionMin().y + end.y };
			a.x += i * GetWindowContentRegionWidth() / LINE_COUNT;
			ImVec2 b = a;
			b.y = GetWindowHeight() + 50;
			win->DrawList->AddLine(a, b, line_color);
			char tmp[256];
			ImFormatString(tmp, sizeof(tmp), "%.0f", i * s_max_timeline_value / LINE_COUNT);
			win->DrawList->AddText({a.x, start.y}, text_color, tmp);
		}

		EndChild();
	}
}