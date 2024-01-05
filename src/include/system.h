//
// Created by Bram Nijenkamp on 05-01-2024.
//

#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>

//! Should be used when dealing with all c-strings and datasets.
using byte = unsigned char;
//! Used as the type of a bit.
using bit_t = unsigned;

// Determine the platform according to the compiler defines.
#if defined(__wasm__) && defined(__EMSCRIPTEN__)
    //! Used when using Web Assembly as output format (compiler).
    #define WASM
    #define CURRENT_PLATFORM_NAME     WASM
    #define CURRENT_PLATFORM_NAME_STR "WASM"
#elif defined(__wasi__) || (defined(__wasm__) && !defined(__EMSCRIPTEN__))
//! Used when using WASI as output format (compiler).
    #define WASI
    #define CURRENT_PLATFORM_NAME     WASI
    #define CURRENT_PLATFORM_NAME_STR "WASI"
#elif defined(__linux__)
//! Used when using Linux-EFI as output format (compiler).
    #define LINUX
    #define CURRENT_PLATFORM_NAME     Linux
    #define CURRENT_PLATFORM_NAME_STR "Linux"
    #define POSIX_NATIVE
#elif defined(__APPLE__)
//! Used when using MacOS-EFI as output format (compiler).
    #define MACOS
    #define CURRENT_PLATFORM_NAME     MacOS
    #define CURRENT_PLATFORM_NAME_STR "MacOS"
    #define POSIX_NATIVE
#elif defined(__unix__)
//! Used when using Unix-EFI as output format (compiler).
    #define UNIX
    #define CURRENT_PLATFORM_NAME     Unix
    #define CURRENT_PLATFORM_NAME_STR "Unix"
    #define POSIX_NATIVE
#elif defined(_WIN32) || defined(_WIN64)
//! Used when using Windows-EFI as output format (compiler).
    #define WINDOWS
    #define CURRENT_PLATFORM_NAME     Windows
    #define CURRENT_PLATFORM_NAME_STR "Windows"
#elif defined(WASM) || defined(LINUX) || defined(UNIX) || defined(WINDOWS) || defined(MACOS)
    #warning "Platform overrwritten by the compiler or CMake."
#else
    #error "Unknown platform."
#endif

// - X86
#if defined(_M_X64) || defined(__x86_64__) || defined(_M_IX86) || defined(__i386__)
    // - info
    #define COMP_ARCH_BASE_STR "X86"
    #define COMP_ARCH_BASE_TAG_X86
    // - details
    #define COMP_ARCH_END 1
#elif defined(_M_X64) || defined(__x86_64__)
    #define COMP_ARCH_STR "x86_64"
    #define COMP_ARCH_TAG_X86_64
    #define COMP_ARCH_BIT 64
#else
    #define COMP_ARCH_STR "x86_32"
    #define COMP_ARCH_TAG_X86_32
    #define COMP_ARCH_BIT 32
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
    #define COMP_CPU_X86_64
    #define COMP_ENV64
    #define COMP_CPU_NAME "x86_64"
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(_X86_)
    #define COMP_CPU_X86
    #define COMP_ENV32
    #define COMP_CPU_NAME "x86"
#elif defined(__aarch64__)
    #define COMP_CPU_AARCH64
    #define COMP_ENV64
    #define COMP_CPU_NAME "ARM64"
#elif defined(__arm__) || defined(_M_ARM)
    #define COMP_CPU_ARM
    #define COMP_ENV32
    #define COMP_CPU_NAME "ARM"
#elif defined(__PPC64__)
    #define COMP_CPU_PPC64
    #define COMP_ENV64
    #define COMP_CPU_NAME "PPC64"
#elif defined(_ARCH_PPC)
    #define COMP_CPU_PPC
    #define COMP_ENV32
    #define COMP_CPU_NAME "PPC"
#elif defined(__wasm__)
    #define COMP_CPU_WASM
    #define COMP_ENV32
    #define COMP_CPU_NAME "WASM"
#endif

//! Assert Endian support
consteval inline bool is_little_endian() {
    constexpr uint32_t magic = 0x01020304;
    constexpr uint8_t _magic = (const uint8_t&) magic;
    // 0x04 Little
    // 0x02 Middle
    // 0x01 Big
    return _magic == 0x04;
}

static_assert(is_little_endian(),
              "We only support Little Endian.");  //!< effective in type
                                                  //!< conversion in covert.h.

//! Determine 32-bit and 64-bit.
#if defined(COMP_ENV64)
//! using for the longptr.
using long_ptr_t = int64_t;
using ulong_ptr_t = uint64_t;
    #define LONGPTR_MAX INT64_MAX
    #define LONGPTR_MIN INT64_MIN
#elif defined(COMP_ENV32)
//! using for the longptr.
using long_ptr_t = int32_t;
using ulong_ptr_t = uint32_t;
    #define LONGPTR_MAX INT32_MAX
    #define LONGPTR_MIN INT32_MIN
#else
    #error "Unkown platform."
#endif

