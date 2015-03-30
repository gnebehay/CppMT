project (CMT)

cmake_minimum_required (VERSION 2.6)

option(BUILD_SHARED_LIBS "Build shared libraries." ON)
option(BUILD_WRAPPER "Build C and Python wrapper." ON)
option(BUILD_TRAX_CLIENT "Build the trax client." OFF)

find_package(OpenCV REQUIRED)

add_definitions ("-Wall")

add_library(cmtlib common.cpp CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp
    Tracker.cpp fastcluster/fastcluster.cpp)
target_link_libraries(cmtlib ${OpenCV_LIBS})
set_target_properties(cmtlib PROPERTIES OUTPUT_NAME "cmt")

add_executable(cmtgui gui.cpp main.cpp)
target_link_libraries(cmtgui cmtlib ${OpenCV_LIBS})
set_target_properties(cmtgui PROPERTIES OUTPUT_NAME "cmt")

if(BUILD_WRAPPER)
    add_library(cmtclib cwrapper.cpp)
    target_link_libraries(cmtclib cmtlib ${OpenCV_LIBS})
    set_target_properties(cmtclib PROPERTIES OUTPUT_NAME "ccmt")

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pycmt.py
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
endif()

if(BUILD_TRAX_CLIENT)
    set(TRAX_DIR "" CACHE FILEPATH "Path to trax")

    include_directories(${TRAX_DIR}/include)

    add_executable (trax_client cmtgui common.cpp gui.cpp trax.cpp
        CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
        fastcluster/fastcluster.cpp)

    find_library(TRAX_LIBRARY NAMES trax HINTS "${TRAX_DIR}/lib")

    target_link_libraries(trax_client ${OpenCV_LIBS} ${TRAX_LIBRARY})

endif()
