#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "KeyFinder::keyfinder" for configuration ""
set_property(TARGET KeyFinder::keyfinder APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(KeyFinder::keyfinder PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libkeyfinder.2.2.8.dylib"
  IMPORTED_SONAME_NOCONFIG "@rpath/libkeyfinder.2.dylib"
  )

list(APPEND _cmake_import_check_targets KeyFinder::keyfinder )
list(APPEND _cmake_import_check_files_for_KeyFinder::keyfinder "${_IMPORT_PREFIX}/lib/libkeyfinder.2.2.8.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
