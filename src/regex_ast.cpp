#include "regex_ast.h"


namespace Regex {
    namespace AST {
        // !!!
        // important invariant: all built nfa's are SINGLE-ENDED

        NFA Symbol::makeNFA() const {
            NFA result;
            NFA::VertexHandler node = result.addVertex();
            result.addFinish(node);
            result.addTransition(result.getStart(), node, symbol_);
            return result;
        }

        NFA Multiply::makeNFA() const {
            NFA left_nfa = left_->makeNFA();
            NFA right_nfa = right_->makeNFA();

            NFA::VertexHandler left_finish = left_nfa.getSingleFinish();
            NFA::VertexHandler right_start = right_nfa.getStart();

            left_nfa.merge(std::move(right_nfa));

            left_nfa.removeFinish(left_finish);
            left_nfa.addTransition(left_finish, right_start, NFA::SymbolT());

            return left_nfa;
        }

        NFA Plus::makeNFA() const {
            NFA result;

            NFA::VertexHandler new_finish = result.addVertex();
            result.addFinish(new_finish);

            for (Handler ast_child : { left_, right_ }) {
                NFA child_nfa = ast_child->makeNFA();

                NFA::VertexHandler child_start = child_nfa.getStart();
                NFA::VertexHandler child_finish = child_nfa.getSingleFinish();

                result.merge(std::move(child_nfa));

                result.addTransition(result.getStart(), child_start, NFA::SymbolT());

                result.removeFinish(child_finish);
                result.addTransition(child_finish, new_finish, NFA::SymbolT());
            }

            return result;
        }

        NFA KleeneStar::makeNFA() const {
            NFA result;
            NFA child_nfa = child_->makeNFA();

            NFA::VertexHandler child_start = child_nfa.getStart();
            NFA::VertexHandler child_finish = child_nfa.getSingleFinish();

            result.merge(std::move(child_nfa));

            result.removeFinish(child_finish);
            result.addFinish(result.getStart());

            result.addTransition(result.getStart(), child_start, NFA::SymbolT());
            result.addTransition(child_finish, result.getStart(), NFA::SymbolT());

            return result;
        }
    }
}
