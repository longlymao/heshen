add_library(rolling-render STATIC
	"src/renderwindow.h"
	"src/renderwindow.cpp"
	"src/renderapp.h"
	"src/renderapp.cpp"
	"src/scene/camera/camera.h" "src/scene/camera/camera.cpp" 
	"src/scene/primitive/line.h" "src/scene/primitive/line.cpp" 
	"src/scene/world/world2d.h" "src/scene/world/world2d.cpp"
	"src/scene/scene.h" "src/scene/scene.cpp")

target_include_directories(rolling-render PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(rolling-render PRIVATE rolling-math)

add_executable(rolling-render-test WIN32
	"test/rendertest.cpp"
)

target_link_libraries(rolling-render-test PRIVATE rolling-render)
target_link_libraries(rolling-render-test PRIVATE rolling-math)