//! Type checking.
static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(int64_t) == 8);
static_assert(sizeof(uint64_t) == 8);
static_assert(sizeof(float_t) == 4);
static_assert(sizeof(double_t) == 8);

// : What version of C++ did this environment use? (provided by the compiler)
#if defined(_MSVC_LANG)
    #define COMP_CPP_VER _MSVC_LANG
#elif defined(__cplusplus)
    #define COMP_CPP_VER __cplusplus
#else
    #error It seems we cant access this information on your environment
    #error Please let us know by sending us a description of the problem along
    #error With information about the operating system and compiler used
#endif

// : What major of C++ did this environment use?
#if COMP_CPP_VER >= 202600L
    #define COMP_CPP_MAJ 26
#elif COMP_CPP_VER >= 202300L
    #define COMP_CPP_MAJ 23
#elif COMP_CPP_VER >= 202000L
    #define COMP_CPP_MAJ 20
#elif COMP_CPP_VER >= 201700L
    #define COMP_CPP_MAJ 17
#elif COMP_CPP_VER >= 201400L
    #define COMP_CPP_MAJ 14
#elif COMP_CPP_VER >= 201100L
    #define COMP_CPP_MAJ 11
#else
    #error It looks like you are using an older version of C++
    #error If you want to use xcc correctly
    #error Please upgrade your C++ version to version 202000L or later
#endif

// - NVIDIA CUDA C++ compiler for GPU
#if defined __CUDACC__
    #define COMP_TAG_CUDA
    #define COMP_STR "NVIDIA CUDA C++"
    #define COMP_MAJ __CUDACC_VER_MAJOR__
    #define COMP_MIN __CUDACC_VER_MINOR__
    #define COMP_PAT __CUDACC_VER_BUILD__

// - GCC-XML emulates other compilers
#elif defined(__GCCXML__)
    #define COMP_TAG_GCC_XML
    #define COMP_STR "GCC XML"
    #define COMP_MAJ __GCCXML_GNUC__
    #define COMP_MIN __GCCXML_GNUC_MINOR__
    #define COMP_PAT __GCCXML_GNUC_PATCHLEVEL__

// - EDG based Cray
#elif defined(_CRAYC)
    #define COMP_TAG_CRAY
    #define COMP_STR "Cray C"
    #define COMP_MAJ _RELEASE_MINOR / 1000
    #define COMP_MIN (_RELEASE_MINOR / 100) % 10
    #define COMP_PAT _RELEASE_MINOR % 1000

// - Comeau C++
#elif defined(__COMO__)
    #define COMP_TAG_COMO
    #define COMP_STR "Comeau C++"
    #define COMP_MAJ __COMO_VERSION__ / 100
    #define COMP_MIN __COMO_VERSION__ % 100
    #define COMP_PAT 0

// - PathScale EKOPath
#elif defined(__PATHCC__)
    #define COMP_TAG_PATHCC
    #define COMP_STR "PathScale EKOPath"
    #define COMP_MAJ __PATHCC__
    #define COMP_MIN __PATHCC_MINOR__
    #define COMP_PAT __PATHCC_PATCHLEVEL__

// - Intel ICX
#elif defined(__INTEL_LLVM_COMPILER)
    #define COMP_TAG_ICX
    #define COMP_STR "Intel ICX"
    #define COMP_MAJ (__INTEL_LLVM_COMPILER / 10000)
    #define COMP_MIN (__INTEL_LLVM_COMPILER / 100) % 100
    #define COMP_PAT __INTEL_LLVM_COMPILER % 100

// - Intel
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
    #define COMP_TAG_ICC
    #define COMP_STR "Intel ICC"
    #define COMP_MAJ (__INTEL_COMPILER_BUILD_DATE / 10000)
    #define COMP_MIN (__INTEL_COMPILER_BUILD_DATE / 100) % 100
    #define COMP_PAT __INTEL_COMPILER_BUILD_DATE % 100

// - Clang C++
#elif defined(__clang__) && !defined(__ibmxl__) && !defined(__CODEGEARC__)
    #define COMP_TAG_CLANG
    #define COMP_STR "Clang"
    #define COMP_MAJ __clang_major__
    #define COMP_MIN __clang_minor__
    #define COMP_PAT __clang_patchlevel__

// - Digital Mars C++
#elif defined(__DMC__)
    #define COMP_TAG_DMC
    #define COMP_STR "Digital Mars"
    #define COMP_MAJ (__DMC__ >> 8) & 0xF
    #define COMP_MIN (__DMC__ >> 4) & 0xF
    #define COMP_PAT __DMC__ & 0xF

// - Wind River Diab C++
#elif defined(__DCC__)
    #define COMP_TAG_DCC
    #define COMP_STR "Diab C/C++"
    #define COMP_MAJ (__VERSION_NUMBER__ / 1000)
    #define COMP_MIN (__VERSION_NUMBER__ / 100) % 10
    #define COMP_PAT __VERSION_NUMBER__ % 100

