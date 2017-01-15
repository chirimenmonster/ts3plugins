
#include "Windows.h"

#define PLUGIN_DLLNAME		"nyushitsu_plugin"
#define PLUGIN_NAME			"Nyushitsu Plugin"
#define PLUGIN_AUTHOR		"Chirimen"
#define PLUGIN_VERSION		"0.6-dev"
#define PLUGIN_DESCRIPTION	"Send Nickname to BouyomiChan"

#define MENU_LABEL_1		"Settings"

#define BOUYOMI_HEADERSIZE	15
#define MAX_NICKNAME		256
#define MAX_BOUYOMICMD		1024
#define MAX_BOUYOMIMSG		(MAX_BOUYOMICMD - BOUYOMI_HEADERSIZE)

/* テキストチャットの最大サイズ (バイト) */
#define MAX_TEXTCHAT		140

#ifdef __cplusplus
extern "C" {
#endif

void logMessage(const char *format, ...);

void nyushitsu_sendMessage(UINT64 oldChannelID, UINT64 newChannelID, UINT64 myChannelID, const char *nickname);
void nyushitsu_sendChatMessage(const char *fromName, const char *message, UINT64 fromID, UINT64 myID);

void nyushitsu_readConfig(char *configPath);
void nyushitsu_writeConfig(void);

void nyushitsu_configDialog(void *qParentWidget);
void nyushitsu_showDialog(void);
void nyushitsu_deleteDialog(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
