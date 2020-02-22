#include "game.h"
#include <string.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <unordered_set>

Game::Game()
{

}

Game::Game(int status,
           int gamesize,
           int hostcolor,
           int time_total,
           int time_inc,
           bool allow_spectator):
status_(status),
size_(gamesize),
player_(player_black),
host_color_(hostcolor),
timer_total_(time_total),
timer_inc_(time_inc),
lastmove_(-1),
timer_black_(time_total, time_inc),
timer_white_(time_total, time_inc),
score_black_(-1),
score_white_(-3),
actions_(),
captures_(),
capture_count_(),
move_number(0),
cap_number(0),
new_actions(),
spectator_on_(allow_spectator),
is_counting_(false)
{
}

void Game::reset()
{
    clearAction();
    clearBoard();
    resetNodes();
    move_number = 0;
    cap_number = 0;
}

bool Game::isLegal(int action)
{
    // checks whether an action can be played by the current
    // player in O(1), does not affect the state of the game
    int x = action / size_, y = action % size_;
    int x_, y_;
    group *g;
    if ((score_black_ == -1 && score_white_ == -3) ||
        player_ != user_color() ||
        move_number < actions_.size() - 1) {
        return false;
    }
    if (board_[x][y] == 0 || board_[x][y] == 4 - player_){
        for (int i = 0; i < 4; i++){
            // for each of 4 adjacent neighbors
            x_ = x + ((i % 2) ? 0 : i - 1);
            y_ = y + ((i % 2) ? i - 2 : 0);
            if (x_ >= 0 && x_ < size_ && y_ >= 0 && y_ < size_){
                //if (x_, y_) is on the board
                g = nodes_[x_][y_].group_;
                if ((g->color_ < 0) ||
                    (g->color_ == player_ && g->liberty_ > 1) ||
                    (g->color_ != player_ && g->liberty_ == 1))
                {
                    return true;
                }

            }
        }
        return false;
    }
    return false;
}

void Game::play(int action)
{
    //printf("%d playing at (%d, %d)\n", player_, action / size_, action % size_);
    if (action != actions_.back()){
        printf("last action is: %d\n", actions_.back());
        update();
    }
    if (action >= 0){
        board_[action / size_][ action % size_] = player_ + 3;
    }
    actions_.push_back(action);
    capture_count_.push_back(0);
    player_ = 1 - player_;
    move_number++;
}

void Game::update()
{
    int a = actions_.back();
    if (a >= 0) {
        int x = a / size_, y = a % size_;
        int x_, y_;
        board_[x][y] -= 2;
        group *neighbors[4];
        //printf("updating at (%d, %d)\n", x, y);
        nodes_[x][y].reset();
        group *cur = nodes_[x][y].group_;
        cur->color_ = 1 - player_;
        cur->size_ = 1;
        group *next;
        for (int i = 0; i < 4; i++){

            x_ = x + ((i % 2) ? 0 : i - 1);
            y_ = y + ((i % 2) ? i - 2 : 0);
            if (x_ >= 0 && x_ < size_ && y_ >= 0 && y_ < size_){

                next = nodes_[x_][y_].group_;
                if (next->color_ == 1 - player_){
                    // merge same colored groups
                    //printf("merging groups\n");
                    if (cur == next){

                    } else if (cur->size_ >= next->size_){
                        cur->size_ += next->size_;
                        (cur->tail_)->next_ = next->head_;
                        cur->tail_ = next->tail_;
                        for (node *pt = next->head_; pt != NULL; pt = pt->next_){
                            pt->group_ = cur;
                        }
                    } else {
                        next->size_ += cur->size_;
                        (next->tail_)->next_ = cur->head_;
                        next->tail_ = cur->tail_;
                        for (node *pt = cur->head_; pt != NULL; pt = pt->next_){
                            pt->group_ = next;
                        }
                        cur = next;
                    }

                } else if (next->color_ == player_){
                    bool flag = true;
                    for (int j = 0; j < i; j++){
                        if (next == neighbors[j]){
                            flag = false;
                            break;
                        }
                    }
                    if (flag && --(next->liberty_) == 0){
                        removeCaptured(next);
                    }
                    neighbors[i] = next;
                } else {
                    cur->liberty_++;
                }
            }
        }
        cur->liberty_ = countLiberty(cur);
        if (cur->liberty_ == 0) {
            removeCaptured(cur);
        }
    }
}

void Game::queue_move(int action)
{
    new_actions.push(action);
}

