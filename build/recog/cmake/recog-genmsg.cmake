# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "recog: 1 messages, 0 services")

set(MSG_I_FLAGS "-Irecog:/home/galazxhy/Depth_Recog/src/recog/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(recog_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg" NAME_WE)
add_custom_target(_recog_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "recog" "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(recog
  "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/recog
)

### Generating Services

### Generating Module File
_generate_module_cpp(recog
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/recog
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(recog_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(recog_generate_messages recog_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg" NAME_WE)
add_dependencies(recog_generate_messages_cpp _recog_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(recog_gencpp)
add_dependencies(recog_gencpp recog_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS recog_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(recog
  "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/recog
)

### Generating Services

### Generating Module File
_generate_module_eus(recog
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/recog
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(recog_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(recog_generate_messages recog_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg" NAME_WE)
add_dependencies(recog_generate_messages_eus _recog_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(recog_geneus)
add_dependencies(recog_geneus recog_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS recog_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(recog
  "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/recog
)

### Generating Services

### Generating Module File
_generate_module_lisp(recog
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/recog
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(recog_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(recog_generate_messages recog_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg" NAME_WE)
add_dependencies(recog_generate_messages_lisp _recog_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(recog_genlisp)
add_dependencies(recog_genlisp recog_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS recog_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(recog
  "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/recog
)

### Generating Services

### Generating Module File
_generate_module_nodejs(recog
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/recog
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(recog_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(recog_generate_messages recog_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg" NAME_WE)
add_dependencies(recog_generate_messages_nodejs _recog_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(recog_gennodejs)
add_dependencies(recog_gennodejs recog_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS recog_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(recog
  "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/recog
)

### Generating Services

### Generating Module File
_generate_module_py(recog
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/recog
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(recog_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(recog_generate_messages recog_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/galazxhy/Depth_Recog/src/recog/msg/Position.msg" NAME_WE)
add_dependencies(recog_generate_messages_py _recog_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(recog_genpy)
add_dependencies(recog_genpy recog_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS recog_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/recog)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/recog
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(recog_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/recog)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/recog
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(recog_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/recog)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/recog
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(recog_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/recog)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/recog
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(recog_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/recog)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/recog\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/recog
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(recog_generate_messages_py std_msgs_generate_messages_py)
endif()
