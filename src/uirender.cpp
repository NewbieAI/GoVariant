#include "uimanager.h"
//#include <GL/glut.h>
#include <GL/freeglut.h>
#include <string>
#include <stdio.h>
#include <math.h>


namespace ui_string
{
    std::string login = "LOG IN";
    std::string regist = "REGISTER";
    std::string exit = "EXIT";
    std::string submit = "SUBMIT";
    std::string help = "HELP";
    std::string username = "Username:";
    std::string password = "Password:";
    std::string confirm = "Confirm Password:";
    std::string country = "Country of Origin:";
    std::string join = "JOIN";
    std::string spectate = "WATCH";
    std::string send = "SEND";
    std::string refresh = "REFRESH";
    std::string host = "CREATE GAME";
    std::string quit = "QUIT";

    std::string option = "Choose Game Settings";
    std::string choose_size = "Board Size:";
    std::string choose_size1 = "9 x 9";
    std::string choose_size2 = "13 x 13";
    std::string choose_size3 = "19 x 19";
    std::string choose_stone = "Play as:";
    std::string choose_stone1 = "Black";
    std::string choose_stone2 = "White";
    std::string choose_stone3 = "Random";
    std::string choose_timer = "Timer:";
    std::string choose_timer1 = "min/game  +";
    std::string choose_timer2 = "sec/move";
    std::string allow_spectator = "Allow spectators:";
    std::string ok = "CONFIRM";
    std::string cancel = "CANCEL";

    std::string pass = "Pass";
    std::string resign = "Resign";
    std::string count_g = "Count";
    std::string ready = "Ready";
    std::string score = "Score";
    std::string option_g = "Option";
    std::string leave = "Leave";
    std::string join_g = "Join";
    std::string spectate_g = "Spectate";
}

//ui_render functions
void ui_render::init()
{
    ui_component::mode = ui_const::UI_START;
    ui_component::has_changed = true;

    ui_component::SELECTED_BUTTON = ui_const::UI_BUTTON_LOGIN;
    ui_component::SELECTED_FIELD = ui_const::UI_FIELD_NAME_LOGIN;
    ui_component::SELECTED_BUFFER = ui_component::USERNAME_BUFFER;
    ui_component::SELECTED_BUFFER_PTR = &ui_component::USERNAME_PTR;

    ui_component::MESSAGE_PTR = 0;
    ui_component::USERNAME_PTR = 0;
    ui_component::PASSWORD_PTR = 0;
    ui_component::CONFIRM_PTR = 0;
    ui_component::COUNTRY_PTR = 0;
    ui_component::MESSAGE_BUFFER[0] = '\0';
    ui_component::USERNAME_BUFFER[0] = '\0';
    ui_component::PASSWORD_BUFFER[0] = '\0';
    ui_component::CONFIRM_BUFFER[0] = '\0';
    ui_component::COUNTRY_BUFFER[0] = '\0';

    ui_component::sys_message = "Welcome to the Game";
    ui_component::authentication_code = -1;

    ui_component::LOGIN_TEXT_COLOR = ui_const::UI_COLOR_SELECTED_TEXT;
    ui_component::REGISTER_TEXT_COLOR = ui_const::UI_COLOR_TEXT;
    ui_component::EXIT_TEXT_COLOR = ui_const::UI_COLOR_TEXT;
    ui_component::SUBMIT_TEXT_COLOR = ui_const::UI_COLOR_TEXT;
    ui_component::HELP_TEXT_COLOR = ui_const::UI_COLOR_TEXT;
    ui_component::LOGIN_BUTTON_COLOR = ui_const::UI_COLOR_SELECTED_BUTTON;
    ui_component::REGISTER_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;
    ui_component::EXIT_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;
    ui_component::SUBMIT_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;
    ui_component::HELP_BUTTON_COLOR = ui_const::UI_COLOR_BUTTON;

    lobby_setup_interface();
    setup_option_panel();
}

void ui_render::render_start_screen()
{
    glBegin(GL_LINES);
        for (float x = -0.75; x < 1; x+=0.25) {
            glColor4f(0.0, 0.4, 0.1, 0.8);
            glVertex2f(x, -1.0);
            glColor4f(0.0, 0.4, 0.1, 0.8);
            glVertex2f(x, 1.0);
        }
        for (float y = -0.6; y < 1; y+=0.4) {
            glColor4f(0.0, 0.4, 0.1, 0.8);
            glVertex2f(-1.0, y);
            glColor4f(0.0, 0.4, 0.1, 0.8);
            glVertex2f(1.0, y);
        }
    glEnd();

    set_color(ui_component::LOGIN_BUTTON_COLOR);
    render_circle(-0.5, 0.2, 45);
    set_color(ui_component::LOGIN_TEXT_COLOR);
    glRasterPos2f(-0.57, 0.19);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::login.c_str());

    set_color(ui_component::REGISTER_BUTTON_COLOR);
    render_circle(-0.5, 0.6, 45);
    set_color(ui_component::REGISTER_TEXT_COLOR);
    glRasterPos2f(-0.59, 0.59);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::regist.c_str());

    set_color(ui_component::EXIT_BUTTON_COLOR);
    render_circle(-0.5, -0.6, 45);
    set_color(ui_component::EXIT_TEXT_COLOR);
    glRasterPos2f(-0.54, -0.61);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::exit.c_str());

    set_color(ui_component::SUBMIT_BUTTON_COLOR);
    render_circle(-0.25, 0.2, 45);
    set_color(ui_component::SUBMIT_TEXT_COLOR);
    glRasterPos2f(-0.32, 0.19);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::submit.c_str());

    set_color(ui_component::HELP_BUTTON_COLOR);
    render_circle(-0.75, 0.2, 45);
    set_color(ui_component::HELP_TEXT_COLOR);
    glRasterPos2f(-0.80, 0.19);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::help.c_str());

    switch(ui_component::SELECTED_BUTTON){
    case ui_const::UI_BUTTON_LOGIN:
        render_login();
        break;
    case ui_const::UI_BUTTON_REGISTER:
        render_registration();
        break;
    case ui_const::UI_BUTTON_HELP:
        render_help();
        break;
    }
}

void ui_render::render_lobby_screen()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, ui_component::LOBBY_QUAD_VERTEX);
        glColorPointer(4, GL_FLOAT, 0, ui_component::LOBBY_QUAD_COLOR);
        glDrawArrays(GL_QUADS, 0, 76);
        glVertexPointer(2, GL_FLOAT, 0, ui_component::LOBBY_BORDER_VERTEX);
        glColorPointer(4, GL_FLOAT, 0, ui_component::LOBBY_BORDER_COLOR);
        glDrawArrays(GL_LINES, 0, 80);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    set_color(ui_const::UI_COLOR_HIGHLIGHTED_TEXT);
    glRasterPos2f(0.17, -0.93);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,
                     (const unsigned char*)ui_string::send.c_str());

    set_color(ui_const::UI_COLOR_TEXT);
    glRasterPos2f(0.52, -0.68);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::host.c_str());

    glRasterPos2f(0.42, -0.90);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::refresh.c_str());

    glRasterPos2f(0.77, -0.90);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::quit.c_str());

    render_games();
    render_join_buttons();

    render_users(0.35, 0.975, -0.50, 0.96);
    render_text_area(&ui_component::LOBBY_MESSAGE_BOARD,
                     -0.975, 0.250, -0.84, -0.24, ui_component::LOBBY_MESSAGE_OFFSET);


    set_color(ui_const::UI_COLOR_TEXT);
    glRasterPos2f(-0.96, -0.94);
    int offset;
    offset = ui_component::MESSAGE_PTR <= 54 ? 0 : ui_component::MESSAGE_PTR - 54;
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                    (const unsigned char*)ui_component::MESSAGE_BUFFER + offset);

    if (ui_component::IS_MESSAGING){
        offset = ui_component::MESSAGE_PTR <= 54 ? ui_component::MESSAGE_PTR : 54;
        render_text_indicator(ui_const::UI_FIELD_LOBBY_MESSAGE,
                              offset);
    }

    if (ui_component::IS_OPTION_ON){
        render_option_panel();
    }

}

