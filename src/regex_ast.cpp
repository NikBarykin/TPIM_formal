#include "regex_ast.h"


namespace Regex {
    namespace AST {
        // !!!
        // important invariant: all built nfa's are SINGLE-ENDED
        
        Symbol::Symbol(NFA::SymbolT symbol): symbol_(symbol) {}

        NFA Symbol::makeNFA() const {
            NFA result(0);
            result.addVertex(1);
            result.addFinish(1);
            result.addTransition(0, 1, symbol_);
            return result;
        }
        
        Multiply::Multiply(Handler left, Handler right)
            : left_(left), right_(right) {}

        NFA Multiply::makeNFA() const {
            NFA left_nfa = left_->makeNFA();
            NFA right_nfa = right_->makeNFA();

            NFA::Vertex left_finish = left_nfa.getSingleFinish();

            size_t shift = left_nfa.getMaxVertex() + 1;
            left_nfa.add(right_nfa, shift);

            left_nfa.removeFinish(left_finish);
            left_nfa.addTransition(left_finish, right_nfa.getStart() + shift);

            return left_nfa;
        }

        Plus::Plus(Handler left, Handler right)
            : left_(left), right_(right) {}

        NFA Plus::makeNFA() const {
            NFA result(0);
            result.addVertex(1);
            result.addFinish(1);

            for (Handler ast_child : { left_, right_ }) {
                NFA child_nfa = ast_child->makeNFA();

                size_t shift = result.getMaxVertex() + 1;
                result.add(child_nfa, shift);

                NFA::Vertex old_start = child_nfa.getStart() + shift;
                NFA::Vertex old_finish = child_nfa.getSingleFinish() + shift;

                result.addTransition(0, old_start);

                result.removeFinish(old_finish);
                result.addTransition(old_finish, 1, NFA::SymbolT());
            }

            return result;
        }


        KleeneStar::KleeneStar(Handler child): child_(child) {}

        NFA KleeneStar::makeNFA() const {
            NFA result(0);
            result.addFinish(0);

            NFA child_nfa = child_->makeNFA();

            size_t shift = 1;
            result.add(child_nfa, shift);

            NFA::Vertex old_start = child_nfa.getStart() + shift;
            NFA::Vertex old_finish = child_nfa.getSingleFinish() + shift;

            result.removeFinish(old_finish);
            result.addTransition(0, old_start);
            result.addTransition(old_finish, 0);

            return result;
        }
    }
}
