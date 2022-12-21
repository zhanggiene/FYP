cmake_minimum_required(VERSION 3.24)
project(fyp)

set(CMAKE_CXX_STANDARD 17)


# Define build options
option(AUTODIFF_BUILD_TESTS "Enable the compilation of the test files." ON)


if(AUTODIFF_BUILD_TESTS)
    # Tell CMake to include the /src/CMakeLists.txt file in the build.
    add_subdirectory(test)
endif()

set(APP_TARGET main.app)

set(GLFW_H /usr/local/Cellar/glfw/3.3.8/include/GLFW)
include_directories( ${GLFW_H})

set(GLFW_LINK /usr/local/Cellar/glfw/3.3.8/lib/libglfw.3.dylib)
link_libraries( ${GLFW_LINK})

# OPENGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Eigen

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo")
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()


set(IMGUI_DIR ./imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(MKL_LINK "-fopenmp=libomp -lpthread")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenMP REQUIRED)
file(GLOB sources *.cpp)
add_executable(fyp main.cpp ${sources} ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp source/App.cpp include/Point.h source/Canvas.cpp include/Canvas.h include/Curve.h include/VisualPoint.h include/stb_image.h include/stb_image_write.h)
target_link_libraries (fyp Eigen3::Eigen OpenMP::OpenMP_CXX)