void ui_render::render_game_screen()
{
    render_gameboard();

    glLoadIdentity();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, ui_component::GAME_UI_VERTEX);
        glColorPointer(4, GL_FLOAT, 0, ui_component::GAME_UI_COLOR);

        glDrawArrays(GL_TRIANGLES, 0, 12);
        glDrawArrays(GL_QUADS, 12, 72);

        if (ui_component::currentGame.is_counting()) {
            render_counting();
        }

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    render_cursor_indicator();
    // render the text for 3 control buttons at the bottom
    set_color(ui_const::UI_COLOR_HIGHLIGHTED_TEXT);
    glRasterPos2f(-0.525, -0.86);
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*)ui_string::pass.c_str());
    glRasterPos2f(-0.300, -0.86);
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*)ui_string::resign.c_str());
    glRasterPos2f(-0.075, -0.86);
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*)ui_string::count_g.c_str());

    // render 4 bottoms on the left
    set_color(ui_const::UI_COLOR_TEXT);
    glRasterPos2f(0.10, 0.55);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::ready.c_str());

    glRasterPos2f(0.10, 0.36);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::score.c_str());

    glRasterPos2f(0.10, 0.17);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::option_g.c_str());

    glRasterPos2f(0.10, -0.02);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::leave.c_str());

    set_color(ui_const::UI_COLOR_HIGHLIGHTED_TEXT);
    glRasterPos2f(0.87, -0.87);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,
                     (const unsigned char*)ui_string::send.c_str());

    //render player name and timer
    char buffer[16];
    set_color(ui_const::UI_COLOR_TEXT);
    ui_component::currentGame.timer_black()->printTo(buffer);
    glRasterPos2f(0.1, 0.76);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
                     (const unsigned char*)buffer);
    ui_component::currentGame.timer_white()->printTo(buffer);
    glRasterPos2f(0.95 - (float)glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)buffer) / 400.0,
                  0.76);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
                     (const unsigned char*)buffer);

    // render user list
    render_users(0.35, 0.95, -0.02, 0.68);

    set_color(ui_const::UI_COLOR_TEXT);
    glRasterPos2f(0.11, -0.88);

    int offset;
    offset = ui_component::MESSAGE_PTR <= 35 ? 0 : ui_component::MESSAGE_PTR - 35;
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*)ui_component::MESSAGE_BUFFER + offset);

    render_text_area(&ui_component::GAME_MESSAGE_BOARD,
                     0.10, -0.78, 0.95, -0.10, ui_component::GAME_MESSAGE_OFFSET);

    if (ui_component::IS_MESSAGING) {
        offset = ui_component::MESSAGE_PTR <= 35 ? ui_component::MESSAGE_PTR : 35;
        render_text_indicator(ui_const::UI_FIELD_GAME_MESSAGE, offset);
    }

}

void ui_render::render_help()
{
    // to be completed later
    // credit to author and hosting service
    // provide contact info in the form of email.
    // Provide instructions for how to use the program
}

void ui_render::render_login()
{
    render_menu_area();
    glLoadIdentity();
    set_color(ui_const::UI_COLOR_LABEL);
    glRasterPos2f(0.0, 0.25);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::username.c_str());
    glRasterPos2f(0.0, -0.27);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::password.c_str());

    set_color(ui_const::UI_COLOR_FIELD);
    glBegin(GL_QUADS);
        glVertex2f(0.25, 0.2);
        glVertex2f(0.75, 0.2);
        glVertex2f(0.75, 0.32);
        glVertex2f(0.25, 0.32);
        glVertex2f(0.25, -0.32);
        glVertex2f(0.75, -0.32);
        glVertex2f(0.75, -0.2);
        glVertex2f(0.25, -0.2);
    glEnd();

    if (ui_component::SELECTED_FIELD == ui_const::UI_FIELD_NAME_LOGIN) {
        set_color(ui_const::UI_COLOR_SELECTED_TEXT);
    } else {
        set_color(ui_const::UI_COLOR_TEXT);
    }
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.25, 0.2);
        glVertex2f(0.75, 0.2);
        glVertex2f(0.75, 0.32);
        glVertex2f(0.25, 0.32);
    glEnd();


    if (ui_component::SELECTED_FIELD == ui_const::UI_FIELD_PASS_LOGIN) {
        set_color(ui_const::UI_COLOR_SELECTED_TEXT);
    } else {
        set_color(ui_const::UI_COLOR_TEXT);
    }
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.25, -0.32);
        glVertex2f(0.75, -0.32);
        glVertex2f(0.75, -0.2);
        glVertex2f(0.25, -0.2);
    glEnd();

    int offset;
    set_color(ui_const::UI_COLOR_TEXT);
    offset = ui_component::USERNAME_PTR - 22 > 0 ? ui_component::USERNAME_PTR - 22 : 0;
    glRasterPos2f(0.27, 0.24);
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*) ui_component::USERNAME_BUFFER + offset);
    glRasterPos2f(0.27, -0.28);
    offset = ui_component::PASSWORD_PTR - 22 > 0 ? ui_component::PASSWORD_PTR - 22 : 0;
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*) ui_component::PASSWORD_BUFFER + offset);

    offset = *ui_component::SELECTED_BUFFER_PTR <= 22 ? *ui_component::SELECTED_BUFFER_PTR : 22;
    render_text_indicator(ui_component::SELECTED_FIELD,
                          offset);

    // displays system message
    int message_width, message_height;
    message_width = glutBitmapLength(GLUT_BITMAP_9_BY_15,
                                     (const unsigned char*) ui_component::sys_message.c_str());
    message_height = glutBitmapHeight(GLUT_BITMAP_9_BY_15);
    glColor4f(0.3, 1.0, 0.3, 1.0);
    glRasterPos2f(-0.50 - message_width / 800.0, -0.2 - message_height / 250.0);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*) ui_component::sys_message.c_str());
    //
}

