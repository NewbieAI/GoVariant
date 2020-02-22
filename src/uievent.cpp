#include "uimanager.h"
#include "networkmanager.h"
//#include <GL/glut.h>
#include <GL/freeglut.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <game.h>

//ui_event functions
bool ui_event::isMouseOverButton(int component, int x, int y)
{
    int x_center, y_center;
    switch (component){
    case ui_const::UI_BUTTON_LOGIN:
        x_center = 200;
        y_center = 200;
        break;
    case ui_const::UI_BUTTON_REGISTER:
        x_center = 200;
        y_center = 100;
        break;
    case ui_const::UI_BUTTON_SUBMIT:
        x_center = 300;
        y_center = 200;
        break;
    case ui_const::UI_BUTTON_EXIT:
        x_center = 200;
        y_center = 400;
        break;
    case ui_const::UI_BUTTON_HELP:
        x_center = 100;
        y_center = 200;
        break;
    }
    return (x - x_center) * (x - x_center) + (y - y_center) * (y - y_center) <= 45 * 45;
}

bool ui_event::isMouseOverField(int component, int x, int y)
{
    float xf = x / 400.0 - 1, yf = 1 - y / 250.0;

    switch(component){
    case ui_const::UI_FIELD_NAME_LOGIN:
        return xf >= 0.25 && xf <= 0.75 && yf >= 0.20 && yf <= 0.32;
        break;
    case ui_const::UI_FIELD_PASS_LOGIN:
        return xf >= 0.25 && xf <= 0.75 && yf >= -0.32 && yf <= -0.20;
        break;
    case ui_const::UI_FIELD_NAME_REG:
        return xf >= 0.25 && xf <= 0.75 && yf >= 0.48 && yf <= 0.60;
        break;
    case ui_const::UI_FIELD_PASS_REG:
        return xf >= 0.25 && xf <= 0.75 && yf >= 0.08 && yf <= 0.20;
        break;
    case ui_const::UI_FIELD_CONFIRM:
        return xf >= 0.25 && xf <= 0.75 && yf >= -0.32 && yf <= -0.20;
        break;
    case ui_const::UI_FIELD_COUNTRY:
        return xf >= 0.25 && xf <= 0.75 && yf >= -0.72 && yf <= -0.60;
        break;
    }
    return false;
}

void ui_event::grabCoordinates(int x, int y)
{
    float cursor_x = x * (ui_component::currentGame.size() + 1) / 420.0 - 0.5;
    float cursor_y = y * (ui_component::currentGame.size() + 1) / 420.0 - 0.5;

    if (cursor_x >= 0 && cursor_x < ui_component::currentGame.size() &&
        cursor_y >= 0 && cursor_y < ui_component::currentGame.size()) {
        ui_component::COORINDATE_X = (int)floorf(cursor_x);
        ui_component::COORINDATE_Y = (int)floorf(cursor_y);
    } else {
        ui_component::COORINDATE_X = -1;
        ui_component::COORINDATE_Y = -1;
    }
}

int ui_event::getSelectedGame(float x, float y)
{
    float xmin, ymin, xmax, ymax;
    for (int i = 0; i < 10; i++){
        xmin = -0.975 + (i % 2) * 0.625;
        xmax = -0.375 + (i % 2) * 0.625;
        ymin = 0.76 - int(i / 2) * 0.24;
        ymax = 0.96 - int(i / 2) * 0.24;
        if (x >= xmin && x <= xmax && y >= ymin && y <= ymax) {
            return i;
        }
    }
    return -1;
}

