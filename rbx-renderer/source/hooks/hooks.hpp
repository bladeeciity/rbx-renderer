#pragma once

#include "render/render.hpp"
#include "vmt/vmt.hpp"

#include "memory"
#include "mutex"

namespace module
{
    class c_hooks
    {
      public:
        c_hooks( ) noexcept;

      private:
        std::recursive_mutex _mutex { };
        std::unique_ptr< c_vmt > _hook { };

        static std::int32_t __stdcall present( IDXGISwapChain *const swap_chain, const std::uint32_t sync_interval,
                                        const std::uint32_t flags ) noexcept;

        static std::int32_t __stdcall resize_buffers( IDXGISwapChain *const swap_chain, const std::uint32_t buffer_count,
                                               const std::uint32_t width, const std::uint32_t height, const std::int32_t new_format,
                                               const std::uint32_t swap_chain_flags ) noexcept;
    };

    inline auto g_hooks { std::unique_ptr< c_hooks > {} };
} // namespace module