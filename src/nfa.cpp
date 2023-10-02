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
    transitions_[vertex] = VertexTransitionsT();
}

NFA::Vertex NFA::getStart() const {
    return start_;
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
    finishes_.insert(new_finish);
}

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

void NFA::add(const NFA& other) {
    // we need shift, so new vertices would not overlap with old ones
    uint32_t shift = transitions_.rbegin()->first + 1;
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

    while (input) {
        if (input.peek() == '\n') {
            break;
        }
        Vertex v;

        input >> v;
        input.ignore();

        result.addVertex(v);
        result.addFinish(v);
    }

    while (input)

    for (std::string line; std::getline(input, line); ) {
        if (line.empty()) {
            break;
        }
        std::istringstream line_stream(std::move(line));
        Vertex v;
        line_stream >> v;
        result.addVertex(v);
        result.addFinish(v);
    }

    for (std::string line; std::getline(input, line); ) {
        if (line.empty()) {
            break;
        }
        std::istringstream line_stream(std::move(line));
        Vertex v1;
        Vertex v2;
        SymbolT symbol;

        line_stream >> v1 >> v2;
        char symbol_ch;
        if (line_stream >> symbol_ch) {
            symbol = symbol_ch;
        } else {
            symbol = kEmptySymbol;
        }

        result.addVertex(v1);
        result.addVertex(v2);

        result.addTransition(v1, v2, symbol);
    }

    return result;
}