int ui_event::getLobbyButton(float x, float y)
{
    if (ui_component::SELECTED_GAME >= 0 &&
        ui_component::SELECTED_GAME + ui_component::SELECTED_PAGE * 10 < ui_component::GAME_LIST.size()) {
        if (x >= -0.50 + (ui_component::SELECTED_GAME % 2) * 0.625 &&
            y <= 0.94 - int(ui_component::SELECTED_GAME / 2) * 0.24 &&
            x <= -0.40 + (ui_component::SELECTED_GAME % 2) * 0.625 &&
            y >= 0.86 - int(ui_component::SELECTED_GAME / 2) * 0.24) {
            return ui_const::UI_BUTTON_JOIN;
        }
        if (x >= -0.50 + (ui_component::SELECTED_GAME % 2) * 0.625 &&
            y <= 0.86 - int(ui_component::SELECTED_GAME / 2) * 0.24 &&
            x <= -0.40 + (ui_component::SELECTED_GAME % 2) * 0.625 &&
            y >= 0.78 - int(ui_component::SELECTED_GAME / 2) * 0.24) {
            return ui_const::UI_BUTTON_SPECTATE;
        }
    }
    if (x >= 0.15 && x <= 0.25 && y >= -0.96 && y <= -0.88) {
        return ui_const::UI_BUTTON_SEND;
    }
    if (x >= 0.35 && x <= 0.975 && y >= -0.76 && y <= -0.56) {
        return ui_const::UI_BUTTON_HOST;
    }
    if (x >= 0.35 && x <= 0.65 && y >= -0.96 && y <= -0.80) {
        return ui_const::UI_BUTTON_REFRESH;
    }
    if (false) {
        return ui_const::UI_BUTTON_NEXT;
    }
    if (false) {
        return ui_const::UI_BUTTON_PREV;
    }
    if (x >= 0.675 && x <= 0.975 && y >= -0.96 && y <= -0.80) {
        return ui_const::UI_BUTTON_QUIT;
    }
    return -1;
}

void ui_event::setBufferSelection(float x, float y)
{
    switch (ui_component::mode) {
    case ui_const::UI_LOBBY:
        if (x >= -0.975 && x <= 0.125&& y >= -0.96 && y <= -0.88) {
            ui_component::IS_MESSAGING = true;
        } else {
            ui_component::IS_MESSAGING = false;
        }
        break;
    case ui_const::UI_GAME:
        if (x >= 0.10 && x <= 0.825 && y >= -0.90 && y <= -0.82) {
            ui_component::IS_MESSAGING = true;
        } else {
            ui_component::IS_MESSAGING = false;
        }
        break;
    }
}

void ui_event::selectGame(int gameNumber)
{
    if (ui_component::SELECTED_GAME >= 0)
    {
        ui_render::setup_border_color(ui_component::LOBBY_BORDER_COLOR,
                                      ui_component::SELECTED_GAME * 32,
                                      0.0, 0.2, 0.1, 0.8);
    } // sets the current selected game back to normal

    if (gameNumber >= 0){
        ui_render::setup_rect(ui_component::LOBBY_QUAD_VERTEX,
                              8 * 17,
                              -0.40 + (gameNumber % 2) * 0.625,
                              0.94 - int(gameNumber / 2) * 0.24,
                              -0.50 + (gameNumber % 2) * 0.625,
                              0.86 - int(gameNumber / 2) * 0.24);
        ui_render::setup_rect(ui_component::LOBBY_QUAD_VERTEX,
                              8 * 18,
                              -0.40 + (gameNumber % 2) * 0.625,
                              0.86 - int(gameNumber / 2) * 0.24,
                              -0.50 + (gameNumber % 2) * 0.625,
                              0.78 - int(gameNumber / 2) * 0.24);
        ui_render::setup_rectcolor(ui_component::LOBBY_QUAD_COLOR,
                                   16 * 17, 0.8, 0.4, 0.0, 1.0, true);
        ui_render::setup_rectcolor(ui_component::LOBBY_QUAD_COLOR,
                                   16 * 18, 0.0, 0.4, 0.8, 1.0, true);
        ui_render::setup_border_color(ui_component::LOBBY_BORDER_COLOR,
                                      gameNumber * 32, 1.0, 0.9, 0.9, 0.8);
    } else {
        ui_render::setup_rectcolor(ui_component::LOBBY_QUAD_COLOR,
                                   16 * 17, 1.0, 1.0, 1.0, 0.0, false);
        ui_render::setup_rectcolor(ui_component::LOBBY_QUAD_COLOR,
                                   16 * 18, 1.0, 1.0, 1.0, 0.0, false);
    }

    //printf("selecting game %d\n", gameNumber);
    ui_component::SELECTED_GAME = gameNumber;

}

