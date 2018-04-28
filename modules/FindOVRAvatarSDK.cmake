#.rst:
# Find OVRAvatarSDK
# --------
#
# Finds the Oculus Avatar SDK library. This module defines:
#
#  OVRAvatarSDK_FOUND                - True if OVR library is found
#  OVRAvatarSDK::OVRAvatarSDK        - OVR imported target
#
# Additionally these variables are defined for internal usage:
#
#  OVRAvatarSDK_LIBRARY              - OVR Avatar SDK library
#  OVRAvatarSDK_INCLUDE_DIR          - Include dir
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

if(NOT OVR_AVATAR_SDK_ROOT)
    find_path(OVR_AVATAR_SDK_ROOT OVRAvatarSDK)

    if(OVR_AVATAR_SDK_ROOT)
        set(OVR_AVATAR_SDK_ROOT "${OVR_AVATAR_SDK_ROOT}/OVRAvatarSDK")
        message(STATUS "Found Oculus Avatar SDK: ${OVR_AVATAR_SDK_ROOT}")
    endif()
endif()

# find include directory
find_path(OVR_AVATAR_INCLUDE_DIR NAMES OVR_Avatar.h HINTS ${OVR_AVATAR_SDK_ROOT}/Include)

if(WIN32)
    find_library(OVR_AVATAR_LIBRARY NAMES libovravatar HINTS "${OVR_AVATAR_SDK_ROOT}/Windows")
else()
    error("The Oculus Avatar SDK does not support ${CMAKE_SYSTEM_NAME}.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OVRAvatarSDK DEFAULT_MSG
    OVR_AVATAR_LIBRARY
    OVR_AVATAR_INCLUDE_DIR)

mark_as_advanced(FORCE
    OVR_AVATAR_LIBRARY
    OVR_AVATAR_INCLUDE_DIR)

if(NOT TARGET OVRAvatarSDK::OVRAvatarSDK)
    add_library(OVRAvatarSDK::OVRAvatarSDK UNKNOWN IMPORTED)
    set_target_properties(OVRAvatarSDK::OVRAvatarSDK PROPERTIES
        IMPORTED_LOCATION ${OVR_AVATAR_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${OVR_AVATAR_INCLUDE_DIR})
endif()
