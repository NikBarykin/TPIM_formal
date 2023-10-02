#pragma once

#include <memory>
#include <optional>
#include <string>
#include <map>
#include <set>


class NFA {
public:
    using Vertex = uint32_t;
    using SymbolT = std::optional<char>;
    using VertexTransitionsT = std::map<SymbolT, std::set<Vertex>>;

    static constexpr SymbolT kEmptySymbol = SymbolT();

private:
    std::set<Vertex> finishes_{};
    std::map<Vertex, VertexTransitionsT> transitions_{};
    const Vertex start_{};
    // it is important that start_ variable is placed after other variables

public:
    NFA(Vertex start);

    void addVertex(Vertex);

    Vertex getStart() const;
    Vertex getMaxVertex() const;
    std::set<Vertex> getVertices() const;
    size_t countVertices() const noexcept;

    bool isFinish(Vertex) const;
    void addFinish(Vertex);
    void removeFinish(Vertex);
    const std::set<Vertex>& getFinishes() const;
    Vertex getSingleFinish() const;
    // if there is only one finish, return it, otherwise throw LogicalError

    void addTransition(Vertex source, Vertex dest, SymbolT symbol = kEmptySymbol);
    void removeTransition(Vertex source, Vertex dest, SymbolT symbol = kEmptySymbol);
    const VertexTransitionsT& getTransitions(Vertex) const;
    VertexTransitionsT& getTransitions(Vertex);

    void add(const NFA& other, size_t shift);
    // add other NFA to 'this' nfa shifting all 'other' vertices by shift value

    NFA& removeEpsTransitions();

    void dumpTo(std::ostream&) const;
    // print NFA to given ostream
    // if two NFA's have the same structure and same labels
    // output is guaranteed to be the same

    static NFA ReadFrom(std::istream&);
};


NFA MakeDFA(const NFA&);
void CompleteDFA(const NFA& dfa);