int ui_event::getGameButton(float x, float y)
{
    if (x >= -0.96 && x <= -0.91 && y >= -0.88 && y <= -0.80){
        // goto first move
        return ui_const::UI_BUTTON_FIRST;
    }
    if (x >= -0.715 && x <= -0.665 && y >= -0.88 && y <= -0.80){
        // goto most recent move
        return ui_const::UI_BUTTON_LAST;
    }
    if (x >= -0.875 && x <= -0.835 && y >= -0.88 && y <= -0.80){
        // goto next move
        return ui_const::UI_BUTTON_PREV;
    }
    if (x >= -0.79 && x <= -0.75 && y >= -0.88 && y <= -0.80){
        // goto previous move
        return ui_const::UI_BUTTON_NEXT;
    }

    if (x >= -0.550 && x <= -0.400 && y >= -0.90 && y <= -0.78){
        // pass
        return ui_const::UI_BUTTON_PASS;
    }
    if (x >= -0.325 && x <= -0.175 && y >= -0.90 && y <= -0.78){
        // resign
        return ui_const::UI_BUTTON_RESIGN;
    }
    if (x >= -0.100 && x <= 0.050 && y >= -0.90 && y <= -0.78){
        // resign
        return ui_const::UI_BUTTON_COUNT;
    }

    if (x >= 0.10 && x <= 0.30 && y >= 0.55 && y <= 0.68){
        // ready
        return ui_const::UI_BUTTON_READY;
    }

    if (x >= 0.10 && x <= 0.30 && y >= 0.36 && y <= 0.49){
        // send score request
        return ui_const::UI_BUTTON_SCORE;
    }

    if (x >= 0.10 && x <= 0.30 && y >= 0.17 && y <= 0.30){
        // change options
        return ui_const::UI_BUTTON_OPTION;
    }

    if (x >= 0.10 && x <= 0.30 && y >= -0.02 && y <= 0.11){
        // leaves game
        return ui_const::UI_BUTTON_LEAVE;
    }

    if (x >= 0.85 && x <= 0.95 && y >= -0.90 && y <= -0.82){
        // sends message
        return ui_const::UI_BUTTON_SEND;
    }

    return -1;

}

