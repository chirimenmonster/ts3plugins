#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define SETTINGS_VERSION_MAJOR	1
#define SETTINGS_VERSION_MINOR	0

int config_init(char *configPath);
int config_read(void);
int config_write(void);
void config_default(void);

typedef struct {
	int versionMajor;
	int versionMinor;
	int enableVoiceOnMove;
	int enableVoiceOnChat;
	int enableNicknameFilter;
	int enableWatchOtherRoom;
} config_t;

extern config_t config;

#ifdef __cplusplus
}
#endif
