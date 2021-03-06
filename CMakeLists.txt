cmake_minimum_required (VERSION 2.8.0)
project (datadogcpp)

set(VERSION "1.0.1")

set (LIB_NAME "${PROJECT_NAME}")
set (PKG_NAME "${PROJECT_NAME}")

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(include)

set(libsrc src/datadog-cpp.cpp src/datadog-c.cpp)

## we could do the following to compile just once, but I don't want
# the static compiled with -PIC, so build twice.
# add_library(objlib OBJECT ${libsrc})
#
# add_library (datadogcpp-static STATIC $<TARGET_OBJECTS:objlib>)
# add_library (datadogcpp-shared SHARED $<TARGET_OBJECTS:objlib>)

#add_library (datadogcpp-static STATIC ${libsrc})
add_library (datadogcpp-shared SHARED ${libsrc})

install(TARGETS datadogcpp-shared DESTINATION lib)
install(FILES include/datadog-c.h include/datadog-cpp.hpp DESTINATION include/datadog-cpp/)
install(FILES include/datadog-c.h include/datadog-cpp.hpp DESTINATION include/datadog-cpp/)

## RPM packaging
# CPACK
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR  "RPM")
# set(CPACK_GENERATOR  "DEB")
set(CPACK_PACKAGE_NAME ${PKG_NAME})
set(CPACK_PACKAGE_SUMMARY ${SUMMARY})
set(CPACK_PACKAGE_LICENSE ${LICENSE})
set(CPACK_PACKAGE_RELEASE 0)
set(CPACK_PACKAGE_APP_GROUP ${RPM_APP_GROUP})
set(CPACK_PACKAGE_CONTACT ${AUTHOR})
set(CPACK_PACKAGE_VENDOR ${VENDOR})
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_RPM_PACKAGE_REQUIRES "cmake")

if(NOT DEFINED CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST)
    set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST /usr /usr/local  /usr/local/lib /usr/local/include)
    if(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION)
      if(CPACK_RPM_PACKAGE_DEBUG)
        message("CPackRPM:Debug: Adding ${CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION} to builtin omit list.")
      endif()
      list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST "${CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION}")
    endif()
endif()


include(CPack)

# source tar ball
add_custom_command(
        OUTPUT ${PKG_NAME}-${CPACK_PACKAGE_VERSION}.tar.gz
        COMMAND git archive --format=tar --prefix=${PKG_NAME}-${CPACK_PACKAGE_VERSION}/ HEAD | gzip -c > ${CMAKE_CURRENT_BINARY_DIR}/${PKG_NAME}-${CPACK_PACKAGE_VERSION}.tar.gz
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(
        src_tgz
        DEPENDS ${PKG_NAME}-${CPACK_PACKAGE_VERSION}.tar.gz
)

# CPack does not have a rule for source rpm
# the following is a way to do that
set(RPM_BUILD_DIR rpm_build)
# rpm spec file
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/rpm.spec.in
        ${CMAKE_CURRENT_BINARY_DIR}/${RPM_BUILD_DIR}/SPECS/${PKG_NAME}.spec )
# rules to build src rpm
set(SRC_RPM ${PKG_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.src.rpm)
add_custom_command(
        OUTPUT ${SRC_RPM}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RPM_BUILD_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RPM_BUILD_DIR}/BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RPM_BUILD_DIR}/BUILDROOT
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RPM_BUILD_DIR}/RPMS
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RPM_BUILD_DIR}/SOURCES
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RPM_BUILD_DIR}/SPECS
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RPM_BUILD_DIR}/SRPMS
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RPM_BUILD_DIR}/tmp
        COMMAND ${CMAKE_COMMAND} -E copy ${PKG_NAME}-${CPACK_PACKAGE_VERSION}.tar.gz ${RPM_BUILD_DIR}/SOURCES
        COMMAND rpmbuild -bs --define=\"_topdir ${RPM_BUILD_DIR}\" --buildroot=${RPM_BUILD_DIR}/tmp  ${RPM_BUILD_DIR}/SPECS/${PKG_NAME}.spec
        COMMAND ${CMAKE_COMMAND} -E copy ${RPM_BUILD_DIR}/SRPMS/${SRC_RPM} ${SRC_RPM}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS src_tgz
)
add_custom_target(
        srpm
        DEPENDS ${SRC_RPM}
)
