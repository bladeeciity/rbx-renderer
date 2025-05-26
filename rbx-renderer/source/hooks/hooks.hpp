#pragma once

#include "render/render.hpp"
#include "vmt/vmt.hpp"

#include <memory>
#include <mutex>

namespace module
{
    class c_hooks
    {
        /**
         * @brief Hooked Present method for the swap chain.
         *
         * Handles per-frame rendering logic and UI rendering.
         */
        static std::int32_t __stdcall present( IDXGISwapChain* swap_chain, std::uint32_t sync_interval, std::uint32_t flags ) noexcept;

        /**
         * @brief Hooked ResizeBuffers method for the swap chain.
         *
         * Handles swap chain resizing and render target recreation.
         */
        static std::int32_t __stdcall resize_buffers( IDXGISwapChain* swap_chain, std::uint32_t buffer_count, std::uint32_t width,
                                                      std::uint32_t height, std::int32_t new_format, std::uint32_t flags ) noexcept;

      public:
        /**
         * @brief Constructs the hooks manager.
         */
        c_hooks( ) noexcept;

      private:
        mutable std::recursive_mutex m_mutex { }; // Mutex for thread-safe hook operations.
        std::unique_ptr< c_vmt >     m_hook { };  // VMT hook manager for swap chain.
    };

    inline auto g_hooks { std::unique_ptr< c_hooks > {} };
} // namespace module