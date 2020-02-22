#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <vector>
#include <string>
#include <GL/freeglut.h>
#include <game.h>



namespace ui_const
{
    const int UI_START = 0;
    const int UI_LOBBY = 1;
    const int UI_GAME = 2;

    const int UI_COLOR_TEXT = 0;
    const int UI_COLOR_HIGHLIGHTED_TEXT = 1;
    const int UI_COLOR_SELECTED_TEXT = 2;
    const int UI_COLOR_BUTTON = 3;
    const int UI_COLOR_SELECTED_BUTTON = 4;
    const int UI_COLOR_BOARD = 5;
    const int UI_COLOR_BLACK_STONE = 6;
    const int UI_COLOR_WHITE_STONE = 7;
    const int UI_COLOR_LABEL = 8;
    const int UI_COLOR_FIELD = 9;

    const int UI_BACKGROUND_START = 0;
    const int UI_BACKGROUND_LOBBY = 1;
    const int UI_BACKGROUND_GAME = 2;

    const int UI_BUTTON_LOGIN = 0;
    const int UI_BUTTON_REGISTER = 1;
    const int UI_BUTTON_SUBMIT = 2;
    const int UI_BUTTON_EXIT = 3;
    const int UI_BUTTON_HELP = 4;
    const int UI_BUTTON_JOIN = 5;
    const int UI_BUTTON_SPECTATE = 6;
    const int UI_BUTTON_SEND = 7;
    const int UI_BUTTON_HOST = 8;
    const int UI_BUTTON_REFRESH = 9;
    const int UI_BUTTON_QUIT = 10;
    const int UI_BUTTON_NEXT = 11;
    const int UI_BUTTON_PREV = 12;
    const int UI_BUTTON_FIRST = 13;
    const int UI_BUTTON_LAST = 14;
    const int UI_BUTTON_PASS = 15;
    const int UI_BUTTON_RESIGN = 16;
    const int UI_BUTTON_COUNT = 17;
    const int UI_BUTTON_READY = 18;
    const int UI_BUTTON_SCORE = 19;
    const int UI_BUTTON_OPTION = 20;
    const int UI_BUTTON_LEAVE = 21;

    const int UI_FIELD_NOSELECTION = -1;
    const int UI_FIELD_NAME_REG = 0;
    const int UI_FIELD_PASS_REG = 1;
    const int UI_FIELD_CONFIRM = 2;
    const int UI_FIELD_COUNTRY = 3;
    const int UI_FIELD_NAME_LOGIN = 4;
    const int UI_FIELD_PASS_LOGIN = 5;
    const int UI_FIELD_LOBBY_MESSAGE = 6;
    const int UI_FIELD_GAME_MESSAGE = 7;
    const int UI_FIELD_TIME = 8;
    const int UI_FIELD_INCREMENT = 9;

    const int GAME_SIZE_S = 9;
    const int GAME_SIZE_M = 13;
    const int GAME_SIZE_L = 19;

    const int STONE_BLACK = 0;
    const int STONE_WHITE = 1;
    const int STONE_RANDOM = 2;
}

namespace ui_component{
    extern bool has_changed;
    extern int mode;

    extern std::string currentUser;
    extern Game currentGame;
    extern int currentAction;
    extern int authentication_code;
    extern std::string lobby_stamp;
    extern std::string game_stamp;

    extern int SELECTED_BUTTON;
    extern int SELECTED_FIELD;
    extern char* SELECTED_BUFFER;
    extern int* SELECTED_BUFFER_PTR;

    extern int LOGIN_TEXT_COLOR;
    extern int REGISTER_TEXT_COLOR;
    extern int EXIT_TEXT_COLOR;
    extern int LOGIN_BUTTON_COLOR;
    extern int REGISTER_BUTTON_COLOR;
    extern int EXIT_BUTTON_COLOR;
    extern int SUBMIT_BUTTON_COLOR;
    extern int SUBMIT_TEXT_COLOR;
    extern int HELP_BUTTON_COLOR;
    extern int HELP_TEXT_COLOR;

    extern char USERNAME_BUFFER[];
    extern char PASSWORD_BUFFER[];
    extern char CONFIRM_BUFFER[];
    extern char COUNTRY_BUFFER[];
    extern char MESSAGE_BUFFER[];
    extern int USERNAME_PTR;
    extern int PASSWORD_PTR;
    extern int CONFIRM_PTR;
    extern int COUNTRY_PTR;
    extern int MESSAGE_PTR;

