cmake_minimum_required(VERSION 3.0.2)
project(shiftmove_tf)

find_package(catkin REQUIRED COMPONENTS
    nav_msgs
    roscpp
    tf
    message_generation
    roslib
    nav_msgs
    urdf
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES farmingo_bringup
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

include_directories(
# include
 ${catkin_INCLUDE_DIRS}
)


add_executable(shiftmove_tf_node src/shiftmove_tf_node.cpp )
target_link_libraries(shiftmove_tf_node  ${catkin_LIBRARIES} )
install(TARGETS shiftmove_tf_node RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION} )