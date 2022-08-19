cmake_minimum_required(VERSION 3.5)
project(web_video_server)

find_package(ament_cmake_ros REQUIRED)

find_package(async_web_server_cpp REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(sensor_msgs REQUIRED)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)

find_package(PkgConfig REQUIRED)
pkg_check_modules(avcodec libavcodec REQUIRED)
pkg_check_modules(avformat libavformat REQUIRED)
pkg_check_modules(avutil libavutil REQUIRED)
pkg_check_modules(swscale libswscale REQUIRED)
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

###################################################
## Declare things to be passed to other projects ##
###################################################

###########
## Build ##
###########

## Specify additional locations of header files
include_directories(include
  ${Boost_INCLUDE_DIRS}
  ${avcodec_INCLUDE_DIRS}
  ${avformat_INCLUDE_DIRS}
  ${avutil_INCLUDE_DIRS}
  ${swscale_INCLUDE_DIRS}
)
add_compile_options(-Wno-deprecated-declarations)

## Declare a cpp library
add_library(${PROJECT_NAME}
  src/web_video_server.cpp
  src/image_streamer.cpp
  src/libav_streamer.cpp
  src/vp8_streamer.cpp
  src/h264_streamer.cpp
  src/vp9_streamer.cpp
  src/multipart_stream.cpp
  src/ros_compressed_streamer.cpp
  src/jpeg_streamers.cpp
  src/png_streamers.cpp
)
rclcpp_components_register_nodes(${PROJECT_NAME} "${PROJECT_NAME}::ComposableWebVideoServer")

ament_target_dependencies(${PROJECT_NAME}
  async_web_server_cpp cv_bridge image_transport rclcpp rclcpp_components sensor_msgs)

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
  ${avcodec_LIBRARIES}
  ${avformat_LIBRARIES}
  ${avutil_LIBRARIES}
  ${swscale_LIBRARIES}
)

## Declare a cpp executable
add_executable(${PROJECT_NAME}_node
  src/web_video_server_node.cpp
)
target_link_libraries(${PROJECT_NAME}_node ${PROJECT_NAME})

#############
## Install ##
#############

## Mark executables and/or libraries for installation
install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(TARGETS ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/${PROJECT_NAME}/
   DESTINATION include/${PROJECT_NAME}
   FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(async_web_server_cpp cv_bridge image_transport rclcpp rclcpp_components sensor_msgs)
ament_package()
