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
        c_render( ) noexcept;

        static std::intptr_t wnd_proc( HWND window, std::uint32_t msg, std::uintptr_t u_param, std::intptr_t i_param ) noexcept;

        bool prepare( IDXGISwapChain *swap_chain ) noexcept;

        void resize_buffers( ) noexcept;

        void new_frame( ) noexcept;

        bool ensure( IDXGISwapChain *swap_chain ) noexcept;

        void render( IDXGISwapChain *swap_chain ) noexcept;

        bool &initialized( )
        {
            return _initialized;
        }

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