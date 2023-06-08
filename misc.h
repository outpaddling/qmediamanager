
#define POPUP_MSG_MAX       256
#define UNMOUNT_TEXT_MAX    64
#define CMD_MAX             1024
#define PREFIX              "/usr/local"

enum { UMOUNT_OK, UMOUNT_FAILED };

void    popup(const char *message);
