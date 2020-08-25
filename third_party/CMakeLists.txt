cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# --------------------------------------------------------------------

FetchContent_Declare(
        wheels
        GIT_REPOSITORY https://gitlab.com/Lipovsky/wheels.git
        GIT_TAG master
)

set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(wheels)

# --------------------------------------------------------------------
