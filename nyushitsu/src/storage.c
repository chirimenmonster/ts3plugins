
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shlwapi.h>

#include "nyushitsu.h"
#include "storage.h"

#define CONFIG_AVOID_OTHER_ROOM     "avoid_other_room"
#define CONFIG_FILTER_NUMBER		"filter_number"

static SOCKET sock;
static TCHAR wConfigFile[MAX_PATH];

config_t config;


/* 設定ファイルルーチン群の初期設定 */
int config_init(char *configPath) {
	TCHAR wConfigPath[MAX_PATH];
	size_t s;
	
	mbstowcs_s(&s, wConfigPath, sizeof(wConfigPath), configPath, strlen(configPath));
	PathCombine(wConfigFile, wConfigPath, TEXT(PLUGIN_DLLNAME));
	PathAddExtension(wConfigFile, TEXT(".ini"));

	logMessage("Config File: %ws", wConfigFile);

	return 0;
}

/* 設定ファイルの読み込み　*/
int config_read(void) {

	config.avoidOtherRoom	= GetPrivateProfileInt(TEXT(PLUGIN_DLLNAME), TEXT(CONFIG_AVOID_OTHER_ROOM), 0, wConfigFile);
	config.filterNumber		= GetPrivateProfileInt(TEXT(PLUGIN_DLLNAME), TEXT(CONFIG_FILTER_NUMBER),	0, wConfigFile);

	logMessage("read config: %s = %d", CONFIG_AVOID_OTHER_ROOM, config.avoidOtherRoom);

	return 0;
}

/* 設定ファイルへの保存 */
int config_write(void) {
	TCHAR value[256];

	swprintf(value, sizeof(value), TEXT("%d"), config.avoidOtherRoom);
	WritePrivateProfileString(TEXT(PLUGIN_DLLNAME), TEXT(CONFIG_AVOID_OTHER_ROOM), value, wConfigFile);

	swprintf(value, sizeof(value), TEXT("%d"), config.filterNumber);
	WritePrivateProfileString(TEXT(PLUGIN_DLLNAME), TEXT(CONFIG_FILTER_NUMBER), value, wConfigFile);

	logMessage("write config: %s = %ls", CONFIG_AVOID_OTHER_ROOM, value);

	return 0;
}
