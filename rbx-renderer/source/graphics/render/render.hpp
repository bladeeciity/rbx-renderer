#pragma once

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>
#include <memory>
#include <mutex>

namespace module
{
    /**
     * @brief Rendering system using DirectX 11 and ImGui.
     */
    class c_render
    {
      public:
        /**
         * @brief Constructor.
         */
        c_render( ) noexcept;

        /**
         * @brief Window procedure hook for ImGui input handling.
         *
         * @param window Window handle.
         * @param msg Message identifier.
         * @param u_param WPARAM.
         * @param i_param LPARAM.
         *
         * @returns Message processing result.
         */
        static std::intptr_t wnd_proc( HWND window, std::uint32_t msg, std::uintptr_t u_param, std::intptr_t i_param ) noexcept;

        /**
         * @brief Prepare the renderer for a given swap chain (initializes device, context, and target view).
         *
         * @param swap_chain Swap chain pointer.
         *
         * @returns true on success, false on failure.
         */
        bool prepare( IDXGISwapChain* swap_chain ) noexcept;

        /**
         * @brief Resize the swap chain's buffers and update render targets.
         */
        void resize_buffers( ) noexcept;

        /**
         * @brief Begin a new ImGui frame.
         */
        void new_frame( ) noexcept;

        /**
         * @brief Ensure the renderer is ready for the given swap chain.
         *
         * @param swap_chain Swap chain pointer.
         *
         * @returns true if ready, false otherwise.
         */
        bool ensure( IDXGISwapChain* swap_chain ) noexcept;

        /**
         * @brief Render the current ImGui draw data.
         *
         * @param swap_chain Swap chain pointer.
         */
        void render( IDXGISwapChain* swap_chain ) noexcept;

        /**
         * @brief Returns reference to the initialization flag.
         *
         * @returns Reference to the initialized flag.
         */
        bool& initialized( ) noexcept
        {
            return m_initialized;
        }

      private:
        ImGuiIO*                m_imgui_io { nullptr };       // Pointer to ImGui IO context.
        ID3D11Device*           m_device { nullptr };         // D3D11 device.
        ID3D11DeviceContext*    m_device_context { nullptr }; // D3D11 device context.
        ID3D11RenderTargetView* m_target_view { nullptr };    // D3D11 render target view.

        mutable std::mutex m_wnd_proc_mutex { };   // Mutex for window procedure operations.
        WNDPROC            m_wnd_proc { nullptr }; // Window procedure backup.

        bool m_initialized { false }; // Flag indicating if the renderer is initialized.
    };

    inline auto g_render { std::unique_ptr< c_render > {} };
} // namespace module