#include <algorithm>
#include <random>
#include <iostream>
#include <thread>
//time
#include <chrono>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"




long long nodes = 0;
int ply = 0;
int best = 0;
int killer_moves[2][64];
int history_moves[12][64];
int pv_table[64][64];
int pv_length[64];
int follow_pv, score_pv;
const int full_depth = 4;
const int red_limit = 3;
// start time initialization
auto start = std::chrono::high_resolution_clock::now();
auto current = std::chrono::high_resolution_clock::now();


int mvv_lva[6][6] = {
 	105, 205, 305, 405, 505, 605,
	104, 204, 304, 404, 504, 604,
	103, 203, 303, 403, 503, 603,
	102, 202, 302, 402, 502, 602,
	101, 201, 301, 401, 501, 601,
	100, 200, 300, 400, 500, 600
};


int evaluate_board7_3(const Board &b){
    int white_pawn_score[49] = {
        4, 4, 5, 5, 8, 0, 0,
        4, 4, 5, 5, 8, 0, 0,
        3, 3, 0, 0, 0, 0, 0,
        2, 2, 0, 0, 0, 0, 0,
        2, 2, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0,
    };
    int black_pawn_score[49] = {
        0, 0, 0, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 2, 2,
        0, 0, 0, 0, 0, 2, 2,
        0, 0, 0, 0, 0, 3, 3,
        0, 0, 8, 5, 5, 4, 4,
        0, 0, 8, 5, 5, 4, 4,
    };
    int white_rook_score[49] = {
        4, 4, 0, 0, 0, 0, 0,
        5, 3, 0, 0, 0, 0, 0,
        4, 2, 0, 0, 0, 0, 0,
        4, 2, 0, 0, 0, 0, 0,
        4, 2, 0, 0, 0, 0, 0,
        3, 2, 1, 1, 1, 0, 0,
        3, 3, 2, 2, 1, 0, 0,
    };
    int black_rook_score[49] = {
        0, 0, 1, 2, 2, 3, 3,
        0, 0, 1, 1, 1, 2, 3,
        0, 0, 0, 0, 0, 2, 4,
        0, 0, 0, 0, 0, 2, 4,
        0, 0, 0, 0, 0, 2, 4,
        0, 0, 0, 0, 0, 3, 5,
        0, 0, 0, 0, 0, 4, 4,
    };
    int white_bishop_score[49] = {
        0, 2, 0, 2, 0, 1, 0,
        2, 0, 2, 0, 2, 0, 1,
        0, 2, 0, 0, 0, 2, 0,
        2, 0, 0, 0, 0, 0, 2,
        0, 1, 0, 0, 0, 0, 0,
        1, 0, 1, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0,
    };
    int black_bishop_score[49] = {
        0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 1, 0, 1,
        0, 0, 0, 0, 0, 1, 0,
        2, 0, 0, 0, 0, 0, 2,
        0, 2, 0, 0, 0, 2, 0,
        1, 0 ,2, 0, 2, 0, 2,
        0, 1 ,0, 2, 0, 2, 0,
    };
    int white_king_score[49] = {
        2, 2, 0, 0, 0, 0, 0,
        2, 2, 0, 0, 0, 0, 0,
        2, 2, 0, 0, 0, 0, 0,
        2, 2, 0, 0, 0, 0, 0,
        2, 2, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0,
    };
    int black_king_score[49] = {
        0, 0, 0, 0, 1, 1, 1,
        0, 0, 0, 0, 1, 1, 1,
        0, 0, 0, 0, 0, 2, 2,
        0, 0, 0, 0, 0, 2, 2,
        0, 0, 0, 0, 0, 2, 2,
        0, 0, 0, 0, 0, 2, 2,
        0, 0, 0, 0, 0, 2, 2,
    };
    int temp_score = 0;
    if(b.data.w_rook_1 != DEAD){
        temp_score += 500;
        temp_score += white_rook_score[getx(b.data.w_rook_1)+(6-gety(b.data.w_rook_1))*7];
    }
    if(b.data.w_rook_2 != DEAD){
        temp_score += 500;
        temp_score += white_rook_score[getx(b.data.w_rook_2)+(6-gety(b.data.w_rook_2))*7];
    }
    if(b.data.w_bishop != DEAD){
        temp_score += 300;
        temp_score += white_bishop_score[getx(b.data.w_bishop)+(6-gety(b.data.w_bishop))*7];
    }
    if(b.data.w_king != DEAD){
        temp_score += white_king_score[getx(b.data.w_king)+(6-gety(b.data.w_king))*7];
    }
    if(b.data.w_pawn_1 != DEAD){
        temp_score += 100;
        temp_score += white_pawn_score[getx(b.data.w_pawn_1)+(6-gety(b.data.w_pawn_1))*7];
    }
    if(b.data.w_pawn_2 != DEAD){
        temp_score += 100;
        temp_score += white_pawn_score[getx(b.data.w_pawn_2)+(6-gety(b.data.w_pawn_2))*7];
    }
    if(b.data.b_rook_1 != DEAD){
        temp_score -= 500;
        temp_score -= black_rook_score[getx(b.data.b_rook_1)+(6-gety(b.data.b_rook_1))*7];
    }
    if(b.data.b_rook_2 != DEAD){
        temp_score -= 500;
        temp_score -= black_rook_score[getx(b.data.b_rook_2)+(6-gety(b.data.b_rook_2))*7];
    }
    if(b.data.b_bishop != DEAD){
        temp_score -= 300;
        temp_score -= black_bishop_score[getx(b.data.b_bishop)+(6-gety(b.data.b_bishop))*7];
    }
    if(b.data.b_king != DEAD){
        temp_score -= black_king_score[getx(b.data.b_king)+(6-gety(b.data.b_king))*7];
    }
    if(b.data.b_pawn_1 != DEAD){
        temp_score -= 100;
        temp_score -= black_pawn_score[getx(b.data.b_pawn_1)+(6-gety(b.data.b_pawn_1))*7];
    }
    if(b.data.b_pawn_2 != DEAD){
        temp_score -= 100;
        temp_score -= black_pawn_score[getx(b.data.b_pawn_2)+(6-gety(b.data.b_pawn_2))*7];
    }
    return temp_score;
}
int evaluate_board8_4(const Board &b){
    return 0;
}
int evaluate_board8_2(const Board &b){
    return 0;
}

