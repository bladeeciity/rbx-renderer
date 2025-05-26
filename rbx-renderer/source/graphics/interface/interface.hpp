#pragma once

#include <imgui.h>

#include <memory>

namespace module
{
    /**
     * @brief User Interface (UI) management class.
     */
    class c_ui
    {
      public:
        /**
         * @brief Default constructor.
         */
        c_ui( ) noexcept;

        /**
         * @brief Returns reference to flag indicating if UI is open.
         */
        bool& opened( ) noexcept
        {
            return m_opened;
        }

        /**
         * @brief Render the UI.
         */
        void render( ) noexcept;

      private:
        bool m_opened { true }; // Flag to indicate whether the UI is open.
    };

    inline auto g_ui { std::unique_ptr< c_ui > {} };
} // namespace module