void ui_render::render_registration()
{
    render_menu_area();
    glLoadIdentity();
    set_color(ui_const::UI_COLOR_LABEL);
    glRasterPos2f(0.25, 0.65);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::username.c_str());
    glRasterPos2f(0.25, 0.25);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::password.c_str());
    glRasterPos2f(0.25, -0.15);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::confirm.c_str());
    glRasterPos2f(0.25, -0.55);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*)ui_string::country.c_str());

    set_color(ui_const::UI_COLOR_FIELD);
    glBegin(GL_QUADS);
        glVertex2f(0.25, 0.6);
        glVertex2f(0.75, 0.6);
        glVertex2f(0.75, 0.48);
        glVertex2f(0.25, 0.48);

        glVertex2f(0.25, 0.2);
        glVertex2f(0.75, 0.2);
        glVertex2f(0.75, 0.08);
        glVertex2f(0.25, 0.08);

        glVertex2f(0.25, -0.2);
        glVertex2f(0.75, -0.2);
        glVertex2f(0.75, -0.32);
        glVertex2f(0.25, -0.32);

        glVertex2f(0.25, -0.6);
        glVertex2f(0.75, -0.6);
        glVertex2f(0.75, -0.72);
        glVertex2f(0.25, -0.72);
    glEnd();

    if (ui_component::SELECTED_FIELD == ui_const::UI_FIELD_NAME_REG) {
        set_color(ui_const::UI_COLOR_SELECTED_TEXT);
    } else {
        set_color(ui_const::UI_COLOR_TEXT);
    }
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.25, 0.6);
        glVertex2f(0.75, 0.6);
        glVertex2f(0.75, 0.48);
        glVertex2f(0.25, 0.48);
    glEnd();

    if (ui_component::SELECTED_FIELD == ui_const::UI_FIELD_PASS_REG) {
        set_color(ui_const::UI_COLOR_SELECTED_TEXT);
    } else {
        set_color(ui_const::UI_COLOR_TEXT);
    }
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.25, 0.2);
        glVertex2f(0.75, 0.2);
        glVertex2f(0.75, 0.08);
        glVertex2f(0.25, 0.08);
    glEnd();

    if (ui_component::SELECTED_FIELD == ui_const::UI_FIELD_CONFIRM) {
        set_color(ui_const::UI_COLOR_SELECTED_TEXT);
    } else {
        set_color(ui_const::UI_COLOR_TEXT);
    }
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.25, -0.2);
        glVertex2f(0.75, -0.2);
        glVertex2f(0.75, -0.32);
        glVertex2f(0.25, -0.32);
    glEnd();

    if (ui_component::SELECTED_FIELD == ui_const::UI_FIELD_COUNTRY) {
        set_color(ui_const::UI_COLOR_SELECTED_TEXT);
    } else {
        set_color(ui_const::UI_COLOR_TEXT);
    }
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.25, -0.6);
        glVertex2f(0.75, -0.6);
        glVertex2f(0.75, -0.72);
        glVertex2f(0.25, -0.72);
    glEnd();

    int offset;
    set_color(ui_const::UI_COLOR_TEXT);
    glRasterPos2f(0.27, 0.52);
    offset = ui_component::USERNAME_PTR - 22 > 0 ? ui_component::USERNAME_PTR - 22 : 0;
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*) ui_component::USERNAME_BUFFER + offset);
    glRasterPos2f(0.27, 0.12);
    offset = ui_component::PASSWORD_PTR - 22 > 0 ? ui_component::PASSWORD_PTR - 22 : 0;
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*) ui_component::PASSWORD_BUFFER + offset);
    glRasterPos2f(0.27, -0.28);
    offset = ui_component::CONFIRM_PTR - 22 > 0 ? ui_component::CONFIRM_PTR - 22 : 0;
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*) ui_component::CONFIRM_BUFFER + offset);
    glRasterPos2f(0.27, -0.68);
    offset = ui_component::COUNTRY_PTR - 22 > 0 ? ui_component::COUNTRY_PTR - 22 : 0;
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*) ui_component::COUNTRY_BUFFER + offset);

    offset = *ui_component::SELECTED_BUFFER_PTR <= 22 ? *ui_component::SELECTED_BUFFER_PTR : 22;
    render_text_indicator(ui_component::SELECTED_FIELD,
                          offset);

    int message_width, message_height;
    message_width = glutBitmapLength(GLUT_BITMAP_9_BY_15,
                                     (const unsigned char*) ui_component::sys_message.c_str());
    message_height = glutBitmapHeight(GLUT_BITMAP_9_BY_15);
    glColor4f(0.3, 1.0, 0.3, 1.0);
    glRasterPos2f(-0.50 - message_width / 800.0, -0.2 - message_height / 250.0);
    glutBitmapString(GLUT_BITMAP_9_BY_15,
                     (const unsigned char*) ui_component::sys_message.c_str());
}

void ui_render::render_menu_area()
{
    glLoadIdentity();
    glBegin(GL_QUADS);
        glColor4f(0.0, 0.0, 0.0, 0.0);
        glVertex2f(0.0, 1.0);
        glColor4f(0.0, 0.4, 0.1, 1.0);
        glVertex2f(0.5, 1.0);
        glColor4f(0.0, 0.4, 0.1, 1.0);
        glVertex2f(0.5, -1.0);
        glColor4f(0.0, 0.0, 0.0, 0.0);
        glVertex2f(0.0, -1.0);

        glColor4f(0.0, 0.0, 0.0, 0.0);
        glVertex2f(1.0, 1.0);
        glColor4f(0.0, 0.4, 0.1, 1.0);
        glVertex2f(0.5, 1.0);
        glColor4f(0.0, 0.4, 0.1, 1.0);
        glVertex2f(0.5, -1.0);
        glColor4f(0.0, 0.0, 0.0, 0.0);
        glVertex2f(1.0, -1.0);

    glEnd();

}

void ui_render::render_join_buttons()
{
    float x, y;
    if (ui_component::SELECTED_GAME < 0) {
        return;
    } else {
        // render the join and spectate button
        glColor4f(1.0, 1.0, 1.0, 1.0);

        x = -0.500 + (ui_component::SELECTED_GAME % 2) * 0.625;
        y = 0.86 - (ui_component::SELECTED_GAME / 2) * 0.24;
        glRasterPos2f(x, y);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,
                         (unsigned char *)ui_string::join.c_str());

        x = -0.500 + (ui_component::SELECTED_GAME % 2) * 0.625;
        y = 0.78 - (ui_component::SELECTED_GAME / 2) * 0.24;
        glRasterPos2f(x, y);
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,
                         (unsigned char *)ui_string::spectate.c_str());
    }

}

void ui_render::render_option_panel()
{

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, ui_component::OPTION_QUAD_VERTEX);
        glColorPointer(4, GL_FLOAT, 0, ui_component::OPTION_QUAD_COLOR);
        glDrawArrays(GL_QUADS, 0, 56);
        glVertexPointer(2, GL_FLOAT, 0, ui_component::OPTION_BORDER_VERTEX);
        glColorPointer(4, GL_FLOAT, 0, ui_component::OPTION_BORDER_COLOR);
        glDrawArrays(GL_LINES, 0, 112);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);


    set_color(ui_const::UI_COLOR_HIGHLIGHTED_TEXT);
    glRasterPos2f(-0.52, 0.53);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::option.c_str());

    //labels for board size setting
    set_color(ui_const::UI_COLOR_TEXT);
    glRasterPos2f(-0.675, 0.35);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_size.c_str());
    glRasterPos2f(-0.625, 0.26);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_size1.c_str());
    glRasterPos2f(-0.425, 0.26);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_size2.c_str());
    glRasterPos2f(-0.225, 0.26);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_size3.c_str());

    //labels for stone setting
    glRasterPos2f(-0.675, 0.15);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_stone.c_str());
    glRasterPos2f(-0.625, 0.06);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_stone1.c_str());
    glRasterPos2f(-0.425, 0.06);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_stone2.c_str());
    glRasterPos2f(-0.225, 0.06);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_stone3.c_str());

    //labels for timer settings
    glRasterPos2f(-0.675, -0.05);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_timer.c_str());
    glRasterPos2f(-0.550, -0.16);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_timer1.c_str());
    glRasterPos2f(-0.225, -0.16);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::choose_timer2.c_str());

    // label for spectator setting
    glRasterPos2f(-0.55, -0.30);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12,
                     (const unsigned char*)ui_string::allow_spectator.c_str());

    // label for buttons
    set_color(ui_const::UI_COLOR_HIGHLIGHTED_TEXT);
    glRasterPos2f(-0.56, -0.46);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,
                     (const unsigned char*)ui_string::ok.c_str());
    glRasterPos2f(-0.300, -0.46);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,
                     (const unsigned char*)ui_string::cancel.c_str());

    switch (ui_component::SELECTED_SIZE) {
    case ui_const::GAME_SIZE_S:
        render_checkmark(-0.675, 0.30);
        break;
    case ui_const::GAME_SIZE_M:
        render_checkmark(-0.475, 0.30);
        break;
    case ui_const::GAME_SIZE_L:
        render_checkmark(-0.275, 0.30);
        break;
    }

    switch (ui_component::SELECTED_STONE) {
    case ui_const::STONE_BLACK:
        render_checkmark(-0.675, 0.10);
        break;
    case ui_const::STONE_WHITE:
        render_checkmark(-0.475, 0.10);
        break;
    case ui_const::STONE_RANDOM:
        render_checkmark(-0.275, 0.10);
        break;
    }

    set_color(ui_const::UI_COLOR_TEXT);
    glRasterPos2f(-0.595 - 0.02 * ui_component::TIME_TOTAL_PTR, -0.16);
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*)ui_component::TIME_TOTAL);
    glRasterPos2f(-0.270 - 0.02 * ui_component::TIME_INCREMENT_PTR, -0.16);
    glutBitmapString(GLUT_BITMAP_8_BY_13,
                     (const unsigned char*)ui_component::TIME_INCREMENT);

    if (ui_component::SELECTED_BUFFER != ui_component::MESSAGE_BUFFER){
        render_text_indicator(ui_component::SELECTED_FIELD, 0);
    }

    if (ui_component::IS_SPECTATOR_ON) {
        render_checkmark(-0.250, -0.26);
    }


}

