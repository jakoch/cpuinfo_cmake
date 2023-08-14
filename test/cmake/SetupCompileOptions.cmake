# ------------------------------------------------------
# Compile Options
# ------------------------------------------------------
function(set_compile_options TARGET EXPOSURE)

  if (CMAKE_CXX_COMPILER_ID MATCHES Clang)

    message("${TARGET} : Detected Clang compiler.")

    target_compile_options(${TARGET} ${EXPOSURE} -Wall -Wextra -Wpedantic -Werror)

  elseif (CMAKE_CXX_COMPILER_ID MATCHES GNU)

	message("${TARGET} : Detected GNU compiler.")

	target_compile_options(${TARGET} ${EXPOSURE} -Wall -Wextra -Wpedantic -Werror)

  elseif (CMAKE_CXX_COMPILER_ID MATCHES MSVC)

	message("${TARGET} : Detected MSVC compiler.")

	target_compile_definitions(${TARGET} ${EXPOSURE}
	  NOMINMAX
	  UNICODE
	  _UNICODE
	  _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
      _CRT_SECURE_NO_WARNINGS
	)

	target_compile_options(${TARGET} ${EXPOSURE}
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

  # turn on diagnostics colors
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/diagnostics:caret /diagnostics:color)
  endif()

endfunction()