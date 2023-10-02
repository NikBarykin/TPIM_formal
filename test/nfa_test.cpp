#include <functional>
#include <gtest/gtest.h>
#include <stdexcept>

#include "nfa.h"
#include "utils.h"


TEST(NFAGeneral, Vertices) {
    NFA nfa(0);

    EXPECT_EQ(nfa.countVertices(), 1);
    // start vertex

    EXPECT_EQ(nfa.getStart(), 0);

    nfa.addVertex(57);

    EXPECT_EQ(nfa.countVertices(), 2);

    nfa.addVertex(57);

    EXPECT_EQ(nfa.countVertices(), 2);

    std::set<NFA::Vertex> expected_vertices{ 0, 57 };
    EXPECT_EQ(nfa.getVertices(), expected_vertices);

    nfa.addTransition(0, 57);
    nfa.addVertex(0);

    EXPECT_EQ(nfa.getTransitions(0).size(), 1);

    EXPECT_EQ(nfa.getMaxVertex(), 57);
}


TEST(NFAGeneral, Finishes) {
    NFA nfa(0);
    nfa.addVertex(1);

    nfa.addFinish(1);

    EXPECT_THROW(nfa.addFinish(57), std::out_of_range);

    std::set<NFA::Vertex> expected_finishes{ 1 };
    EXPECT_EQ(nfa.getFinishes(), expected_finishes);

    EXPECT_EQ(nfa.getSingleFinish(), 1);

    EXPECT_TRUE(nfa.isFinish(1));
    EXPECT_FALSE(nfa.isFinish(0));
    EXPECT_THROW(nfa.isFinish(57), std::out_of_range);

    nfa.removeFinish(1);

    EXPECT_THROW(nfa.removeFinish(57), std::out_of_range);
    EXPECT_THROW(nfa.removeFinish(1), std::out_of_range);

    EXPECT_THROW(nfa.getSingleFinish(), std::logic_error);
    // there is no finishes
}


TEST(NFAGeneral, Transitions) {
    NFA nfa(0);
    nfa.addVertex(1);
    nfa.addVertex(2);

    EXPECT_THROW(nfa.addTransition(57, 1), std::out_of_range);
    EXPECT_THROW(nfa.addTransition(1, 57), std::out_of_range);

    nfa.addTransition(0, 1, 'a');
    nfa.addTransition(0, 2, 'b');
    nfa.addTransition(2, 1, NFA::kEmptySymbol);

    EXPECT_THROW(nfa.getTransitions(57), std::out_of_range);

    EXPECT_EQ(nfa.getTransitions(0).at('a').size(), 1);
    EXPECT_EQ(static_cast<const NFA&>(nfa).getTransitions(0).at('a').size(), 1);

    EXPECT_EQ(nfa.getTransitions(0).at('b').size(), 1);

    EXPECT_EQ(nfa.getTransitions(1).size(), 0);
    EXPECT_EQ(nfa.getTransitions(2).size(), 1);

    EXPECT_THROW(nfa.removeTransition(1, 57), std::out_of_range);

    EXPECT_NO_THROW(nfa.removeTransition(0, 1, 'a'));
    EXPECT_THROW(nfa.removeTransition(0, 1, 'a'), std::out_of_range);

    EXPECT_EQ(nfa.getTransitions(0).at('a').size(), 0);
    EXPECT_EQ(nfa.getTransitions(0).at('b').size(), 1);
}

TEST(NFAGeneral, Dump) {
    NFA nfa(0);

    nfa.addVertex(1);
    nfa.addVertex(2);

    nfa.addFinish(1);

    nfa.addTransition(0, 1, 'a');
    nfa.addTransition(0, 2, 'b');
    nfa.addTransition(2, 1);

    std::ostringstream output;

    nfa.dumpTo(output);

    std::string expected_output = R"(0

1

0 1 a
0 2 b
2 1
)";

    EXPECT_EQ(output.str(), expected_output);
}


TEST(NFAGeneral, Read) {
    std::string serialized_nfa = R"(0

0
1

0 1 b
1 1
1 2 c
2 0 a
)";
    std::istringstream input(serialized_nfa);

    NFA parsed_nfa = NFA::ReadFrom(input);

    EXPECT_EQ(parsed_nfa.countVertices(), 3);
    EXPECT_EQ(parsed_nfa.getTransitions(0).size(), 1);
    EXPECT_EQ(parsed_nfa.getTransitions(1).size(), 2);

    ExpectDumps(parsed_nfa, serialized_nfa);
}


TEST(NFAGeneral, Add) {
    std::istringstream nfa1_stream(R"(0

0

0 1 a
)");

    std::istringstream nfa2_stream(R"(0

1

1 0 b
)");

    NFA nfa1 = NFA::ReadFrom(nfa1_stream);
    NFA nfa2 = NFA::ReadFrom(nfa2_stream);

    nfa1.add(nfa2, 2);

    std::string expected_output = R"(0

0
3

0 1 a
3 2 b
)";
    ExpectDumps(nfa1, expected_output);
}
