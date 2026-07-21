cmake_minimum_required(VERSION 3.30)

project(CcpMath)

include(cmake/CcpTargetConfigurations.cmake)
include(cmake/CcpDocsGenerator.cmake)
include(cmake/CcpPackageConfigHelpers.cmake)
include(cmake/CcpBuildConfigurations.cmake)

find_package(DirectXMath CONFIG REQUIRED)

ccp_add_library(CcpMath STATIC)
target_sources(CcpMath PRIVATE
    src/AxisAlignedBox.cpp
    src/Color.cpp
    src/Float16.cpp
    src/Matrix.cpp
    src/Plane.cpp
    src/Quaternion.cpp
    src/Sphere.cpp
    src/Vector2.cpp
    src/Vector3.cpp
    src/Vector4.cpp
)

set(PUBLIC_HEADER_FILES
    include/AxisAlignedBox.h
    include/AxisAlignedBox_inline.h
    include/AxisAlignedEllipsoid.h
    include/AxisAlignedEllipsoid_inline.h
    include/NumConst.h
    include/CcpMath.h
    include/Color.h
    include/Color_inline.h
    include/Float16.h
    include/Float16_inline.h
    include/Matrix.h
    include/Matrix_inline.h
    include/Plane.h
    include/Plane_inline.h
    include/Quaternion.h
    include/Quaternion_inline.h
    include/Ray.h
    include/Ray_inline.h
    include/Requirements.h
    include/Sphere.h
    include/Sphere_inline.h
    include/Vector2.h
    include/Vector2_inline.h
    include/Vector3.h
    include/Vector3_inline.h
    include/Vector4.h
    include/Vector4_inline.h
)
target_sources(CcpMath PRIVATE ${PUBLIC_HEADER_FILES})

set_target_properties(CcpMath PROPERTIES POSITION_INDEPENDENT_CODE ON EXPORT_NAME CcpMath)

target_include_directories(CcpMath
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(CcpMath PUBLIC Microsoft::DirectXMath)

target_compile_options(CcpMath PRIVATE "${MATH_OPTIMIZE_FLAG}")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(BUILD_TESTING "Build and run tests. Enabled by default." ON)
    include(CTest)
    if(BUILD_TESTING)
	    add_subdirectory(tests)
    endif()
endif()

# Provide an install target if this is the top level project only
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option (INSTALL_TO_MONOLITH "Install this package to a perforce branch vendor folder - provided for backwards compatability" OFF)
    if(INSTALL_TO_MONOLITH)
        # Install rule to ensure that our runtime and linker files are in the expected, platform-specific folders
        install(
                TARGETS CcpMath
                EXPORT CcpMathTargets
                CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
                ARCHIVE DESTINATION lib/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
                RUNTIME DESTINATION bin/${CCP_PLATFORM}/${CCP_ARCHITECTURE}/${CCP_TOOLSET}/
        )
        # Install rule for available public headers
        install(FILES ${PUBLIC_HEADER_FILES} DESTINATION include)

        configure_ccp_vendor_config_file(
                TARGET CcpMath
                DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/CcpMathConfig.cmake"
                DEPENDENCIES DirectXMath
        )
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CcpMathConfig.cmake DESTINATION .)
    else() # VCPKG install mode
        # Install rule to ensure that our runtime and linker files are in the vcpkg structure
        install(
            TARGETS CcpMath
            EXPORT CcpMathTargets
            CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES}
            ARCHIVE DESTINATION lib/
            LIBRARY DESTINATION lib/
        )
        # Install rule for available public headers
        install(FILES ${PUBLIC_HEADER_FILES} DESTINATION include)

        install(
            EXPORT CcpMathTargets
            DESTINATION ${CMAKE_INSTALL_PREFIX}/share/carbon-math
            FILE carbon-math.cmake
        )
        install(
            FILES carbon-mathConfig.cmake
            DESTINATION ${CMAKE_INSTALL_PREFIX}/share/carbon-math
        )
    endif()
endif()
