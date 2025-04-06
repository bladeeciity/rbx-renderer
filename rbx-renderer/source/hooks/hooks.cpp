#include "hooks.hpp"

namespace module
{
    void c_hooks::hook( ) noexcept
    {
        /*\
         * @xref: "rbxasset://configs/PerformanceConfigs/rofiler.tools.js"
         * @note: v16 = (unsigned __int8 (__fastcall *)(_QWORD *, _QWORD *))qword_5FFA5E8;
         * @note: ^^ xref, that first xref and above it is the ptr to visual engine
        \*/

        const auto base          = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( nullptr ) );
        const auto visual_engine = *reinterpret_cast< std::uintptr_t * >( base + 0x5ffa5e0 );
        if ( !visual_engine )
            return;

        const auto rbx_device = *reinterpret_cast< std::uintptr_t * >( visual_engine + 0xa8 );
        if ( !rbx_device )
            return;

        const auto swap_chain = *reinterpret_cast< IDXGISwapChain ** >( rbx_device + 0xc8 );

        g_render->swapchain( swap_chain );

        _hook = std::make_unique< c_vmt >( swap_chain );

        _hook->hook( 8, &present );
        _hook->hook( 13, &resize_buffers );
    }

    std::int32_t __stdcall c_hooks::present( IDXGISwapChain *const swap_chain, const std::uint32_t sync_interval,
                                             const std::uint32_t flags ) noexcept
    {
        auto result = reinterpret_cast< decltype( &present ) >( g_hooks->_hook->old( 8 ) );

        std::unique_lock lock { g_hooks->_mutex, std::try_to_lock };
        if ( !lock.owns_lock( ) )
            return result( swap_chain, sync_interval, flags );

        try
        {
            if ( !g_render || !g_render->initialized( ) && !g_render->prepare( swap_chain ) || !g_render->ensure( swap_chain ) )
                return result( swap_chain, sync_interval, flags );

            {
                g_render->new_frame( );

                ImGui::Begin( "Test" );
                ImGui::End( );

                g_render->render( swap_chain );
            }
        }
        catch ( ... )
        {
            // do stuff
        }

        return result( swap_chain, sync_interval, flags );
    }

    std::int32_t __stdcall c_hooks::resize_buffers( IDXGISwapChain *const swap_chain, const std::uint32_t buffer_count,
                                                    const std::uint32_t width, const std::uint32_t height, const std::int32_t new_format,
                                                    const std::uint32_t swap_chain_flags ) noexcept
    {
        auto                                     result = reinterpret_cast< decltype( &resize_buffers ) >( g_hooks->_hook->old( 13 ) );
        std::unique_lock< std::recursive_mutex > lock( g_hooks->_mutex );

        g_render->resize_buffers( );
        return result( swap_chain, buffer_count, width, height, new_format, swap_chain_flags );
    }
} // namespace module