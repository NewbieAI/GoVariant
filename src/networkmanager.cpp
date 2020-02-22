#define _WIN32_WINNT 0x0501 //this is for XP

#include "networkmanager.h"
#include "uimanager.h"
#include "game.h"
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>

WSAData Network::wsa_data;
SOCKET Network::local_socket;
struct addrinfo *Network::dns_result = NULL;

char Network::outgoing_buffer[Network::buffer_size];
char Network::incoming_buffer[Network::buffer_size];
char Network::temp_buffer[Network::buffer_size];
int Network::buffer_offset;
std::string Network::response = "";
int Network::expecting = -1;
int Network::chunk_size = 0;
int Network::header_line = 0;
bool Network::connection_live;

int Network::initialize()
{
    int status;
    WORD version = MAKEWORD(2, 2);
    status = WSAStartup(version, &wsa_data);
    if (status) {
        return WSASTARTUP_FAILURE;
    }
    printf("WSA STARTUP SUCCESSFUL\n");


    struct addrinfo hint;
    ZeroMemory(&hint, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_TCP;

    int retVal = getaddrinfo(__SERVER__, __PORT__, &hint, &dns_result);
    if (retVal != 0){
        unwind();
        return DNS_LOOKUP_FAILURE;
    }

    struct sockaddr_in *remote = (struct sockaddr_in*)dns_result->ai_addr;
    printf("IP address: %s\n", inet_ntoa(remote->sin_addr));
    printf("Port: %d\n", ntohs(remote->sin_port));

    buffer_offset = sprintf(outgoing_buffer, "POST / HTTP/1.1\r\nHost: tlsiejkjwiiiils.dx.am\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ");

    if (reconnect() == SOCKET_ERROR){
        unwind();
        return CONNECTION_FAILURE;
    }
    printf("Connected to %s at port %s\n", __SERVER__, __PORT__);

    return NETWORK_SUCCESS;
}

void Network::unwind()
{
    freeaddrinfo(dns_result);
    closesocket(local_socket);
    WSACleanup();
    printf("network unwinding\n");
}

int Network::reconnect()
{
    // attempt to reconnect to the server here.
    closesocket(local_socket);
    local_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (local_socket == INVALID_SOCKET) {
        printf("FAILED TO CREATE SOCKET: %d\n", WSAGetLastError());
        return SOCKET_ERROR;
    }

    int connectResult = connect(local_socket, dns_result->ai_addr, dns_result->ai_addrlen);

    if (connectResult == 0) {
        printf("CONNECTION SUCCESS\n");
    } else {
        printf("CONNECTION FAILURE: %d\n", WSAGetLastError());
        return SOCKET_ERROR;
    }
    ioctlsocket(local_socket, FIONBIO, (u_long*)&nonblocking);
    connection_live = true;
    return 0;
}

void Network::form_request(int type)
{
    // fills the request struct with the appropriate information
    int request_length;
    switch (type) {
    case Network::REQUEST_REGISTER:
        request_length = req_register();
        break;
    case Network::REQUEST_LOGIN:
        request_length = req_login();
        break;
    case Network::REQUEST_LOGOUT:
        request_length = req_logout();
        break;
    case Network::REQUEST_HOSTGAME:
        request_length = req_hostgame();
        break;
    case Network::REQUEST_JOINGAME:
        request_length = req_joingame();
        break;
    case Network::REQUEST_SPECTATE:
        request_length = req_spectate();
        break;
    case Network::REQUEST_LEAVE:
        request_length = req_leave();
        break;
    case Network::REQUEST_UPDATE:
        request_length = req_update();
        break;
    case Network::REQUEST_PLAY:
        request_length = req_play();
        break;
    case Network::REQUEST_MESSAGE:
        request_length = req_message();
        break;
    case Network::REQUEST_SCORE:
        request_length = req_score();
        break;
    case Network::REQUEST_RENEW:
        request_length = req_renew();
        break;
    }


    sprintf(outgoing_buffer + buffer_offset,
           "%d\r\n\r\n%s",
           request_length,
           temp_buffer);
    printf("request message generated:\n%s\n", outgoing_buffer);
}

void Network::get_response()
{
    int byte;
    int bytes_read;
    char *ptr;
    while ((byte = recv(local_socket, incoming_buffer, buffer_size, 0)) > 0) {
        /*
        expecting = -1: expecting header. reads an entire line
        expecting = 0: expecting a chunk, reads an entire line
        expecting > 0: expecting response body, reads as many bytes as available
        */

        //printf("%d bytes received\n", byte);
        //printf("%s\n", incoming_buffer);

        ptr = incoming_buffer;
        while (ptr < incoming_buffer + byte){
            //printf("expecting = %d\n", expecting);
            if (expecting > 0) {
                // reads as many bytes as available, increment ptr by the amount read
                bytes_read = expecting > incoming_buffer + byte - ptr ? incoming_buffer + byte - ptr : expecting;
                expecting -= bytes_read;

                response += std::string(ptr, bytes_read - (expecting >= 2 ? 0 : 2 - expecting));

                ptr += bytes_read;

            } else if (expecting == 0) {

                // read until CRLF symbol, get the size of the next chunk.
                while (ptr < incoming_buffer + byte && *ptr != '\n') {
                    if (*ptr != '\r') {
                        chunk_size = (chunk_size << 4) + (*ptr < 'a' ? *ptr - '0' : 10 + *ptr - 'a');
                    }
                    ptr++;
                }

                if (ptr < incoming_buffer + byte) {
                    if (chunk_size) {
                        // received a non-zero chunk
                        // proceed to expect the chunk
                        //printf("chunk_size = %d\n", chunk_size);

                        expecting = chunk_size + 2;
                        chunk_size = 0;
                    } else {
                        parse_response();
                        expecting = -1;
                        ptr += 2;
                    }
                    ptr++;
                }

            } else {
                // read a line until CRLF. if line is empty, start
                // expecting a response body.

                while (ptr < incoming_buffer + byte && *ptr != '\n') {
                    header_line++;
                    ptr++;
                }
                if (ptr < incoming_buffer + byte) {

                    if (header_line == 1){
                        expecting = 0;
                    }
                    //printf("header line length: %d\t", header_line);
                    header_line = 0;
                    ptr++;
                }
            }

        }
    }
    if (byte == 0) {
        connection_live = false;
    }
}

void show_response()
{
    // function only used for debugging
    using namespace Network;
    printf("\nResponse:\n");
    for (std::string::size_type x = 0; x < response.size(); x++) {
        if (response[x] == '\0'){
            printf("\\0");
        } else if (response[x] == '\r') {
            printf("\\r");
        } else {
            printf("%c", response[x]);
        }
    }
    printf("\n\n");
}

void print_games()
{
    typedef std::vector<Game_Descriptor>::iterator Iter;
    using namespace ui_component;
    for (Iter i = GAME_LIST.begin(); i != GAME_LIST.end(); ++i){
        printf("Game #%d, %d players:\n", i->id_, i->player_count);
        printf("\tHost: %s\n", i->gamehost_);
        printf("\tOpponent: %s\n", i->opponent_);
        printf("\tTimer: %s\n", i->timer_);
        printf("\tSize: %s\n", i->size_);
        printf("\tStatus: %s\n", i->status_);
        printf("\tSpectator: %s\n", i->spectator_on);
    }
}

void print_users()
{
    typedef std::vector<Player>::iterator Iter;
    using namespace ui_component;
    int a = 1;
    for (Iter i = PLAYER_LIST.begin(); i != PLAYER_LIST.end(); ++i){
        printf("User #%d:\n", a++);
        printf("\tname: %s\n", i->name_);
        printf("\twins: %d\n", i->win_);
        printf("\tloss: %d\n", i->loss_);
        printf("\tcountry: %s\n", i->country_);
    }
}

void Network::parse_response()
{
    using namespace ui_component;

    show_response();
    has_changed = true;
    // first reads which response it is for
    std::string::size_type i = 0;
    int has_update = 0;
    auto next = [&i](char delimiter)
    {
        int j = response.find(delimiter, i);
        std::string s = response.substr(i, j - i);
        i = j + 1;
        //std::cout << "debug: s = " << s << std::endl;
        return s;
    };

    switch (std::stoi(next('='))){
    case REQUEST_REGISTER:
        if (std::stoi(next('\0'))){
            sys_message = "Registration success!";
        } else {
            sys_message = "Registration failure!";
        }

        break;
    case REQUEST_LOGIN:
        if (std::stoi(next('\0'))){
            sys_message = "Login success!";
            mode = ui_const::UI_LOBBY;
            ui_render::set_background(ui_const::UI_BACKGROUND_LOBBY);
            SELECTED_BUFFER = MESSAGE_BUFFER;
            SELECTED_BUFFER_PTR = &MESSAGE_PTR;
            SELECTED_FIELD = ui_const::UI_FIELD_LOBBY_MESSAGE;
            next('=');
            authentication_code = std::stoi(next('\0'));
            next('=');
            has_update = std::stoi(next('\0'));

        } else {
            sys_message = "Login failure!";
        }
        break;
    case REQUEST_LOGOUT:
        if (std::stoi(next('\0'))){

            currentUser = "";
            authentication_code = -1;
            mode = ui_const::UI_START;
            LOBBY_MESSAGE_OFFSET = 0;
            *SELECTED_BUFFER_PTR = 0;
            SELECTED_BUFFER[0] = '\0';
            SELECTED_BUFFER = USERNAME_BUFFER;
            SELECTED_BUFFER_PTR = &USERNAME_PTR;
            SELECTED_FIELD = ui_const::UI_FIELD_NAME_LOGIN;
            ui_render::set_background(ui_const::UI_BACKGROUND_START);
            sys_message = "You have logged out.";

        } else {

        }
        break;
    case REQUEST_HOSTGAME:
        if (std::stoi(next('\0'))){
            //printf("hosting game response received...\n");
            IS_OPTION_ON = false;
            mode = ui_const::UI_GAME;
            GAME_MESSAGE_OFFSET = 0;
            *SELECTED_BUFFER_PTR = 0;
            SELECTED_BUFFER[0] = '\0';
            next('=');
            has_update = std::stoi(next('\0'));
        } else {

        }
        break;
    case REQUEST_JOINGAME:
        if (std::stoi(next('\0'))){
            // successful joining the game;

            mode = ui_const::UI_GAME;
            GAME_MESSAGE_OFFSET = 0;
            *SELECTED_BUFFER_PTR = 0;
            SELECTED_BUFFER[0] = '\0';
            next('=');
            has_update = std::stoi(next('\0'));

        } else {

        }
        break;
    case REQUEST_SPECTATE:
        if (std::stoi(next('\0'))){
            // successful spectating the game

            mode = ui_const::UI_GAME;
            GAME_MESSAGE_OFFSET = 0;
            *SELECTED_BUFFER_PTR = 0;
            SELECTED_BUFFER[0] = '\0';
            next('=');
            has_update = std::stoi(next('\0'));

        } else {

        }
        break;
    case REQUEST_LEAVE:
        if (std::stoi(next('\0'))){
            mode = ui_const::UI_LOBBY;
            LOBBY_MESSAGE_OFFSET = 0;
            *SELECTED_BUFFER_PTR = 0;
            SELECTED_BUFFER[0] = '\0';
            next('=');
            has_update = std::stoi(next('\0'));
        } else {

        }
        break;
    case REQUEST_READY:
        if (std::stoi(next('\0'))){

        } else {

        }
        break;
    case REQUEST_UPDATE:
        if (std::stoi(next('\0'))){
            // successful update
            has_update = true;
        } else {
            // failed update, retry connection?
            Network::reconnect();
        }
        break;
    case REQUEST_PLAY:
        if (std::stoi(next('\0'))){
            next('=');
            has_update = std::stoi(next('\0'));
        } else {

        }
        break;
    case REQUEST_MESSAGE:
        if (std::stoi(next('\0'))){
            next('=');
            has_update = std::stoi(next('\0'));
        } else {

        }
        break;
    case REQUEST_SCORE:
        if (std::stoi(next('\0'))){

        } else {

        }
        break;
    case REQUEST_RENEW:
        if (std::stoi(next('\0'))){

        } else {

        }
        break;
    }

    if (has_update) {
        std::string s;
        if (mode == ui_const::UI_LOBBY) {
            // load ongoing games
            GAME_LIST.clear();
            PLAYER_LIST.clear();

            next('=');
            lobby_stamp = next('\0');

            next('=');
            int game_count = std::stoi(next('\0'));


            Game_Descriptor cur;
            for (int i = 0; i < game_count; i++) {

                cur.id_ = std::stoi(next('\r'));
                cur.player_count = std::stoi(next('\r'));
                s = next('\r');
                cur.gamehost_[s.copy(cur.gamehost_, 32)] = '\0';
                s = next('\r');
                cur.opponent_[s.copy(cur.opponent_, 32)] = '\0';
                s = next('\r');
                cur.size_[s.copy(cur.size_, 10)] = '\0';
                s = next('\r');
                cur.color_[s.copy(cur.color_, 10)] = '\0';
                s = next('\r');
                cur.timer_[s.copy(cur.timer_, 20)] = '\0';
                s = next('\r');
                cur.status_[s.copy(cur.status_, 10)] = '\0';
                s = next('\r');
                cur.spectator_on[s.copy(cur.spectator_on, 5)] = '\0';
                GAME_LIST.push_back(cur);
                next('\0');
            }

            //print_games();
            // load current players
            next('=');
            int player_count = std::stoi(next('\0'));
            Player p;

            for (int i = 0; i < player_count; i++){
                s = next('\r');
                p.name_[s.copy(p.name_, 32)] = '\0';

                p.win_ = std::stoi(next('\r'));
                p.loss_ = std::stoi(next('\r'));

                s = next('\r');
                p.country_[s.copy(p.country_, 32)] = '\0';

                next('\0');
                PLAYER_LIST.push_back(p);
            }

            //print_users();
            // remember to modify the count
            // load all new messages
            next('=');
            int message_count = std::stoi(next('\0'));

            for (int i = 0; i < message_count; i++) {
                ui_event::appendMessage(&LOBBY_MESSAGE_BOARD,
                                        next('\0'),
                                        1.225);
            }

            if (LOBBY_MESSAGE_OFFSET) {
                LOBBY_MESSAGE_OFFSET += message_count;
            }

        } else {
            PLAYER_LIST.clear();
            // load the players
            next('=');
            std::string moves = next(':');
            game_stamp = moves + ":" + next('\0');
            int move_number = std::stoi(moves);

            // updates game information such has host_color_
            // in case game status changes, host leaves, or
            // opponent changes
            currentGame.set_hostname(next('\r'));
            currentGame.set_opponent(next('\r'));
            currentGame.set_playerblack(next('\r'));
            currentGame.set_playerwhite(next('\r'));

            if (currentGame.status() == Game::status_player &&
                currentGame.hostname() == currentUser) {
                currentGame.setStatus(Game::status_host);
            }
            next('\0');


            next('=');
            int player_count = std::stoi(next('\0'));
            printf("%d players found\n\n", player_count);

            Player p;
            for (int i = 0; i < player_count; i++) {
                s = next('\r');
                p.name_[s.copy(p.name_, 32)] = '\0';

                p.win_ = std::stoi(next('\r'));
                p.loss_ = std::stoi(next('\r'));

                s = next('\r');
                p.country_[s.copy(p.country_, 32)] = '\0';

                PLAYER_LIST.push_back(p);
                next('\0');
            }

            // load the new moves

            next('=');
            int move_count = std::stoi(next('\0'));

            if (currentGame.moves() + move_count == move_number) {
                for (int i = 0; i < move_count; i++) {
                    printf("\tloading new move!\n");
                    if (currentGame.is_uptodate()) {
                        // revise this section for rule change


                        currentGame.play(std::stoi(next('\0')));
                    } else {
                        currentGame.queue_move(std::stoi(next('\0')));
                    }
                }
            }

            // load new messages
            next('=');
            int message_count = std::stoi(next('\0'));
            printf("%d messages received\n\n", message_count);

            for (int i = 0; i < message_count; i++) {
                ui_event::appendMessage(&GAME_MESSAGE_BOARD,
                                        next('\0'),
                                        0.85);
            }

            if (GAME_MESSAGE_OFFSET) {
                GAME_MESSAGE_OFFSET += message_count;
            }


            if (move_number == (int)currentGame.moves()) {

                // game has started, start the timer
                // in this version server does not respond
                // where the timer needs to be stopped
                if (move_number % 2) {
                    // white's turn
                    currentGame.timer_white()->stop();
                    currentGame.timer_black()->start();

                } else {
                    // black's turn
                    currentGame.timer_black()->stop();
                    currentGame.timer_white()->start();

                }

            } else {

                // this section will be deleted after
                // everything starts working.
                printf("\nGame NOT Started\n");
                printf("server moves %d != local_moves %d\n",
                       move_number,
                       currentGame.moves());
            }

        }
    }
    response.clear();
}

int Network::make_request()
{
    if (!connection_live) {
        reconnect();
    }

    int bytes = send(local_socket, outgoing_buffer, strlen(outgoing_buffer), 0);

    if (bytes == SOCKET_ERROR){
        printf("FAILURE SENDING REQUEST: %d\n", WSAGetLastError());
        return Network::SEND_FAILURE;
    } else {
        printf("%d bytes sent to server\n", bytes);
    }
    return Network::NETWORK_SUCCESS;
}

int Network::req_register()
{
    int bytes = sprintf(temp_buffer,
                        "type=register&username=%s&password=%s&country=%s",
                        ui_component::USERNAME_BUFFER,
                        ui_component::PASSWORD_BUFFER,
                        ui_component::COUNTRY_BUFFER);
    return bytes;
}

int Network::req_login()
{
    int bytes = sprintf(temp_buffer,
                        "type=login&username=%s&password=%s",
                        ui_component::USERNAME_BUFFER,
                        ui_component::PASSWORD_BUFFER);
    return bytes;
}

int Network::req_logout()//unfinished
{
    int bytes = sprintf(temp_buffer,
                        "type=logout&authentication_code=%d",
                        ui_component::authentication_code);
    return bytes;
}

int Network::req_hostgame()//unfinished
{
    std::string color;
    switch (ui_component::currentGame.host_color()){
    case Game::player_black:
        color = "black";
        break;
    case Game::player_white:
        color = "white";
        break;
    case Game::player_random:
        color = "random";
        break;
    }

    int bytes = sprintf(temp_buffer,
                        "type=host&authentication_code=%d&size=%dx%d&host_color=%s&timer=%d min %%2B %d sec&spectator_on=%d",
                        ui_component::authentication_code,
                        ui_component::currentGame.size(),
                        ui_component::currentGame.size(),
                        color.c_str(),
                        ui_component::currentGame.timer_total(),
                        ui_component::currentGame.timer_inc(),
                        ui_component::currentGame.spectator_on());
    return bytes;
}

int Network::req_joingame()//unfinished
{
    int id = ui_component::SELECTED_GAME + ui_component::SELECTED_PAGE * 10;
    //printf("id = %d\n", id);
    int bytes = sprintf(temp_buffer,
                        "type=join&authentication_code=%d&game_id=%d",
                        ui_component::authentication_code,
                        ui_component::GAME_LIST[id].id_);
    return bytes;
}

int Network::req_spectate()//unfinished
{
    int id = ui_component::SELECTED_GAME + ui_component::SELECTED_PAGE * 10;
    //printf("id = %d\n", id);
    int bytes = sprintf(temp_buffer,
                        "type=spectate&authentication_code=%d&game_id=%d",
                        ui_component::authentication_code,
                        ui_component::GAME_LIST[id].id_);
    return bytes;
}

int Network::req_leave()
{

    int bytes = sprintf(temp_buffer,
                        "type=leave&authentication_code=%d",
                        ui_component::authentication_code);

    return bytes;
}

int Network::req_ready()
{
    // this function has been rendered redundant by the req_score function
    // to be removed later on.
    // However, I do believe that using the user_score request for
    // multiple functions violated the principle that functions
    // should have a clear and separate responsibility.
}

int Network::req_update()//unfinished
{
    int bytes;
    if (ui_component::mode == ui_const::UI_LOBBY) {
        bytes = sprintf(temp_buffer,
                        "type=update&authentication_code=%d&stamp=%s",
                        ui_component::authentication_code,
                        ui_component::lobby_stamp.c_str());
    } else {
        bytes = sprintf(temp_buffer,
                        "type=update&authentication_code=%d&stamp=%s",
                        ui_component::authentication_code,
                        ui_component::game_stamp.c_str());
    }
    return bytes;
    // cool story for the times we are facing. I must resolve to
    // complete the job rather quickly
    // Did the font really change?
}

int Network::req_play()//unfinished
{
    int bytes = sprintf(temp_buffer,
                        "type=play&authentication_code=%d&action=%d&stamp=%s",
                        ui_component::authentication_code,
                        ui_component::currentAction,
                        ui_component::game_stamp.c_str());
    return bytes;
}

int Network::req_message()//unfinished
{
    int bytes;

    if (ui_component::mode == ui_const::UI_LOBBY){
        bytes = sprintf(temp_buffer,
                        "type=message&authentication_code=%d&message=k[%s]: %s&stamp=%s",
                        ui_component::authentication_code,
                        ui_component::currentUser.c_str(),
                        ui_component::MESSAGE_BUFFER,
                        ui_component::lobby_stamp.c_str());
    } else {
        bytes = sprintf(temp_buffer,
                        "type=message&authentication_code=%d&message=k[%s]: %s&stamp=%s",
                        ui_component::authentication_code,
                        ui_component::currentUser.c_str(),
                        ui_component::MESSAGE_BUFFER,
                        ui_component::game_stamp.c_str());
    }
    return bytes;
}

int Network::req_score()
{
    int val, black, white;
    black = ui_component::currentGame.score_black();
    white = ui_component::currentGame.score_white();
    if ((ui_component::currentGame.host_color() == Game::player_white) ^
        (ui_component::currentGame.status() == Game::status_host)) {
        val = black < 0 ? black : black * 362 + white;
    } else {
        val = white < 0 ? white : black * 362 + white;
    }

    int bytes = sprintf(temp_buffer,
                        "type=score&authentication_code=%d&score=%d",
                        ui_component::authentication_code,
                        val);
    return bytes;
}

int Network::req_renew()
{
    int bytes = sprintf(temp_buffer,
                        "type=renew");
    return bytes;
}

//