// - Portland Group C/C++
#elif defined(__PGI)
    #define COMP_TAG_PGI
    #define COMP_STR "Portland Group C/C++"
    #define COMP_MAJ __PGIC__
    #define COMP_MIN __PGIC_MINOR__
    #define COMP_PAT __PGIC_PATCHLEVEL__

// - GNU C++
#elif defined(__GNUC__) && !defined(__ibmxl__)
    #define COMP_TAG_GCC
    #define COMP_STR "GCC"
    #define COMP_MAJ __GNUC__
    #define COMP_MIN __GNUC_MINOR__
    #define COMP_PAT __GNUC_PATCHLEVEL__

// - Kai C++
#elif defined(__KCC)
    #define COMP_TAG_KCC
    #define COMP_STR "Kai C++"
    #define COMP_MAJ (__KCC_VERSION >> 12) & 0xF
    #define COMP_MIN (__KCC_VERSION >> 8) & 0xF
    #define COMP_PAT (((__KCC_VERSION >> 4) & 0xF) * 10) + (__KCC_VERSION & 0xF)

// - SGI MIPSpro C++
#elif defined(__sgi)
    #define COMP_TAG_SGI
    #define COMP_STR "SGI MIPSpro C++"
    #define COMP_MAJ (_COMPILER_VERSION / 100)
    #define COMP_MIN (_COMPILER_VERSION / 10) % 10
    #define COMP_PAT _COMPILER_VERSION % 10

// - Compaq Tru64 Unix cxx
#elif defined(__DECCXX)
    #define COMP_TAG_COMPAQ
    #define COMP_STR "Compaq C/C++"
    #define COMP_MAJ __DECC_VER / 10000000
    #define COMP_MIN (__DECC_VER / 100000) % 100
    #define COMP_PAT __DECC_VER % 10000

// - Greenhills C++
#elif defined(__ghs)
    #define COMP_TAG_GHS
    #define COMP_STR "Green Hill C/C++"
    #define COMP_MAJ __GHS_VERSION_NUMBER__ / 100
    #define COMP_MIN (__GHS_VERSION_NUMBER__ / 10) % 10
    #define COMP_PAT __GHS_VERSION_NUMBER__ % 10

// - CodeGear
#elif defined(__CODEGEARC__)
    #define COMP_TAG_CODEGEARC
    #define COMP_STR "CodeGear"
    #define COMP_MAJ (__CODEGEARC_VERSION__ >> 24) & 0xF
    #define COMP_MIN ((__CODEGEARC_VERSION__ >> 20) & 0xF * 10) + ((__CODEGEARC_VERSION__ >> 16) & 0xF)
    #define COMP_PAT (__CODEGEARC_VERSION__ >> 12) & 0xF

// - Borland
#elif defined(__BORLANDC__)
    #define COMP_TAG_BORLANDC
    #define COMP_STR "Borland C++"
    #define COMP_MAJ (__BORLANDC__ >> 8) & 0xF
    #define COMP_MIN (__BORLANDC__ >> 4) & 0xF
    #define COMP_PAT __BORLANDC__ & 0xF

// - Metrowerks CodeWarrior
#elif defined(__MWERKS__)
    #define COMP_TAG_MWERKS
    #define COMP_STR "Metrowerks CodeWarrior"
    #define COMP_MAJ (__MWERKS__ >> 12) & 0xF
    #define COMP_MIN (__MWERKS__ >> 8) & 0xF
    #define COMP_PAT (((__MWERKS__ >> 4) & 0xF) * 10) + (__MWERKS__ & 0xF)

// - Sun Workshop Compiler C++
#elif defined(__SUNPRO_CC)
    #define COMP_TAG_SUNPRO
    #define COMP_STR "Oracle Solaris Studio"
    #define COMP_MAJ (__SUNPRO_CC >> 12) & 0xF
    #define COMP_MIN (((__SUNPRO_CC >> 8) & 0xF) * 10) + ((__SUNPRO_CC >> 4) & 0xF)
    #define COMP_PAT (__SUNPRO_CC & 0xF)

// - HP aCC
#elif defined(__HP_aCC)
    #define COMP_TAG_HPACC
    #define COMP_STR "HP aC++"
    #define COMP_MAJ __HP_aCC / 10000
    #define COMP_MIN (__HP_aCC / 100) % 100
    #define COMP_PAT __HP_aCC % 100

// - MPW MrCpp or SCpp
#elif defined(__MRC__) || defined(__SC__)
    #define COMP_TAG_MRC
    #define COMP_STR "MPW C++"
    #define COMP_MAJ (((__MRC__ >> 12) & 0xF) * 10) + ((__MRC__ >> 8) & 0xF)
    #define COMP_MIN (((__MRC__ >> 4) & 0xF) * 10) + (__MRC__ & 0xF)
    #define COMP_PAT 0

// - IBM z/OS XL C/C++
#elif defined(__IBMCPP__) && defined(__COMPILER_VER__) && defined(__MVS__)
    #define COMP_TAG_IBMXL_ZOS
    #define COMP_STR "IBM z/OS XL C/C++"
    #define COMP_MAJ (__IBMCPP__ / 1000) % 10
    #define COMP_MIN (__IBMCPP__ / 10) % 100
    #define COMP_PAT __IBMCPP__ % 10

