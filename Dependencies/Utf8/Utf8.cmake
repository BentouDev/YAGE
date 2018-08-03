# Utf8
yage_include(YAGE Dependencies/Utf8/Source/source)
add_library(utf8 STATIC Dependencies/Utf8/Source/source/utf8.h Dependencies/Utf8/Source/source/utf8/checked.h Dependencies/Utf8/Source/source/utf8/core.h Dependencies/Utf8/Source/source/utf8/unchecked.h)
set_target_properties(utf8 PROPERTIES LINKER_LANGUAGE CXX)