    extern std::string sys_message;
    extern std::vector<Game_Descriptor> GAME_LIST;
    extern std::vector<Player> PLAYER_LIST;
    extern std::vector<Player>::size_type PLAYER_OFFSET;
    extern std::vector<std::string> LOBBY_MESSAGE_BOARD;
    extern std::vector<std::string> GAME_MESSAGE_BOARD;
    extern std::vector<std::string>::size_type LOBBY_MESSAGE_OFFSET;
    extern std::vector<std::string>::size_type GAME_MESSAGE_OFFSET;
    extern GLfloat LOBBY_QUAD_VERTEX[];
    extern GLfloat LOBBY_QUAD_COLOR[];
    extern GLfloat LOBBY_BORDER_VERTEX[];
    extern GLfloat LOBBY_BORDER_COLOR[];

    extern int SELECTED_GAME;  // -1 TO 9
    extern int SELECTED_PAGE;
    extern bool IS_MESSAGING;

    extern GLfloat OPTION_QUAD_VERTEX[];
    extern GLfloat OPTION_QUAD_COLOR[];
    extern GLfloat OPTION_BORDER_VERTEX[];
    extern GLfloat OPTION_BORDER_COLOR[];
    extern int SELECTED_SIZE;
    extern int SELECTED_STONE;
    extern char TIME_TOTAL[];
    extern char TIME_INCREMENT[];
    extern int TIME_TOTAL_PTR;
    extern int TIME_INCREMENT_PTR;
    extern bool IS_SPECTATOR_ON;

    extern GLfloat BOARD_VERTEX[];
    extern GLfloat STONE_VERTEX[];
    extern GLfloat STAR_VERTEX[];
    extern GLfloat STAR_LOC[];
    extern GLfloat CROSS_VERTEX[];
    extern GLfloat CROSS_COLOR[];
    extern GLfloat GAME_UI_VERTEX[];
    extern GLfloat GAME_UI_COLOR[];
    extern GLfloat GAME_BORDER_VERTEX[];
    extern GLfloat GAME_BORDER_COLOR[];

    extern int COORINDATE_X;
    extern int COORINDATE_Y;

    extern bool IS_OPTION_ON;
}

namespace ui_event
{
    bool isMouseOverButton(int, int, int);
    bool isMouseOverField(int, int, int);
    bool isMouseOverBoard(int, int, int, int);
    void grabCoordinates(int, int);
    void appendMessage(std::vector<std::string>*, std::string, float);

    int getLobbyButton(float, float);
    int getSelectedGame(float, float);
    void setBufferSelection(float, float);
    void selectGame(int);

    int getGameButton(float, float);

    void handleClickInStartScreen(int, int);
    void handleClickInLobbyScreen(int, int);
    void handleScrollInLobbyScreen(int);
    void handleClickInGameScreen(int, int);
    void handleScrollInGameScreen(int);
    void handleClickInOptionPanel(float, float);

    void handleKeyInputStartScreen(unsigned char);
    void handleKeyInputLobbyScreen(unsigned char);
    void handleKeyInputGameScreen(unsigned char);
    void handleKeyInputOptionPanel(unsigned char);
}

namespace ui_render
{
    void init();
    void render_start_screen();
    void render_lobby_screen();
    void render_game_screen();
    void set_color(int);
    void set_background(int);
    void lobby_setup_interface();
    void game_setup_interface();
    void game_update_interface();

    void setup_rect(GLfloat*, int, float, float, float, float);
    void setup_rectcolor(GLfloat*, int, float, float, float, float, bool);
    void setup_border(GLfloat*, int, float, float, float, float);
    void setup_border_color(GLfloat*, int, float, float, float, float);

    void render_option_panel();
    void setup_option_panel();

    void render_help();
    void render_login();
    void render_registration();
    void render_menu_area();
    void render_join_buttons();
    void render_circle(float, float, int);
    void render_text_indicator(int, int);
    void render_games();
    void render_users(float, float, float, float);
    void render_text_area(std::vector<std::string>*, float, float, float, float,
                          std::vector<std::string>::size_type);
    void render_checkmark(float, float);
    void render_gameboard();
    void render_counting();
    void update_counting();
    void render_starpoint(float, float);
    void render_stone(int, int);
    void render_cursor_indicator();
}

//create the initial UI window
void UI_initialize(int, char*[]);

//reset the window size to default
void UI_resize(int, int);

//define display callback
void UI_display();

//define idle callback
void UI_idle();

//defines the timer callback
void UI_timer(int);

//defines on close
void UI_onclose();

//defines mouse input callback
void UI_mouse(int, int, int, int);

//defines mouse motion callback
void UI_passive_mouse(int, int);

//defines keyboard input callback
void UI_keyboard(unsigned char, int, int);


#endif // UIMANAGER_H
