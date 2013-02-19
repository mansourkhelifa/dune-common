file(GLOB_RECURSE makefiles RELATIVE ${RELPATH} "CMakeLists.txt")
foreach(_file ${makefiles})
  string(REGEX MATCH ".*test" _testdir ${_file})
  if(_testdir)
    list(APPEND _makefiles ${_testdir})
  endif(_testdir)
endforeach(_file ${_makefiles})
message("${_makefiles}")