void ui_event::handleClickInStartScreen(int x, int y)
{
    using namespace ui_component;
    if (ui_event::isMouseOverButton(ui_const::UI_BUTTON_LOGIN, x, y)){
        SELECTED_BUTTON = ui_const::UI_BUTTON_LOGIN;
        LOGIN_BUTTON_COLOR = ui_const::UI_COLOR_SELECTED_BUTTON;
        REGISTER_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;
        HELP_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;
        SELECTED_FIELD = ui_const::UI_FIELD_NAME_LOGIN;
        SELECTED_BUFFER = USERNAME_BUFFER;
        SELECTED_BUFFER_PTR = &USERNAME_PTR;
        goto check_field;
    }
    if (ui_event::isMouseOverButton(ui_const::UI_BUTTON_REGISTER, x, y)){
        SELECTED_BUTTON = ui_const::UI_BUTTON_REGISTER;
        REGISTER_BUTTON_COLOR = ui_const::UI_COLOR_SELECTED_BUTTON;
        LOGIN_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;
        HELP_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;
        SELECTED_FIELD = ui_const::UI_FIELD_NAME_REG;
        SELECTED_BUFFER = USERNAME_BUFFER;
        USERNAME_BUFFER[0] = '\0';
        USERNAME_PTR = 0;
        PASSWORD_BUFFER[0] = '\0';
        PASSWORD_PTR = 0;
        CONFIRM_BUFFER[0] = '\0';
        CONFIRM_PTR = 0;
        SELECTED_BUFFER_PTR = &USERNAME_PTR;
        goto check_field;
    }
    if (isMouseOverButton(ui_const::UI_BUTTON_EXIT, x, y)){
        glutLeaveMainLoop();
        //Network::unwind();
        return;
    }
    if (isMouseOverButton(ui_const::UI_BUTTON_SUBMIT, x, y)){
        //printf("information submitted to server\n");
        if (SELECTED_BUTTON == ui_const::UI_BUTTON_LOGIN){
            ////* transfer user to the lobby page.

            //*/
            Network::form_request(Network::REQUEST_LOGIN);
            Network::make_request();
            ui_component::sys_message = "Logging in...";
            ui_component::currentUser = std::string(ui_component::USERNAME_BUFFER);
            ui_component::lobby_stamp = "9999999:9999999";

        } else if (SELECTED_BUTTON == ui_const::UI_BUTTON_REGISTER) {
            // sends a register request to the server
            //printf("%s\n", PASSWORD_BUFFER);
            //printf("%s\n", CONFIRM_BUFFER);
            if (strcmp(PASSWORD_BUFFER, CONFIRM_BUFFER) == 0) {
                Network::form_request(Network::REQUEST_REGISTER);
                Network::make_request();
                ui_component::sys_message = "Registering...";
            } else {
                ui_component::sys_message = "Passwords do not agree";
            }
        }
    }
    if (isMouseOverButton(ui_const::UI_BUTTON_HELP, x, y)){
        ui_component::SELECTED_BUTTON = ui_const::UI_BUTTON_HELP;
        ui_component::HELP_BUTTON_COLOR = ui_const::UI_COLOR_SELECTED_BUTTON;
        ui_component::LOGIN_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;
        ui_component::REGISTER_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;
    }

    // change of page inside parse function, which is called during the idle function
    // the clicking event should only send request to the server

    // set selected text field based on incoming click
check_field:
    switch (ui_component::SELECTED_BUTTON){
    case ui_const::UI_BUTTON_LOGIN:
        if (isMouseOverField(ui_const::UI_FIELD_NAME_LOGIN, x, y)) {
            ui_component::SELECTED_FIELD = ui_const::UI_FIELD_NAME_LOGIN;
            ui_component::SELECTED_BUFFER = ui_component::USERNAME_BUFFER;
            ui_component::SELECTED_BUFFER_PTR = &ui_component::USERNAME_PTR;
            break;
        }
        if (isMouseOverField(ui_const::UI_FIELD_PASS_LOGIN, x, y)) {
            ui_component::SELECTED_FIELD = ui_const::UI_FIELD_PASS_LOGIN;
            ui_component::SELECTED_BUFFER = ui_component::PASSWORD_BUFFER;
            ui_component::SELECTED_BUFFER_PTR = &ui_component::PASSWORD_PTR;
            break;
        }
        break;
    case ui_const::UI_BUTTON_REGISTER:
        if (isMouseOverField(ui_const::UI_FIELD_NAME_REG, x, y)) {
            ui_component::SELECTED_FIELD = ui_const::UI_FIELD_NAME_REG;
            ui_component::SELECTED_BUFFER = ui_component::USERNAME_BUFFER;
            ui_component::SELECTED_BUFFER_PTR = &ui_component::USERNAME_PTR;
            break;
        }
        if (isMouseOverField(ui_const::UI_FIELD_PASS_REG, x, y)) {
            ui_component::SELECTED_FIELD = ui_const::UI_FIELD_PASS_REG;
            ui_component::SELECTED_BUFFER = ui_component::PASSWORD_BUFFER;
            ui_component::SELECTED_BUFFER_PTR = &ui_component::PASSWORD_PTR;
            break;
        }
        if (isMouseOverField(ui_const::UI_FIELD_CONFIRM, x, y)) {
            ui_component::SELECTED_FIELD = ui_const::UI_FIELD_CONFIRM;
            ui_component::SELECTED_BUFFER = ui_component::CONFIRM_BUFFER;
            ui_component::SELECTED_BUFFER_PTR = &ui_component::CONFIRM_PTR;
            break;
        }
        if (isMouseOverField(ui_const::UI_FIELD_COUNTRY, x, y)) {
            ui_component::SELECTED_FIELD = ui_const::UI_FIELD_COUNTRY;
            ui_component::SELECTED_BUFFER = ui_component::COUNTRY_BUFFER;
            ui_component::SELECTED_BUFFER_PTR = &ui_component::COUNTRY_PTR;
            break;
        }
        break;
    case ui_const::UI_BUTTON_HELP:
        if (false) {
            // goto next page
        }
        if (false) {
            // go to previous page
        }
        break;
    }
}

