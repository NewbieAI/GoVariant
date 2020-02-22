#ifndef GAME_H
#define GAME_H

#include <vector>
#include <queue>
#include <time.h>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>


// using a disjoint set union datastructure
// to make isLegal() routine O(1), which gets
// called for every mouse click.
typedef struct node node;
typedef struct group group;

typedef struct group {
    node *head_;
    node *tail_;

    int size_;
    int color_;
    int liberty_;

    group()
    {
        head_ = NULL;
        tail_ = NULL;

        size_ = 0;
        color_ = -1;
        liberty_ = 0;
    }
} group;

typedef struct node {
    int val_;
    node *next_;
    group *group_;
    group *default_group;


    node(){
        val_ = -1;
        next_ = NULL;
        group_ = NULL;
        default_group = NULL;
    }

    void reset()
    {
        next_ = NULL;
        group_ = default_group;
        if (group_ != NULL){
            group_->liberty_ = 0;
            group_->size_ = 0;
            group_->color_ = -1;
            group_->head_ = this;
            group_->tail_ = this;
        }
    }

} node;


struct game_timer {
    clock_t time_remain_;
    clock_t increment_;
    clock_t start_;
    bool is_running;

    game_timer(){};
    game_timer(int time_total, int time_inc) {
        time_remain_ = float(time_total * 60) * CLOCKS_PER_SEC;
        increment_ = float(time_inc) * CLOCKS_PER_SEC;
        is_running = false;
    }
    void start() {
        if (!is_running) {
            start_ = clock();
            is_running = true;
        }

    }
    void stop() {
        if (is_running) {
            time_remain_ += increment_ - (clock() - start_);
            is_running = false;
        }
    }

    void printTo(char *buffer) {
        clock_t t = time_remain_ - (is_running ? clock() - start_ : 0);
        long secs = t / CLOCKS_PER_SEC;
        sprintf(buffer, "%d : %02d", secs / 60, secs % 60);
    }

};

typedef struct game_timer timer;

typedef struct {
    int id_;
    int player_count;
    char gamehost_[32];
    char opponent_[32];
    char status_[10];
    char size_[10];
    char color_[10];
    char timer_[20];
    char spectator_on[5];

    int size(){
        int val;
        for (int i = 0; i < 10 ;i++) {
            if (size_[i] == 'x') {
                size_[i] = '\0';
                val = atoi(size_);
                size_[i] = 'x';
                return val;
            }
        }
    }
    int color(){
        if (strcmp(color_, "random") == 0) {
            return 2;
        }
        if (strcmp(color_, "black") == 0) {
            return 0;
        }
        if (strcmp(color_, "white") == 0) {
            return 1;
        }
        return -1;
    }

    void set_time(int* time_total, int* time_inc){
        int t = 0;
        char *ptr = timer_;
        while (*ptr >= '0' && *ptr <= '9') {
            t = t * 10 + *(ptr++) - '0';
        }
        *time_total = t;
        while (*ptr < '0' || *ptr > '9') {
            ptr++;
        }
        t = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            t = t * 10 + *(ptr++) - '0';
        }
        *time_inc = t;
    }

    bool spectator(){
        if (strcmp(color_, "On") == 0) {
            return 1;
        }
        if (strcmp(color_, "Off") == 0) {
            return 0;
        }
        return -1;
    }

} Game_Descriptor;

typedef struct {
    char name_[32];
    char country_[32];
    int win_;
    int loss_;
} Player;

class Game
{
    public:
        static const int status_host = 0;
        static const int status_player = 1;
        static const int status_spectator = 2;

        static const int player_black = 0;
        static const int player_white = 1;
        static const int player_random = 2;

        static const int empty_spot = 0;
        static const int blackstone_pmn = 1;
        static const int whitestone_pmn = 2;
        static const int blackstone_tmp = 3;
        static const int whitestone_tmp = 4;

        Game(int status,
             int gamesize,
             int hostcolor,
             int time_total,
             int time_inc,
             bool allow_spectator);
        Game();

        bool isLegal(int action);
        void play(int action);
        void update();
        void reset();
        void queue_move(int);
        void start_counting();
        void flip_group(int);
        int countLiberty(group*);

        void goto_first();
        void goto_last();
        void goto_next();
        void goto_prev();

        int status();
        int size();
        int host_color();
        int user_color();
        int timer_total();
        int timer_inc();
        timer* timer_black();
        timer* timer_white();
        int score_black();
        int score_white();
        int moves();
        int lastmove();

        bool spectator_on();
        bool is_counting();
        bool is_uptodate();
        int getVal(int, int);

        std::string hostname();
        std::string opponent();
        std::string playerblack();
        std::string playerwhite();


        void setStatus(int);
        void setPlayer(int);
        void setSize(int);
        void setColor(int);
        void set_spectator_on(bool);
        void set_counting(bool);
        void set_blackscore(int);
        void set_whitescore(int);
        void setVal(int, int);
        void set_lastmove(int);

        void clearBoard();
        void clearAction();
        void clearCapture();
        void resetNodes();

        void set_hostname(std::string);
        void set_opponent(std::string);
        void set_playerblack(std::string);
        void set_playerwhite(std::string);

        int getCountVal(int, int);
        //Game_Descriptor describe();



        //debug use only
        void printInfo();
        void printGroup(int action);
        void printBoard();

    private:
        int status_;
        int size_;
        int player_;
        int host_color_;
        int score_black_;
        int score_white_;
        int timer_total_;
        int timer_inc_;
        int lastmove_;
        timer timer_black_;
        timer timer_white_;
        bool spectator_on_;
        bool is_counting_;
        int board_[19][19];
        int counting_board[19][19];
        group groups_[19][19];
        node nodes_[19][19];
        std::vector<int> actions_;//the location of the move played
        std::vector<int> captures_;//the location of captured stones
        std::vector<int> capture_count_;//the number of captures for every move
        std::vector<int>::size_type move_number;
        std::vector<int>::size_type cap_number;
        std::queue<int> new_actions;

        std::string hostname_;
        std::string opponent_;
        std::string playerblack_;
        std::string playerwhite_;

        void removeCaptured(group*);

};

#endif // GAME_H
