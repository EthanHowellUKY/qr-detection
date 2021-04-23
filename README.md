
- [BAV QR Code Module](#bav-qr-code-module)
- [Required Packages](#required-packages)
- [Building the Project](#building-the-project)
- [Example](#example)
  - [Example 1](#example-1)
  - [Example 2](#example-2)
  - [Example 3](#example-3)

# BAV QR Code Module

The intent of this module is to provide BAV users a generic library to handle generating, reading, and writing QR codes. This module currently has the following capabilities.

- Generate a QR code from a string.
- Save a QR code to a PNG file.
- Read a single QR code from an image provided via file path.
- Open the device camera, recognize a QR code and decode it.

# Required Packages

A few packages are needed to be able to compile the tracking program. These packages are readily available using [vcpkg](https://github.com/microsoft/vcpkg) provided by Microsoft. The installation of `vcpkg` should reside in the root directory, `C:\`. The following commands will install the needed packages.

```powershell
PS C:\Users> vcpkg install libpng:x64-windows
PS C:\Users> vcpkg install libqrencode:x64-windows
```

Additionally, you will need [OpenCV4](https://opencv.org/releases/) to be able to decode the QR Codes generated. For this you will need to build from source to get the drivers necessary for OpenCV to access the device cameras.

# Building the Project

This project is configured to build as a library and will produce no executable. The provided `CMakeLists.txt` build file is built to adhere to the following file structure.

```txt
./
+-- libs/
|   +-- BAVQR/
|   |   +-- .gitignore
|   |   +-- include/
|   |   |   +-- BAVQR/
|   |   |   |   +-- qr.h
|   |   +-- src/
|   |   |   +-- qr.cpp
|   |   +-- CMakeLists.txt
+-- src/
|   +-- main.cpp
|   +-- CMakeLists.txt
+-- CMakeLists.txt
```

To follow this structure, add these lines to  `./CMakeLists.txt`

```cmake
...
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(libs/BAVQR)
add_subdirectory(src)
...
```

And add these lines to  `src/CMakeLists.txt`

```cmake
...
project(${CMAKE_PROJECT_NAME} CXX)
find_package(OpenCV CONFIG REQUIRED)
find_path(QRENCODE_INCLUDE_DIR NAMES qrencode.h)
find_library(QRENCODE_LIBRARY_RELEASE qrencode)
find_library(QRENCODE_LIBRARY_DEBUG qrencoded)
set(QRENCODE_LIBRARIES optimized ${QRENCODE_LIBRARY_RELEASE} debug ${QRENCODE_LIBRARY_DEBUG})

find_package(PNG REQUIRED)
file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SRC_FILES *.cpp)
...

...
target_link_libraries(${CMAKE_PROJECT_NAME}
    bav::qr
    ${OpenCV_LIBS}
    ${QRENCODE_LIBRARIES}
    ${PNG_LIBRARIES}
)
...
```

# Example

## Example 1

The following example shows how the module can be used to encode a string into a QR Code and write its output to a PNG File.

```cpp
#include <iostream>
#include <string>
#include "BAVQR/qr.h

int main(int argc, char *argv[])
{
    // argv[1] = string-to-encode
    // argv[2] = PNG File name with extension
    QR qr;
    qr.encode_qr(argv[1]);
    qr.create_png(argv[2]);

    return 0;
}
```

## Example 2

The following example shows how the module can be used to read an image file containing a QR code, decode it, and write the decoded output.

```cpp
#include <iostream>
#include <string>
#include "BAVQR/qr.h

int main(int argc, char *argv[])
{
    // argv[1] = PNG File name with extension
    QR qr;
    std::string decoded_data = qr.decode_qr(argv[1]);
    std::cout << decoded_data << '\n';

    return 0;
}
```

## Example 3

The following example shows how the module can be used to access the device camera, read until a QR code is detected, and decode the detected QR code.

```cpp
#include <iostream>
#include <string>
#include "BAVQR/qr.h

int main(int argc, char *argv[])
{
    QR qr;
    qr.read_qr();
    std::cout << qr.decoded_data() << '\n';

    return 0;
}
```