void ui_event::handleClickInLobbyScreen(int x, int y)
{
    float xf = (x - 400) / 400.0, yf = (250 - y) / 250.0;
    if (ui_component::IS_OPTION_ON) {
        handleClickInOptionPanel(xf, yf);
        return;
    }

    std::vector<Player>::size_type a = ui_component::SELECTED_GAME + ui_component::SELECTED_PAGE * 10;
    int time_total, time_inc;
    switch (getLobbyButton(xf, yf)){

    case ui_const::UI_BUTTON_JOIN:
        printf("join button clicked\n");
        ui_component::GAME_LIST[a].set_time(&time_total, &time_inc);
        ui_component::currentGame = Game(Game::status_player, // status
                                         ui_component::GAME_LIST[a].size(), // gamesize
                                         ui_component::GAME_LIST[a].color(), // hostcolor
                                         time_total, // time total
                                         time_inc, // time increment
                                         ui_component::GAME_LIST[a].spectator()); // allow spectator
        ui_component::currentGame.reset();
        ui_render::game_setup_interface();
        Network::form_request(Network::REQUEST_JOINGAME);
        Network::make_request();
        break;
    case ui_const::UI_BUTTON_SPECTATE:
        printf("spectate button clicked\n");
        ui_component::GAME_LIST[a].set_time(&time_total, &time_inc);
        ui_component::currentGame = Game(Game::status_spectator, // status
                                         ui_component::GAME_LIST[a].size(), // gamesize
                                         ui_component::GAME_LIST[a].color(), // hostcolor
                                         time_total, // time total
                                         time_inc, // time increment
                                         ui_component::GAME_LIST[a].spectator()); // allow spectator
        ui_component::currentGame.reset();
        ui_render::game_setup_interface();
        Network::form_request(Network::REQUEST_SPECTATE);
        Network::make_request();
        break;

    case ui_const::UI_BUTTON_SEND:
        Network::form_request(Network::REQUEST_MESSAGE);
        Network::make_request();
        ui_component::LOBBY_MESSAGE_OFFSET = 0;
        *ui_component::SELECTED_BUFFER_PTR = 0;
        ui_component::SELECTED_BUFFER[0] = '\0';
        break;
    case ui_const::UI_BUTTON_HOST:
        // temporary implementations
        //printf("this should turn on options\n");
        ui_component::IS_OPTION_ON = true;
        break;
    case ui_const::UI_BUTTON_REFRESH:
        // attempts reconnection
        break;
    case ui_const::UI_BUTTON_PREV:
        break;
    case ui_const::UI_BUTTON_NEXT:
        break;
    case ui_const::UI_BUTTON_QUIT:

        Network::form_request(Network::REQUEST_LOGOUT);
        Network::make_request();
        ui_component::LOBBY_MESSAGE_BOARD.clear();

        break;
    }

    selectGame(getSelectedGame(xf, yf));
    setBufferSelection(xf, yf);

}

