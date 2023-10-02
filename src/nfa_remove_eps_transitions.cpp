#include "nfa.h"


namespace {
    void RemoveEpsTransisionsImpl(
            NFA::Vertex source,
            std::set<NFA::Vertex>& visited,
            NFA& nfa) {

        visited.insert(source);

        const NFA::VertexTransitionsT& source_transitions = nfa.getTransitions(source);
        auto eps_transitions_it = source_transitions.find(NFA::kEmptySymbol);
        if (eps_transitions_it == source_transitions.end()) {
            return;
        }

        for (NFA::Vertex dest : *eps_transitions_it) {
            if (!visited.count(dest)) {
                RemoveEpsTransisionsImpl(dest, visited, nfa);
                if (nfa.isFinish(dest)) {
                    nfa.addFinish(source);
                }
            }
            nfa.removeTransition(source, dest, NFA::kEmptySymbol);
            for (const auto& [symbol, dest] : nfa.getTransitions(dest)) {
                if (symbol != NFA::kEmptySymbol) {
                    nfa.addTransition(source, dest, symbol);
                }
            }
        }
        return;
    }
}

NFA& NFA::removeEpsTransitions() {
    std::set<Vertex> visited;
    for (Vertex vertex : getVertices()) {
        if (!visited.count(vertex)) {
            RemoveEpsTransisionsImpl(vertex, visited, *this);
        }
    }
    return *this;
}