// - IBM XL C/C++ for Linux (Little Endian)
#elif defined(__ibmxl__)
    #define COMP_TAG_IBMXL_CLANG
    #define COMP_STR "IBM XL C/C++ (Clang-based versions)"
    #define COMP_MAJ __ibmxl_version__
    #define COMP_MIN __ibmxl_release__
    #define COMP_PAT __ibmxl_modification__

// - IBM Visual Age or IBM XL C/C++ for Linux (Big Endian)
#elif defined(__IBMCPP__)
    #define COMP_TAG_IBMXL_LEGACY
    #define COMP_STR "IBM XL C/C++ (legacy versions)"
    #define COMP_MAJ (((__IBMCPP__ >> 20) & 0xF) * 10) + ((__IBMCPP__ >> 16) & 0xF)
    #define COMP_MIN (((__IBMCPP__ >> 12) & 0xF) * 10) + ((__IBMCPP__ >> 8) & 0xF)
    #define COMP_PAT (((__IBMCPP__ >> 4) & 0xF) * 10) + (__IBMCPP__ & 0xF)

// - Microsoft Visual C++
#elif defined(_MSC_VER)
    #define COMP_TAG_MSVC
    #define COMP_STR "Microsoft Visual C++"
    #define COMP_MAJ (_MSC_VER / 100)
    #define COMP_MIN (_MSC_VER % 100)
    #define COMP_PAT _MSC_FULL_VER % 100000

// - Unknown
#else
    #define COMP_TAG_UNKNOWN
    #define COMP_STR "Unknown"
    #define COMP_MAJ 0
    #define COMP_MIN 0
    #define COMP_PAT 0
#endif

// - GNU libstdc++
#if defined(__GLIBCXX__)
    #define COMP_SLIB_TAG_GNU
    #define COMP_SLIB_STR "GNU"
    #define COMP_SLIB_MAJ __GLIBCXX__ / 10000
    #define COMP_SLIB_MIN (__GLIBCXX__ / 100) % 100
    #define COMP_SLIB_PAT __GLIBCXX__ % 100

// - MSVC STL
#elif defined(_CPPLIB_VER)
    #define COMP_SLIB_TAG_MSVC
    #define COMP_SLIB_STR "MSVC"
    #define COMP_SLIB_MAJ _MSVC_STL_UPDATE / 100
    #define COMP_SLIB_MIN _MSVC_STL_UPDATE % 100
    #define COMP_SLIB_PAT 0

// - libc++
#elif defined(_LIBCPP_VERSION)
    #define COMP_SLIB_TAG_LLVM
    #define COMP_SLIB_STR "LLVM"
    #define COMP_SLIB_MAJ _LIBCPP_VERSION / 1000
    #define COMP_SLIB_MIN 0
    #define COMP_SLIB_PAT 0

// - Unknown
#else
    #define COMP_SLIB_TAG_UNKNOWN
    #define COMP_SLIB_STR "Unknown"
    #define COMP_SLIB_MAJ 0
    #define COMP_SLIB_MIN 0
    #define COMP_SLIB_PAT 0
#endif

#define COMP_NO_ASAN
#define COMP_NO_MSAN
#define COMP_NO_TSAN
#if defined(__has_feature)  // Clang flavor
    #if __has_feature(address_sanitizer)
        #define _COMPILER_ASAN_ENABLED_
        #undef COMP_NO_ASAN
        #define COMP_NO_ASAN __attribute__((no_sanitize("address")))
    #endif
    #if __has_feature(memory_sanitizer)
        #define _COMPILER_MSAN_ENABLED_
        #undef COMP_NO_MSAN
        #define COMP_NO_MSAN __attribute__((no_sanitize("memory")))
    #endif
    #if __has_feature(thread_sanitizer)
        #if __clang_major__ < 11
            #error Thread sanitizer runtime libraries in clang < 11 leak memory and cannot be used
        #endif
        #define _COMPILER_TSAN_ENABLED_
        #undef COMP_NO_TSAN
        #define COMP_NO_TSAN __attribute__((no_sanitize("thread")))
    #endif
#else  // GCC flavor
    #if defined(__SANITIZE_ADDRESS__)
        #define _COMPILER_ASAN_ENABLED_
        #undef COMP_NO_ASAN
        #define COMP_NO_ASAN __attribute__((no_sanitize("address")))
    #endif
#endif  // __has_feature
#define COMP_NO_SANITIZE  COMP_NO_ASAN COMP_NO_MSAN COMP_NO_TSAN

//! Create macros for exporting functions for external use.
#define COMP_C_FUNCTION   extern "C"
#define COMP_CXX_FUNCTION extern "C++"
//! Create macros for exporting symbols for external use.
#define COMP_C_SYMBOL     extern "C"
#define COMP_CXX_SYMBOL   extern "C++"

