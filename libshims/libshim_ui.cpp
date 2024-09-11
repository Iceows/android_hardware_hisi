#include <stdint.h>
#include <ui/GraphicBufferMapper.h>
#include <ui/Rect.h>
#include <utils/Errors.h>

using android::PixelFormat;
using android::Rect;
using android::status_t;

extern "C" {


extern "C" void _ZN7android13GraphicBuffer4lockEjPPvPiS3_(void* thisptr, uint32_t inUsage,
        void** vaddr, int32_t* outBytesPerPixel, int32_t* outBytesPerStride);

extern "C" void _ZN7android13GraphicBuffer4lockEjPPv(void* thisptr, uint32_t inUsage,
        void** vaddr) {
    _ZN7android13GraphicBuffer4lockEjPPvPiS3_(thisptr, inUsage, vaddr, nullptr, nullptr);
}

#if defined(__LP64__)
status_t _ZN7android19GraphicBufferMapper12importBufferEPK13native_handlejjjimjPS3_(
        void* thisptr, buffer_handle_t rawHandle, uint32_t width, uint32_t height,
        uint32_t layerCount, PixelFormat format, uint64_t usage, uint32_t stride,
        buffer_handle_t* outHandle);
#else
status_t _ZN7android19GraphicBufferMapper12importBufferEPK13native_handlejjjiyjPS3_(
        void* thisptr, buffer_handle_t rawHandle, uint32_t width, uint32_t height,
        uint32_t layerCount, PixelFormat format, uint64_t usage, uint32_t stride,
        buffer_handle_t* outHandle);
#endif

status_t _ZN7android19GraphicBufferMapper12importBufferEPK13native_handlePS3_(
        void* thisptr, buffer_handle_t rawHandle, buffer_handle_t* outHandle) {
#if defined(__LP64__)
    return _ZN7android19GraphicBufferMapper12importBufferEPK13native_handlejjjimjPS3_(
            thisptr, rawHandle, -1, -1, -1, android::PIXEL_FORMAT_NONE, -1, -1, outHandle);
#else
    return _ZN7android19GraphicBufferMapper12importBufferEPK13native_handlejjjiyjPS3_(
            thisptr, rawHandle, -1, -1, -1, android::PIXEL_FORMAT_NONE, -1, -1, outHandle);
#endif
}
}