void ui_render::set_color(int color)
{
    //printf("setting color to %d", color);
    switch(color){
    case ui_const::UI_COLOR_TEXT:
        glColor4f(0.1, 0.1, 0.0, 1.0);
        break;
    case ui_const::UI_COLOR_HIGHLIGHTED_TEXT:
        glColor4f(0.9, 1.0, 0.9, 1.0);
        break;
    case ui_const::UI_COLOR_SELECTED_TEXT:
        glColor4f(1.0, 0.4, 0.0, 1.0);
        break;
    case ui_const::UI_COLOR_BUTTON:
        glColor4f(0.2, 0.3, 0.55, 1.0);
        break;
    case ui_const::UI_COLOR_SELECTED_BUTTON:
        glColor4f(0.7, 0.7, 0.7, 1.0);
        break;
    case ui_const::UI_COLOR_BOARD:
        glColor4f(0.85, 0.75, 0.75, 1.0);
        break;
    case ui_const::UI_COLOR_BLACK_STONE:
        glColor4f(0.0, 0.0, 0.0, 1.0);
        break;
    case ui_const::UI_COLOR_WHITE_STONE:
        glColor4f(1.0, 1.0, 1.0, 1.0);
        break;
    case ui_const::UI_COLOR_LABEL:
        glColor4f(1.0, 0.5, 0.0, 1.0);
        break;
    case ui_const::UI_COLOR_FIELD:
        glColor4f(0.8, 1.0, 1.0, 0.8);
        break;
    }
}

void ui_render::set_background(int background)
{
    switch (background){
    case ui_const::UI_BACKGROUND_START:
        glClearColor(0.0, 0.0, 0.0, 0.0);
        break;
    case ui_const::UI_BACKGROUND_LOBBY:
        glClearColor(0.6, .8, 0.8, 0.0);
        break;
    case ui_const::UI_BACKGROUND_GAME:
        glClearColor(0.8, 0.8, 1.0, 0.0);
        break;
    }
}

void ui_render::lobby_setup_interface()
{
    //set up the game list
    ui_component::SELECTED_GAME = -1;
    ui_component::SELECTED_PAGE = 0;
    for (int i = 0; i < 10; i++){
        setup_rect(ui_component::LOBBY_QUAD_VERTEX,
                   i * 8,
                   -0.975 + (i % 2) * 0.625,
                   0.96 - int(i / 2) * 0.24,
                   -0.375 + (i % 2) * 0.625,
                   0.76 - int(i / 2) * 0.24);
        setup_rectcolor(ui_component::LOBBY_QUAD_COLOR, 16 * i, 0.7, 0.7, 0.9, 0.8, true);
        setup_border(ui_component::LOBBY_BORDER_VERTEX,
                     16 * i,
                     -0.975 + (i % 2) * 0.625,
                     0.96 - int(i / 2) * 0.24,
                     -0.375 + (i % 2) * 0.625,
                     0.76 - int(i / 2) * 0.24);
        setup_border_color(ui_component::LOBBY_BORDER_COLOR, 32 * i, 0.0, 0.2, 0.1, 0.8);
    }
    // set up the chat area
    setup_rect(ui_component::LOBBY_QUAD_VERTEX, 8 * 10, -0.975, -0.24, 0.25, -0.84);
    setup_rectcolor(ui_component::LOBBY_QUAD_COLOR, 16 * 10, 0.9, 0.9, 1.0, 1.0, false);

    // chat input box
    setup_rect(ui_component::LOBBY_QUAD_VERTEX, 8 * 11, -0.975, -0.88, 0.125, -0.96);
    setup_rectcolor(ui_component::LOBBY_QUAD_COLOR, 16 * 11, 0.9, 0.9, 1.0, 1.0, false);

    // chat send button
    setup_rect(ui_component::LOBBY_QUAD_VERTEX, 8 * 12, 0.15, -0.88, 0.25, -0.96);
    setup_rectcolor(ui_component::LOBBY_QUAD_COLOR, 16 * 12, 0.0, 0.9, 0.3, 1.0, true);

    // user list area
    setup_rect(ui_component::LOBBY_QUAD_VERTEX, 8 * 13, 0.35, 0.96, 0.975, -0.50);
    setup_rectcolor(ui_component::LOBBY_QUAD_COLOR, 16 * 13, 0.9, 0.9, 1.0, 1.0, false);

    // host game button
    setup_rect(ui_component::LOBBY_QUAD_VERTEX, 8 * 14, 0.35, -0.56, 0.975, -0.76);
    setup_rectcolor(ui_component::LOBBY_QUAD_COLOR, 16 * 14, 1.0, 0.5, 0.3, 1.0, true);

    // refresh button
    setup_rect(ui_component::LOBBY_QUAD_VERTEX, 8 * 15, 0.35, -0.80, 0.65, -0.96);
    setup_rectcolor(ui_component::LOBBY_QUAD_COLOR, 16 * 15, 1.0, 0.5, 0.3, 1.0, true);

    // exit button
    setup_rect(ui_component::LOBBY_QUAD_VERTEX, 8 * 16, 0.675, -0.80, 0.975, -0.96);
    setup_rectcolor(ui_component::LOBBY_QUAD_COLOR, 16 * 16, 1.0, 0.5, 0.3, 1.0, true);

    ui_component::GAME_MESSAGE_OFFSET = 0;
    ui_component::PLAYER_OFFSET = 0;
}

