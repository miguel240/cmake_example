file(REMOVE_RECURSE
  "../../../build/libfoo.pdb"
  "../../../build/libfoo.so"
  "CMakeFiles/foo.dir/foo.cpp.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/foo.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
