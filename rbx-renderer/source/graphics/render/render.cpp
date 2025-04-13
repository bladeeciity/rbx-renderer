#include "render.hpp"

#include "interface/interface.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam );

namespace module
{
    c_render::c_render( ) noexcept
    {
        if ( !ImGui::GetCurrentContext( ) )
            ImGui::CreateContext( );

        _imgui_io = &ImGui::GetIO( );

        _imgui_io->LogFilename = _imgui_io->IniFilename  = nullptr;
        _imgui_io->ConfigFlags                          |= ImGuiConfigFlags_NoMouseCursorChange | ImGuiConfigFlags_NavNoCaptureKeyboard;
    }

    std::intptr_t c_render::wnd_proc( HWND window, std::uint32_t msg, std::uintptr_t u_param, std::intptr_t i_param ) noexcept
    {
        std::unique_lock lock( g_render->_wnd_proc_mutex, std::try_to_lock );

        if ( ImGui::GetCurrentContext( ) )
            ImGui_ImplWin32_WndProcHandler( window, msg, u_param, i_param );

        if ( msg == WM_KEYDOWN && u_param == VK_INSERT )
            g_ui->opened( ) = !g_ui->opened( );

        return CallWindowProc( g_render->_wnd_proc, window, msg, u_param, i_param );
    }

    bool c_render::prepare( IDXGISwapChain *swap_chain ) noexcept
    {
        if ( FAILED( swap_chain->GetDevice( IID_PPV_ARGS( &_device ) ) ) )
            return false;

        _device->GetImmediateContext( &_device_context );

        DXGI_SWAP_CHAIN_DESC back_buffer_desc { };
        if ( FAILED( swap_chain->GetDesc( &back_buffer_desc ) ) )
            return false;

        ID3D11Texture2D *back_buffer { };
        if ( FAILED( swap_chain->GetBuffer( 0, IID_PPV_ARGS( &back_buffer ) ) ) )
            return false;

        if ( FAILED( _device->CreateRenderTargetView( back_buffer, nullptr, &_target_view ) ) )
            return false;

        back_buffer->Release( );

        if ( !_initialized )
        {
            IMGUI_CHECKVERSION( );
            if ( !ImGui::GetCurrentContext( ) )
                ImGui::CreateContext( );

            if ( !ImGui_ImplWin32_Init( back_buffer_desc.OutputWindow ) )
                return false;

            if ( !ImGui_ImplDX11_Init( _device, _device_context ) )
                return false;

            module::g_ui = std::make_unique< module::ui_t >( );

            _wnd_proc = reinterpret_cast< WNDPROC >(
                SetWindowLongPtrA( back_buffer_desc.OutputWindow, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( &wnd_proc ) ) );

            _initialized = true;
        }

        return true;
    }

    void c_render::resize_buffers( ) noexcept
    {
        if ( _target_view )
        {
            _target_view->Release( );
            _target_view = nullptr;
        }
    }

    void c_render::new_frame( ) noexcept
    {
        if ( !_initialized )
            return;

        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );
    }

    bool c_render::ensure( IDXGISwapChain *swap_chain ) noexcept
    {
        if ( _target_view )
            return true;

        ID3D11Texture2D *back_buffer { };
        if ( FAILED( swap_chain->GetBuffer( 0, IID_PPV_ARGS( &back_buffer ) ) ) )
            return false;

        const bool success = SUCCEEDED( _device->CreateRenderTargetView( back_buffer, nullptr, &_target_view ) );
        back_buffer->Release( );

        return success;
    }

    void c_render::render( IDXGISwapChain *swap_chain ) noexcept
    {
        ImGui::Render( );

        ID3D11RenderTargetView *og_target_view { };
        g_render->_device_context->OMGetRenderTargets( 1, &og_target_view, nullptr );

        if ( auto target_view = g_render->_target_view )
        {
            g_render->_device_context->OMSetRenderTargets( 1, &target_view, nullptr );
            ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
            g_render->_device_context->OMSetRenderTargets( 1, &og_target_view, nullptr );
        }

        if ( og_target_view )
            og_target_view->Release( );
    }
} // namespace module
