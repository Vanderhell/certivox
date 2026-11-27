cmake_minimum_required(VERSION 3.16)
project(Certivox VERSION 0.2.0 LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(CERTIVOX_BUILD_TESTS "Build Certivox tests" OFF)
option(CERTIVOX_BUILD_EXAMPLES "Build Certivox examples" OFF)
option(CERTIVOX_BUILD_BENCHMARK "Build the host benchmark" OFF)
option(CERTIVOX_STRICT_WARNINGS "Treat supported compiler warnings as errors" ON)

function(certivox_apply_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
        if(CERTIVOX_STRICT_WARNINGS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        if(CERTIVOX_STRICT_WARNINGS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

add_library(certivox STATIC
    src/certivox_crc8_compute.c
    src/certivox_crc16_compute.c
    src/certivox_crc24_compute.c
    src/certivox_crc32_compute.c
    src/certivox_crc64_compute.c
    src/certivox_crc_update_byte_u32.c
    src/certivox_crc_combine_u32.c
    src/certivox_crc_reflect_poly_u32.c
    src/certivox_hamming74_encode.c
    src/certivox_hamming74_decode.c
    src/certivox_hamming84_encode.c
    src/certivox_hamming84_decode.c
    src/certivox_secded_parity_u32.c
    src/certivox_secded_check_u32.c
)
add_library(Certivox::Certivox ALIAS certivox)
target_compile_features(certivox PUBLIC c_std_99)
set_target_properties(certivox PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO
    EXPORT_NAME Certivox
)
target_include_directories(certivox PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
certivox_apply_warnings(certivox)

if(CERTIVOX_BUILD_TESTS)
    enable_testing()
    enable_language(CXX)
    set(CERTIVOX_TESTS
        crc8_compute crc16_compute crc24_compute crc32_compute crc64_compute
        crc_update_byte_u32 crc_combine_u32 crc_reflect_poly_u32
        hamming74_encode hamming74_decode hamming84_encode hamming84_decode
        secded_parity_u32 secded_check_u32 integration audit_differential
    )
    foreach(test_name IN LISTS CERTIVOX_TESTS)
        add_executable(certivox_test_${test_name} tests/test_${test_name}.c)
        target_link_libraries(certivox_test_${test_name} PRIVATE Certivox::Certivox)
        target_compile_features(certivox_test_${test_name} PRIVATE c_std_99)
        set_target_properties(certivox_test_${test_name} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        certivox_apply_warnings(certivox_test_${test_name})
        add_test(NAME certivox.${test_name} COMMAND certivox_test_${test_name})
    endforeach()
    set(CERTIVOX_PUBLIC_HEADERS
        crc8_compute.h crc16_compute.h crc24_compute.h crc32_compute.h crc64_compute.h
        crc_update_byte_u32.h crc_combine_u32.h crc_reflect_poly_u32.h
        hamming74_encode.h hamming74_decode.h hamming84_encode.h hamming84_decode.h
        secded_parity_u32.h secded_check_u32.h
    )
    foreach(header IN LISTS CERTIVOX_PUBLIC_HEADERS)
        string(REPLACE ".h" "" header_test_name "${header}")
        set(header_smoke_source "${CMAKE_CURRENT_BINARY_DIR}/header_${header_test_name}.c")
        file(WRITE "${header_smoke_source}"
            "#include <certivox/${header}>\nint main(void) { return 0; }\n")
        add_executable(certivox_header_${header_test_name} "${header_smoke_source}")
        target_include_directories(certivox_header_${header_test_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_compile_features(certivox_header_${header_test_name} PRIVATE c_std_99)
        set_target_properties(certivox_header_${header_test_name} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        certivox_apply_warnings(certivox_header_${header_test_name})
        add_test(NAME certivox.header.${header_test_name} COMMAND certivox_header_${header_test_name})
    endforeach()
    add_executable(certivox_headers_cpp tests/headers_cpp.cpp)
    target_link_libraries(certivox_headers_cpp PRIVATE Certivox::Certivox)
    certivox_apply_warnings(certivox_headers_cpp)
    add_test(NAME certivox.headers_cpp COMMAND certivox_headers_cpp)
endif()

if(CERTIVOX_BUILD_EXAMPLES)
    add_executable(certivox_example examples/crc_and_ecc.c)
    target_link_libraries(certivox_example PRIVATE Certivox::Certivox)
    target_compile_features(certivox_example PRIVATE c_std_99)
    set_target_properties(certivox_example PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    certivox_apply_warnings(certivox_example)
endif()

if(CERTIVOX_BUILD_BENCHMARK)
    add_executable(certivox_host_benchmark bench/host_benchmark.c)
    target_link_libraries(certivox_host_benchmark PRIVATE Certivox::Certivox)
    target_compile_features(certivox_host_benchmark PRIVATE c_std_99)
    set_target_properties(certivox_host_benchmark PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    certivox_apply_warnings(certivox_host_benchmark)
endif()

install(TARGETS certivox EXPORT CertivoxTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT CertivoxTargets FILE CertivoxTargets.cmake NAMESPACE Certivox::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Certivox)
configure_package_config_file(cmake/CertivoxConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/CertivoxConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Certivox)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/CertivoxConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CertivoxConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/CertivoxConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Certivox)
