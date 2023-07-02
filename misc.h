
#define POPUP_MSG_MAX       256
#define UNMOUNT_TEXT_MAX    64
#define CMD_MAX             1024
#define GIB                 (1024.0 * 1024.0 * 1024.0)
#define PREFIX              "/usr/local"

enum { UMOUNT_OK, UMOUNT_FAILED };

void    popup(const char *message);
char    *get_parent_device_name(const char *partition);
int     umount(const char *mount_point);
