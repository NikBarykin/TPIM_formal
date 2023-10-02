#include <string>
#include <stdexcept>
#include <ostream>
#include <memory>
#include <istream>
#include <iostream>
#include <sstream>

#include "nfa.h"


NFA::Vertex::Vertex(std::string label): label(std::move(label)) {}


NFA::NFA(std::string start_label): start_(addVertex(std::move(start_label))) {}


NFA::VertexHandler NFA::addVertex(std::string label) {
    if (label.empty()) {
        label = std::to_string(transitions_.size());
    }
    auto it = vertices_by_labels_.find(label);
    if (it != vertices_by_labels_.end()) {
        return it->second;
    }
    VertexHandler new_vertex = std::make_shared<Vertex>(label);
    vertices_by_labels_[std::move(label)] = new_vertex;
    transitions_[new_vertex] = VertexTransitionsT();
    return new_vertex;
}

NFA::VertexHandler NFA::getVertexByLabel(std::string label) const {
    return vertices_by_labels_.at(label);
}

NFA::VertexHandler NFA::getStart() const {
    return start_;
}

std::set<NFA::VertexHandler> NFA::getVertices() const {
    std::set<VertexHandler> result;
    for (const auto& [vertex, _] : transitions_) {
        result.insert(vertex);
    }
    return result;
}

bool NFA::isFinish(VertexHandler vertex) const {
    return getFinishes().count(vertex);
}

void NFA::addFinish(VertexHandler new_finish) {
    finishes_.insert(new_finish);
}

void NFA::removeFinish(VertexHandler vertex) {
    finishes_.erase(vertex);
}

const std::set<NFA::VertexHandler>& NFA::getFinishes() const {
    return finishes_;
}

NFA::VertexHandler NFA::getSingleFinish() const {
    if (finishes_.size() != 1) {
        throw std::logic_error(
                "Number of finishes is " + std::to_string(finishes_.size()) + " != 1");
    }
    return *finishes_.begin();
}

void NFA::addTransition(VertexHandler source, VertexHandler dest, SymbolT symbol) {
    transitions_.at(source)[symbol].insert(dest);
}

void NFA::removeTransition(VertexHandler source, VertexHandler dest, SymbolT symbol) {
    transitions_.at(source)[symbol].erase(dest);
}

const NFA::VertexTransitionsT& NFA::getTransitions(VertexHandler source) const {
    return transitions_.at(source);
}

void NFA::merge(NFA other) {
    vertices_by_labels_.merge(std::move(other.vertices_by_labels_));
    finishes_.merge(std::move(other.finishes_));
    transitions_.merge(std::move(other.transitions_));
}

size_t NFA::countVertices() const noexcept {
    return transitions_.size();
}

void NFA::dumpTo(std::ostream& output) const {
    output << getStart()->label << '\n';
    output << '\n';

    for (VertexHandler finish_v : getFinishes()) {
        output << finish_v->label << '\n';
    }

    output << '\n';

    for (VertexHandler source : getVertices()) {
        for (const auto& [symbol, destinations] : getTransitions(source)) {
            for (VertexHandler destination : destinations) {
                output << source->label << ' ' << destination->label;
                if (symbol != NFA::kEmptySymbol) {
                    output << ' ' << *symbol;
                }
                output << '\n';
            }
        }
    }
}


NFA NFA::ReadFrom(std::istream& input) {
    std::string start_label;
    std::getline(input, start_label);
    NFA result(std::move(start_label));

    // newline char
    input.ignore();

    for (std::string line; std::getline(input, line); ) {
        if (line.empty()) {
            break;
        }
        NFA::VertexHandler v = result.addVertex(std::move(line));
        result.addFinish(v);
    }

    for (std::string line; std::getline(input, line); ) {
        if (line.empty()) {
            break;
        }
        std::istringstream line_stream(std::move(line));
        std::string v1_label;
        std::string v2_label;
        NFA::SymbolT symbol;

        line_stream >> v1_label >> v2_label;
        char symbol_ch;
        if (line_stream >> symbol_ch) {
            symbol = symbol_ch;
        } else {
            symbol = NFA::kEmptySymbol;
        }

        NFA::VertexHandler v1 = result.addVertex(std::move(v1_label));
        NFA::VertexHandler v2 = result.addVertex(std::move(v2_label));

        result.addTransition(v1, v2, symbol);
    }

    return result;
}
