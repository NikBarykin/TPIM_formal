#include <functional>
#include <gtest/gtest.h>
#include <stdexcept>

#include "nfa.h"
#include "utils.h"


TEST(NFARemoveEpsTransitions, Simple) {
    std::istringstream input(R"(0

2

0 1
0 1 b
1 0
1 2
2 1 a
)");
    NFA nfa = NFA::ReadFrom(input);
    nfa.removeEpsTransitions();

    std::string expected_output = R"(0

0
1
2

0 1 a
0 1 b
1 1 a
1 1 b
2 1 a
)";

    ExpectDumps(nfa, expected_output);
}


TEST(NFARemoveEpsTransitions, Complex) {
    std::istringstream input(R"(0

4

0 1
1 3
1 6
1 2 a
3 1
3 4
3 2 a
4 5 a
6 7 a
)");

    NFA nfa = NFA::ReadFrom(input);
    nfa.removeEpsTransitions();

    std::string expected_output = R"(0

0
1
3
4

0 2 a
0 5 a
0 7 a
1 2 a
1 5 a
1 7 a
3 2 a
3 5 a
3 7 a
4 5 a
6 7 a
)";

    ExpectDumps(nfa, expected_output);
}