void ui_render::game_setup_interface()
{
    // set up the board
    // set up the buttons
    int gsize = ui_component::currentGame.size();

    float xi, yi;
    int r_star, r_stone;

    // set up the board background color and the grid
    switch (gsize){
    case ui_const::GAME_SIZE_S:
        xi = 42.0 / 400.0;
        yi = 42.0 / 250.0;
        r_star = 4;
        r_stone = 18;
        break;
    case ui_const::GAME_SIZE_M:
        xi = 30.0 / 400.0;
        yi = 30.0 / 250.0;
        r_star = 3;
        r_stone = 13;
        break;
    case ui_const::GAME_SIZE_L:
        xi = 21.0 / 400.0;
        yi = 21.0 / 250.0;
        r_star = 2;
        r_stone = 9;
        break;
    }

    setup_rect(ui_component::BOARD_VERTEX, 0, -1.0, 1.0, 0.05, -0.68);

    for (int i = 0; i < gsize; i++){
        ui_component::BOARD_VERTEX[4 * i + 8] = -1.0 + xi;
        ui_component::BOARD_VERTEX[4 * i + 9] = 1.0 - (i + 1) * yi;
        ui_component::BOARD_VERTEX[4 * i + 10] = 0.05 - xi;
        ui_component::BOARD_VERTEX[4 * i + 11] = 1.0 - (i + 1) * yi;

        ui_component::BOARD_VERTEX[4 * (i + gsize) + 8] = -1.0 + (i + 1) * xi;
        ui_component::BOARD_VERTEX[4 * (i + gsize) + 9] = 1.0 - yi;
        ui_component::BOARD_VERTEX[4 * (i + gsize) + 10] = -1.0 + (i + 1) * xi;
        ui_component::BOARD_VERTEX[4 * (i + gsize) + 11] = -0.68 + yi;

        for (int j = 0; j < gsize; j++) {
            ui_component::CROSS_VERTEX[8 * (gsize * i + j) + 0] = -1.0 + (i + 0.6) * xi;
            ui_component::CROSS_VERTEX[8 * (gsize * i + j) + 1] = 1.0 - (j + 0.6) * yi;
            ui_component::CROSS_VERTEX[8 * (gsize * i + j) + 2] = -1.0 + (i + 1.4) * xi;
            ui_component::CROSS_VERTEX[8 * (gsize * i + j) + 3] = 1.0 - (j + 1.4) * yi;
            ui_component::CROSS_VERTEX[8 * (gsize * i + j) + 4] = -1.0 + (i + 1.4) * xi;
            ui_component::CROSS_VERTEX[8 * (gsize * i + j) + 5] = 1.0 - (j + 0.6) * yi;
            ui_component::CROSS_VERTEX[8 * (gsize * i + j) + 6] = -1.0 + (i + 0.6) * xi;
            ui_component::CROSS_VERTEX[8 * (gsize * i + j) + 7] = 1.0 - (j + 1.4) * yi;

            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 0] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 1] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 2] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 3] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 4] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 5] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 6] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 7] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 8] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 9] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 10] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 11] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 12] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 13] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 14] = 0.0;
            ui_component::CROSS_COLOR[16 * (gsize * i + j) + 15] = 0.0;
        }
    }

    // setup the shape of stones
    float r_star_x = r_star / 400.0, r_star_y = r_star / 250.0;
    float r_stone_x = r_stone / 400.0, r_stone_y = r_stone / 250.0;
    float angle;

    for (int degree = 0; degree < 60; degree++) {
        angle = 2 * M_PI * degree / 60.0;
        ui_component::STAR_VERTEX[2 * degree] = r_star_x * cosf(angle);
        ui_component::STAR_VERTEX[2 * degree + 1] = r_star_y * sinf(angle);
        ui_component::STONE_VERTEX[2 * degree] = r_stone_x * cosf(angle);
        ui_component::STONE_VERTEX[2 * degree + 1] = r_stone_y * sinf(angle);
    }

    // setup location of star points
    int corner = gsize == ui_const::GAME_SIZE_S ? 3 : 4;
    int gap = (gsize - 2 * corner + 1) / 2;
    for (int i = 0; i < 9; i++){
        ui_component::STAR_LOC[2 * i] = -1.0 + (corner + gap * (i % 3)) * xi;
        ui_component::STAR_LOC[2 * i + 1] = 1.0 - (corner + gap * (i / 3)) * yi;
    }

    // setup the buttons
    // for triangular control buttons
    for (int i = 0; i < 4; i++){
        ui_component::GAME_UI_VERTEX[6 * i + 0] = -0.95 + i * 0.075 + (i <= 1) * 0.04 + (i > 1) * 0.01;
        ui_component::GAME_UI_VERTEX[6 * i + 1] = -0.80;
        ui_component::GAME_UI_VERTEX[6 * i + 2] = -0.95 + i * 0.075 + (i > 1) * 0.05;
        ui_component::GAME_UI_VERTEX[6 * i + 3] = -0.84;
        ui_component::GAME_UI_VERTEX[6 * i + 4] = -0.95 + i * 0.075 + (i <= 1) * 0.04 + (i > 1) * 0.01;
        ui_component::GAME_UI_VERTEX[6 * i + 5] = -0.88;

        for (int j = 0; j < 3; j++){
            ui_component::GAME_UI_COLOR[12 * i + 4 * j + 0] = 0.0;
            ui_component::GAME_UI_COLOR[12 * i + 4 * j + 1] = 0.0;
            ui_component::GAME_UI_COLOR[12 * i + 4 * j + 2] = 0.0;
            ui_component::GAME_UI_COLOR[12 * i + 4 * j + 3] = 1.0;
        }
    }

    setup_rect(ui_component::GAME_UI_VERTEX,
               24, -0.95, -0.80, -0.96, -0.88);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    48, 0.0, 0.0, 0.0, 1.0, false);
    setup_rect(ui_component::GAME_UI_VERTEX,
               32, -0.675, -0.80, -0.665, -0.88);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    64, 0.0, 0.0, 0.0, 1.0, false);


    // buttons under the board
    setup_rect(ui_component::GAME_UI_VERTEX,
               40, -0.550, -0.78, -0.400, -0.90);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    80, 0.7, 0.7, 0.7, 1.0, true);

    setup_rect(ui_component::GAME_UI_VERTEX,
               48, -0.325, -0.78, -0.175, -0.90);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    96, 0.7, 0.7, 0.7, 1.0, true);

    setup_rect(ui_component::GAME_UI_VERTEX,
               56, -0.100, -0.78, 0.050, -0.90);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    112, 0.7, 0.7, 0.7, 1.0, true);

    // status bar
    setup_rect(ui_component::GAME_UI_VERTEX,
               64, 0.10, 0.96, 0.50, 0.86);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    128, 1.0, 1.0, 1.0, 1.0, false);
    setup_rect(ui_component::GAME_UI_VERTEX,
               72, 0.55, 0.96, 0.95, 0.86);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    144, 1.0, 1.0, 1.0, 1.0, false);

    // game control buttons
    setup_rect(ui_component::GAME_UI_VERTEX,
               80, 0.10, 0.68, 0.30, 0.55);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    160, 1.0, 0.75, 0.5, 1.0, false);
    setup_rect(ui_component::GAME_UI_VERTEX,
               88, 0.10, 0.49, 0.30, 0.36);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    176, 1.0, 0.75, 0.5, 1.0, false);
    setup_rect(ui_component::GAME_UI_VERTEX,
               96, 0.10, 0.30, 0.30, 0.17);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    192, 1.0, 0.75, 0.5, 1.0, false);
    setup_rect(ui_component::GAME_UI_VERTEX,
               104, 0.10, 0.11, 0.30, -0.02);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    208, 1.0, 0.75, 0.5, 1.0, false);

    // user list
    setup_rect(ui_component::GAME_UI_VERTEX,
               112, 0.35, 0.68, 0.95, -0.02);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    224, 1.0, 1.0, 1.0, 1.0, false);

    // message area
    setup_rect(ui_component::GAME_UI_VERTEX,
               120, 0.10, -0.10, 0.95, -0.78);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    240, 1.0, 1.0, 1.0, 1.0, false);
    setup_rect(ui_component::GAME_UI_VERTEX,
               128, 0.10, -0.82, 0.825, -0.90);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    256, 1.0, 1.0, 1.0, 1.0, false);
    setup_rect(ui_component::GAME_UI_VERTEX,
               136, 0.85, -0.82, 0.95, -0.90);
    setup_rectcolor(ui_component::GAME_UI_COLOR,
                    272, 0.0, 0.6, 0.2, 1.0, true);

}

