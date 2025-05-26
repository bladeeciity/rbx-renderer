#include "interface.hpp"

namespace module
{
    c_ui::c_ui( ) noexcept
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

    void c_ui::render( ) noexcept
    {
        if ( !m_opened )
            return;

        if ( ImGui::Begin( "Window" ) )
        {
            ImGui::Text( "Hello, world!" );
        }
        ImGui::End( );
    }
} // namespace module