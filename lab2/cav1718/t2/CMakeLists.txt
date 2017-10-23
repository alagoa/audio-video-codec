#FILE(GLOB SRCFILES *.cxx)

add_executable( wavCopy wavCopy.c)
target_link_libraries(wavCopy sndfile)
