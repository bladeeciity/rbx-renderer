#pragma once

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "d3d11.h"

#include "memory"
#include "mutex"

namespace module
{
    class c_render
    {
      public:
        /*
         * @brief: constructor - initializes ImGui context and configures IO settings
         */
        c_render( ) noexcept;

        /*
         * @brief: todo
         */
        static std::intptr_t wnd_proc( HWND window, std::uint32_t msg, std::uintptr_t u_param, std::intptr_t i_param ) noexcept;

        /*
         * @brief: prepares the renderer by setting up D3D11 device, context and render targets
         */
        bool prepare( IDXGISwapChain *swap_chain ) noexcept;

        /*
         * @brief: handles buffer resizing by releasing existing render target views
         */
        void resize_buffers( ) noexcept;

        /*
         * @brief: starts a new ImGui frame
         */
        void new_frame( ) noexcept;

        /*
         * @brief: ensures that the render target exists
         */
        bool ensure( IDXGISwapChain *swap_chain ) noexcept;

        /*
         * @brief: renders the stuff
         */
        void render( IDXGISwapChain *swap_chain ) noexcept;

        /*
         * @brief: returns the _initialized bool state
         */
        bool &initialized( )
        {
            return _initialized;
        }

        /*
         * @brief: sets the class' swapchain
         */
        void swapchain( IDXGISwapChain *swap_chain ) noexcept
        {
            if ( _swap_chain )
                return;

            _swap_chain = swap_chain;
        }

      private:
        ImGuiIO                *_imgui_io { };
        IDXGISwapChain         *_swap_chain { };
        ID3D11Device           *_device { };
        ID3D11DeviceContext    *_device_context { };
        ID3D11RenderTargetView *_target_view { };

        WNDPROC _wnd_proc { };

        std::mutex _wnd_proc_mutex { };

        bool _initialized { };
    };

    inline auto g_render { std::unique_ptr< c_render > {} };
} // namespace module