#if defined(_MSC_VER)
    #define COMP_DISABLE_WARNING_PUSH           __pragma(warning(push))
    #define COMP_DISABLE_WARNING_POP            __pragma(warning(pop))
    #define COMP_DISABLE_WARNING(warningNumber) __pragma(warning(disable : warningNumber))
#elif defined(__GNUC__) || defined(__clang__)
    #define COMP_PRAGMA(X)                    _Pragma(#X)
    #define COMP_DISABLE_WARNING_PUSH         COMP_PRAGMA(GCC diagnostic push)
    #define COMP_DISABLE_WARNING_POP          COMP_PRAGMA(GCC diagnostic pop)
    #define COMP_DISABLE_WARNING(warningName) COMP_PRAGMA(GCC diagnostic ignored warningName)
#else
    #error unsupported compiler
#endif

#define COMP_SUPPRESS_WARNING(warning, ...) \
    COMP_DISABLE_WARNING_PUSH               \
    COMP_DISABLE_WARNING(warning)           \
    __VA_ARGS__                             \
    COMP_DISABLE_WARNING_POP

#if defined(_MSC_VER)
    #if defined(WIN_CALLLING_CONV)
        #if defined(__cplusplus)
            //! DLLs and EXEs can export functions. Based on WIN_CALLING_CONVENTION we can
            //! also export Windows-C-stack-calling.
            #define COMP_C_EXPORT   COMP_C_FUNCTION __declspec(dllexport) _stdcall
            #define COMP_CXX_EXPORT COMP_CXX_FUNCTION __declspec(dllexport) _stdcall
        #else
        //! DLLs and EXEs can export functions. Based on WIN_CALLING_CONVENTION we can
        //! also export Windows-C-stack-calling.
            #define COMP_C_EXPORT __declspec(dllexport) _stdcall
        #endif
    #else
        #if defined(__cplusplus)
        //! DLLs and EXEs can export functions. Based on WIN_CALLING_CONVENTION we can
        //! also export Windows-C-stack-calling.
            #define COMP_C_EXPORT   COMP_C_FUNCTION __declspec(dllexport)
            #define COMP_CXX_EXPORT COMP_CXX_FUNCTION __declspec(dllexport)
        #else
        //! DLLs and EXEs can export functions. Based on WIN_CALLING_CONVENTION we can
        //! also export Windows-C-stack-calling.
            #define COMP_C_EXPORT __declspec(dllexport)
        #endif
    #endif

    #define __FUNCTION_SIGNATURE__                     __FUNCSIG__

    #define strncasecmp                                _strnicmp
    #define strcasecmp                                 _stricmp

    #define COMP_SUPPRESS_MSVC_WARNING(code, ...)      COMP_SUPPRESS_WARNING(code, __VA_ARGS__)
    #define COMP_SUPPRESS_GCC_CLANG_WARNING(code, ...) __VA_ARGS__

#else

    #if defined(WASM)
        #if defined(__cplusplus)
        //! POSIX-C exports functions visible by default. We force it either way in the
        //! compiler with the visibility attribute.
            #define COMP_C_EXPORT   COMP_C_FUNCTION __attribute__((visibility("default"))) __attribute__((used))
            #define COMP_CXX_EXPORT COMP_CXX_FUNCTION __attribute__((visibility("default"))) __attribute__((used))
        #else
        //! POSIX-C exports functions visible by default. We force it either way in the
        //! compiler with the visibility attribute.
            #define COMP_C_EXPORT __attribute__((visibility("default"))) __attribute__((used))
        #endif
    #else
        #if defined(__cplusplus)
        //! POSIX-C exports functions visible by default. We force it either way in the
        //! compiler with the visibility attribute.
            #define COMP_C_EXPORT   COMP_C_FUNCTION __attribute__((visibility("default")))
            #define COMP_CXX_EXPORT COMP_CXX_FUNCTION __attribute__((visibility("default")))
        #else
        //! POSIX-C exports functions visible by default. We force it either way in the
        //! compiler with the visibility attribute.
            #define COMP_C_EXPORT __attribute__((visibility("default")))
        #endif
    #endif

    #define __FUNCTION_SIGNATURE__                     __PRETTY_FUNCTION__

    #define COMP_SUPPRESS_MSVC_WARNING(code, ...)      __VA_ARGS__
    #define COMP_SUPPRESS_GCC_CLANG_WARNING(code, ...) COMP_SUPPRESS_WARNING(code, __VA_ARGS__)

#endif

#define COMP_CONCAT_IMPL(x, y)  x##y
#define COMP_MACRO_CONCAT(x, y) COMP_CONCAT_IMPL(x, y)
#define COMP_COUNTER            __COUNTER__

//! Some systems might not support threading.
//! These defines are to seperate threading from non-threading.
#if defined(__wasm__)
    #define COMP_THREAD_LOCAL
#else
    #define COMP_THREAD_LOCAL thread_local
#endif

