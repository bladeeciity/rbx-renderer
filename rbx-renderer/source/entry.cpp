#include "windows.h"
#include "cstdint"
#include "thread"

namespace module
{
    static void on_attach( ) noexcept
    {
        MessageBoxA( 0, "hello", "world", 0 );
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