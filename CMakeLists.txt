cmake_minimum_required(VERSION 3.8)
project(Automata CXX)

include(ExternalProject)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake)

set(EXTERNAL_INSTALL_LOCATION ${Automata_SOURCE_DIR}/3rdparty)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Automata_SOURCE_DIR}/bin)

SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
if (APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

SET(CPR_USE_SYSTEM_GTEST ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)

if (APPLE)

	set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@1.1" CACHE INTERNAL "")
	set(OPENSSL_LIBRARIES "/usr/local/opt/openssl@1.1/lib" CACHE INTERNAL "")
    FetchContent_Declare(
		cpr_repo
		GIT_REPOSITORY https://github.com/libcpr/cpr.git
	)
else()
    FetchContent_Declare(
		cpr_repo
		GIT_REPOSITORY https://github.com/libcpr/cpr.git
	)
endif()

FetchContent_MakeAvailable(cpr_repo googletest)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
include_directories(${CPR_INCLUDE_DIRS})



include(CodeCoverage)
add_custom_target(coverage_report)

function(run_coverage test_sources sources coverage_target target_name coverage_name)
    message("INFO test sources ${test_sources}")
    set(test_name "${target_name}-test")
    message("INFO sources ${sources}")
    add_executable(${test_name} ${test_sources} ${sources})


    target_link_libraries(
		${test_name} gtest_main gtest Threads::Threads cpr curl
	)

    setup_target_for_coverage(
            ${coverage_target} ${target_name} ${test_name} ${coverage_name}
    )

    add_dependencies(coverage_report ${coverage_target})
endfunction()

# set(sources include/nfa.h src/nfa.cpp)
# set(general-source-files src/general/nfa.cpp)

# add_executable(test_nfa nfa_test.cpp)
# target_link_libraries(test_nfa ${GTEST_LIBRARIES} Threads::Threads)

include_directories(include)

# set(TARGET_NAME "general")
# set(COVERAGE_NAME "general-coverage")
# set(COVERAGE_TARGET "general-coverage")
# set(SOURCE_DIR_MAIN "${general_SOURCE_DIR}")
# set(SOURCES ${general-source-files})

# message(INFO ${SOURCES})

# set(TEST_SOURCES tests/general/nfa_test.cpp)
# run_coverage(
#         "${TEST_SOURCES}"
#         "${SOURCES}"
#         "${COVERAGE_TARGET}"
#         "${TARGET_NAME}"
#         "${COVERAGE_NAME}"
# )
add_subdirectory(src)
add_subdirectory(test)


# set(TARGET_NAME "general")
# set(COVERAGE_NAME "general-coverage")
# set(COVERAGE_TARGET "general-coverage")
# set(SOURCE_DIR_MAIN "${Automata_SOURCE_DIR}")

# message(INFO "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa")
# message(INFO ${SOURCES})

# set(TEST_SOURCES tests/general/nfa_test.cpp)

# run_coverage(
#         "${TEST_SOURCES}"
#         "${sources}"
#         "${COVERAGE_TARGET}"
#         "${TARGET_NAME}"
#         "${COVERAGE_NAME}"
# )