#ifdef __has_cpp_attribute
    #if __has_cpp_attribute(nodiscard)
        #define COMP_NO_DISCARD [[nodiscard]]
    #else
        #define COMP_NO_DISCARD
    #endif
    #if __has_cpp_attribute(fallthrough)
        #define COMP_FALLTHROUGH [[fallthrough]]
    #else
        #define COMP_FALLTHROUGH
    #endif
    #if __has_cpp_attribute(likely)
        #define COMP_LIKELY [[likely]]
    #else
        #define COMP_LIKELY
    #endif
    #if __has_cpp_attribute(unlikely)
        #define COMP_UNLIKELY [[unlikely]]
    #else
        #define COMP_UNLIKELY
    #endif
    #if __has_cpp_attribute(maybe_unused)
        #define COMP_KEEP            [[maybe_unused]]
        #define COMP_KEEP_EX(reason) [[maybe_unused(reason)]]
    #else
        #define COMP_KEEP
        #define COMP_KEEP_EX(reason)
    #endif
    #if __has_cpp_attribute(deprecated)
        #define COMP_DEPRECATED            [[deprecated]]
        #define COMP_DEPRECATED_EX(reason) [[deprecated(reason)]]
    #else
        #define COMP_DEPRECATED
        #define COMP_DEPRECATED_EX(reason)
    #endif
    #if defined(__clang__)
        #define COMP_ALWAYS_INLINE [[gnu::always_inline]] [[gnu::gnu_inline]] extern inline
    #elif defined(__GNUC__)
        #define COMP_ALWAYS_INLINE [[gnu::always_inline]] inline
    #elif defined(_MSC_VER)
        #pragma warning(error : 4714)
        #define COMP_ALWAYS_INLINE __forceinline
    #else
        #warning Compiler does not support always inlining.
        #define COMP_ALWAYS_INLINE
    #endif
#else
    #define COMP_NO_DISCARD
    #define COMP_FALLTHROUGH
    #define COMP_LIKELY
    #define COMP_UNLIKELY
    #define COMP_KEEP
    #define COMP_KEEP_EX(reason)
    #define COMP_DEPRECATED
    #define COMP_DEPRECATED_EX(reason)
    #define COMP_INLINE
#endif

#define CONSTEXPR_COMPILER_ERROR_IF_NOT(boolean_expr) ((boolean_expr) ? void() : std::abort())
#define CONSTEXPR_COMPILER_ERROR_IF(boolean_expr)     CONSTEXPR_COMPILER_ERROR_IF_NOT(!(boolean_expr))
#define CONSTEXPR_COMPILER_ERROR()                    CONSTEXPR_COMPILER_ERROR_IF_NOT(false)

template<typename T = size_t>
consteval inline T const_strlen(const char* str) {
    T counter = 0;
    while (*str++) counter++;
    return counter;
}

template<typename T, typename U>
constexpr size_t offsetOf(U T::*member) {
    return (char*) &((T*) nullptr->*member) - (char*) nullptr;
}

template<typename T>
inline static constexpr T const_pow(T num, T pow) {
    T ret = num;
    while (--pow) {
        ret *= num;
    }
    return ret;
}

consteval inline std::string_view __comp_filename() {
    std::string_view filename(__FILE__);
#if defined(WINDOWS)
    const char del = '\\';
#else
    const char del = '/';
#endif
    if (filename.rfind(del) != std::string_view::npos) {
        return { filename.data() + filename.rfind(del) };
    }
    return filename;
}

template<typename T, size_t S>
constexpr size_t __comp_get_file_name_offset(const T (&str)[S], size_t i = S - 1) {
    return (str[i] == '/' || str[i] == '\\') ? i + 1 : (i > 0 ? __comp_get_file_name_offset(str, i - 1) : 0);
}

template<typename T>
constexpr size_t __comp_get_file_name_offset(T (&str)[1]) {
    return 0;
}

#define COMP_FILE     (__FILE__)
#define COMP_FILENAME (&COMP_FILE[__comp_get_file_name_offset(COMP_FILE)])
#define COMP_LINE     (__LINE__)
#if defined(_MSC_VER)
    #define COMP_FUNCTION (__FUNCSIG__)
#else
    #define COMP_FUNCTION (__PRETTY_FUNCTION__)
#endif
#define COMP_DATE (__DATE__)
#define COMP_TIME (__TIME__)

//! Overwrite the new for a dfnew which could be used for tracking and such.
#if defined(DFSYSTEM_MEMORY_TRACKING)
    #include "utils/memory_tracker.h"
    #define dfdelete delete
#else
    #define dfnew    new
    #define dfdelete delete
#endif

