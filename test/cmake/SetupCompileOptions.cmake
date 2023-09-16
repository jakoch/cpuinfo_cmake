# ------------------------------------------------------
# Compile Options - Global
# ------------------------------------------------------

# turn on diagnostics colors
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(/diagnostics:caret /diagnostics:color)
endif()


# ------------------------------------------------------
# Compile Options
# ------------------------------------------------------
# target = options are used when compiling the given <target>
# scope  = the visibility <INTERFACE|PUBLIC|PRIVATE>
#
# Usage: set_compile_options(your_binary PUBLIC)
#
function(set_compile_options TARGET SCOPE)

  if (CMAKE_CXX_COMPILER_ID MATCHES Clang)

    message("${TARGET} : Detected Clang compiler.")

    target_compile_options(${TARGET} ${SCOPE} -Wall -Wextra -Wpedantic -Werror)

  elseif (CMAKE_CXX_COMPILER_ID MATCHES GNU)

	message("${TARGET} : Detected GNU compiler.")

	target_compile_options(${TARGET} ${SCOPE} -Wall -Wextra -Wpedantic -Werror)

  elseif (CMAKE_CXX_COMPILER_ID MATCHES MSVC)

	message("${TARGET} : Detected MSVC compiler.")

	target_compile_definitions(${TARGET} ${SCOPE}
	  NOMINMAX
	  UNICODE
	  _UNICODE
	  _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
      _CRT_SECURE_NO_WARNINGS
	)

	target_compile_options(${TARGET} ${SCOPE}
	  /Zc:__cplusplus
	  /Zc:alignedNew
	  /permissive-
	  /W4
	  /WX
	  /utf-8
	  /MP
	)

  else()
	message(FATAL_ERROR "${TARGET} : Compiler unsupported, aborting.\n")
  endif()

endfunction()