void ui_event::handleClickInGameScreen(int x, int y)
{
    using namespace ui_component;
    float xf, yf;
    xf = (x - 400) / 400.0;
    yf = (250 - y) / 250.0;
    char words[100];

    int action = currentGame.size() * COORINDATE_X + COORINDATE_Y;
    if (action >= 0){
        if (currentGame.is_counting()) {
            currentGame.flip_group(action);
            ui_render::update_counting();
            //currentGame.printBoard();
        } else {
            if (currentGame.isLegal(action)) {
                currentAction = action;
                Network::form_request(Network::REQUEST_PLAY);
                Network::make_request();
            } else {
                currentGame.printGroup(action);
            }
        }
    } else {

        switch(getGameButton(xf, yf)){
        case ui_const::UI_BUTTON_FIRST:
            //printf("first is clicked\n");
            currentGame.goto_first();
            break;
        case ui_const::UI_BUTTON_LAST:
            //printf("last is clicked\n");
            currentGame.goto_last();
            break;
        case ui_const::UI_BUTTON_PREV:
            //printf("prev is clicked\n");
            currentGame.goto_prev();
            break;
        case ui_const::UI_BUTTON_NEXT:
            //printf("next is clicked\n");
            currentGame.goto_next();
            break;
        case ui_const::UI_BUTTON_PASS:
            //printf("pass is clicked\n");
            break;
        case ui_const::UI_BUTTON_RESIGN:
            //printf("resign is clicked\n");
            break;
        case ui_const::UI_BUTTON_COUNT:
            printf("count is clicked\n");
            currentGame.set_counting(!currentGame.is_counting());
            if (currentGame.is_counting()){
                currentGame.start_counting();
                ui_render::update_counting();
            }

            break;
        case ui_const::UI_BUTTON_READY:
            printf("ready is clicked\n");

            sprintf(words, "rhostcolor = %d, status = %d", currentGame.host_color(), currentGame.status());
            ui_event::appendMessage(&ui_component::GAME_MESSAGE_BOARD, words,0.85);

            if ((currentGame.host_color() == Game::player_white) ^
                (currentGame.status() == Game::status_host)) {
                currentGame.set_blackscore(currentGame.score_black() % 2 == -1 ? -2 : -1);
            } else {
                currentGame.set_whitescore(currentGame.score_white() % 2 == -1 ? -4 : -3);
            }

            Network::form_request(Network::REQUEST_SCORE);
            Network::make_request();

            break;
        case ui_const::UI_BUTTON_SCORE:
            printf("score is clicked\n");

            Network::form_request(Network::REQUEST_SCORE);
            Network::make_request();
            break;
        case ui_const::UI_BUTTON_OPTION:
            //printf("option is clicked\n");
            break;
        case ui_const::UI_BUTTON_JOIN:
            //printf("join is clicked\n");
            break;
        case ui_const::UI_BUTTON_SPECTATE:
            //printf("spectate is clicked\n");
            break;
        case ui_const::UI_BUTTON_LEAVE:
            //printf("leave is clicked\n");
            Network::form_request(Network::REQUEST_LEAVE);
            Network::make_request();
            LOBBY_MESSAGE_BOARD.clear();
            break;
        case ui_const::UI_BUTTON_SEND:
            Network::form_request(Network::REQUEST_MESSAGE);
            Network::make_request();
            GAME_MESSAGE_OFFSET = 0;
            *SELECTED_BUFFER_PTR = 0;
            SELECTED_BUFFER[0] = '\0';
            break;
        }

    }

    setBufferSelection(xf, yf);
}

