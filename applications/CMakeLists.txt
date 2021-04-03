cmake_minimum_required(VERSION 2.8)

message(STATUS ">>> applications subdirectory")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

find_library(LISTAS_UTILS NAMES listas_utils)

add_executable(hello hello.cpp)
add_executable(pixels pixels.cpp)
add_executable(2_2regions 2_2regions.cpp)
add_executable(2_2trocaregioes 2_2trocaregioes.cpp)
add_executable(3_floodfill 3_floodfill.cpp)
add_executable(4_histogram 4_histogram.cpp)
add_executable(4_motionDetector 4_motionDetector.cpp)
add_executable(5_filtroEspacial 5_filtroEspacial.cpp)
add_executable(6_tilt 6_tilt.cpp)
add_executable(6_tilt_video 6_tilt_video.cpp)
add_executable(7_filter_frequency 7_filter_frequency.cpp)
add_executable(8_canny 8_canny.cpp)
add_executable(8_points 8_points.cpp)

target_link_libraries(hello ${OpenCV_LIBS})
target_link_libraries(pixels ${OpenCV_LIBS})
target_link_libraries(2_2regions ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(2_2trocaregioes ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(3_floodfill ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(4_histogram ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(4_motionDetector ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(5_filtroEspacial ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(6_tilt ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(6_tilt_video ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(7_filter_frequency ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(8_canny ${OpenCV_LIBS} ${LISTAS_UTILS})
target_link_libraries(8_points ${OpenCV_LIBS} ${LISTAS_UTILS})

install(TARGETS hello pixels 2_2regions 2_2trocaregioes 3_floodfill 4_histogram 4_motionDetector
    5_filtroEspacial 6_tilt 6_tilt_video 7_filter_frequency 8_canny 8_points EXPORT ListasTargets
            RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)

