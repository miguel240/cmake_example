add_subdirectory(tests)
add_subdirectory(fractions)

create_library(NAME instrument)


add_executable(example instrument.cpp)