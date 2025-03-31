add_library(rolling-audio STATIC
	"src/wavereader.h" 
	"src/wavereader.cpp"
)

target_link_libraries(rolling-audio PRIVATE rolling-utils)

add_executable(rolling-audio-test
	"src/audiotest.cpp"
)

target_link_libraries(rolling-audio-test PRIVATE rolling-audio)