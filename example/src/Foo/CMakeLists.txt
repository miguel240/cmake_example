add_subdirectory(tests)
add_subdirectory(dayCountFraction)
add_subdirectory(common)

create_library(NAME instrument)
add_executable(example instrument.cpp)
