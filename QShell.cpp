#include <iostream>
#include <QFileDialog>
#include <stdlib.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <unistd.h>     // fork()
#include <sys/wait.h>
#include <xtend/proc.h> // xt_get_home_dir()
#include <inttypes.h>   // PRIu64
#include "statfs.h"
#include "QShell.hpp"
#include "QFormatMenu.hpp"
#include "misc.h"


void    QShell::setMount_point(char *new_mount_point)

{
    mount_point = new_mount_point;
}


void    QShell::setDevice(char *new_device)

{
    device = new_device;
}


void    QShell::setFs_type(char *new_fs_type)

{
    fs_type = new_fs_type;
}


int     QShell::fm(void)

{
    // FIXME: Find a more robust way to select a file manager, e.g.
    // list them in a config file that users can edit.
    // For now, check for likely add-ons first, then FMs that come
    // with desktop environments.
    char    fms[][20] = { "pcmanfm-qt", "pcmanfm", "qtfm", "4pane",
			  "caja", "dolphin", "lumina-fm", "konqueror",
			  "krusader", "nautilus", "nemo", "thunar", "" },
	    *fm,
	    fm_array[PATH_MAX + 1],
	    path[PATH_MAX + 1],
	    home_dir[PATH_MAX + 1],
	    message[POPUP_MSG_MAX + 1];
    int     c, status;
    FILE    *fp;
    struct stat     st;
    
    if ( (fm = getenv("QMEDIA_FM")) == NULL )
    {
	if ( xt_get_home_dir(home_dir, PATH_MAX + 1) == NULL )
	{
	    popup("Failed to get home directory.");
	    exit(EX_SOFTWARE);
	}
	snprintf(path, PATH_MAX + 1, "%s/.config/qmediamanager", home_dir);
	if ( (fp = fopen(path, "r")) != NULL )
	{
	    if ( fgets(fm_array, PATH_MAX, fp) != NULL )
	    {
		fm = fm_array;
		fm[strlen(fm)-1] = '\0';
	    }
	    else
		popup(".config/qmediamanager exists, but is empty.");
	    fclose(fp);
	}
	else
	{
	    for (c = 0; fms[c][0] != '\0'; ++c)
	    {
		snprintf(path, PATH_MAX + 1, "%s/bin/%s", PREFIX, fms[c]);
		if ( stat(path, &st) == 0 )
		{
		    fm = fms[c];
		    break;
		}
	    }
	}
    }
    
    if ( fork() == 0 )
    {
	execlp(fm, fm, mount_point, NULL);
	
	snprintf(message, POPUP_MSG_MAX + 1,
		 "qmediamanager: exec failed: %s\n", strerror(errno));
	popup(message);
	return EX_SOFTWARE;
    }
    else
    {
	wait(&status);
	if ( status != 0 )
	{
	    snprintf(message, POPUP_MSG_MAX + 1,
		     "qmediamanager: File manager error: %s\n", strerror(errno));
	    popup(message);
	}
	return status;
    }
}


void    QShell::unmount(void)

{
    int     status;
    char    message[POPUP_MSG_MAX + 1];
    
    if ( fork() == 0 )
    {
	execlp("npmount", "npmount", "umount", mount_point, NULL);
	
	// execlp() should not return
	snprintf(message, POPUP_MSG_MAX + 1,
		 "qmediamanager: exec failed: %s\n", strerror(errno));
	popup(message);
    }
    else
    {
	wait(&status);
	if ( status == 0 )
	    exit(EX_OK);
	
	snprintf(message, POPUP_MSG_MAX + 1,
		 "Unmount failed for %s.\n"
		 "Was the device improperly removed before unmounting?\n"
		 "Run \"pkill qmediamanager\" to force termination.\n",
		 mount_point);
	popup(message);
    }
}


int     QShell::image(void)

{
    QString fileName;
    char    temp_file[PATH_MAX + 1] = "/tmp/qmed.XXXXXXX";
    char    *parent_device;
    char    message[POPUP_MSG_MAX + 1];
    char    home_dir[PATH_MAX + 1];
    int     status;
    struct stat st;
    
    fileName = QFileDialog::getOpenFileName(nullptr,
	tr("Open Image"), xt_get_home_dir(home_dir, PATH_MAX+1),
	tr("Image Files (*.img *.iso)"));
    if ( fileName.isNull() )
	return EX_OK;
    
    if ( mktemp(temp_file) == NULL )
    {
	popup("Error generating temp file name.");
	exit(EX_SOFTWARE);
    }

    if ( (parent_device = parent_device_name(device)) != NULL )
    {
	if ( fork() == 0 )
	{
	    if ( umount(mount_point) == UMOUNT_OK )
	    {
		// FIXME: Use auto-image2dev when it's more robust
		execlp("urxvt", "urxvt", "-e", "auto-image2dev",
			"--success-file", temp_file,
			fileName.toLocal8Bit().data(), parent_device, NULL);
	
		// execlp() should not return
		popup("exec failed.");
	    }
	    else
		popup("Failed to unmount.");
	}
	else
	{
	    // FIXME: urxvt, xterm do not pass back exit status
	    wait(&status);
	    if ( stat(temp_file, &st) != 0 )
	    {
		popup("Image copy failed.");
		exit(EX_UNAVAILABLE);
	    }
	    else
	    {
		popup("Copy complete." RECONNECT_MSG);
		unlink(temp_file);
		exit(EX_OK);
	    }
	}
	free(parent_device);
    }
    else
    {
	snprintf(message, POPUP_MSG_MAX + 1,
		"%s: Cannot determine parent device name", device);
	popup(message);
    }
    
    return EX_OK;
}


int     QShell::reformat(void)

{
    QFormatMenu *menu = new QFormatMenu(mount_point, device);
    menu->show();

    return EX_OK;
}


void    QShell::info(void)

{
    statfs_t    fs;
    char        message[POPUP_MSG_MAX + 1];
    uint64_t    total_blocks, used_blocks, free_blocks, block_size;
    
    if ( STATFS(mount_point, &fs) == 0 )
    {
	total_blocks = fs.f_blocks;
	free_blocks = fs.f_bfree;
	used_blocks = total_blocks - free_blocks;
	block_size = fs.f_bsize;
	
	snprintf(message, POPUP_MSG_MAX + 1,
		 "Mount point:\t%s\n"
		 "Filesystem:\t%s\n"
		 "Device:\t%s\n"
		 "Total:\t%10.2f GB  (%10.2f GiB)\n"
		 "Used:\t%10.2f GB  (%10.2f GiB,  %4.1f%%)\n"
		 "Available:\t%10.2f GB  (%10.2f GiB,  %4.1f%%)\n",
		 fs.f_mntonname,    // From CLI
		 fs_type,           // Frmo CLI
		 device,
		 total_blocks * block_size / 1000000000.0,
		 total_blocks * block_size / GIB,
		 used_blocks * block_size / 1000000000.0,
		 used_blocks * block_size / GIB,
		 100.0 * used_blocks / total_blocks,
		 free_blocks * block_size / 1000000000.0,
		 free_blocks * block_size / GIB,
		 100.0 * free_blocks / total_blocks);
	popup(message);
    }
    else
	popup("Cannot get filesystem info.");
}
