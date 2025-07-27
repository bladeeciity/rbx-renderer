#include "hooks.hpp"
#include "interface/interface.hpp"

namespace module
{
    IDXGISwapChain* g_swap_chain = nullptr;

    c_hooks::c_hooks( ) noexcept
    {
        const auto base = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( nullptr ) );

        // Retrieve the address of the VisualEngine.
        const auto visual_engine = *reinterpret_cast< std::uintptr_t* >( base + 0x6719638ui64 );
        if ( !visual_engine )
            return;

        // Retrieve Roblox's custom device.
        const auto rbx_device = *reinterpret_cast< std::uintptr_t* >( visual_engine + 0x90ui64 );
        if ( !rbx_device )
            return;

        // Retrieve the IDXGISwapChain pointer from the Roblox device.
        g_swap_chain = *reinterpret_cast< IDXGISwapChain** >( rbx_device + 0xc8 );

        m_hook = std::make_unique< c_vmt >( g_swap_chain );
        m_hook->hook( 8, &present );
        m_hook->hook( 13, &resize_buffers );
    }

    std::int32_t __stdcall c_hooks::present( IDXGISwapChain* swap_chain, std::uint32_t sync_interval, std::uint32_t flags ) noexcept
    {
        auto original_present = reinterpret_cast< decltype( &present ) >( g_hooks->m_hook->old( 8 ) );

        try
        {
            // This is quite long but I couldn't find a better way to do this without crashing :/
            if ( !g_render || ( !g_render->initialized( ) && !g_render->prepare( swap_chain ) ) || !g_render->ensure( swap_chain ) )
                return original_present( swap_chain, sync_interval, flags );

            g_render->new_frame( );
            g_ui->render( );
            g_render->render( swap_chain );
        }
        catch ( ... )
        {
            // Handle any exceptions that may occur during rendering.
        }

        return original_present( swap_chain, sync_interval, flags );
    }

    std::int32_t __stdcall c_hooks::resize_buffers( IDXGISwapChain* swap_chain, std::uint32_t buffer_count, std::uint32_t width,
                                                    std::uint32_t height, std::int32_t new_format, std::uint32_t flags ) noexcept
    {
        auto original_resize = reinterpret_cast< decltype( &resize_buffers ) >( g_hooks->m_hook->old( 13 ) );

        g_render->resize_buffers( );
        return original_resize( swap_chain, buffer_count, width, height, new_format, flags );
    }
} // namespace module
