# =============================================================================
# A CMake library script for the `set_compiler_flags' function
# =============================================================================

# ============================================================================
#
# 2025-09-21 Ljubomir Kurij <ljubomir_kurij@proton.me>
#
# * Created.
#
# ============================================================================

# -----------------------------------------------------------------------------
# Function to set compiler flags based on compiler and build type
# -----------------------------------------------------------------------------
function (set_compiler_flags)
  # Check which compiler is being used and set flags accordingly
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message (STATUS "Using Clang compiler ...")
    # -------------------------------------------------------------------------
    # Clang-specific flags
    # -------------------------------------------------------------------------
    set (DEBUG_FLAGS
      # "-g3 -O0 -fno-omit-frame-pointer -Wall -Wextra -Wpedantic -Werror -lstdc++"
      "-g3 -O0 -fno-omit-frame-pointer -Wall -Wextra -Wpedantic -lstdc++"
      )
    set (RELEASE_FLAGS
      "-O3 -DNDEBUG -march=native -fvectorize -flto -lstdc++"
      )
    set (MINSIZEREL_FLAGS
      "-Os -DNDEBUG -ffunction-sections -fdata-sections -flto -lstdc++"
      )
    set (RELWITHDEBINFO_FLAGS
      "-O2 -g -DNDEBUG -march=native -fvectorize -lstdc++"
      )
    set (MINSIZEREL_LINKER_FLAGS
      "-Wl,--gc-sections -Wl,--strip-all"
      )
      
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message (STATUS "Using GNU compiler ...")
    # -------------------------------------------------------------------------
    # GNU-specific flags
    # -------------------------------------------------------------------------
    set (DEBUG_FLAGS
      # "-g3 -O0 -fno-omit-frame-pointer -Wall -Wextra -Wpedantic -Werror"
      "-g3 -O0 -fno-omit-frame-pointer -Wall -Wextra -Wpedantic -lstdc++"
      )
    set (RELEASE_FLAGS
      "-O3 -DNDEBUG -march=native -ftree-vectorize -flto -lstdc++"
      )
    set (MINSIZEREL_FLAGS
      "-Os -DNDEBUG -ffunction-sections -fdata-sections -flto -lstdc++"
      )
    set (RELWITHDEBINFO_FLAGS
      "-O2 -g -DNDEBUG -march=native -ftree-vectorize -lstdc++"
      )
    set (MINSIZEREL_LINKER_FLAGS
      "-Wl,--gc-sections -Wl,--strip-all"
      )
      
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message (STATUS "Using MSVC compiler ...")
    # -------------------------------------------------------------------------
    # MSVC-specific flags
    # -------------------------------------------------------------------------
    set (DEBUG_FLAGS
      # "/Zi /Od /Ob0 /MDd /W4 /WX /permissive- /RTC1"
      "/Zi /Od /Ob0 /MDd /W4 /permissive- /RTC1"
      )
    set (RELEASE_FLAGS
      "/O2 /MD /DNDEBUG /GL /fp:fast"
      )
    set (MINSIZEREL_FLAGS
      "/O1 /MD /DNDEBUG /GL"
      )
    set (RELWITHDEBINFO_FLAGS
      "/O2 /Zi /MD /DNDEBUG"
      )
    set (MINSIZEREL_LINKER_FLAGS
      "/OPT:REF /OPT:ICF /LTCG"
      )
      
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel"
    OR CMAKE_CXX_COMPILER_ID MATCHES "IntelLLVM"
    )
    message (STATUS "Using Intel compiler ...")
    # -------------------------------------------------------------------------
    # Intel-specific flags
    # -------------------------------------------------------------------------
    set(DEBUG_FLAGS
      # "-g3 -O0 -Wall -Wextra -Werror -traceback -lstdc++"
      "-g3 -O0 -Wall -Wextra -traceback -lstdc++"
      )
    set(RELEASE_FLAGS
      "-O3 -DNDEBUG -xHost -ipo -lstdc++"
      )
    set(MINSIZEREL_FLAGS
      "-Os -DNDEBUG -ipo -lstdc++"
      )
    set(RELWITHDEBINFO_FLAGS
      "-O2 -g -DNDEBUG -xHost -ipo -lstdc++")
    set(MINSIZEREL_LINKER_FLAGS
      "-Wl,--gc-sections -Wl,--strip-all"
      )
  endif()
  
  # Apply the flags to the appropriate CMake variables with PARENT_SCOPE
  # to make them available in the parent scope
  set(CMAKE_CXX_FLAGS_DEBUG "${DEBUG_FLAGS}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS_RELEASE "${RELEASE_FLAGS}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${MINSIZEREL_FLAGS}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${RELWITHDEBINFO_FLAGS}" PARENT_SCOPE)
  
  if(DEFINED MINSIZEREL_LINKER_FLAGS)
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${MINSIZEREL_LINKER_FLAGS}" PARENT_SCOPE)
  endif()
endfunction()

# End of `set_compiler_flags.cmake'
