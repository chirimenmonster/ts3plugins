int bouyomi_connect(void);
int bouyomi_close(void);
int bouyomi_sendMessage(const char *bMessage);

void nyushitsu_sendMessage(uint64 oldChannelID, uint64 newChannelID, uint64 myChannelID, const char* nickname);

#define LOG_PLUGIN_NAME "Nyushitsu Plugin"

extern struct TS3Functions ts3Functions;