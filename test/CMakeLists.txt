set(TARGET_NAME "automata")
set(COVERAGE_NAME "automata-coverage")
set(COVERAGE_TARGET "automata-coverage")
set(SOURCE_DIR_MAIN "${automata-sources_SOURCE_DIR}")
set(SOURCES ${automata-source-files})

message(INFO ${SOURCES})

set(TEST_SOURCES
    nfa_test.cpp
    regex_ast_test.cpp
    nfa_remove_eps_transitions_test.cpp
    )

run_coverage(
        "${TEST_SOURCES}"
        "${SOURCES}"
        "${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)

