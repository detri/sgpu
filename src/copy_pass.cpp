#include "sgpu/copy_pass.hpp"

namespace sgpu
{
    copy_pass::copy_pass(SDL_GPUCopyPass* pass) : m_pass(pass) {}

    copy_pass::~copy_pass()
    {
        SDL_EndGPUCopyPass(m_pass);
    }

    void copy_pass::upload_to_buffer(const handle<transfer_buffer>& source, uint32_t source_offset,
                                     const handle<buffer>& destination, uint32_t destination_offset, uint32_t size,
                                     bool one_shot)
    {
        const bool cycle = !one_shot;
        m_tracked_resources.emplace_back(source);
        m_tracked_resources.emplace_back(destination);
        const SDL_GPUTransferBufferLocation src{
            .transfer_buffer = source->get_transfer_buffer(),
            .offset = source_offset
        };
        const SDL_GPUBufferRegion dest{
            .buffer = destination->get_buffer(),
            .offset = destination_offset,
            .size = size
        };
        SDL_UploadToGPUBuffer(m_pass, &src, &dest, cycle);
    }

    void copy_pass::upload_to_texture(const handle<transfer_buffer>& source, uint32_t source_offset,
                                      const handle<texture>& texture, uint32_t layer_width, uint32_t layer_height,
                                      bool one_shot)
    {
        const bool cycle = !one_shot;
        m_tracked_resources.emplace_back(source);
        m_tracked_resources.emplace_back(texture);
        const SDL_GPUTextureTransferInfo transfer_info{
            .transfer_buffer = source->get_transfer_buffer(),
            .offset = source_offset,
            .pixels_per_row = layer_width,
            .rows_per_layer = layer_height
        };
        const SDL_GPUTextureRegion texture_region{
            .texture = texture->get_texture()
        };
        SDL_UploadToGPUTexture(m_pass, &transfer_info, &texture_region, cycle);
    }

    void copy_pass::download_from_buffer(const handle<transfer_buffer>& destination, uint32_t destination_offset,
                                         const handle<buffer>& source, uint32_t source_offset, uint32_t size)
    {
        m_tracked_resources.emplace_back(destination);
        m_tracked_resources.emplace_back(source);

        const SDL_GPUBufferRegion src{
            .buffer = source->get_buffer(),
            .offset = source_offset,
            .size = size
        };
        const SDL_GPUTransferBufferLocation dest{
            .transfer_buffer = destination->get_transfer_buffer(),
            .offset = destination_offset
        };
        SDL_DownloadFromGPUBuffer(m_pass, &src, &dest);
    }

    void copy_pass::download_from_texture(const handle<transfer_buffer>& destination, uint32_t destination_offset,
                                          const handle<texture>& source, uint32_t source_width, uint32_t source_height)
    {
        m_tracked_resources.emplace_back(source);
        m_tracked_resources.emplace_back(destination);
        const SDL_GPUTextureRegion texture_region{
            .texture = source->get_texture()
        };
        const SDL_GPUTextureTransferInfo transfer_info{
            .transfer_buffer = destination->get_transfer_buffer(),
            .offset = destination_offset,
            .pixels_per_row = source_width,
            .rows_per_layer = source_height
        };
        SDL_DownloadFromGPUTexture(m_pass, &texture_region, &transfer_info);
    }
}
