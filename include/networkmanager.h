#include <ws2tcpip.h>
#include <string>

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#define __PORT__ "80"
#define __SERVER__ "tlsiejkjwiiiils.dx.am"

//typedef char (*cryptionFunc)(char);

namespace Network{


    const int NETWORK_SUCCESS = 0;
    const int NETWORK_FAILURE = -1;
    const int WSASTARTUP_FAILURE = -2;
    const int SOCKET_FAILURE = -3;
    const int CONNECTION_FAILURE = -4;
    const int DNS_LOOKUP_FAILURE = -5;
    const int SEND_FAILURE = -6;
    const int RECV_FAILURE = -7;

    const int REQUEST_REGISTER = 0;
    const int REQUEST_LOGIN = 1;
    const int REQUEST_LOGOUT = 2;
    const int REQUEST_HOSTGAME = 3;
    const int REQUEST_JOINGAME = 4;
    const int REQUEST_SPECTATE = 5;
    const int REQUEST_LEAVE = 6;
    const int REQUEST_READY = 7;
    const int REQUEST_UPDATE = 8;
    const int REQUEST_PLAY = 9;
    const int REQUEST_MESSAGE = 10;
    const int REQUEST_SCORE = 11;
    const int REQUEST_RENEW = 12;

    const int buffer_size = 1024;
    const u_long blocking = 0;
    const u_long nonblocking = 1;
    const u_long keepalive = 1;

    extern WSAData wsa_data;
    extern SOCKET local_socket;
    extern struct addrinfo *dns_result;


    extern int buffer_offset;
    extern char outgoing_buffer[];
    extern char incoming_buffer[];
    extern char temp_buffer[];
    extern std::string response;
    extern int expecting;
    extern int chunk_size;
    extern int header_line;
    extern bool connection_live;


    int initialize();
    void unwind();
    int reconnect();
    int make_request();

    void form_request(int);
    int req_register();
    int req_login();
    int req_logout();
    int req_hostgame();
    int req_joingame();
    int req_spectate();
    int req_leave();
    int req_ready();
    int req_update();
    int req_play();
    int req_message();
    int req_score();
    int req_renew();

    void get_response();
    void parse_response();
}



#endif // NETWORKMANAGER_H
