#include "sgpu/graphics_pipeline.hpp"

namespace sgpu
{
    graphics_pipeline::graphics_pipeline(SDL_GPUDevice* device, SDL_GPUGraphicsPipeline* graphics_pipeline):
        m_device(device),
        m_graphics_pipeline(graphics_pipeline) {
    }

    graphics_pipeline::~graphics_pipeline() {
        SDL_ReleaseGPUGraphicsPipeline(m_device, m_graphics_pipeline);
    }
    SDL_GPUGraphicsPipeline* graphics_pipeline::get_graphics_pipeline() const noexcept {
        return m_graphics_pipeline;
    }
}