void ui_render::setup_option_panel()
{
    ui_component::IS_OPTION_ON = false;
    ui_component::SELECTED_SIZE = ui_const::GAME_SIZE_L;
    ui_component::SELECTED_STONE = ui_const::STONE_RANDOM;
    strncpy(ui_component::TIME_TOTAL, "30", 2);
    strncpy(ui_component::TIME_INCREMENT, "30", 2);
    ui_component::TIME_TOTAL_PTR = 2;
    ui_component::TIME_INCREMENT_PTR = 2;
    ui_component::IS_SPECTATOR_ON = false;


    setup_rect(ui_component::OPTION_QUAD_VERTEX,
               0, -0.75, 0.6, 0.0, 0.5);
    setup_rectcolor(ui_component::OPTION_QUAD_COLOR,
                    0, 0.2, 0.3, 0.3, 1.0, false);
    setup_border(ui_component::OPTION_BORDER_VERTEX,
                 0, -0.75, 0.6, 0.0, 0.5);
    setup_border_color(ui_component::OPTION_BORDER_COLOR,
                       0, 0.2, 0.2, 0.8, 1.0);

    setup_rect(ui_component::OPTION_QUAD_VERTEX,
               8, -0.75, 0.5, 0.0, -0.6);
    setup_rectcolor(ui_component::OPTION_QUAD_COLOR,
                    16, 1.0, 0.8, 0.6, 1.0, false);
    setup_border(ui_component::OPTION_BORDER_VERTEX,
                 16, -0.75, 0.5, 0.0, -0.6);
    setup_border_color(ui_component::OPTION_BORDER_COLOR,
                       32, 0.2, 0.2, 0.8, 1.0);

    for (int i = 0; i < 6; i++){
        setup_rect(ui_component::OPTION_QUAD_VERTEX,
                   16 + 8 * i,
                   -0.675 + (i % 3) * 0.20,
                   0.30 - int(i / 3) * 0.20,
                   -0.650 + (i % 3) * 0.20,
                   0.26 - int(i / 3) * 0.20);
        setup_rectcolor(ui_component::OPTION_QUAD_COLOR,
                        32 + 16 * i, 1.0, 1.0, 1.0, 1.0, true);
        setup_border(ui_component::OPTION_BORDER_VERTEX,
                     32 + 16 * i,
                     -0.675 + (i % 3) * 0.20,
                     0.30 - int(i / 3) * 0.20,
                     -0.650 + (i % 3) * 0.20,
                     0.26 - int(i / 3) * 0.20);
        setup_border_color(ui_component::OPTION_BORDER_COLOR,
                           64 + 32 * i, 0.0, 0.6, 0.2, 1.0);
    }


    setup_rect(ui_component::OPTION_QUAD_VERTEX,
               72, -0.675, -0.10, -0.575, -0.18);
    setup_rectcolor(ui_component::OPTION_QUAD_COLOR,
                    144, 1.0, 1.0, 1.0, 1.0, false);
    setup_border(ui_component::OPTION_BORDER_VERTEX,
                 144, -0.675, -0.10, -0.575, -0.18);
    setup_border_color(ui_component::OPTION_BORDER_COLOR,
                       288, 0.0, 0.6, 0.2, 1.0);

    setup_rect(ui_component::OPTION_QUAD_VERTEX,
               80, -0.350, -0.10, -0.250, -0.18);
    setup_rectcolor(ui_component::OPTION_QUAD_COLOR,
                    160, 1.0, 1.0, 1.0, 1.0, false);
    setup_border(ui_component::OPTION_BORDER_VERTEX,
                 160, -0.350, -0.10, -0.250, -0.18);
    setup_border_color(ui_component::OPTION_BORDER_COLOR,
                       320, 0.0, 0.6, 0.2, 1.0);


    setup_rect(ui_component::OPTION_QUAD_VERTEX,
               88, -0.250, -0.26, -0.225, -0.30);
    setup_rectcolor(ui_component::OPTION_QUAD_COLOR,
                    176, 1.0, 1.0, 1.0, 1.0, true);
    setup_border(ui_component::OPTION_BORDER_VERTEX,
                 176, -0.250, -0.26, -0.225, -0.30);
    setup_border_color(ui_component::OPTION_BORDER_COLOR,
                       352, 0.0, 0.6, 0.2, 1.0);

    setup_rect(ui_component::OPTION_QUAD_VERTEX,
               96, -0.575, -0.40, -0.425, -0.50);
    setup_rectcolor(ui_component::OPTION_QUAD_COLOR,
                    192, 0.5, 0.6, 1.0, 1.0, false);
    setup_border(ui_component::OPTION_BORDER_VERTEX,
                 192, -0.575, -0.40, -0.425, -0.50);
    setup_border_color(ui_component::OPTION_BORDER_COLOR,
                       384, 0.0, 0.0, 0.0, 1.0);

    setup_rect(ui_component::OPTION_QUAD_VERTEX,
               104, -0.325, -0.40, -0.175, -0.50);
    setup_rectcolor(ui_component::OPTION_QUAD_COLOR,
                    208, 0.5, 0.6, 1.0, 1.0, false);
    setup_border(ui_component::OPTION_BORDER_VERTEX,
                 208, -0.325, -0.40, -0.175, -0.50);
    setup_border_color(ui_component::OPTION_BORDER_COLOR,
                       416, 0.0, 0.0, 0.0, 1.0);


}

void ui_render::setup_rect(GLfloat *vertices,
                                 int start,
                                 float x1,
                                 float y1,
                                 float x2,
                                 float y2)
{
    vertices[start + 0] = x1;
    vertices[start + 1] = y1;
    vertices[start + 2] = x2;
    vertices[start + 3] = y1;
    vertices[start + 4] = x2;
    vertices[start + 5] = y2;
    vertices[start + 6] = x1;
    vertices[start + 7] = y2;
}

void ui_render::setup_rectcolor(GLfloat *colors,
                                int start,
                                float r,
                                float g,
                                float b,
                                float alpha,
                                bool modifier_on)
{
    float modifier;
    for (int i = 0; i < 4; i++) {
        modifier = (i % 2) && modifier_on ? 0.75 : 1.0;
        colors[start + 4*i + 0] = r * modifier;
        colors[start + 4*i + 1] = g * modifier;
        colors[start + 4*i + 2] = b * modifier;
        colors[start + 4*i + 3] = alpha;
    }
}

void ui_render::setup_border(GLfloat *vertices,
                             int start,
                             float x1,
                             float y1,
                             float x2,
                             float y2)
{
    vertices[start +  0] = x1;
    vertices[start +  1] = y1;
    vertices[start +  2] = x2;
    vertices[start +  3] = y1;

    vertices[start +  4] = x2;
    vertices[start +  5] = y1;
    vertices[start +  6] = x2;
    vertices[start +  7] = y2;

    vertices[start +  8] = x2;
    vertices[start +  9] = y2;
    vertices[start + 10] = x1;
    vertices[start + 11] = y2;

    vertices[start + 12] = x1;
    vertices[start + 13] = y2;
    vertices[start + 14] = x1;
    vertices[start + 15] = y1;
}

