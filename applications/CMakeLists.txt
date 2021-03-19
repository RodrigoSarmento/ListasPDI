cmake_minimum_required(VERSION 2.8)

message(STATUS ">>> applications subdirectory")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#find_library(LISTAS_SRC NAMES listas_src)
#find_library(LISTAS_INCLUDE NAMES listas_include)

add_executable(hello hello.cpp)
add_executable(pixels pixels.cpp)
target_link_libraries(hello ${OpenCV_LIBS})
target_link_libraries(pixels ${OpenCV_LIBS})
install(TARGETS hello EXPORT ListasTargets
            RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)

