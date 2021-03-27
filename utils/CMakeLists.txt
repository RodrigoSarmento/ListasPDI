cmake_minimum_required(VERSION 2.6)

message(STATUS ">>> UTILS subdirectory")

set(LISTAS_UTILS_SRC config_loader.cpp)
set(LISTAS_UTILS_INCLUDES config_loader.h)

add_library(listas_utils SHARED ${LISTAS_UTILS_SRC})
target_link_libraries(listas_utils ${OpenCV_LIBS})

set_target_properties(listas_utils PROPERTIES OUTPUT_NAME "listas_utils")
set_target_properties(listas_utils PROPERTIES VERSION 1.0)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

install(TARGETS listas_utils
          EXPORT listasTargets
  		  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  		  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  		  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/listas_utils"
    	  COMPONENT dev)

install(FILES ${LISTAS_UTILS_INCLUDES} DESTINATION include)