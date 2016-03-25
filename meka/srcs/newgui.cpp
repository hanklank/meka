#include "shared.h"
#include "newgui.h"
#include "imgui/imgui.h"

#include "app_game.h"
#include "app_palview.h"
#include "palette.h"
#include "app_filebrowser.h"
#include "saves.h"
#include "app_options.h"

// FIXME-IMGUI: Skinning: font, colors, etc.

void    NewGui_AppGame()
{
    // New GUI
    // FIXME-IMGUI: Resize in fixed steps
    // FIXME-IMGUI: Calculate client size
    char game_id[256];
    snprintf(game_id, ARRAYSIZE(game_id), "%s###Game", gamebox_get_name());
    float scale = g_configuration.game_window_scale;
    int x_start = g_driver->x_start;
    int y_start = g_driver->y_show_start;
    int x_res = g_driver->x_res;
    int y_res = g_driver->y_res;
    ImGui::SetNextWindowSize(ImVec2(x_res * scale + 2*2, y_res * scale + 2*2 + ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y*2));
    if (ImGui::Begin(game_id, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
    {
        /*
        if ((g_driver->id == DRV_SMS) && (Mask_Left_8))
        {
            // Center screen when 8 left columns are masked - this not logical but looks good
            al_draw_filled_rectangle(x_dst, y_dst, x_dst + 4 * scale, y_dst + y_len*scale, COLOR_BLACK);
            al_draw_filled_rectangle(x_dst + (x_len - 4)*scale, y_dst, x_dst + x_len*scale, y_dst + y_len*scale, COLOR_BLACK);
            x_len -= 8;
            x_start += 8;
            x_dst += 4 * scale;
        }
        */
        ImVec2 uv0(0,0);
        ImVec2 uv1((float)x_res / al_get_bitmap_width(screenbuffer), (float)y_res / al_get_bitmap_height(screenbuffer));
        ImGui::Image((ImTextureID)screenbuffer, ImVec2(x_res*scale, y_res*scale), uv0, uv1);
    }
    ImGui::End();
}

namespace ImGui
{
    void ColorSquare(const ImVec4& col, const ImVec4& border_col, const ImVec2& size)
    {
        ImVec2 uv = ImGui::GetFontTexUvWhitePixel();
        ImGui::Image(ImGui::GetIO().Fonts->TexID, size, uv, uv, col, border_col);
    }
}

// FIXME-IMGUI: Window resize steps
void    NewGui_AppPalette()
{
    if (ImGui::Begin("Palette", &PaletteViewer.active, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImVec2 block_size(8,64);
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        for (int n = 0; n < PaletteViewer.palette_size; n++)
            draw_list->AddRectFilled(ImVec2(p.x + block_size.x * n, p.y), ImVec2(p.x + block_size.x * (n+1), p.y + block_size.y), ImColor(*(ImVec4*)&Palette_Emulation[n]));
        
        int hovered_n = -1;
        ImGui::Dummy(ImVec2(block_size.x * PaletteViewer.palette_size, block_size.y - 2));

        // FIXME-IMGUI: Retrieve hovered relative mouse position?
        if (ImGui::IsItemHovered())
            hovered_n = (ImGui::GetIO().MousePos.x - ImGui::GetItemRectMin().x) / block_size.x;

        // FIXME-IMGUI: ColorSquare
        ImGui::ColorSquare(hovered_n != -1 ? *(ImVec4*)&Palette_Emulation[hovered_n] : ImColor(0,0,0), ImColor(0,0,0), ImVec2(12,12));

        if (hovered_n != -1)
        {
            ImGui::SameLine();
            ImGui::Text("Color %d ($%02x)", hovered_n, hovered_n);

            char color_bits[20];
            switch (g_driver->id)
            {
                case DRV_SMS:
                    StrWriteBitfield(PRAM[hovered_n], 8, color_bits);
                    ImGui::SameLine();
                    ImGui::Text("- %%%s", color_bits);
                    break;
                case DRV_GG:
                    StrWriteBitfield(PRAM[hovered_n * 2 + 1], 8, color_bits);
                    StrWriteBitfield(PRAM[hovered_n * 2 + 0], 8, color_bits + 8+1);
                    color_bits[9] = '.';
                    ImGui::SameLine();
                    ImGui::Text("- %%%s", color_bits);
                    break;
            }
        }
    }
    ImGui::End();
}

void    NewGui_MainMenu()
{
    if (!ImGui::BeginMainMenuBar())
        return;

    if (ImGui::BeginMenu(Msg_Get(MSG_Menu_Main)))
    {
        // FIXME-IMGUI: FB_Switch() toggle
        if (ImGui::MenuItem(Msg_Get(MSG_Menu_Main_LoadROM), "ALT+L", FB.active))        FB_Switch(); 
        if (ImGui::MenuItem(Msg_Get(MSG_Menu_Main_FreeROM), NULL))                      Free_ROM(); 
        ImGui::Separator();
        if (ImGui::MenuItem(Msg_Get(MSG_Menu_Main_SaveState_Save), "F5"))               SaveState_Save();
        if (ImGui::MenuItem(Msg_Get(MSG_Menu_Main_SaveState_Load), "F7"))               SaveState_Load();
        if (ImGui::MenuItem(Msg_Get(MSG_Menu_Main_SaveState_PrevSlot), "F6"))           SaveState_SetPrevSlot();
        if (ImGui::MenuItem(Msg_Get(MSG_Menu_Main_SaveState_PrevSlot), "F8"))           SaveState_SetNextSlot();
        ImGui::Separator();
        if (ImGui::MenuItem(Msg_Get(MSG_Menu_Main_Options), "Alt+O", Options.active))   Options_Switch();
        if (ImGui::BeginMenu(Msg_Get(MSG_Menu_Main_Language)))
        {
            for (t_list* langs = Messages.Langs; langs; langs = langs->next)
            {
                t_lang* lang = (t_lang*)langs->elem;
                if (ImGui::MenuItem(lang->Name, NULL, lang == Messages.Lang_Cur))
                {
                	Messages.Lang_Cur = lang;
                    Msg(MSGT_USER, Msg_Get(MSG_Language_Set), Messages.Lang_Cur->Name);
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem(Msg_Get(MSG_Menu_Main_Quit), "F10")) Action_Quit();

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void    NewGui_Draw()
{
    NewGui_MainMenu();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2,2));
    NewGui_AppGame();
    NewGui_AppPalette();
    ImGui::PopStyleVar(2);
}
