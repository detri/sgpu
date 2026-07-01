#pragma once

#include <iterator>
#include <vector>

#include "sgpu/handle.hpp"

namespace sgpu::detail
{
    struct pass
    {
        virtual ~pass() = default;

        void consume_tracked_resources(std::vector<handle<resource>>& resources)
        {
            resources.insert(
                resources.end(),
                std::make_move_iterator(m_tracked_resources.begin()),
                std::make_move_iterator(m_tracked_resources.end())
            );
            m_tracked_resources.clear();
        }
    protected:
        std::vector<handle<resource>> m_tracked_resources;
    };
}
