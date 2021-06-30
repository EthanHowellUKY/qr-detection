
project(QR)

if (WIN32)
    set(VCPKG_DIR "C:/vcpkg/installed/x64-windows")
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
elseif(UNIX)
    if(APPLE)
        set(VCPKG_DIR "$ENV{HOME}/vcpkg/installed/x86-osx")
        set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        set(ZBAR_INCLUDE_DIR "/usr/local/Cellar/zbar/0.23.90/include")
        set(ZBAR_LIB_DIR "/usr/local/Cellar/zbar/0.23.90/lib")
    endif()
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
add_library(echo::qr ALIAS ${PROJECT_NAME})

find_package(OpenCV CONFIG REQUIRED)
find_path(QRENCODE_INCLUDE_DIR NAMES qrencode.h)
find_library(QRENCODE_LIBRARY_RELEASE qrencode)
if (WIN32)
    find_library(QRENCODE_LIBRARY_DEBUG qrencoded)
elseif(UNIX)
    find_library(QRENCODE_LIBRARY_DEBUG qrencode)
endif()
set(QRENCODE_LIBRARIES optimized ${QRENCODE_LIBRARY_RELEASE} debug ${QRENCODE_LIBRARY_DEBUG})

find_package(PNG REQUIRED)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${QRENCODE_INCLUDE_DIR}
    PRIVATE ${PNG_INCLUDE_DIR}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${ZBAR_INCLUDE_DIR}
)

target_link_directories( ${PROJECT_NAME}
    PRIVATE ${ZBAR_LIB_DIR}
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE ${OpenCV_LIBS}
    PRIVATE ${QRENCODE_LIBRARIES}
    PRIVATE ${PNG_LIBRARIES}
    PRIVATE ${ZBAR_LIBRARIES} zbar
)
