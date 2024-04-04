cmake_minimum_required(VERSION 3.11)

project(c-bal)

include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/include/CMakeLists.txt)

add_subdirectory(${CMAKE_SOURCE_DIR}/ext/kissfft-131.1.0)

add_executable(${PROJECT_NAME} ${C_BAL_SRCS} ${C_BAL_HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC kissfft)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/ext/kissfft-131.1.0)