#define COMP_MAKE_BIT_CAPABLE(T)                                  \
    inline T operator~(T a) {                                     \
        auto& arg1 = (typename std::underlying_type<T>::type&) a; \
        return (T) ~a;                                            \
    }                                                             \
    inline T operator|(T a, T b) {                                \
        auto& arg1 = (typename std::underlying_type<T>::type&) a; \
        auto& arg2 = (typename std::underlying_type<T>::type&) b; \
        return (T) (arg1 | arg2);                                 \
    }                                                             \
    inline T operator&(T a, T b) {                                \
        auto& arg1 = (typename std::underlying_type<T>::type&) a; \
        auto& arg2 = (typename std::underlying_type<T>::type&) b; \
        return (T) (arg1 & arg2);                                 \
    }                                                             \
    inline T operator^(T a, T b) {                                \
        auto& arg1 = (typename std::underlying_type<T>::type&) a; \
        auto& arg2 = (typename std::underlying_type<T>::type&) b; \
        return (T) (arg1 ^ arg2);                                 \
    }                                                             \
    inline T& operator|=(T& a, T b) {                             \
        auto& arg1 = (typename std::underlying_type<T>::type&) a; \
        auto& arg2 = (typename std::underlying_type<T>::type&) b; \
        return (T&) (arg1 |= arg2);                               \
    }                                                             \
    inline T& operator&=(T& a, T b) {                             \
        auto& arg1 = (typename std::underlying_type<T>::type&) a; \
        auto& arg2 = (typename std::underlying_type<T>::type&) b; \
        return (T&) (arg1 &= arg2);                               \
    }                                                             \
    inline T& operator^=(T& a, T b) {                             \
        auto& arg1 = (typename std::underlying_type<T>::type&) a; \
        auto& arg2 = (typename std::underlying_type<T>::type&) b; \
        return (T&) (arg1 ^= arg2);                               \
    }

//! This namespace contains ostream operators for coloring text in the output.
namespace terminal_coloring {
    inline const char* reset = "\033[0m";

    inline const char* foreground_black = "\033[30m";
    inline const char* foreground_red = "\033[31m";
    inline const char* foreground_green = "\033[32m";
    inline const char* foreground_yellow = "\033[33m";
    inline const char* foreground_blue = "\033[34m";
    inline const char* foreground_purple = "\033[35m";
    inline const char* foreground_cyan = "\033[36m";
    inline const char* foreground_white = "\033[37m";

    inline const char* foreground_bold_black = "\033[1;30m";
    inline const char* foreground_bold_red = "\033[1;31m";
    inline const char* foreground_bold_green = "\033[1;32m";
    inline const char* foreground_bold_yellow = "\033[1;33m";
    inline const char* foreground_bold_blue = "\033[1;34m";
    inline const char* foreground_bold_purple = "\033[1;35m";
    inline const char* foreground_bold_cyan = "\033[1;36m";
    inline const char* foreground_bold_white = "\033[1;37m";

    inline const char* foreground_underline_black = "\033[4;30m";
    inline const char* foreground_underline_red = "\033[4;31m";
    inline const char* foreground_underline_green = "\033[4;32m";
    inline const char* foreground_underline_yellow = "\033[4;33m";
    inline const char* foreground_underline_blue = "\033[4;34m";
    inline const char* foreground_underline_purple = "\033[4;35m";
    inline const char* foreground_underline_cyan = "\033[4;36m";
    inline const char* foreground_underline_white = "\033[4;37m";

    inline const char* high_foreground_black = "\033[90m";
    inline const char* high_foreground_red = "\033[91m";
    inline const char* high_foreground_green = "\033[92m";
    inline const char* high_foreground_yellow = "\033[93m";
    inline const char* high_foreground_blue = "\033[94m";
    inline const char* high_foreground_purple = "\033[95m";
    inline const char* high_foreground_cyan = "\033[96m";
    inline const char* high_foreground_white = "\033[97m";

    inline const char* high_foreground_bold_black = "\033[1;90m";
    inline const char* high_foreground_bold_red = "\033[1;90m";
    inline const char* high_foreground_bold_green = "\033[1;90m";
    inline const char* high_foreground_bold_yellow = "\033[1;90m";
    inline const char* high_foreground_bold_blue = "\033[1;90m";
    inline const char* high_foreground_bold_purple = "\033[1;90m";
    inline const char* high_foreground_bold_cyan = "\033[1;90m";
    inline const char* high_foreground_bold_white = "\033[1;90m";

    inline const char* background_black = "\033[40m";
    inline const char* background_red = "\033[41m";
    inline const char* background_green = "\033[42m";
    inline const char* background_yellow = "\033[43m";
    inline const char* background_blue = "\033[44m";
    inline const char* background_purple = "\033[45m";
    inline const char* background_cyan = "\033[46m";
    inline const char* background_white = "\033[47m";

    inline const char* high_background_black = "\033[0;100m";
    inline const char* high_background_red = "\033[0;100m";
    inline const char* high_background_green = "\033[0;100m";
    inline const char* high_background_yellow = "\033[0;100m";
    inline const char* high_background_blue = "\033[0;100m";
    inline const char* high_background_purple = "\033[0;100m";
    inline const char* high_background_cyan = "\033[0;100m";
    inline const char* high_background_white = "\033[0;100m";

    inline const char* underline = "\033[4m";
}  // namespace terminal_coloring

#include <sstream>
#include <string>
#include <string_view>

