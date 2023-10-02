#include <string>
#include <stdexcept>
#include <ostream>
#include <memory>
#include <istream>
#include <iostream>
#include <sstream>

#include "nfa.h"


NFA::NFA(Vertex start) {
    addVertex(start);
}

void NFA::addVertex(Vertex vertex) {
    transitions_.emplace(vertex, VertexTransitionsT());
}

NFA::Vertex NFA::getStart() const {
    return start_;
}

NFA::Vertex NFA::getMaxVertex() const {
    return transitions_.rbegin()->first;
}

std::set<NFA::Vertex> NFA::getVertices() const {
    std::set<Vertex> result;
    for (const auto& [vertex, _] : transitions_) {
        result.insert(vertex);
    }
    return result;
}

bool NFA::isFinish(Vertex vertex) const {
    transitions_.at(vertex);
    return getFinishes().count(vertex);
}

void NFA::addFinish(Vertex new_finish) {
    // check that 'new_finish' exists
    transitions_.at(new_finish);
    finishes_.insert(new_finish); }

void NFA::removeFinish(Vertex vertex) {
    if (!finishes_.erase(vertex)) {
        throw std::out_of_range("Vertex " + std::to_string(vertex) + " is not finish");
    }
}

const std::set<NFA::Vertex>& NFA::getFinishes() const {
    return finishes_;
}

NFA::Vertex NFA::getSingleFinish() const {
    if (finishes_.size() != 1) {
        throw std::logic_error(
                "Number of finishes is " + std::to_string(finishes_.size()) + " != 1");
    }
    return *finishes_.begin();
}

void NFA::addTransition(Vertex source, Vertex dest, SymbolT symbol) {
    transitions_.at(dest);
    transitions_.at(source)[symbol].insert(dest);
}

void NFA::removeTransition(Vertex source, Vertex dest, SymbolT symbol) {
    if (!transitions_.at(source).at(symbol).erase(dest)) {
        throw std::out_of_range(
                "Trying to remove transition that doesn't exist");
    }
}

const NFA::VertexTransitionsT& NFA::getTransitions(Vertex source) const {
    return transitions_.at(source);
}

NFA::VertexTransitionsT& NFA::getTransitions(Vertex source) {
    return transitions_.at(source);
}

void NFA::add(const NFA& other, size_t shift) {
    for (Vertex vertex : other.getVertices()) {
        addVertex(vertex + shift);
    }
    for (Vertex vertex : other.getFinishes()) {
        addFinish(vertex + shift);
    }
    for (Vertex source : other.getVertices()) {
        for (const auto& [symbol, destinations] : other.getTransitions(source)) {
            for (Vertex dest : destinations) {
                addTransition(source + shift, dest + shift, symbol);
            }
        }
    }
}

size_t NFA::countVertices() const noexcept {
    return transitions_.size();
}

void NFA::dumpTo(std::ostream& output) const {
    output << getStart() << '\n';
    output << '\n';

    for (Vertex finish_v : getFinishes()) {
        output << finish_v << '\n';
    }

    output << '\n';

    for (Vertex source : getVertices()) {
        for (const auto& [symbol, destinations] : getTransitions(source)) {
            for (Vertex destination : destinations) {
                output << source << ' ' << destination;
                if (symbol != NFA::kEmptySymbol) {
                    output << ' ' << *symbol;
                }
                output << '\n';
            }
        }
    }
}


NFA NFA::ReadFrom(std::istream& input) {
    Vertex start;
    input >> start;
    // newline char
    input.ignore();
    NFA result(start);

    // newline char
    input.ignore();

    while (input && input.peek() != '\n') {
        Vertex v;

        input >> v;
        input.ignore();

        result.addVertex(v);
        result.addFinish(v);
    }

    input.ignore();

    while (input && input.peek() != '\n') {
        Vertex v1;
        Vertex v2;
        SymbolT symbol;

        input >> v1 >> v2;

        if (!input) {
            break;
        }

        while (input.peek() == ' ') {
            input.ignore();
        }

        if (input.peek() == '\n') {
            symbol = kEmptySymbol;
        } else {
            symbol = input.get();
        }

        input.ignore();

        result.addVertex(v1);
        result.addVertex(v2);

        result.addTransition(v1, v2, symbol);
    }

    return result;
}
