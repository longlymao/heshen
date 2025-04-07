add_library(rolling-render STATIC
	"src/texture.h"
	"src/texture.cpp"
	"src/line.h"
	"src/line.cpp"
	"src/renderwindow.h"
	"src/renderwindow.cpp"
	"src/renderapp.h"
	"src/renderapp.cpp"
)

target_include_directories(rolling-render PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(rolling-render-test WIN32
	"test/rendertest.cpp"
)

target_link_libraries(rolling-render-test PRIVATE rolling-render)