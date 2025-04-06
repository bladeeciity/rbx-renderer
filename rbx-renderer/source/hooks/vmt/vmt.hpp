#pragma once

#include "cstdint"
#include "memory"
#include "windows.h"

namespace module
{
    // @credits: @gogo1000
    class c_vmt
    {
      public:
        explicit c_vmt( void *pointer );

        void *old( const int index );

        void hook( const int index, void *pointer );
        void unhook( const int index );

        void  restore( );
        void *replace( const int index, const int protection, std::uintptr_t *vftable, const std::uintptr_t pointer );

      private:
        std::unique_ptr< std::uintptr_t[] > m_vt_new;
        std::uintptr_t                     *m_vt_backup;
        std::uintptr_t                      m_vt_functions;
    };
} // namespace module