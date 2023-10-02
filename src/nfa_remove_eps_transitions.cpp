#include "nfa.h"


namespace {
    void CompressEpsTransitionsAndFinishes(
            NFA::Vertex current,
            NFA::Vertex target,
            std::set<NFA::Vertex>& visited,
            NFA& nfa) {
        visited.insert(current);

        for (NFA::Vertex next : nfa.getTransitions(current)[NFA::kEmptySymbol]) {
            if (!visited.count(next)) {
                CompressEpsTransitionsAndFinishes(next, target, visited, nfa);
            }
        }

        if (nfa.isFinish(current)) {
            nfa.addFinish(target);
        }

        for (const auto& [symbol, destinations] : nfa.getTransitions(current)) {
            if (symbol == NFA::kEmptySymbol) {
                continue;
            }
            for (NFA::Vertex dest : destinations) {
                nfa.addTransition(target, dest, symbol);
            }
        }


    }
}

NFA& NFA::removeEpsTransitions() {
    for (Vertex vertex : getVertices()) {
        std::set<Vertex> visited;
        CompressEpsTransitionsAndFinishes(vertex, vertex, visited, *this);
    }
    // remove all eps-transitions
    for (Vertex vertex : getVertices()) {
        getTransitions(vertex).erase(kEmptySymbol);
    }
    return *this;
}
