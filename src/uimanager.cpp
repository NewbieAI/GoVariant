#include "uimanager.h"
#include "networkmanager.h"
//#include <GL/glut.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <math.h>
#include <time.h>

bool ui_component::has_changed;
int ui_component::mode;
std::string ui_component::currentUser = "";
std::string ui_component::lobby_stamp = "";
std::string ui_component::game_stamp = "";
int ui_component::authentication_code = -1;
Game ui_component::currentGame;
int ui_component::currentAction;

int ui_component::SELECTED_BUTTON;
int ui_component::SELECTED_FIELD;
char *ui_component::SELECTED_BUFFER;

int ui_component::LOGIN_TEXT_COLOR;
int ui_component::REGISTER_TEXT_COLOR;
int ui_component::EXIT_TEXT_COLOR;
int ui_component::LOGIN_BUTTON_COLOR;
int ui_component::REGISTER_BUTTON_COLOR;
int ui_component::EXIT_BUTTON_COLOR;
int ui_component::SUBMIT_BUTTON_COLOR;
int ui_component::SUBMIT_TEXT_COLOR;
int ui_component::HELP_BUTTON_COLOR;
int ui_component::HELP_TEXT_COLOR;

char ui_component::MESSAGE_BUFFER[128];
char ui_component::USERNAME_BUFFER[32];
char ui_component::PASSWORD_BUFFER[32];
char ui_component::CONFIRM_BUFFER[32];
char ui_component::COUNTRY_BUFFER[32];

int ui_component::MESSAGE_PTR;
int ui_component::USERNAME_PTR;
int ui_component::PASSWORD_PTR;
int ui_component::CONFIRM_PTR;
int ui_component::COUNTRY_PTR;
int *ui_component::SELECTED_BUFFER_PTR;

GLfloat ui_component::LOBBY_QUAD_VERTEX[160];
GLfloat ui_component::LOBBY_QUAD_COLOR[320];
GLfloat ui_component::LOBBY_BORDER_VERTEX[320];
GLfloat ui_component::LOBBY_BORDER_COLOR[640];

GLfloat ui_component::OPTION_QUAD_VERTEX[112];
GLfloat ui_component::OPTION_QUAD_COLOR[224];
GLfloat ui_component::OPTION_BORDER_VERTEX[224];
GLfloat ui_component::OPTION_BORDER_COLOR[448];

GLfloat ui_component::BOARD_VERTEX[160];
GLfloat ui_component::STONE_VERTEX[120];
GLfloat ui_component::STAR_VERTEX[120];
GLfloat ui_component::STAR_LOC[18];
GLfloat ui_component::CROSS_VERTEX[2888];
GLfloat ui_component::CROSS_COLOR[5776];
GLfloat ui_component::GAME_UI_VERTEX[144];
GLfloat ui_component::GAME_UI_COLOR[288];
GLfloat ui_component::GAME_BORDER_VERTEX[272];
GLfloat ui_component::GAME_BORDER_COLOR[544];


int ui_component::COORINDATE_X;
int ui_component::COORINDATE_Y;

int ui_component::SELECTED_SIZE;
int ui_component::SELECTED_STONE;
char ui_component::TIME_TOTAL[4];
char ui_component::TIME_INCREMENT[4];
int ui_component::TIME_TOTAL_PTR;
int ui_component::TIME_INCREMENT_PTR;
bool ui_component::IS_SPECTATOR_ON;
bool ui_component::IS_OPTION_ON;

int ui_component::SELECTED_GAME;
int ui_component::SELECTED_PAGE;
bool ui_component::IS_MESSAGING;
std::string ui_component::sys_message;
std::vector<Game_Descriptor> ui_component::GAME_LIST;
std::vector<Player> ui_component::PLAYER_LIST;
std::vector<Player>::size_type ui_component::PLAYER_OFFSET;
std::vector<std::string> ui_component::LOBBY_MESSAGE_BOARD;
std::vector<std::string> ui_component::GAME_MESSAGE_BOARD;
std::vector<std::string>::size_type ui_component::LOBBY_MESSAGE_OFFSET;
std::vector<std::string>::size_type ui_component::GAME_MESSAGE_OFFSET;

void UI_initialize(int argc, char* argv[])
{
    glutInit(&argc, argv);
    ui_render::init();
    glutSetOption(GLUT_MULTISAMPLE, 4);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Go Variant");


    glDisable(GL_DEPTH_TEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(UI_display);
    glutIdleFunc(UI_idle);
    glutTimerFunc(500, UI_timer, 0);
    glutReshapeFunc(UI_resize);
    glutMouseFunc(UI_mouse);
    glutPassiveMotionFunc(UI_passive_mouse);
    glutKeyboardFunc(UI_keyboard);
    glutWMCloseFunc(UI_onclose);
    glutMainLoop();

}

void UI_resize(int w, int h)
{
    int w_ = 800;
    int h_ = 500;
    glutReshapeWindow(w_, h_);
    glViewport(0, 0, w_, h_);
}

void UI_display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    switch (ui_component::mode){
    case ui_const::UI_START:
        ui_render::render_start_screen();
        break;
    case ui_const::UI_LOBBY:
        ui_render::render_lobby_screen();
        break;
    case ui_const::UI_GAME:
        ui_render::render_game_screen();
        break;
    }

    glutSwapBuffers();
}

