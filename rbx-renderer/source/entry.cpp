#include "windows.h"
#include "cstdint"
#include "thread"

#include "render/render.hpp"
#include "hooks/hooks.hpp"

namespace module
{
    static void on_attach( ) noexcept
    {
        module::g_render = std::make_unique< module::c_render >( );
        module::g_hooks  = std::make_unique< module::c_hooks >( );
    }
} // namespace module

bool __stdcall DllMain( [[maybe_unused]] void* handle, std::uint8_t reason, [[maybe_unused]] void* reserved )
{
    if ( reason == DLL_PROCESS_ATTACH )
    {
        std::thread( module::on_attach ).detach( );
        return true;
    }

    return false;
}