void ui_event::handleClickInOptionPanel(float xf, float yf)
{
    // check if board size setting is clicked
    if (xf >= -0.700 && xf <= -0.525 && yf >= 0.24 && yf <= 0.32) {
        //printf("setting board size to 9x9\n");
        ui_component::SELECTED_SIZE = ui_const::GAME_SIZE_S;
        goto wrap_up;
    }
    if (xf >= -0.500 && xf <= -0.325 && yf >= 0.24 && yf <= 0.32) {
        //printf("setting board size to 13x13\n");
        ui_component::SELECTED_SIZE = ui_const::GAME_SIZE_M;
        goto wrap_up;
    }
    if (xf >= -0.300 && xf <= -0.125 && yf >= 0.24 && yf <= 0.32) {
        //printf("setting board size to 19x19\n");
        ui_component::SELECTED_SIZE = ui_const::GAME_SIZE_L;
        goto wrap_up;
    }

    // check if stone color setting is clicked
    if (xf >= -0.700 && xf <= -0.525 && yf >= 0.04 && yf <= 0.12) {
        //printf("Play as black\n");
        ui_component::SELECTED_STONE = ui_const::STONE_BLACK;
        goto wrap_up;
    }
    if (xf >= -0.500 && xf <= -0.325 && yf >= 0.04 && yf <= 0.12) {
        //printf("Play as white\n");
        ui_component::SELECTED_STONE = ui_const::STONE_WHITE;
        goto wrap_up;
    }
    if (xf >= -0.300 && xf <= -0.125 && yf >= 0.04 && yf <= 0.12) {
        //printf("Play as random\n");
        ui_component::SELECTED_STONE = ui_const::STONE_RANDOM;
        goto wrap_up;
    }

    // check if either time buffer is clicked
    if (xf >= -0.675 && xf <= -0.575 && yf >= -0.18 && yf <= -0.10) {
        //printf("setting total time\n");
        ui_component::SELECTED_FIELD = ui_const::UI_FIELD_TIME;
        ui_component::SELECTED_BUFFER = ui_component::TIME_TOTAL;
        ui_component::SELECTED_BUFFER_PTR = &ui_component::TIME_TOTAL_PTR;
        return;
    }
    if (xf >= -0.350 && xf <= -0.150 && yf >= -0.18 && yf <= -0.10) {
        //printf("setting time increment\n");
        ui_component::SELECTED_FIELD = ui_const::UI_FIELD_INCREMENT;
        ui_component::SELECTED_BUFFER = ui_component::TIME_INCREMENT;
        ui_component::SELECTED_BUFFER_PTR = &ui_component::TIME_INCREMENT_PTR;
        return;
    }

    // check if spectator setting is clicked
    if (xf >= -0.275 && xf <= -0.200 && yf >= -0.32 && yf <= -0.24) {
        //printf("setting spectator setting\n");
        ui_component::IS_SPECTATOR_ON = !ui_component::IS_SPECTATOR_ON;
        goto wrap_up;
    }

    // check if "CONFIRM" or "CANCEL" is clicked
    if (xf >= -0.575 && xf <= -0.425 && yf >= -0.50 && yf <= -0.40) {
        printf("confirm is clicked\n");
        ui_component::currentGame = Game(Game::status_host,
                                         ui_component::SELECTED_SIZE,
                                         ui_component::SELECTED_STONE,
                                         atoi(ui_component::TIME_TOTAL),
                                         atoi(ui_component::TIME_INCREMENT),
                                         ui_component::IS_SPECTATOR_ON);
        ui_component::currentGame.reset();
        ui_render::game_setup_interface();
        printf("game initialized\n");
        // temporarily changed
        Network::form_request(Network::REQUEST_HOSTGAME);
        Network::make_request();
        printf("request sent\n");
        goto wrap_up;
    }

    if (xf >= -0.325 && xf <= -0.175 && yf >= -0.50 && yf <= -0.40) {
        ui_component::IS_OPTION_ON = false;
    }

wrap_up:
    ui_component::SELECTED_FIELD = ui_const::UI_FIELD_NOSELECTION;
    ui_component::SELECTED_BUFFER = ui_component::MESSAGE_BUFFER;
    ui_component::SELECTED_BUFFER_PTR = &ui_component::MESSAGE_PTR;
}

void ui_event::handleKeyInputStartScreen(unsigned char c)
{
    if (c == '\b'){
        if (*ui_component::SELECTED_BUFFER_PTR > 0) {
            (*ui_component::SELECTED_BUFFER_PTR)--;
            ui_component::SELECTED_BUFFER[*ui_component::SELECTED_BUFFER_PTR] = '\0';
            ui_component::has_changed = true;
        }
    } else if (*ui_component::SELECTED_BUFFER_PTR < 31 &&
               c > 31 && c < 127) {
        ui_component::SELECTED_BUFFER[*ui_component::SELECTED_BUFFER_PTR] = c;
        (*ui_component::SELECTED_BUFFER_PTR)++;
        ui_component::SELECTED_BUFFER[*ui_component::SELECTED_BUFFER_PTR] = '\0';
        ui_component::has_changed = true;
    }
}

void ui_event::handleKeyInputLobbyScreen(unsigned char c)
{
    if (ui_component::IS_OPTION_ON) {
        handleKeyInputOptionPanel(c);
        return;
    }
    if (ui_component::IS_MESSAGING) {
        switch (c){
        case 8:
            if (*ui_component::SELECTED_BUFFER_PTR > 0) {
                ui_component::SELECTED_BUFFER[--(*ui_component::SELECTED_BUFFER_PTR)] = '\0';
            }
            break;
        case 13:
            Network::form_request(Network::REQUEST_MESSAGE);
            Network::make_request();
            ui_component::LOBBY_MESSAGE_OFFSET = 0;
            *ui_component::SELECTED_BUFFER_PTR = 0;
            ui_component::SELECTED_BUFFER[0] = '\0';
            break;
        default:
            if (*ui_component::SELECTED_BUFFER_PTR < 127 &&
                c > 31 && c < 127) {
                ui_component::SELECTED_BUFFER[*ui_component::SELECTED_BUFFER_PTR] = c;
                ui_component::SELECTED_BUFFER[++(*ui_component::SELECTED_BUFFER_PTR)] = '\0';
            }
        }
        ui_component::has_changed = true;
        //printf("selected buffer contains: %s\n", ui_component::SELECTED_BUFFER);
    }
}

