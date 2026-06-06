set(ZEPHYR_SOURCES
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

set(ZEPHYR_PUBLIC_HEADERS
    include/Zephyr/Zephyr.hpp
    include/Zephyr/Core/ByteReader.hpp
    include/Zephyr/Core/Result.hpp
    include/Zephyr/Pak/PakArchive.hpp
    include/Zephyr/Pak/PakTypes.hpp
)

set(ZEPHYR_PRIVATE_HEADERS
    src/Core/Crypto/Aes.hpp
    src/Core/Crypto/Sha1.hpp
    src/Core/Hash/Fnv.hpp
    src/Core/Compression/ICompressor.hpp
    src/Pak/PakProfile.hpp
    src/Pak/Codec/IFooterCodec.hpp
    src/Pak/Codec/IIndexCodec.hpp
    src/Pak/Codec/IEntryCodec.hpp
    src/Pak/Codec/Footer/FooterCodec_V8toV11.hpp
    src/Pak/Codec/Index/IndexCodec_PathHash_V10toV11.hpp
    src/Pak/Codec/Index/SecondaryIndex.hpp
    src/Pak/Codec/Entry/EntryCodec_Encoded_V11.hpp
    src/Pak/Codec/Entry/EntryFlags.hpp
    src/Pak/Mount/VirtualFileSystem.hpp
)

add_library(Zephyr STATIC
    ${ZEPHYR_SOURCES}
    ${ZEPHYR_PUBLIC_HEADERS}
    ${ZEPHYR_PRIVATE_HEADERS}
)

target_include_directories(Zephyr
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
             PREFIX "Source Files"
             FILES ${ZEPHYR_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
             PREFIX "Header Files"
             FILES ${ZEPHYR_PRIVATE_HEADERS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
             PREFIX "Public Headers"
             FILES ${ZEPHYR_PUBLIC_HEADERS})

set_target_properties(Zephyr PROPERTIES
    OUTPUT_NAME "Zephyr"
    DEBUG_POSTFIX "d"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

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