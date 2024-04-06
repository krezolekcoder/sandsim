cmake_minimum_required(VERSION 3.0)
project(SandSim)
set(CMAKE_CXX_STANDARD 11)
# Find raylib package
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(raylib REQUIRED)

# Add your main executable
add_executable(SandSim main.cpp)

# Link raylib and other required libraries
target_link_libraries(SandSim PRIVATE raylib)
# target_link_libraries(SandSim PRIVATE GL)
# target_link_libraries(SandSim PRIVATE m)
# target_link_libraries(SandSim PRIVATE pthread)
# target_link_libraries(SandSim PRIVATE dl)
# target_link_libraries(SandSim PRIVATE rt)
# target_link_libraries(SandSim PRIVATE X11)

