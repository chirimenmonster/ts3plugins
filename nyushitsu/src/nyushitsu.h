
#include "Windows.h"

#define PLUGIN_DLLNAME		"nyushitsu_plugin"
#define PLUGIN_NAME			"Nyushitsu Plugin"
#define PLUGIN_AUTHOR		"Chirimen"
#define PLUGIN_VERSION		"0.3"
#define PLUGIN_DESCRIPTION	"Send Nickname to BouyomiChan"

#define MENU_LABEL_1		"Settings"

#ifdef __cplusplus
extern "C" {
#endif

int config_init(char *configPath);
int config_read(void);
int config_write(void);

int bouyomi_connect(void);
int bouyomi_close(void);
int bouyomi_sendMessage(const char *bMessage);

void nyushitsu_sendMessage(UINT64 oldChannelID, UINT64 newChannelID, UINT64 myChannelID, const char* nickname);

#ifdef __cplusplus
}
#endif /* __cplusplus */
