#pragma once

#include "engine_base.hpp"
#include <atomic>

class Engine : public AbstractEngine {

    // add extra items here. 
    // Note that your engine will always be instantiated with the default 
    // constructor.
    
    public:
    // make copy of b into board
    Board board;
    void find_best_move(const Board& b) override;
    int negamax(Board &b,int depth, int alpha, int beta);
    int quiescence(Board &b, int alpha, int beta);
    int move_order(Board &b, int move);
    std::vector<int> move_sort(Board &b, std::unordered_set<U16> &moves, int n_moves);

    // Node* select(Node* node);
    // void expand(Node* node);
    // double simulate(Node* node);
    // void backpropagate(Node* node, double result);
    // Node* get_best_child(Node* node);

    int best_move;


};
