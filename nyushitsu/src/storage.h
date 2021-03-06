#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define SETTINGS_VERSION_MAJOR	1
#define SETTINGS_VERSION_MINOR	1

typedef struct {
	int versionMajor;
	int versionMinor;
	int enableVoiceOnMove;
	int enableVoiceOnChat;
	int enableNicknameFilter;
	int enableWatchOtherRoom;
	int enableTextChatFilter;
	int enableMyMessage;
	int enableCooldownTimer;
} config_t;

extern config_t config;

int config_init(char *configPath);
int config_read(void);
int config_write(void);
void config_getDefault(config_t *ptr);

#ifdef __cplusplus
}
#endif