int Engine::move_order(Board &b, int move){
    int from = getp0(move);
    int to = getp1(move);
    int piece = b.data.board_0[from];
    int target = b.data.board_0[to];
    int x = 0;
    int y = 0;
    if(piece & 0x02) x = 0;
    else if(piece & 0x04) x = 3;
    else if(piece & 0x08) x = 4;
    else if(piece & 0x10) x = 2;
    else if(piece & 0x20) x = 1;
    if(target & 0x02) y = 0;
    else if(target & 0x04) y = 3;
    else if(target & 0x08) y = 4;
    else if(target & 0x10) y = 2;
    else if(target & 0x20) y = 1;
    if(score_pv){
        if(pv_table[0][ply] == move){
            score_pv = 0;
            return 20000;
        }
    }
    if(!piece || !target){
        if(killer_moves[0][ply] == move){
            return 9000;
        }
        else if(killer_moves[1][ply] == move){
            return 8000;
        }
        else{
            return history_moves[x][to];
        }
    }
    else{
        return mvv_lva[x][y];
    }
}
// input move array in function

int Engine::quiescence(Board &b, int alpha, int beta){
    int temp;
    if(b.data.player_to_play == WHITE){
        if(b.data.board_type == SEVEN_THREE) temp = evaluate_board7_3(b);
        else if(b.data.board_type == EIGHT_FOUR) temp = evaluate_board8_4(b);
        else temp = evaluate_board8_2(b);
    }
    else{
        if(b.data.board_type == SEVEN_THREE) temp = -evaluate_board7_3(b);
        else if(b.data.board_type == EIGHT_FOUR) temp = -evaluate_board8_4(b);
        else temp = -evaluate_board8_2(b);
    }
    if(temp >= beta){
        return beta;
    }
    if(alpha < temp){
        alpha = temp;
    }
    auto moveset = b.get_legal_moves();
    for(auto m : moveset){
        std::string s = move_to_str(m);
        int target = getp1(m);
        if(b.data.board_0[target] == EMPTY && !b.in_check()){
            continue;
        }
        nodes++;
        Board t = b;
        ply++;
        t.do_move_(m);
        int score = -quiescence(t,-beta,-alpha);
        ply--;
        if(score >= beta){
            return beta;
        }
        if(score > alpha){
            alpha = score;
        }
    }
    return alpha;
}

std::vector<int> Engine::move_sort(Board &b, std::unordered_set<U16> &moves, int n_moves){
    int score[1<<15];
    std::vector<int> output(n_moves);
    int index = 0;
    // Board t;
    for(auto m : moves){
        // t = b;
        // t.do_move_(m);
        score[m] = move_order(b,m);
        output[index] = m;
        index++;
    }
    sort(output.begin(),output.end(),[&](int m1, int m2){
        return score[m1] > score[m2];
    });
    return output;
}

