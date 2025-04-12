add_compile_options(
    -std=c++23
    -Werror
    -Wall
    -Wextra
    -fPIC
    -fno-gnu-unique
    -g
    -O3
    -Wno-restrict
)
#enable ccache if found
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message( "-- ccache was found, enabling ccache for current projet"  )
    set(CMAKE_C_COMPILER_LAUNCHER ccache)
    set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif(CCACHE_FOUND)