void ui_render::setup_border_color(GLfloat *colors,
                                   int start,
                                   float r,
                                   float g,
                                   float b,
                                   float alpha)
{
    for (int i = 0; i < 8; i++) {
        colors[start + 4 * i + 0] = r;
        colors[start + 4 * i + 1] = g;
        colors[start + 4 * i + 2] = b;
        colors[start + 4 * i + 3] = alpha;
    }
}



void ui_render::render_circle(float x, float y, int r)
{
    glLoadIdentity();
    glTranslatef(x, y, 0);

    float rx = r / 400.0;
    float ry = r / 250.0;

    glBegin(GL_POLYGON);
        for (float theta = 0; theta < 2 * M_PI; theta += 0.01){
            glVertex2f(rx * cosf(theta), ry * sinf(theta));
        }
    glEnd();

    glBegin(GL_LINE_LOOP);
        for (float theta = 0; theta < 2 * M_PI; theta += 0.01){
            glVertex2f(rx * cosf(theta), ry * sinf(theta));
        }
    glEnd();

    glLoadIdentity();
}

void ui_render::render_text_indicator(int component, int offset)
{
    float x1, x2, y1, y2;
    switch (component){
    case ui_const::UI_FIELD_NAME_LOGIN:
        x1 = 0.27 + 0.02 * offset;
        x2 = 0.27 + 0.02 * offset;
        y1 = 0.22;
        y2 = 0.30;
        break;
    case ui_const::UI_FIELD_PASS_LOGIN:
        x1 = 0.27 + 0.02 * offset;
        x2 = 0.27 + 0.02 * offset;
        y1 = -0.30;
        y2 = -0.22;
        break;
    case ui_const::UI_FIELD_NAME_REG:
        x1 = 0.27 + 0.02 * offset;
        x2 = 0.27 + 0.02 * offset;
        y1 = 0.58;
        y2 = 0.50;
        break;
    case ui_const::UI_FIELD_PASS_REG:
        x1 = 0.27 + 0.02 * offset;
        x2 = 0.27 + 0.02 * offset;
        y1 = 0.18;
        y2 = 0.10;
        break;
    case ui_const::UI_FIELD_CONFIRM:
        x1 = 0.27 + 0.02 * offset;
        x2 = 0.27 + 0.02 * offset;
        y1 = -0.22;
        y2 = -0.30;
        break;
    case ui_const::UI_FIELD_COUNTRY:
        x1 = 0.27 + 0.02 * offset;
        x2 = 0.27 + 0.02 * offset;
        y1 = -0.62;
        y2 = -0.70;
        break;
    case ui_const::UI_FIELD_LOBBY_MESSAGE:
        x1 = -0.96 + 0.02 * offset;
        x2 = -0.96 + 0.02 * offset;
        y1 = -0.89;
        y2 = -0.95;
        break;
    case ui_const::UI_FIELD_GAME_MESSAGE:
        x1 = 0.11 + 0.02 * offset;
        x2 = 0.11 + 0.02 * offset;
        y1 = -0.83;
        y2 = -0.89;
        break;
    case ui_const::UI_FIELD_TIME:
        x1 = -0.595;
        x2 = -0.595;
        y1 = -0.11;
        y2 = -0.17;
        break;
    case ui_const::UI_FIELD_INCREMENT:
        x1 = -0.270;
        x2 = -0.270;
        y1 = -0.11;
        y2 = -0.17;
        break;
    }

    if (component > ui_const::UI_FIELD_PASS_LOGIN){
        glColor4f(1.0, 0.2, 0.2, 1.0);
    } else {
        glColor4f(0.0, 0.0, 0.2, 1.0);
    }

    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
}

void ui_render::render_games()
{
    // render all the current games.
    typedef std::vector<Game_Descriptor>::size_type var;
    for (var i = 0; i < 10; i++) {
        var cur = i + var(ui_component::SELECTED_PAGE) * 10;
        if (cur >= ui_component::GAME_LIST.size()) {
            break;
        }
        // render the game
        float x = -0.975 + (i % 2) * 0.625;
        float y = 0.96 - (i / 2) * 0.24;


        char row[100];
        glColor4f(0.0, 0.0, 0.0, 1.0);

        sprintf(row, "Game #%d  %s vs. %s",
                ui_component::GAME_LIST[i].id_,
                ui_component::GAME_LIST[i].gamehost_,
                ui_component::GAME_LIST[i].opponent_);
        glRasterPos2f(x + 0.025, y - 0.06);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)row);

        sprintf(row, "Size: %s   Timer: %s",
                ui_component::GAME_LIST[i].size_,
                ui_component::GAME_LIST[i].timer_);
        glRasterPos2f(x + 0.025, y - 0.12);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)row);

        sprintf(row, "Spectator: %s  Players: %d  Status: %s",
                ui_component::GAME_LIST[i].spectator_on,
                ui_component::GAME_LIST[i].player_count,
                ui_component::GAME_LIST[i].status_);
        glRasterPos2f(x + 0.025, y - 0.18);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)row);
    }

}

void ui_render::render_users(float xmin, float xmax, float ymin, float ymax)
{
    // render the list of users inside the designated area
    // basically should look like a table
    typedef std::vector<Player>::size_type var;
    var lines = (var)((ymax - ymin) / 0.06) - 1;
    if (ui_component::PLAYER_LIST.size() - ui_component::PLAYER_OFFSET < lines) {
        lines = ui_component::PLAYER_LIST.size() - ui_component::PLAYER_OFFSET;
    }

    float x1 = xmin + (xmax - xmin) * 0.425;
    float x2 = xmin + (xmax - xmin) * 0.575;
    float x3 = xmin + (xmax - xmin) * 0.725;
    int len;
    glColor4f(0.8, 0.6, 0.4, 0.6);
    glBegin(GL_LINES);
        glVertex2f(x1, ymax);
        glVertex2f(x1, ymin);
        glVertex2f(x2, ymax);
        glVertex2f(x2, ymin);
        glVertex2f(x3, ymax);
        glVertex2f(x3, ymin);
    glEnd();


    char buffer[32];
    glColor4f(0.0, 0.0, 0.0, 1.0);
    sprintf(buffer, "Users(%d)", ui_component::PLAYER_LIST.size());
    len = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);
    glRasterPos2f((xmin + x1 - float(len) / 400.0) / 2, ymax - 0.06);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);

    sprintf(buffer, "win");
    len = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);
    glRasterPos2f((x1 + x2 - float(len) / 400.0) / 2, ymax - 0.06);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);

    sprintf(buffer, "loss");
    len = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);
    glRasterPos2f((x2 + x3 - float(len) / 400.0) / 2, ymax - 0.06);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);

    sprintf(buffer, "Country");
    len = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);
    glRasterPos2f((x3 + xmax - float(len) / 400.0) / 2, ymax - 0.06);
    glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);


    glColor4f(0.6, 0.3, 0.0, 1.0);
    for (var i = 0; i < lines; ++i) {
        sprintf(buffer, "%s", ui_component::PLAYER_LIST[i].name_);
        glRasterPos2f(xmin + 0.01, ymax - (i + 2) * 0.06);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);
        sprintf(buffer, "%s", ui_component::PLAYER_LIST[i].country_);
        glRasterPos2f(x3 + 0.01, ymax - (i + 2) * 0.06);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);
    }

    glColor4f(0.2, 0.8, 0.2, 1.0);
    for (var i = 0; i < lines; ++i) {
        sprintf(buffer, "%d", ui_component::PLAYER_LIST[i].win_);
        glRasterPos2f(x1 + 0.01, ymax - (i + 2) * 0.06);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);
    }

    glColor4f(0.8, 0.2, 0.2, 1.0);
    for (var i = 0; i < lines; ++i) {
        sprintf(buffer, "%d", ui_component::PLAYER_LIST[i].loss_);
        glRasterPos2f(x2 + 0.01, ymax - (i + 2) * 0.06);
        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)buffer);
    }
    //
}