int Engine::negamax(Board &b, int depth, int alpha, int beta){
    if(ply > 64){
        return 0;
    }
    pv_length[ply] = ply;
    if(depth==0){
       return quiescence(b,alpha,beta);
    }
    nodes++;
    auto moveset = b.get_legal_moves();
    if(b.in_check()){
        depth++;
    }
    if(depth > 3 && b.in_check() && ply){
        Board t = b;
        t.flip_player_();
        int score = -negamax(t,depth-1-2,-beta,-beta-1);
        if(score >= beta){
            return beta;
        }
    }
    if(follow_pv){
        follow_pv = 0;
        for(auto m : moveset){
            if(pv_table[0][ply] == m){
                score_pv = 1;
                follow_pv = 1;

            }
        }
    }
    int n_moves = moveset.size();
    int eval = 0;
    std::vector<int> sorted = move_sort(b,moveset,n_moves);
    int moves_searched = 0;
    for(auto m : sorted){
        Board t = b;
        ply++;
        t.do_move_(m);
        int from = getp0(m);
        int to = getp1(m);
        int piece = b.data.board_0[from];
        int target = b.data.board_0[to];
        int x = 0;
        int y = 0;
        if(piece & 0x02) x = 0;
        else if(piece & 0x04) x = 3;
        else if(piece & 0x08) x = 4;
        else if(piece & 0x10) x = 2;
        else if(piece & 0x20) x = 1;
        if(target & 0x02) y = 0;
        else if(target & 0x04) y = 3;
        else if(target & 0x08) y = 4;
        else if(target & 0x10) y = 2;
        else if(target & 0x20) y = 1;
        if(moves_searched == 0) eval = -negamax(t,depth-1,-beta,-alpha);
        else{
            if(moves_searched >= full_depth && depth >= red_limit && b.in_check() == 0 && target == 0 && getpromo(m) == 0){
                eval = -negamax(t,depth-2,-alpha-1,-alpha);
            }
            else eval = alpha + 1;
            if(eval > alpha){
                eval = -negamax(t,depth-1,-alpha-1,-alpha);
                if(eval > alpha && eval < beta){
                    eval = -negamax(t,depth-1,-beta,-alpha);
                }
            }
        }
        ply--;
        moves_searched++;
        
        if(eval >= beta){
            if(!target){
                killer_moves[1][ply] = killer_moves[0][ply];
                killer_moves[0][ply] = m;
            }
            return beta;
        }
        if(eval>alpha){
            if(!target) history_moves[x][to] += depth;
            alpha = eval;
            pv_table[ply][ply] = m;
            for(int i = ply+1; i < pv_length[ply+1]; i++){
                pv_table[ply][i] = pv_table[ply+1][i];
            }
            pv_length[ply] = pv_length[ply+1];
        }
    }
    if(moveset.size() == 0){
        if(b.data.player_to_play == WHITE){
            if(b.in_check()){
                return 49000 - ply;
            }
            else{
                return 0;
            }
        } 
        else{
            if(b.in_check()){
                return -49000 + ply;
            }
            else{
                return 0;
            }
        }
    }
    return alpha;
}

void Engine::find_best_move(const Board& b) {
    // make history moves 0
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < 64; j++){
            history_moves[i][j] = 0;
        }
    }
    // make killer moves 0
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 64; j++){
            killer_moves[i][j] = 0;
        }
    }
    // make pv table 0
    for(int i = 0; i < 64; i++){
        for(int j = 0; j < 64; j++){
            pv_table[i][j] = 0;
        }
        pv_length[i] = 0;
    }

    // start = std::chrono::high_resolution_clock::now();

    auto moveset = b.get_legal_moves();
    this->board = b;
    nodes = 0;
    follow_pv = 0;
    score_pv = 0;
    if (moveset.size() == 0) {
        std::cout << "Could not get any moves from board!\n";
        std::cout << board_to_str(&b.data);
        this->best_move = 0;
    }
    else {
        int t = negamax(board,7,-50000,50000);
        // std::cout<<"aaaaaaaaaaaaaaaaaaaaaaaaaaa"<<std::endl;
        std::cout<<"depth fix nodes: "<<nodes<<std::endl;
        nodes = 0;
        // make history moves 0
        for(int i = 0; i < 12; i++){
            for(int j = 0; j < 64; j++){
                history_moves[i][j] = 0;
            }
        }
        // make killer moves 0
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 64; j++){
                killer_moves[i][j] = 0;
            }
        }
        // make pv table 0
        for(int i = 0; i < 64; i++){
            for(int j = 0; j < 64; j++){
                pv_table[i][j] = 0;
            }
            pv_length[i] = 0;
        }
        // iterative till 5
        int alpha = -50000;
        int beta = 50000;
        for(int i = 1; i < 8; i++){
            follow_pv = 1;
            t = negamax(board,i,alpha,beta);
            std::cout<<"depth: "<<i<<" nodes: "<<nodes<<std::endl;
            if((t<alpha) || (t>beta)){
                alpha = -50000;
                beta = 50000;
                continue;
            }
            alpha = t - 50;
            beta = t + 50;
        }

        // current = std::chrono::high_resolution_clock::now();
        // cout time
        // std::chrono::duration<double> elapsed = current - start;
        // std::cout<<"time: "<<elapsed.count()<<std::endl;
        std::cout<<board_to_str(&board.data)<<std::endl;
        this->best_move = pv_table[0][0];
        // this->best_move = moves[0];
        // std::cout<<"nodes: "<<nodes<<std::endl;
    }


    // just for debugging, to slow down the moves
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}
