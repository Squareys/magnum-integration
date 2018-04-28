#.rst:
# Find OVRPlatformSDK
# --------
#
# Finds the Oculus Platform SDK library. This module defines:
#
#  OVRPlatformSDK_FOUND                - True if OVR library is found
#  OVRPlatformSDK::OVRPlatformSDK        - OVR imported target
#
# Additionally these variables are defined for internal usage:
#
#  OVRPlatformSDK_LIBRARY              - OVR Platform SDK library
#  OVRPlatformSDK_INCLUDE_DIR          - Include dir
#

#
#   This file is part of Magnum.
#
#   Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016
#             Vladimír Vondruš <mosra@centrum.cz>
#   Copyright © 2017 Jonathan Hale <squareys@googlemail.com>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

if(NOT OVR_PLATFORM_SDK_ROOT)
    find_path(OVR_PLATFORM_SDK_ROOT OVRPlatformSDK)

    if(OVR_PLATFORM_SDK_ROOT)
        set(OVR_PLATFORM_SDK_ROOT "${OVR_PLATFORM_SDK_ROOT}/OVRPlatformSDK")
        message(STATUS "Found Oculus Platform SDK: ${OVR_PLATFORM_SDK_ROOT}")
    endif()
endif()

# find include directory
find_path(OVR_PLATFORM_INCLUDE_DIR NAMES OVR_Platform.h HINTS ${OVR_PLATFORM_SDK_ROOT}/Include)

if(WIN32)
    find_library(OVR_PLATFORM_LIBRARY NAMES LibOVRPlatform64_1.lib HINTS "${OVR_PLATFORM_SDK_ROOT}/Windows")
    find_file(OVR_PLATFORM_LOADER NAMES OVR_PlatformLoader.cpp HINTS "${OVR_PLATFORM_SDK_ROOT}/Windows")
else()
    error("The Oculus Platform SDK does not support ${CMAKE_SYSTEM_NAME}.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OVRPlatformSDK DEFAULT_MSG
    OVR_PLATFORM_LIBRARY
    OVR_PLATFORM_INCLUDE_DIR
    OVR_PLATFORM_LOADER)

mark_as_advanced(FORCE
    OVR_PLATFORM_LIBRARY
    OVR_PLATFORM_INCLUDE_DIR
    OVR_PLATFORM_LOADER)

if(NOT TARGET OVRPlatformSDK::OVRPlatformSDK)
    add_library(OVRPlatformSDK::OVRPlatformSDK UNKNOWN IMPORTED)
    set_target_properties(OVRPlatformSDK::OVRPlatformSDK PROPERTIES
        IMPORTED_LOCATION ${OVR_PLATFORM_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${OVR_PLATFORM_INCLUDE_DIR})
endif()
