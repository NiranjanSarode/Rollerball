// #include <algorithm>
// #include <random>
// #include <iostream>
// #include <thread>
// #include <vector>
// #include <cmath>

// #include "board.hpp"
// #include "engine.hpp"
// #include "butils.hpp"

// class Node {
// public:
//     Board state;
//     Node* parent;
//     std::vector<Node> children;
//     int visits;
//     double value;
//     int last_move;

//     Node(const Board& state, Node* parent = nullptr, int last_move = 0)
//         : state(state), parent(parent), visits(0), value(0), last_move(last_move) {}

//     ~Node() {
//         for (Node& child : children) {
//             delete &child;
//         }
//     }
// };

// bool game_over(const Board& b) {
//     auto moveset = b.get_legal_moves();
//     if(moveset.size() == 0){
//         return true;
//     }
// }

// Node* Engine::select(Node *node) {
//     while (!game_over(node->state) && node->children.size() > 0) {
//         node = get_best_child(node);
//     }
//     return node;
// }
// void Engine::expand(Node* node) {
//     auto legal_moves = node->state.get_legal_moves();
//     for (auto& move : legal_moves) {
//         Board new_state = node->state;
//         new_state.do_move_(move);
//         node->children.emplace_back(new_state, node, move);
//     }
// }

// double Engine::simulate(Node* node) {
//     Board temp_state = node->state;
//     while (!game_over(temp_state)) {
//         auto moves = temp_state.get_legal_moves();
//         std::vector<int> legal_moves;
//         for(auto m : moves){
//             legal_moves.push_back(m);
//         }
//         if (legal_moves.empty()) {
//             break;
//         }
//         auto random_move = legal_moves[std::rand() % legal_moves.size()];
//         temp_state.do_move_(random_move);
//     }
//     return evaluate_board(temp_state);
// }


// void Engine::find_best_move(const Board& b) {

//     // Constants
//     const int iterations = 1000;  // Adjust as needed

//     // UCT (Upper Confidence Bound for Trees) constant
//     const double exploration_param = 1.0;

//     // Root node
//     Node root_node(b);

//     for (int i = 0; i < iterations; ++i) {
//         // Selection

//         Node* selected_node = select(&root_node);

//         // Expansion
//         if (!game_over(selected_node->state)) {
//             expand(selected_node);
//         }

//         // Simulation
//         double result = simulate(selected_node);

//         // Backpropagation
//         backpropagate(selected_node, result);
//     }

//     // Choose the best move based on visits
//     Node* best_child = get_best_child(&root_node);
//     this->best_move = best_child->last_move;

//     // Debugging: Slow down the moves
//     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
// }







// void Engine::backpropagate(Node* node, double result) {
//     while (node != nullptr) {
//         node->visits++;
//         node->value += result;
//         node = node->parent;
//     }
// }

// Node* Engine::get_best_child(Node* node) {
//     double best_value = -std::numeric_limits<double>::infinity();
//     Node* best_child = nullptr;

//     for (auto& child : node->children) {
//         double uct_value = (child.value / child.visits) + 0.1 * sqrt(log(node->visits) / child.visits);

//         if (uct_value > best_value) {
//             best_value = uct_value;
//             best_child = &child;
//         }
//     }

//     return best_child;
// }
