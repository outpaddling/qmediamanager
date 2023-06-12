# QMediaManager

## Purpose

QMediaManager is a GUI application for managing removable media.  It is  
meant to be launched by an automount service such as FreeBSD's           
sysutils/automount, to provide the user options when removable media     
are inserted.                                                            

[Screenshot](https://github.com/outpaddling/qmediamanager/blob/main/qmediamanager.png "Screenshot")

## Description

QMediaManager presents the following options:                            
									 
1.  Launch file manager: Launches the file manager indicated by the       
    environment variable QMEDIA_FM if defined.  If QMEDIA_FM is not define,  
    but ~/.config/qmediamanager exists, the command name or absolute path    
    of the file manager is read from this file.  If neither the QMEDIA_FM    
    not ~/.config/qmediamanager exists, qmediamanager searches for common    
    file managers, such as those included with popular desktop               
    environments, and uses the first one discovered.                         
									     
    The mount point is provided as a command-line argument to the file
    manager, e.g. "lumina-fm /media/cd1".

2.  Unmount: Unmounts the mount point associated with the window using
    "npmount umount /mount/point".  See npmount(1) for more information.

3.  Reformat: Partitions and reformats the media with the user-selected
    filesystem.  All partitions on the device are deleted.  The mount point
    associated with the qmediamanager process is unmounted before
    attempting to reformat.
    
    Note that there will be multiple qmediamanager processes/windows open
    if a connected device contains more than 1 mountable partition.
    Reformat will fail if any other partitions on the device are mounted,
    so all partitions except one must be unmounted before attempting a
    reformat operation.
    
    Permission to reformat devices is determined by device daemons such as
    FreeBSD's devd.  See devfs.conf(1) for details.  The FreeBSD desktop-
    installer(1) script configures typical removable devices with write
    permission for members of the operator group.  If your system was
    configured using desktop-installer, formatting should work out of the
    box in most cases.  Use of the auto-adduser(1) script, part of auto-
    admin, is recommended for adding users, as it supports addition to
    important groups such as operator, wheel, and video.

4.  Filesystem info: Shows the mount point, filesystem type, device, and
    basic statistics on the associated filesystem.


