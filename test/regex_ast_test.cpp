#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "nfa.h"
#include "regex_ast.h"
#include "utils.h"


namespace {
    Regex::AST::Handler MakeSymbol(NFA::SymbolT symbol) {
        return std::make_shared<Regex::AST::Symbol>(symbol);
    }
}


TEST(RegexAST, Symbol) {
    auto test_symbol_node = [](NFA::SymbolT symbol) {
        Regex::AST::Symbol node(symbol);
        NFA nfa = node.makeNFA();

        std::string expected_output = R"(0

1

0 1)";
        if (symbol != NFA::kEmptySymbol) {
            expected_output.push_back(' ');
            expected_output.push_back(*symbol);
        }
        expected_output.push_back('\n');

        ExpectDumps(nfa, expected_output);
    };

    test_symbol_node('a');
    test_symbol_node(NFA::kEmptySymbol);
}


TEST(RegexAST, Multiply) {
    Regex::AST::Multiply node(MakeSymbol('a'), MakeSymbol('b'));

    NFA nfa = node.makeNFA();

    std::string expected_output = R"(0

3

0 1 a
1 2
2 3 b
)";
    ExpectDumps(nfa, expected_output);
}


TEST(RegexAST, Plus) {
    Regex::AST::Plus node(MakeSymbol('a'), MakeSymbol('b'));

    NFA nfa = node.makeNFA();

    std::string expected_output = R"(0

1

0 2
0 4
2 3 a
3 1
4 5 b
5 1
)";
    ExpectDumps(nfa, expected_output);
}

TEST(RegexAST, KleeneStar) {
    Regex::AST::KleeneStar node(MakeSymbol('a'));

    NFA nfa = node.makeNFA();

    std::string expected_output = R"(0

0

0 1
1 2 a
2 0
)";
    ExpectDumps(nfa, expected_output);
}


TEST(RegexAST, ComplexScenario) {
    auto a  = MakeSymbol('a');
    auto b  = MakeSymbol('b');
    auto c  = MakeSymbol('c');
    auto b_plus_c = std::make_shared<Regex::AST::Plus>(b, c);
    auto b_plus_c_kleene = std::make_shared<Regex::AST::KleeneStar>(b_plus_c);
    auto a_multiply_b_plus_c_kleene = std::make_shared<Regex::AST::Multiply>(a, b_plus_c_kleene);

    NFA nfa = a_multiply_b_plus_c_kleene->makeNFA();

    std::string expected_output = R"(0

2

0 1 a
1 2
2 3
3 5
3 7
4 2
5 6 b
6 4
7 8 c
8 4
)";
    ExpectDumps(nfa, expected_output);
}
