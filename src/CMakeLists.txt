project(AutomataSources)

set(automata-source-files
    "${PROJECT_SOURCE_DIR}/nfa.cpp"
    "${PROJECT_SOURCE_DIR}/regex_ast.cpp"
    "${PROJECT_SOURCE_DIR}/nfa_remove_eps_transitions.cpp"
    CACHE INTERNAL "")

# add_library(
#     gtomata-sources SHARED ${general-source-files} ${Automata_SOURCE_DIR}/include/nfa.h
# )
