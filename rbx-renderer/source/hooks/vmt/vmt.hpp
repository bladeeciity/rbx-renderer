#pragma once

// clang-format off
#include <windows.h>
// clang-format on

#include <cstdint>
#include <memory>

// @Credits gogo1000
namespace module
{
    /**
     * @brief Virtual Method Table (VMT) hook utility.
     *
     * Provides functionality to hook and manipulate an object's virtual method table.
     */
    class c_vmt
    {
        std::unique_ptr< std::uintptr_t[] > m_vt_new;           // New VMT storage (for hooks)
        std::uintptr_t*                     m_vt_backup;        // Backup of original VMT
        std::uintptr_t                      m_vt_functions = 0; // Number of functions in the VMT

      public:
        /**
         * @brief Construct and initialize a VMT hook object for the given instance.
         *
         * @param pointer Pointer to the instance whose VMT should be hooked.
         */
        c_vmt( void* pointer );

        /**
         * @brief Get the original function pointer at the given VMT index.
         *
         * @param index Index of the function in the VMT.
         *
         * @returns Pointer to the original function.
         */
        void* old( int index );

        /**
         * @brief Hook a function at the specified VMT index.
         *
         * @param index Index in the VMT to hook.
         * @param pointer Pointer to the replacement function.
         */
        void hook( int index, void* pointer );

        /**
         * @brief Restore the original function at the specified index.
         *
         * @param index Index in the VMT to unhook.
         */
        void unhook( int index );

        /**
         * @brief Restore the original VMT for the instance.
         */
        void restore( );

        /**
         * @brief Replace a function pointer in the VMT with a new one, with memory protection management.
         *
         * @param index Index in the VMT to replace.
         * @param protection Memory protection flags to apply.
         * @param vftable Pointer to the VMT.
         * @param pointer New function pointer.
         *
         * @returns Pointer to the replaced (old) function.
         */
        void* replace( int index, int protection, std::uintptr_t* vftable, std::uintptr_t pointer );
    };
} // namespace module