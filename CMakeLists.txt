cmake_minimum_required(VERSION 3.16)
project(certivox VERSION 0.1.0 LANGUAGES C)

option(CERTIVOX_BUILD_TESTS "Build Certivox host tests" ON)
option(CERTIVOX_BUILD_EXAMPLES "Build Certivox examples" ON)
option(CERTIVOX_BUILD_BENCHMARK "Build the host smoke benchmark" OFF)

add_library(certivox STATIC
    src/cv_crc8_compute.c
    src/cv_crc16_compute.c
    src/cv_crc24_compute.c
    src/cv_crc32_compute.c
    src/cv_crc64_compute.c
    src/cv_crc_update_byte_u32.c
    src/cv_crc_combine_u32.c
    src/cv_crc_reflect_poly_u32.c
    src/cv_hamming74_encode.c
    src/cv_hamming74_decode.c
    src/cv_hamming84_encode.c
    src/cv_hamming84_decode.c
    src/cv_secded_parity_u32.c
    src/cv_secded_check_u32.c
)

target_include_directories(certivox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(certivox PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)

if(MSVC)
    target_compile_options(certivox PRIVATE /W4)
else()
    target_compile_options(certivox PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CERTIVOX_BUILD_TESTS)
    enable_testing()
    set(CERTIVOX_TESTS
        crc8_compute
        crc16_compute
        crc24_compute
        crc32_compute
        crc64_compute
        crc_update_byte_u32
        crc_combine_u32
        hamming74_encode
        hamming74_decode
        hamming84_encode
        hamming84_decode
        secded_parity_u32
        secded_check_u32
        crc_reflect_poly_u32
        integration
    )
    foreach(test_name IN LISTS CERTIVOX_TESTS)
        add_executable(certivox_test_${test_name} tests/test_${test_name}.c)
        target_link_libraries(certivox_test_${test_name} PRIVATE certivox)
        set_target_properties(certivox_test_${test_name} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        add_test(NAME ${test_name} COMMAND certivox_test_${test_name})
    endforeach()
endif()

if(CERTIVOX_BUILD_EXAMPLES)
    add_executable(certivox_example examples/crc_and_ecc.c)
    target_link_libraries(certivox_example PRIVATE certivox)
    set_target_properties(certivox_example PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
endif()

if(CERTIVOX_BUILD_BENCHMARK)
    add_executable(certivox_host_benchmark bench/host_benchmark.c)
    target_link_libraries(certivox_host_benchmark PRIVATE certivox)
    set_target_properties(certivox_host_benchmark PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
endif()
