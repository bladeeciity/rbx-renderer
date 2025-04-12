#pragma once

#include "imgui.h"

#include "memory"

namespace module
{
    struct ui_t
    {
        ui_t( ) noexcept;

        bool &opened( ) noexcept
        {
            return _opened;
        }

        void render( ) const noexcept;

      private:
        bool _opened { true };
    };

    inline auto g_ui { std::unique_ptr< ui_t > {} };
} // namespace module