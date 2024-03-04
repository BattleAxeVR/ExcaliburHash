#pragma once

#include <stdint.h>

#if defined(_MSC_VER)
    #define EXLBR_VISUAL_STUDIO (1)
    #define EXLBR_FORCE_INLINE __forceinline
#endif

#if defined(__clang__)
    #define EXLBR_CLANG (1)
    #define EXLBR_FORCE_INLINE __attribute__((always_inline))
#endif

#if defined(__GNUC__)
    #define EXLBR_GCC (1)
#endif

#if defined(_M_X64) || defined(__aarch64__) || defined(__x86_64__) || defined(__x86_64) || defined(__amd64__) || defined(__amd64) ||       \
    defined(__LP64__) || defined(_WIN64)
    #define EXLBR_64 (1)
#else
    #define EXLBR_32 (1)
#endif

#if EXLBR_VISUAL_STUDIO
    #include <intrin.h>
#endif

namespace Excalibur
{

namespace wyhash
{

#if EXLBR_64

inline uint64_t _hash64(uint64_t v)
{
    #if EXLBR_VISUAL_STUDIO
    {
        uint64_t h;
        uint64_t l = _umul128(v, UINT64_C(0x9E3779B97F4A7C15), &h);
        return l ^ h;
    }
    #elif
    {
        __uint128_t r = v;
        r *= UINT64_C(0x9E3779B97F4A7C15);
        return (uint64_t)(r >> 64U) ^ (uint64_t)(r);
    }
    #else
    {
        #error Unsupported compiler or platform
    }
    #endif
}

#elif EXLBR_32

inline uint32_t _hash32(uint32_t v)
{
    #if EXLBR_VISUAL_STUDIO
    {
        uint64_t lh = __emulu(v, UINT32_C(0x9e3779b1));
        return (uint32_t)(lh >> 32U) ^ (uint32_t)(lh);
    }
    #elif
    {
        uint64_t lh = uint64_t(v) * uint64_t(0x9e3779b1);
        return (uint32_t)(lh >> 32U) ^ (uint32_t)(lh);
    }
    #else
    {
        #error Unsupported compiler or platform
    }
    #endif
}
#else
    #error Unsupported compiler or platform
#endif

inline size_t hash(uint64_t v)
{
#if EXLBR_64
    return _hash64(v);
#elif EXLBR_32
    uint32_t vv = (uint32_t)(v >> 32U) ^ (uint32_t)(v);
    return _hash32(vv);
#else
    #error Unsupported compiler or platform
#endif
}

inline size_t hash(uint32_t v)
{
#if EXLBR_64
    return _hash64(uint64_t(v));
#elif EXLBR_32
    return _hash32(v);
#else
    #error Unsupported compiler or platform
#endif
}


inline size_t hash(int64_t v) { return hash(uint64_t(v)); }
inline size_t hash(int32_t v) { return hash(uint32_t(v)); }


} // namespace wyhash

} // namespace Excalibur