#include <GL/freeglut.h>
#include "uimanager.h"
#include "networkmanager.h"
#include <stdio.h>

int main(int argc, char **args)
{
    if (Network::initialize() != Network::NETWORK_SUCCESS){
        return -1;
    }

    UI_initialize(argc, args);
    //Network::unwind();
    return 0;
}
