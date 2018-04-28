
find_package(Magnum REQUIRED Text Shapes)
find_package(OVRPlatformSDK REQUIRED)
find_package(OVRAvatarSDK REQUIRED)
find_package(Corrade REQUIRED Interconnect)

set(SOURCES
    OvrPlatformIntegration.cpp
    AvatarShader.cpp
    ${OVR_PLATFORM_LOADER}
)

set(HEADERS
    OvrPlatformIntegration.h
    AvatarShader.h
)

corrade_add_resource(MagnumOvrPlatformIntegration_RCS resources.conf)

add_library(MagnumOvrPlatformIntegration STATIC ${SOURCES} ${HEADERS} ${MagnumOvrPlatformIntegration_RCS})
target_link_libraries(MagnumOvrPlatformIntegration
    WonderlandEngine
    WonderlandEnginePlatform
    WonderlandEngineSceneGraph
    OVRAvatarSDK::OVRAvatarSDK
    OVRPlatformSDK::OVRPlatformSDK
    Corrade::Interconnect
)

