add_library(rolling-utils STATIC
	"src/filehelper.h"
	"src/filehelper.cpp"
)

target_include_directories(rolling-utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")