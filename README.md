# QMediaManager

## Purpose

QMediaManager is a desktop-agnostic
GUI application for managing removable media.  It is
meant to be launched by an auto-mount service such as FreeBSD's
sysutils/devd-mount or NetBSD's devpub-mount, to present options to
the user when removable media are inserted.

As most media are attached via USB and similar bus connections (which
cannot be physically locked while mounted), it is
the responsibility of the user to unmount the media before unplugging it.
The QMediaManager popup menu is meant to serve as a reminder that
something is mounted and should not be unplugged.

[Screenshot](https://github.com/outpaddling/qmediamanager/blob/main/qmediamanager.png "Screenshot")
[Screenshot](https://github.com/outpaddling/qmediamanager/blob/main/qmediamanager-fs.png "Screenshot")

## Description

For each filesystem detected on newly attached media,
QMediaManager displays a popup menu with the following options:

1.  Filesystem info: Shows the mount point, filesystem type, device, and
    basic statistics on the associated filesystem.

2.  Launch file manager: Launches the file manager indicated by the
    environment variable QMEDIA_FM if defined.  If QMEDIA_FM is not define,
    but ~/.config/qmediamanager exists, the command name or absolute path
    of the file manager is read from this file.  If neither the QMEDIA_FM
    not ~/.config/qmediamanager exists, qmediamanager searches for common
    file managers, such as those included with popular desktop
    environments, and uses the first one discovered.

    The mount point is provided as a command-line argument to the file
    manager, e.g. "lumina-fm /media/cd1".

3.  Reformat: Partitions and reformats the media with the user-selected
    filesystem.  All partitions on the device are deleted.  The mount point
    associated with the qmediamanager process is unmounted before
    attempting to reformat.

    Note that there will be multiple qmediamanager processes/windows open
    if a connected device contains more than 1 mountable partition.
    Reformat will fail if any other partitions on the device are mounted,
    so all partitions except one must be unmounted before attempting a
    reformat operation.

    Only the most popular filesystems are offered by qmediamanager for
    formatting.  Many others are available by using auto-media-format
    from the command-line or by using fusefs tools directly.

    Permission to reformat devices is determined by device daemons such as
    FreeBSD's devd.  See devfs.conf(1) for details.  The FreeBSD desktop-
    installer(1) script configures typical removable devices with write
    permission for members of the operator group.  If your system was
    configured using desktop-installer, formatting should work out of the
    box in most cases.  Use of the auto-adduser(1) script, part of auto-
    admin, is recommended for adding users, as it supports addition to
    important groups such as operator, wheel, and video.

4.  Copy disk image to device.  Useful for creating installation media,
    bootable live thumb drives, etc.

5.  Unmount: Unmounts the mount point associated with the window, using
    "npmount umount /mount/point".  See npmount(1) or
    [https://github.com/outpaddling/npmount](https://github.com/outpaddling/npmount)
    for more information.

