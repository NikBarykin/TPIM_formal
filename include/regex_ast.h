#include "nfa.h"


namespace Regex {
    namespace AST {
        struct Base {
            virtual NFA makeNFA() const = 0;
            virtual ~Base() = default;
        };

        using Handler = std::shared_ptr<Base>;

        struct Symbol : public Base {
            // 'a' or 1
            NFA::SymbolT symbol_;

            Symbol(NFA::SymbolT symbol = NFA::kEmptySymbol);

            NFA makeNFA() const override;
        };

        struct Multiply : public Base {
            // (left)(right)
            // i.e. concat
            Handler left_;
            Handler right_;

            Multiply(Handler left, Handler right);

            NFA makeNFA() const override;
        };

        struct Plus : public Base {
            // left + right
            Handler left_;
            Handler right_;

            Plus(Handler left, Handler right);

            NFA makeNFA() const override;
        };

        struct KleeneStar : public Base {
            // child^*
            Handler child_;

            KleeneStar(Handler child);

            NFA makeNFA() const override;
        };
    };
}
