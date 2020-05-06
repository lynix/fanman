set(GIT_VERSION "unknown")

find_package(Git)

if(DEFINED GIT_FOUND)

    # determine SHA
    exec_program(${GIT_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}"
        ARGS "rev-parse HEAD"
        OUTPUT_VARIABLE GIT_HASH
        RETURN_VALUE RET)
    if(RET EQUAL 0)
        string(SUBSTRING "${GIT_HASH}" 0 8 GIT_VERSION)
    endif()

    # determine tag, if any
    exec_program(${GIT_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}"
        ARGS "tag --points-at HEAD"
        OUTPUT_VARIABLE GIT_TAG
        RETURN_VALUE RET)
    if(RET EQUAL 0)
        string(LENGTH "${GIT_TAG}" LEN_TAG)
        if(LEN_TAG GREATER 0)
            set(GIT_VERSION "${GIT_TAG}")
        endif()
    endif()

    # add '*' as suffix for dirty workspace
    exec_program(${GIT_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}"
        ARGS "status --porcelain"
        OUTPUT_VARIABLE GIT_STATUS
        RETURN_VALUE RET)
    if(RET EQUAL 0)
        string(LENGTH "${GIT_STATUS}" LEN_STATUS)
        if(LEN_STATUS GREATER 0)
            set(GIT_VERSION "${GIT_VERSION}*")
        endif()
    endif()

endif()

message(STATUS "Version: ${GIT_VERSION}")
add_definitions(-DGIT_VERSION="${GIT_VERSION}")

# vim: ts=4 et
