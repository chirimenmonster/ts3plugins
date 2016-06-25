int bouyomi_connect(void);
int bouyomi_close(void);
int bouyomi_sendMessage(const char *bMessage);

void nyushitsu_sendMessage(uint64 oldChannelID, uint64 newChannelID, uint64 myChannelID, const char* nickname);

#define PLUGIN_NAME			"Nyushitsu Plugin"
#define PLUGIN_AUTHOR		"Chirimen"
#define PLUGIN_VERSION		"0.2"
#define PLUGIN_DESCRIPTION	"Send Nickname to BouyomiChan"


extern struct TS3Functions ts3Functions;