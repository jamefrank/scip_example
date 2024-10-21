project(y)

# set(YAML_CPP_BUILD_TESTS OFF)
# set(YAML_CPP_BUILD_TOOLS OFF)
# set(YAML_CPP_BUILD_CONTRIB OFF)
# add_subdirectory(libs/yaml-cpp)
# add_executable(y src/main.cpp)
# target_include_directories(y PUBLIC libs/yaml-cpp/include)
# target_link_libraries(y yaml-cpp)


find_package(yaml-cpp REQUIRED)
add_executable(y main.cpp)
target_include_directories(y PUBLIC ${YAML_CPP_INCLUDE_DIRS})
target_link_libraries(y ${YAML_CPP_LIBRARIES})