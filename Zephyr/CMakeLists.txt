add_library(Zephyr STATIC
    src/Core/ByteReader.cpp
    src/Core/Crypto/Aes.cpp
    src/Core/Crypto/Sha1.cpp
    src/Core/Hash/Fnv.cpp
    src/Core/Compression/ZlibCompressor.cpp
    src/Core/Compression/CompressionRegistry.cpp
    src/Pak/PakArchive.cpp
    src/Pak/PakProfileRegistry.cpp
    src/Pak/Codec/Footer/FooterCodec_V8toV11.cpp
    src/Pak/Codec/Index/IndexCodec_PathHash_V10toV11.cpp
    src/Pak/Codec/Index/SecondaryIndex.cpp
    src/Pak/Codec/Entry/EntryCodec_Encoded_V11.cpp
    src/Pak/Mount/VirtualFileSystem.cpp
)

target_include_directories(Zephyr
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(Zephyr PROPERTIES
    OUTPUT_NAME              "Zephyr"
    DEBUG_POSTFIX            "d"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

if(MSVC)
    target_compile_definitions(Zephyr PRIVATE _MBCS)
    get_target_property(_zdefs Zephyr COMPILE_DEFINITIONS)
    if(_zdefs)
        list(REMOVE_ITEM _zdefs UNICODE _UNICODE)
        set_target_properties(Zephyr PROPERTIES COMPILE_DEFINITIONS "${_zdefs}")
    endif()
    target_compile_options(Zephyr PRIVATE /permissive- /Zc:__cplusplus /W4)
    set_property(TARGET Zephyr PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

option(ZEPHYR_WITH_OODLE "Build Oodle decompressor" OFF)
if(ZEPHYR_WITH_OODLE)
    target_sources(Zephyr PRIVATE src/Core/Compression/OodleCompressor.cpp)
    target_compile_definitions(Zephyr PRIVATE ZEPHYR_WITH_OODLE)
endif()