void ui_event::handleScrollInLobbyScreen(int button)
{
    std::vector<std::string>::size_type max_offset;
    if (ui_component::LOBBY_MESSAGE_BOARD.size() > 11){
        max_offset = ui_component::LOBBY_MESSAGE_BOARD.size() - 11;
    } else {
        max_offset = 0;
    }

    if (button == 3 && ui_component::LOBBY_MESSAGE_OFFSET < max_offset){
        ui_component::LOBBY_MESSAGE_OFFSET++;
        //printf("offset = %d", ui_component::LOBBY_MESSAGE_OFFSET);
    }
    if (button == 4 && ui_component::LOBBY_MESSAGE_OFFSET > 0){
        ui_component::LOBBY_MESSAGE_OFFSET--;
        //printf("offset = %d", ui_component::LOBBY_MESSAGE_OFFSET);
    }
}

void ui_event::handleScrollInGameScreen(int button)
{

}

void ui_event::handleKeyInputGameScreen(unsigned char c)
{
    if (ui_component::IS_MESSAGING) {
        switch (c){
        case 8:
            if (*ui_component::SELECTED_BUFFER_PTR > 0) {
                ui_component::SELECTED_BUFFER[--(*ui_component::SELECTED_BUFFER_PTR)] = '\0';
            }
            break;
        case 13:
            Network::form_request(Network::REQUEST_MESSAGE);
            Network::make_request();
            ui_component::GAME_MESSAGE_OFFSET = 0;
            *ui_component::SELECTED_BUFFER_PTR = 0;
            ui_component::SELECTED_BUFFER[0] = '\0';
            break;
        default:
            if (*ui_component::SELECTED_BUFFER_PTR < 127 &&
                c > 31 && c < 127) {
                ui_component::SELECTED_BUFFER[*ui_component::SELECTED_BUFFER_PTR] = c;
                ui_component::SELECTED_BUFFER[++(*ui_component::SELECTED_BUFFER_PTR)] = '\0';
            }
        }
        ui_component::has_changed = true;
    }
}

void ui_event::handleKeyInputOptionPanel(unsigned char c)
{
    if (ui_component::SELECTED_BUFFER != ui_component::MESSAGE_BUFFER){
        if (c == 8 && *ui_component::SELECTED_BUFFER_PTR > 0) {
            ui_component::SELECTED_BUFFER[--(*ui_component::SELECTED_BUFFER_PTR)] = '\0';
        } else if (c >= 48 && c < 57 && *ui_component::SELECTED_BUFFER_PTR < 3){
            ui_component::SELECTED_BUFFER[(*ui_component::SELECTED_BUFFER_PTR)++] = c;
        }
    }
    ui_component::has_changed = true;
}

void ui_event::appendMessage(std::vector<std::string> *board,
                             std::string message,
                             float width)
{
    // breaks message into lines, left justified
    // breaks long words that could potentially
    // be url links.

    std::string::size_type max_chars = (std::string::size_type)((width - 0.01) / 0.02);
    std::string::size_type wordlen = 0, last_space = 1, start = 1;
    std::string color = message.substr(0, 1);
    for (std::string::size_type i = 1; i < message.size(); ++i){
        if (i - start + 1 == max_chars){
            if (wordlen <= 35){
                board->push_back(color + message.substr(start, last_space - start));
                start = last_space + 1;
            } else {
                board->push_back(color + message.substr(start, max_chars));
                start = i + 1;
            }

        } else if (message[i] == ' ') {
            last_space = i;
            wordlen = 0;
        } else {
            wordlen++;
        }
    }
    if (start < message.size()){
        board->push_back(color + message.substr(start));
    }

}
//*******************
