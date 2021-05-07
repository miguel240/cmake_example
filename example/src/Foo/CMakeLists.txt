add_subdirectory(tests)
add_subdirectory(dayCountFraction)

create_library(NAME instrument)


add_executable(example instrument.cpp)