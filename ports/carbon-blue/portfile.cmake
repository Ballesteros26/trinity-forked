# 将本地源码快照复制到 buildtrees，避免直接修改仓库中的受控目录  
set(SOURCE_PATH "${CURRENT_BUILDTREES_DIR}/src/carbon-blue-local")  
  
file(COPY  
    "${CMAKE_CURRENT_LIST_DIR}/../../vcpkg-dependency-source/carbon-blue-245d07a431099232cb6d42c497467d2e7daea097/"  
    DESTINATION "${SOURCE_PATH}"  
)  
  
# 应用补丁：修复 NetworkLogger.cpp C2440 隐式 int→PyObject* 转换错误  
vcpkg_apply_patches(  
    SOURCE_PATH "${SOURCE_PATH}"  
    PATCHES fix-networklogger-return.patch  
)  
  
vcpkg_cmake_configure(  
    SOURCE_PATH "${SOURCE_PATH}"  
    OPTIONS  
        -DBUILD_TESTING=OFF  
        -DVCPKG_USE_HOST_TOOLS=ON  
        -DVCPKG_HOST_TRIPLET=${HOST_TRIPLET}  
        -DCMAKE_BUILD_TYPE=${CARBON_BUILD_TYPE}  
)  
  
vcpkg_cmake_install()  
vcpkg_cmake_config_fixup()  
  
set(BUILD_PATHS  
    "${CURRENT_PACKAGES_DIR}/bin/*.pyd"  
    "${CURRENT_PACKAGES_DIR}/debug/bin/*.pyd"  
)  
vcpkg_copy_pdbs(BUILD_PATHS ${BUILD_PATHS})  
ccp_externalize_apple_debuginfo()