int Game::countLiberty(group *g)
{
    int liberty = 0;
    std::unordered_set<int> visited;
    int x, y, x_, y_;
    for (node *pt = g->head_; pt != NULL; pt = pt->next_){
        x = (pt->val_) / size_;
        y = (pt->val_) % size_;
        for (int i = 0; i < 4; i++){
            x_ = x + ((i % 2) ? 0 : i - 1);
            y_ = y + ((i % 2) ? i - 2 : 0);
            if (x_ >= 0 && x_ < size_ && y_ >= 0 && y_ < size_ &&
                visited.count(x_ * size_ + y_) == 0 &&
                board_[x_][y_] == 0) {
                liberty++;
                visited.insert(x_ * size_ + y_);
            }
        }
    }
    return liberty;
}

void Game::start_counting()
{
    // initialize the counting board based on the
    // current board state.
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++){
            counting_board[i][j] = -1;
        }
    }

//    for (int i = 0; i < size_; i++) {
//        for (int j = 0; j < size_; j++){
//            printf("%d ", counting_board[i][j]);
//        }
//        printf("\n");
//    }
//    printf("\n");

    std::queue<int> q1, q2; // queue 1 contains whats searched, queue 2 contains whats yet to search
    int state, cur, x, y, x_, y_;
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++){
            if (counting_board[i][j] < 0){
                // use bfs to get paint adjacent empty spots
                if (board_[i][j] == empty_spot || board_[i][j] > whitestone_pmn) {
                    q1.push(i * size_ + j);
                    state = empty_spot;

                    while (!q1.empty()){
                        cur = q1.front();
                        q1.pop();
                        q2.push(cur);

                        x = cur / size_;
                        y = cur % size_;
                        for (int k = 0; k < 4; k++){
                            x_ = x + (k % 2 ? 0 : k - 1);
                            y_ = y + (k % 2 ? k - 2 : 0);
                            if (x_ >= 0 && x_ < size_ && y_ >= 0 && y_ < size_){
                                if (counting_board[x_][y_] < 0 &&
                                    (board_[x_][y_] == empty_spot || board_[x_][y_] > whitestone_pmn)) {
                                    q1.push(x_ * size_ + y_);
                                    counting_board[x_][y_] = 0;
                                } else {
                                    switch (state){
                                    case empty_spot:
                                        state = board_[x_][y_];
                                        break;
                                    case blackstone_pmn:
                                        state = board_[x_][y_] == 2 ? 3 : 1;
                                        break;
                                    case whitestone_pmn:
                                        state = board_[x_][y_] == 1 ? 3 : 2;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    while (!q2.empty()) {
                        cur = q2.front();
                        q2.pop();
                        counting_board[cur / size_][cur % size_] = state;
                    }
                } else {
                    counting_board[i][j] = board_[i][j];
                }
            }
        }
    }

//    for (int i = 0; i < size_; i++) {
//        for (int j = 0; j < size_; j++){
//            printf("%d ", counting_board[i][j]);
//        }
//        printf("\n");
//    }

}

void Game::flip_group(int action)
{
    // flips an group on the counting board. use BFS algorithm
    // to color all connected unoccupied spots
    int x = action / size_;
    int y = action % size_;

    if (board_[x][y] != empty_spot) {

        std::queue<int> q1, q2;
        group *cur = nodes_[x][y].group_;
        int val, x_, y_;
        int state = counting_board[x][y];

        // determine the new state for the clicked group
        if (cur->color_ + 1 == state) {
            state = -1;
        } else {
            state = cur->color_ + 1;
        }

        for (node *p = cur->head_; p != NULL; p = p->next_) {
            q1.push(p->val_);
            counting_board[p->val_ / size_][p->val_ % size_] = state;
        }

        while (!q1.empty()){

            val = q1.front();
            q1.pop();
            x = val / size_;
            y = val % size_;

            for (int i = 0; i < 4; i++){
                x_ = x + (i % 2 ? 0 : i - 1);
                y_ = y + (i % 2 ? i - 2 : 0);

                if (x_ >= 0 && x_ < size_ && y_ >= 0 && y_ < size_ && counting_board[x_][y_] >= 0) {

                    if (counting_board[x_][y_] != board_[x_][y_] ||
                        counting_board[x_][y_] == 0 ||
                        counting_board[x_][y_] == 3) {
                        q1.push(x_ * size_ + y_);
                        counting_board[x_][y_] = -1;
                    }

                }
            }

        }


        for (node *p = cur->head_; p != NULL; p = p->next_) {

            for (int i = 0; i < 4; i++) {

                x = (p->val_) / size_ + (i % 2 ? 0 : i - 1);
                y = (p->val_) % size_ + (i % 2 ? i - 2 : 0);

                if (x >= 0 && x < size_ && y >= 0 && y < size_ && counting_board[x][y] < 0) {

                    q1.push(x * size_ + y);
                    counting_board[x][y] = 0;
                    int new_state = state;
                    //printf("initialize new state to %d\n", state);
                    while (!q1.empty()) {
                        val = q1.front();
                        q1.pop();
                        q2.push(val);

                        for (int j = 0; j < 4; j++) {
                            // for each adjacent point
                            x_ = val / size_ + (j % 2 ? 0 : j - 1);
                            y_ = val % size_ + (j % 2 ? j - 2 : 0);
                            if (x_ >= 0 && x_ < size_ && y_ >= 0 && y_ < size_){
                                // if the point is on the board

                                if (counting_board[x_][y_] < 0) {
                                    // if the point has not being painted yet
                                    q1.push(x_ * size_ + y_);
                                    counting_board[x_][y_] = 0;
                                } else {
                                    // point is already painted
                                    switch (new_state) {
                                    case -1:
                                        new_state = counting_board[x_][y_];
                                        break;
                                    case 0:
                                        new_state = counting_board[x_][y_];
                                        break;
                                    case 1:
                                        new_state = counting_board[x_][y_] == 2 ? 3 : 1;
                                        break;
                                    case 2:
                                        new_state = counting_board[x_][y_] == 1 ? 3 : 2;
                                        break;
                                    }

                                }

                            }

                        }
                    }

                    while (!q2.empty()){
                        val = q2.front();
                        q2.pop();
                        counting_board[val / size_][val % size_] = new_state;
                    }
                }
            }


        }

    }
}

void Game::clearBoard()
{
    memset(board_, empty_spot, sizeof(board_));
}

void Game::clearAction()
{
    actions_.clear();
    actions_.push_back(-1);
    std::queue<int>().swap(new_actions);
}

void Game::clearCapture()
{
    captures_.clear();
    capture_count_.clear();
    capture_count_.push_back(0);
}

void Game::resetNodes()
{
    for (int i = 0; i < size_; i++){
        for (int j = 0; j < size_; j++){
            nodes_[i][j].val_ = i * size_ + j;
            nodes_[i][j].default_group = &groups_[i][j];
            nodes_[i][j].reset();
        }
    }
}

void Game::goto_first()
{
    // goto the first move
    clearBoard();
    move_number = 0;
    cap_number = 0;
    player_ = player_black;
    // disables the first button and the prev button
}

void Game::goto_last()
{
    // go to the newest move and process any new moves
    typedef std::vector<int>::size_type var;
    var move_size = actions_.size() + new_actions.size();
    for (var i = move_number; i < move_size; i++) {
        goto_next();
    }

}

void Game::goto_prev()
{

    if (move_number){
        player_ = 1 - player_;

        int action = actions_[move_number--];
        board_[action / size_][ action % size_] = 0;
        action = actions_[move_number];

        if (action >= 0){
            board_[action / size_][action % size_] += 2;
        }

        // restore captured stones
        for (int i = 1; i <= capture_count_[move_number]; i++) {
            action = captures_[cap_number - i];
            board_[action / size_][ action % size_] = player_;
        }
        cap_number -= capture_count_[move_number];
    }
}

void Game::goto_next()
{
    if (move_number < actions_.size() - 1) {
        int action = actions_[move_number];
        if (action >= 0) {
            board_[action / size_][ action % size_] -= 2;
            // remove captured stones
            for (std::vector<int>::size_type i = 0; i < capture_count_[move_number]; i++) {
                action = captures_[i + cap_number];
                board_[action / size_][ action % size_] = 0;
            }
            cap_number += capture_count_[move_number];
        }
        action = actions_[++move_number];
        if (action >= 0){
            board_[action / size_][ action % size_] = player_ + 3;
        }

    } else if (new_actions.size() > 0) {

        play(new_actions.front());
        new_actions.pop();

    } else {
        return;
    }

    player_ = 1 - player_;
}


// getters and setters
int Game::status()
{
    return status_;
}

int Game::size()
{
    return size_;
}

int Game::host_color()
{
    return host_color_;
}

int Game::user_color()
{
    // returns the stone color assigned to the current user.
    if (playerblack_ != "") {
        if (status_ == status_host) {
            return hostname_ == playerblack_ ? player_black : player_white;
        }
        if (status_ == status_player) {
            return hostname_ == playerwhite_ ? player_black : player_white;
        }
    }
    return -1;
}

int Game::timer_total()
{
    return timer_total_;
}

int Game::timer_inc()
{
    return timer_inc_;
}

timer* Game::timer_black()
{
    return &timer_black_;
}

timer* Game::timer_white()
{
    return &timer_white_;
}

int Game::score_black()
{
    return score_black_;
}

int Game::score_white()
{
    return score_white_;
}

int Game::moves()
{
    return actions_.size() + new_actions.size();
}

int Game::lastmove()
{
    return lastmove_;
}

bool Game::spectator_on()
{
    return spectator_on_;
}

bool Game::is_counting()
{
    return is_counting_;
}

bool Game::is_uptodate()
{
    //printf("\tmove number = %d, size_ = %d\n", move_number, actions_.size());
    return move_number + 1 == actions_.size();
}

int Game::getVal(int x, int y)
{
    return board_[x][y];
}

std::string Game::hostname()
{
    return hostname_;
}

std::string Game::opponent()
{
    return opponent_;
}

std::string Game::playerblack()
{
    return playerblack_;
}

std::string Game::playerwhite()
{
    return playerwhite_;
}

void Game::setStatus(int status)
{
    status_ = status;
}

void Game::setSize(int gamesize)
{
    size_ = gamesize;
}

void Game::setPlayer(int player)
{
    player_ = player;
}

void Game::setColor(int hostcolor)
{
    host_color_ = hostcolor;
}


void Game::set_spectator_on(bool allow_spectator)
{
    spectator_on_ = allow_spectator;
}

void Game::set_counting(bool is_counting)
{
    is_counting_ = is_counting;
}

void Game::set_blackscore(int score)
{
    score_black_ = score;
}

void Game::set_whitescore(int score)
{
    score_white_ = score;
}

void Game::setVal(int action, int value)
{
    board_[action / size_][action % size_] = value;
}

void Game::set_lastmove(int action)
{
    lastmove_ = action;
}

void Game::set_hostname(std::string name)
{
    hostname_ = name;
}

void Game::set_opponent(std::string name)
{
    opponent_ = name;
}

void Game::set_playerblack(std::string name)
{
    playerblack_ = name;
}

void Game::set_playerwhite(std::string name)
{
    playerwhite_ = name;
}

int Game::getCountVal(int x, int y)
{
    return counting_board[x][y];
}

void Game::printGroup(int action)
{
    group *g  = nodes_[action / size_][action % size_].group_;
    printf("player = %d, color = %d, size = %d, liberty = %d\n",
           player_, g->color_, g->size_, g->liberty_);
    printf("black score = %d, white score = %d\n", score_black_, score_white_);
    printf("player = %d, user color = %d\n", player_, user_color());
    printf("move number = %d, action size = %d\n", move_number, actions_.size());
}

void Game::printInfo()
{
    printf("node group address:\n");
    for (int i = 0; i < size_; i++){
        printf("[");
        for (int j = 0; j < size_; j++){
            printf(" %d, ", nodes_[i][j].group_);
        }
        printf("]\n");
    }
    printf("\n");

    printf("default group address:\n");
    for (int i = 0; i < size_; i++){
        printf("[");
        for (int j = 0; j < size_; j++){
            printf(" %d, ", nodes_[i][j].default_group);
        }
        printf("]\n");
    }
    printf("\n");

    printf("group address:\n");
    for (int i = 0; i < size_; i++){
        printf("[");
        for (int j = 0; j < size_; j++){
            printf(" %d, ", &groups_[i][j]);
        }
        printf("]\n");
    }
    printf("\n");

    printf("this = %d\n", this);
    printf("size_ = %d\n", size_);
    printf("status_ = %d\n", status_);
    printf("board location = %d\n", board_);
    printf("nodes location = %d\n", nodes_);
    printf("groups location = %d\n", groups_);
}

void Game::printBoard()
{
    printf("\n");
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
            printf("%d ", board_[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void Game::removeCaptured(group *g)
{
    //O(n) time where n is the size of g
    // remember to store the captured stones.
    group *neighbors[4];
    int x, y;
    bool flag;
    node *pt, *next;
    capture_count_.back() += g->size_;
    cap_number += g->size_;

    for (pt = g->head_; pt != NULL; pt = next){
        captures_.push_back(pt->val_);
        setVal(pt->val_, 0);
        next = pt->next_;
        for (int j = 0; j < 4; j++){
            flag = true;
            x = (pt->val_) / size_ + ((j % 2) ? 0 : j - 1);
            y = (pt->val_) % size_ + ((j % 2) ? j - 2 : 0);
            if (x < 0 || x >= size_ || y < 0 || y >= size_){
                neighbors[j] = NULL;
                continue;
            }
            for (int k = 0; k < j; k++){
                if (neighbors[k] == nodes_[x][y].group_) {
                    flag = false;
                    break;
                }
            }
            neighbors[j] = nodes_[x][y].group_;
            if (flag) {
                neighbors[j]->liberty_++;
            }
        }
        pt->reset();
    }

}

//*******************************