namespace std {
    // Escapes an std::string or std::string_view to a hex chars if control chars.
    template<typename T>
    inline std::string escape_to_hex(const T& value) {
        std::stringstream hex;
        for (typename T::size_type i = 0; i < value.size(); ++i) {
            auto ch = *(::byte*) &value[i];
            if (ch >= 32 && ch <= 126) {
                hex << ch;
                continue;
            }
            hex << "0x";
            hex << "0123456789ABCDEF"[ch >> 4];
            hex << "0123456789ABCDEF"[ch & 0x0F];
        }
        return hex.str();
    }
}  // namespace std

inline void g_replace(std::string& str, const std::string& from, const std::string& to, bool& error) {
    size_t startPos = str.find(from);
    if (startPos == std::string::npos) {
        error = true;
        return;
    }
    str.replace(startPos, from.length(), to);
}

template<typename T>
inline std::string g_format_value(const T& value) {
    std::stringstream stream {};

    // TODO: Don't have a has_underlying to check so we hard code the const char*.
    if constexpr (std::is_pointer_v<T> || std::is_same_v<T, const char*&> || std::is_same_v<T, char*&> ||
                  std::is_same_v<T, const unsigned char*&> || std::is_same_v<T, unsigned char*&>) {
        if (!value) return "[nullptr]";
    }
    stream << value;
    return stream.str();
}

template<typename... T>
inline std::string g_format(std::string_view fmt, const T&... args) {
    bool error = false;
    std::string result(fmt);
    (g_replace(result, "{}", g_format_value(args), error), ...);
    return error ? "[invalid fmt]" : result;
}

#define FORMAT(...) g_format(__VA_ARGS__)

namespace log {
    void print_enable_ansi_coloring(bool enabled);
    void print_debug(const char* color, const std::string& str);
}  // namespace log

#if !defined(NDEBUG) && !defined(WASM)
// Commented this out to increase capacity of debug emscripten build
    #define DEBUG(...)                print_debug(::terminal_coloring::foreground_bold_blue, FORMAT(__VA_ARGS__))
    #define DEBUG_COLORED(color, ...) print_debug(color, FORMAT(__VA_ARGS__))
#else
    #define DEBUG(...)
    #define DEBUG_COLORED(...)
#endif
#if !defined(PRINT)
namespace log {
    void print(const std::string& str);
}  // namespace log
    #define PRINT(...) print(FORMAT(__VA_ARGS__))
#endif
#if !defined(PRINTLN)
namespace log {
    void println(const std::string& str);
}  // namespace log
    #define PRINTLN(...) println(FORMAT(__VA_ARGS__))
#endif
#if !defined(LOG)
namespace log {
    void print_log(const std::string& str);
}  // namespace log
    #define LOG(...) print_log(FORMAT(__VA_ARGS__))
#endif
#if !defined(SUCCESS)
namespace log {
    void print_success(const std::string& str);
}  // namespace log
    #define SUCCESS(...) print_success(FORMAT(__VA_ARGS__))
#endif
#if !defined(WARN)
namespace log {
    void print_warn(const std::string& str);
}  // namespace log
    #define WARN(...) print_warn(FORMAT(__VA_ARGS__))
#endif
#if !defined(ERROR)
namespace log {
    void print_error(const std::string& str);
}  // namespace log
    #define ERROR(...) print_error(FORMAT(__VA_ARGS__))
#endif

#define SOURCE_LOCATION         (FORMAT("file: {}({}) `{}`.", COMP_FILENAME, COMP_LINE, COMP_FUNCTION))
#define SOURCE_LOCATION_EX(...) (FORMAT("file: {}({}) `{}`: {}", COMP_FILENAME, COMP_LINE, COMP_FUNCTION, FORMAT(__VA_ARGS__)))

#define UNREACHABLE()                                         \
    ERROR(SOURCE_LOCATION_EX("This should not be reached.")); \
    assert(false);                                            \
    std::abort()

#define UNREACHABLE_EX(...)                         \
    ERROR(SOURCE_LOCATION_EX(FORMAT(__VA_ARGS__))); \
    assert(false);                                  \
    std::abort()

#if !defined(NDEBUG)
    #define ASSERT(expr)                                              \
        if (!(expr)) {                                                \
            ERROR(SOURCE_LOCATION_EX("This should not be reached.")); \
            assert(false);                                            \
            std::abort();                                             \
        }

    #define ASSERT_EX(expr, ...)                            \
        if (!(expr)) {                                      \
            ERROR(SOURCE_LOCATION_EX(FORMAT(__VA_ARGS__))); \
            assert(false);                                  \
            std::abort();                                   \
        }
#else
    #define ASSERT(expr)
    #define ASSERT_EX(expr, ...)
#endif

#define CHECK(expr)                                               \
    if (!(expr)) {                                                \
        ERROR(SOURCE_LOCATION_EX("This should not be reached.")); \
        assert(false);                                            \
        std::abort();                                             \
    }

#define CHECK_EX(expr, ...)                             \
    if (!(expr)) {                                      \
        ERROR(SOURCE_LOCATION_EX(FORMAT(__VA_ARGS__))); \
        assert(false);                                  \
        std::abort();                                   \
    }
