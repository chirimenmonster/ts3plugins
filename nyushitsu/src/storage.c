
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shlwapi.h>

#include "nyushitsu.h"
#include "storage.h"

#define PROFNAME						TEXT(PLUGIN_DLLNAME)
#define CONFIG_VERSION_MAJOR			L"config_version_major"
#define CONFIG_VERSION_MINOR			L"config_version_minor"
#define CONFIG_ENABLE_VOICE_ON_MOVE		L"enable_voice_on_move"
#define CONFIG_ENABLE_VOICE_ON_CHAT		L"enable_voice_on_chat"
#define CONFIG_ENABLE_NICKNAME_FILTER	L"enable_nickname_filter"
#define CONFIG_ENABLE_WATCH_OTHER_ROOM	L"enable_watch_other_room"
#define CONFIG_ENABLE_TEXTCHAT_FILTER	L"enable_textchat_filter"
#define CONFIG_ENABLE_MYMESSAGE			L"enable_mymessage"

static SOCKET sock;
static TCHAR wConfigFile[MAX_PATH];

config_t config;

/* デフォルト設定値 */
void config_getDefault(config_t *ptr)
{
	/* デフォルト設定 */
	ptr->versionMajor = SETTINGS_VERSION_MAJOR;
	ptr->versionMinor = SETTINGS_VERSION_MINOR;
	ptr->enableVoiceOnMove = 1;
	ptr->enableVoiceOnChat = 0;
	ptr->enableNicknameFilter = 1;
	ptr->enableWatchOtherRoom = 0;
	ptr->enableTextChatFilter = 1;
	ptr->enableMyMessage = 0;
}

/* 設定ファイルルーチン群の初期設定 */
int config_init(char *configPath)
{
	TCHAR wConfigPath[MAX_PATH];
	size_t s;

	mbstowcs_s(&s, wConfigPath, sizeof(wConfigPath), configPath, strlen(configPath));
	PathCombine(wConfigFile, wConfigPath, PROFNAME);
	PathAddExtension(wConfigFile, L".ini");

	logMessage("Config File: %ws", wConfigFile);

	/* デフォルト設定の取得 */
	config_getDefault(&config);

	return 0;
}


/* 設定ファイルの読み込み　*/
int config_read(void)
{
	int versionMajor, versionMinor;

	versionMajor = GetPrivateProfileInt(PROFNAME, CONFIG_VERSION_MAJOR, -1, wConfigFile);
	versionMinor = GetPrivateProfileInt(PROFNAME, CONFIG_VERSION_MINOR, -1, wConfigFile);

	if (versionMajor != config.versionMajor || versionMinor < config.versionMinor) {
		logMessage("mismatch config version: current %d.%d, file %d.%d", config.versionMajor, config.versionMinor, versionMajor, versionMinor);
		return 0;
	}

	config.enableVoiceOnMove	= GetPrivateProfileInt(PROFNAME, CONFIG_ENABLE_VOICE_ON_MOVE,		config.enableVoiceOnMove,		wConfigFile);
	config.enableVoiceOnChat	= GetPrivateProfileInt(PROFNAME, CONFIG_ENABLE_VOICE_ON_CHAT,		config.enableVoiceOnChat,		wConfigFile);
	config.enableNicknameFilter	= GetPrivateProfileInt(PROFNAME, CONFIG_ENABLE_NICKNAME_FILTER,		config.enableNicknameFilter,	wConfigFile);
	config.enableWatchOtherRoom	= GetPrivateProfileInt(PROFNAME, CONFIG_ENABLE_WATCH_OTHER_ROOM,	config.enableWatchOtherRoom,	wConfigFile);
	config.enableTextChatFilter = GetPrivateProfileInt(PROFNAME, CONFIG_ENABLE_TEXTCHAT_FILTER,		config.enableTextChatFilter,	wConfigFile);
	config.enableMyMessage		= GetPrivateProfileInt(PROFNAME, CONFIG_ENABLE_MYMESSAGE,			config.enableMyMessage,			wConfigFile);

	return 0;
}


static void WritePrivateProfileInt(LPCWSTR appName, LPCWSTR keyName, int value, LPCWSTR fileName)
{
	TCHAR s[16];

	swprintf(s, 15, L"%d", value);
	WritePrivateProfileString(appName, keyName, s, fileName);
}


/* 設定ファイルへの保存 */
int config_write(void)
{
	WritePrivateProfileInt(PROFNAME, CONFIG_VERSION_MAJOR,				config.versionMajor,			wConfigFile);
	WritePrivateProfileInt(PROFNAME, CONFIG_VERSION_MINOR,				config.versionMinor,			wConfigFile);
	WritePrivateProfileInt(PROFNAME, CONFIG_ENABLE_VOICE_ON_MOVE,		config.enableVoiceOnMove,		wConfigFile);
	WritePrivateProfileInt(PROFNAME, CONFIG_ENABLE_VOICE_ON_CHAT,		config.enableVoiceOnChat,		wConfigFile);
	WritePrivateProfileInt(PROFNAME, CONFIG_ENABLE_NICKNAME_FILTER,		config.enableNicknameFilter,	wConfigFile);
	WritePrivateProfileInt(PROFNAME, CONFIG_ENABLE_WATCH_OTHER_ROOM,	config.enableWatchOtherRoom,	wConfigFile);
	WritePrivateProfileInt(PROFNAME, CONFIG_ENABLE_TEXTCHAT_FILTER,		config.enableTextChatFilter,	wConfigFile);
	WritePrivateProfileInt(PROFNAME, CONFIG_ENABLE_MYMESSAGE,			config.enableMyMessage,			wConfigFile);

	logMessage("write config file");

	return 0;
}