void UI_mouse(int button, int state, int x, int y)
{
    switch (ui_component::mode){
    case ui_const::UI_START:
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
            ui_event::handleClickInStartScreen(x, y);
        }
        break;
    case ui_const::UI_LOBBY:
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
            ui_event::handleClickInLobbyScreen(x, y);
        }
        if (button == 3 || button == 4) {
            ui_event::handleScrollInLobbyScreen(button);
        }
        break;
    case ui_const::UI_GAME:
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
            ui_event::handleClickInGameScreen(x, y);
        }
        if (button == 3 || button == 4) {
            ui_event::handleScrollInGameScreen(button);
        }
        break;
    }
    ui_component::has_changed = true;
}

void UI_passive_mouse(int x, int y)
{

    switch (ui_component::mode){
    case ui_const::UI_START:
        //printf("Mouse moved to (%d, %d)\n", x, y);
        if (ui_event::isMouseOverButton(ui_const::UI_BUTTON_LOGIN, x, y))
        {
            ui_component::LOGIN_TEXT_COLOR = ui_const::UI_COLOR_HIGHLIGHTED_TEXT;
        } else if (ui_component::SELECTED_BUTTON != ui_const::UI_BUTTON_LOGIN) {
            ui_component::LOGIN_TEXT_COLOR = ui_const::UI_COLOR_TEXT;
        } else {
            ui_component::LOGIN_TEXT_COLOR = ui_const::UI_COLOR_SELECTED_TEXT;
        }
        if (ui_event::isMouseOverButton(ui_const::UI_BUTTON_REGISTER, x, y))
        {
            ui_component::REGISTER_TEXT_COLOR = ui_const::UI_COLOR_HIGHLIGHTED_TEXT;
        } else if (ui_component::SELECTED_BUTTON != ui_const::UI_BUTTON_REGISTER) {
            ui_component::REGISTER_TEXT_COLOR = ui_const::UI_COLOR_TEXT;
        } else {
            ui_component::REGISTER_TEXT_COLOR = ui_const::UI_COLOR_SELECTED_TEXT;
        }
        if (ui_event::isMouseOverButton(ui_const::UI_BUTTON_SUBMIT, x, y))
        {
            ui_component::SUBMIT_TEXT_COLOR = ui_const::UI_COLOR_HIGHLIGHTED_TEXT;
        } else if (ui_component::SELECTED_BUTTON != ui_const::UI_BUTTON_SUBMIT) {
            ui_component::SUBMIT_TEXT_COLOR = ui_const::UI_COLOR_TEXT;
        }
        if (ui_event::isMouseOverButton(ui_const::UI_BUTTON_EXIT, x, y))
        {
            ui_component::EXIT_TEXT_COLOR = ui_const::UI_COLOR_HIGHLIGHTED_TEXT;
        } else if (ui_component::SELECTED_BUTTON != ui_const::UI_BUTTON_EXIT) {
            ui_component::EXIT_TEXT_COLOR = ui_const::UI_COLOR_TEXT;
        }
        if (ui_event::isMouseOverButton(ui_const::UI_BUTTON_HELP, x, y))
        {
            ui_component::HELP_TEXT_COLOR = ui_const::UI_COLOR_HIGHLIGHTED_TEXT;
        } else if (ui_component::SELECTED_BUTTON != ui_const::UI_BUTTON_HELP) {
            ui_component::HELP_TEXT_COLOR = ui_const::UI_COLOR_TEXT;
        } else {
            ui_component::HELP_TEXT_COLOR = ui_const::UI_COLOR_SELECTED_TEXT;
        }
        ui_component::has_changed = true;
        break;
    case ui_const::UI_LOBBY:
        break;
    case ui_const::UI_GAME:
        //printf("passive mouse at (%d, %d)\n", x, y);
        ui_event::grabCoordinates(x, y);
        ui_component::has_changed = true;
        break;
    }
}

void UI_keyboard(unsigned char c, int x, int y)
{
    switch (ui_component::mode){
    case ui_const::UI_START:
        ui_event::handleKeyInputStartScreen(c);
        break;
    case ui_const::UI_LOBBY:
        ui_event::handleKeyInputLobbyScreen(c);
        break;
    case ui_const::UI_GAME:
        ui_event::handleKeyInputGameScreen(c);
        break;
    }
}

void UI_idle()
{
    Network::get_response();
    if (ui_component::has_changed || ui_component::mode == ui_const::UI_GAME){
        UI_display();
    }
    ui_component::has_changed = false;
}

void UI_timer(int t)
{
    //printf("timer function called at: %d\n", time(NULL));

    switch(ui_component::mode) {
    case ui_const::UI_START:
        Network::form_request(Network::REQUEST_RENEW);
        Network::make_request();
        glutTimerFunc(2000, UI_timer, 0);
        break;
    case ui_const::UI_LOBBY:
        Network::form_request(Network::REQUEST_UPDATE);
        Network::make_request();
        glutTimerFunc(3000, UI_timer, 0);
        break;
    case ui_const::UI_GAME:
        Network::form_request(Network::REQUEST_UPDATE);
        Network::make_request();
        glutTimerFunc(2000, UI_timer, 0);
        break;
    }
}

void UI_onclose()
{
    // gracefully resolve everything when user simply clicks the 'x'


    if (ui_component::mode == ui_const::UI_GAME) {
        // leaves game if user is in a game
        if (true) {
        // resigns game if user is currently playing
        }
        Network::form_request(Network::REQUEST_LEAVE);
        Network::make_request();
    }

    if (ui_component::authentication_code > 0) {
        // logout before exiting
        Network::form_request(Network::REQUEST_LOGOUT);
        Network::make_request();
    }
    Network::unwind();
    //glutLeaveMainLoop();
}



