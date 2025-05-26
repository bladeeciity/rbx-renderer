#include "vmt.hpp"

namespace module
{
    c_vmt::c_vmt( void *pointer )
    {
        // Backup the original vtable.
        m_vt_backup = *reinterpret_cast< std::uintptr_t ** >( pointer );

        // Count the number of functions in the vtable.
        while ( reinterpret_cast< std::uintptr_t * >( *reinterpret_cast< std::uintptr_t * >( pointer ) )[ m_vt_functions ] )
            m_vt_functions++;

        // Calculate the size of the vtable.
        const auto vt_size = static_cast< int >( ( m_vt_functions * 0x4 ) + 0x4 );

        // Allocate memory for the new vtable.
        m_vt_new = std::make_unique< std::uintptr_t[] >( m_vt_functions + 1 );

        // Copy the original vtable into the new vtable.
        std::memcpy( m_vt_new.get( ), &m_vt_backup[ -1 ], vt_size );

        // Redirect the pointer to the new vtable.
        *reinterpret_cast< std::uintptr_t ** >( pointer ) = &m_vt_new.get( )[ 1 ];
    }

    void *c_vmt::old( const int index )
    {
        return reinterpret_cast< void * >( m_vt_backup[ index ] );
    }

    void c_vmt::hook( const int index, void *pointer )
    {
        m_vt_new[ index + 1 ] = reinterpret_cast< std::uintptr_t >( pointer );
    }

    void c_vmt::unhook( const int index )
    {
        m_vt_new[ index + 1 ] = m_vt_backup[ index ];
    }

    void c_vmt::restore( )
    {
        m_vt_new.get( )[ 1 ] = m_vt_backup[ 0 ];
        delete[] m_vt_backup;
    }

    void *c_vmt::replace( const int index, const int protection, std::uintptr_t *vftable, const std::uintptr_t pointer )
    {
        // Change the protection to allow writing.
        DWORD old_protection;
        VirtualProtect( &vftable[ index ], 14 * sizeof( void * ), protection, &old_protection );

        // Replace the function pointer at the specified index.
        void *old_func   = reinterpret_cast< void * >( vftable[ index ] );
        vftable[ index ] = pointer;

        // Restore the original protection.
        VirtualProtect( &vftable[ index ], 14 * sizeof( void * ), old_protection, &old_protection );

        return old_func;
    }
} // namespace module