
project(BAVQR)

if (WIN32)
    set(VCPKG_DIR "C:/vcpkg/installed/x64-windows")
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
elseif(UNIX)
    set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu")
endif(WIN32)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else(NOT MSCV)
    set(CMAKE_CXX_STANDARD 17)
endif(MSVC)

file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_library(
    ${PROJECT_NAME}
    ${SRC_FILES}
    ${HEADER_FILES}
)
add_library(bav::qr ALIAS ${PROJECT_NAME})

find_package(OpenCV CONFIG REQUIRED PATHS C:/Users/ethan/Downloads/opencv-4.5.1/opencv-4.5.1/build)
find_path(QRENCODE_INCLUDE_DIR NAMES qrencode.h)
find_library(QRENCODE_LIBRARY_RELEASE qrencode)
find_library(QRENCODE_LIBRARY_DEBUG qrencoded)
set(QRENCODE_LIBRARIES optimized ${QRENCODE_LIBRARY_RELEASE} debug ${QRENCODE_LIBRARY_DEBUG})

find_package(PNG REQUIRED)
#find_package(libharu REQUIRED)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${QRENCODE_INCLUDE_DIR}
    PRIVATE ${PNG_INCLUDE_DIR}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE ${OpenCV_LIBS}
    PRIVATE ${QRENCODE_LIBRARIES}
    PRIVATE ${PNG_LIBRARIES}
)
