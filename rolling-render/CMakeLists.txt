add_library(rolling-render STATIC
	"src/renderwindow.h"
	"src/renderwindow.cpp"
	"src/renderapp.h"
	"src/renderapp.cpp"
	"src/scene/camera/camera2d.h" "src/scene/camera/camera2d.cpp" 
	"src/scene/world/world2d.h" "src/scene/world/world2d.cpp"
	"src/scene/scene.h" "src/scene/scene.cpp"
	"src/scene/node/node2d.h" "src/scene/node/node2d.cpp"
	"src/scene/node/node3d.h" "src/scene/node/node3d.cpp"
	"src/scene/node/cube.h" "src/scene/node/cube.cpp"
	"src/base/color.h"
	"src/base/ref.h"
	"src/renderer/renderer.h"
	"src/scene/node/line2d.h" "src/scene/node/line2d.cpp"
	"src/renderer/commandlist.h"
	"src/renderer/shader/lineshader.h"
	"src/renderer/shader/shaders.h"
	"src/renderer/shader/shaderdef.h"
	"src/renderer/shader/shaderbuffer.h"
	"src/renderer/renderer.cpp"
	"src/scene/world/world3d.h" "src/scene/world/world3d.cpp"
	"src/scene/camera/camera3d.h" "src/scene/camera/camera3d.cpp"
	"src/app/input/inputmanager.h" "src/app/input/inputmanager.cpp")

target_include_directories(rolling-render PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(rolling-render PRIVATE rolling-math)

add_executable(rolling-render-test WIN32
	"test/rendertest.cpp"
)

target_link_libraries(rolling-render-test PRIVATE rolling-render)
target_link_libraries(rolling-render-test PRIVATE rolling-math)