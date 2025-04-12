#include "interface.hpp"

namespace module
{
    ui_t::ui_t( ) noexcept
    {
        auto &style { ImGui::GetStyle( ) };

        style.WindowRounding    = 0.0f;
        style.ChildRounding     = 6.f;
        style.PopupRounding     = 2.f;
        style.FrameRounding     = 2.f;
        style.ScrollbarRounding = 2.f;
        style.GrabRounding      = 2.f;
        style.TabRounding       = 2.f;

        ImGui::StyleColorsClassic( );
    }

    void ui_t::render( ) const noexcept
    {
        if ( !_opened )
            return;

        ImGui::Begin( "Window" );
        ImGui::End( );
    }
} // namespace module