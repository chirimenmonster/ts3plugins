
int config_init(void);
int config_read(void);
int config_write(void);
int config_applymenu(char* pluginID, int menuID, int enable);

int bouyomi_connect(void);
int bouyomi_close(void);
int bouyomi_sendMessage(const char *bMessage);

void nyushitsu_sendMessage(uint64 oldChannelID, uint64 newChannelID, uint64 myChannelID, const char* nickname);

#define PLUGIN_DLLNAME		"nyushitsu_plugin"
#define PLUGIN_NAME			"Nyushitsu Plugin"
#define PLUGIN_AUTHOR		"Chirimen"
#define PLUGIN_VERSION		"0.3-dev"
#define PLUGIN_DESCRIPTION	"Send Nickname to BouyomiChan"

#define MENU_LABEL_1		u8"他チャンネル間移動を無視する"
#define MENU_LABEL_2		u8"他チャンネル間移動を無視しない"

extern struct TS3Functions ts3Functions;