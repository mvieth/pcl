set(SUBSYS_NAME stereo)
set(SUBSYS_DESC "Point cloud stereo library")
set(SUBSYS_DEPS common io)

PCL_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
PCL_SUBSYS_DEPEND(build NAME ${SUBSYS_NAME} DEPS ${SUBSYS_DEPS})

PCL_ADD_DOC("${SUBSYS_NAME}")

if(NOT build)
  return()
endif()

set(incs
  include/pcl/${SUBSYS_NAME}/stereo_grabber.h
  include/pcl/${SUBSYS_NAME}/stereo_matching.h
  include/pcl/${SUBSYS_NAME}/disparity_map_converter.h
  include/pcl/${SUBSYS_NAME}/digital_elevation_map.h
)

set(impl_incs
  include/pcl/${SUBSYS_NAME}/impl/disparity_map_converter.hpp
)

set(srcs
  src/stereo_grabber.cpp
  src/stereo_matching.cpp
  src/stereo_block_based.cpp
  src/stereo_adaptive_cost_so.cpp
  src/disparity_map_converter.cpp
  src/digital_elevation_map.cpp
)

set(LIB_NAME "pcl_${SUBSYS_NAME}")
PCL_ADD_LIBRARY(${LIB_NAME} COMPONENT ${SUBSYS_NAME} SOURCES ${srcs} ${incs} ${impl_incs})
target_link_libraries("${LIB_NAME}" pcl_common pcl_io)
PCL_MAKE_PKGCONFIG(${LIB_NAME} COMPONENT ${SUBSYS_NAME} DESC ${SUBSYS_DESC} PCL_DEPS ${SUBSYS_DEPS})
# Install include files
PCL_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
PCL_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}/impl" ${impl_incs})
