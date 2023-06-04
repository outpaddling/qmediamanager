#include <iostream>
#include <stdlib.h>
#include "QShell.h"

using namespace std;


int     QShell::mount(void)

{
    return system("echo Mount clicked."); 
}


int     QShell::unmount(void)

{
    return system("echo Unmount clicked.");
}


int     QShell::format(void)

{
    return system("echo Format clicked.");
}