void set_chat_color(char c){
    switch (c) {
    case 'r':
        glColor4f(1.0, 0.0, 0.0, 1.0);
        break;
    case 'g':
        glColor4f(0.0, 1.0, 0.0, 1.0);
        break;
    case 'b':
        glColor4f(0.0, 0.0, 1.0, 1.0);
        break;
    case 'k':
        glColor4f(0.0, 0.0, 0.0, 1.0);
        break;
    }
}

void ui_render::render_text_area(std::vector<std::string> *m,
                                 float xmin,
                                 float xmax,
                                 float ymin,
                                 float ymax,
                                 std::vector<std::string>::size_type offset)
{
    //calculates the maximum number of lines need to be rendered
    typedef std::vector<std::string>::size_type var;
    var max_lines = (var)((ymax - ymin) / 0.05);
    var iStart = m->size() <= max_lines ? 0 : m->size() - offset - max_lines;
    var iEnd = m->size() < iStart + max_lines? m->size() : iStart + max_lines;
    float x = xmin + 0.01, y = ymax - 0.05;
    for (var i = iStart; i != iEnd ; i++){
        set_chat_color((m->at(i))[0]);
        glRasterPos2f(x, y);
        glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)(m->at(i).c_str() + 1));
        y -= 0.05;
    }

}

void ui_render::render_checkmark(float x, float y)
{
    glLoadIdentity();
    glColor4f(1.0, 0.2, 0.2, 1.0);
    glBegin(GL_LINE_STRIP);
        glVertex2f(x - 0.005, y + 0.01);
        glVertex2f(x + 0.01, y - 0.04);
        glVertex2f(x + 0.06, y + 0.02);
    glEnd();
}

void ui_render::render_gameboard()
{
    glLoadIdentity();
    int gsize = ui_component::currentGame.size();
    // render the board first
    glEnableClientState(GL_VERTEX_ARRAY);

        glColor4f(0.8, 0.7, 0.4, 1.0);
        glVertexPointer(2, GL_FLOAT, 0, ui_component::BOARD_VERTEX);
        glDrawArrays(GL_QUADS, 0, 4);

        glColor4f(0.0, 0.0, 0.0, 1.0);
        glDrawArrays(GL_LINES, 4, gsize * 4);

        glVertexPointer(2, GL_FLOAT, 0, ui_component::STAR_VERTEX);

        glColor4f(0.25, 0.25, 0.25, 1.0);
        int increment = gsize == ui_const::GAME_SIZE_L ? 1 : 2;
        for (int i = 0; i < 9; i += increment){
            render_starpoint(ui_component::STAR_LOC[2 * i], ui_component::STAR_LOC[2 * i + 1]);
        }

        glVertexPointer(2, GL_FLOAT, 0, ui_component::STONE_VERTEX);
        for (int i = 0; i < gsize; i++){
            for (int j = 0; j < gsize; j++){
                render_stone(i, j);
            }
        }

    glDisableClientState(GL_VERTEX_ARRAY);
}

void ui_render::render_starpoint(float x, float y)
{
    glLoadIdentity();
    glTranslatef(x, y, 0.0);

    glVertexPointer(2, GL_FLOAT, 0, ui_component::STAR_VERTEX);
    glDrawArrays(GL_POLYGON, 0, 60);
    glDrawArrays(GL_LINE_LOOP, 0, 60);
}

void ui_render::render_stone(int x, int y)
{
    int val = ui_component::currentGame.getVal(x, y);
    if (val == 0){
        return;
    }

    glLoadIdentity();
    float xi = 1.05 / (ui_component::currentGame.size() + 1);
    float yi = 1.68 / (ui_component::currentGame.size() + 1);
    float intensity = 0.85 * ((val + 1) % 2) + 0.15 * (val % 2);
    float alpha = 1.0 - 0.6 * (val > 2);

    glTranslatef(-1.0 + xi * (x + 1), 1.0 - yi * (y + 1), 0);
    glColor4f(intensity, intensity, intensity, alpha);
    glDrawArrays(GL_POLYGON, 0, 60);
    glDrawArrays(GL_LINE_LOOP, 0, 60);
}

void ui_render::render_counting()
{
    int gsize = ui_component::currentGame.size();
    glVertexPointer(2, GL_FLOAT, 0, ui_component::CROSS_VERTEX);
    glColorPointer(4, GL_FLOAT, 0, ui_component::CROSS_COLOR);
    glDrawArrays(GL_LINES, 0, 4 * gsize * gsize);
}

void ui_render::update_counting()
{
    int gsize = ui_component::currentGame.size();
    int val, black_score, white_score;

    for (int i = 0; i < gsize; i++) {
        for (int j = 0; j < gsize; j++) {
            val = ui_component::currentGame.getCountVal(i, j);

            for (int k = 0; k < 4; k++) {

                if (val == 1){
                    ui_component::CROSS_COLOR[16 * (i * gsize + j) + 4 * k + 0] = 0.0;
                    ui_component::CROSS_COLOR[16 * (i * gsize + j) + 4 * k + 1] = 0.0;
                    ui_component::CROSS_COLOR[16 * (i * gsize + j) + 4 * k + 2] = 0.0;
                    ui_component::CROSS_COLOR[16 * (i * gsize + j) + 4 * k + 3] = 1.0;
                    black_score++;
                } else if (val == 2){
                    ui_component::CROSS_COLOR[16 * (i * gsize + j) + 4 * k + 0] = 1.0;
                    ui_component::CROSS_COLOR[16 * (i * gsize + j) + 4 * k + 1] = 1.0;
                    ui_component::CROSS_COLOR[16 * (i * gsize + j) + 4 * k + 2] = 1.0;
                    ui_component::CROSS_COLOR[16 * (i * gsize + j) + 4 * k + 3] = 1.0;
                    white_score++;
                } else {
                    ui_component::CROSS_COLOR[16 * (i * gsize + j) + 4 * k + 3] = 0.0;
                }

            }

        }
    }
    ui_component::currentGame.set_blackscore(black_score);
    ui_component::currentGame.set_whitescore(white_score);
}

void ui_render::render_cursor_indicator()
{
    using namespace ui_component;
    if (COORINDATE_X < 0){
        return;
    }

    float xi = 1.05 / (currentGame.size() + 1);
    float yi = 1.68 / (currentGame.size() + 1);

    float x1 = -1.0 + (COORINDATE_X + 0.80) * xi;
    float y1 = 1.0 - (COORINDATE_Y + 0.80) * yi;
    float x2 = -1.0 + (COORINDATE_X + 1.20) * xi;
    float y2 = 1.0 - (COORINDATE_Y + 1.20) * yi;

    glLoadIdentity();
    if (currentGame.is_counting()) {
        glColor4f(0.2, 0.2, 0.6, 1.0);
    } else if (currentGame.isLegal(COORINDATE_X * currentGame.size() + COORINDATE_Y)) {
        glColor4f(0.0, 0.6, 0.2, 1.0);
    } else {
        glColor4f(0.8, 0.0, 0.0, 1.0);
    }
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x1, y2);
        glVertex2f(x2, y2);
        glVertex2f(x2, y1);
    glEnd();
}
//*******************
