#include <iostream>
#include <stdlib.h>
#include <sysexits.h>
#include "QShell.h"

#define CMD_MAX 256

using namespace std;


void    QShell::setMount_point(char *new_mount_point)

{
    mount_point = new_mount_point;
}


int     QShell::fm(void)

{
    char    cmd[CMD_MAX + 1];
    
    snprintf(cmd, CMD_MAX + 1, "lumina-fm %s", mount_point);
    return system(cmd);
}


void    QShell::unmount(void)

{
    char    cmd[CMD_MAX + 1];
    
    snprintf(cmd, CMD_MAX + 1, "npmount umount %s", mount_point);
    system(cmd);
    exit(EX_OK);
}


int     QShell::reformat(void)

{
    return system("echo Reformat not yet implemented.");
}


int     QShell::minimize(void)

{
    return system("echo Minimize not yet implemented.");
}
