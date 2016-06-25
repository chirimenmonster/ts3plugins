/*
 * TeamSpeak 3 demo plugin
 *
 * Copyright (c) 2008-2016 TeamSpeak Systems GmbH
 */

#ifdef _WIN32
#pragma warning (disable : 4100)  /* Disable Unreferenced parameter warning */
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <winsock2.h>

#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"
#include "plugin.h"
#include "nyushitsu.h"

struct TS3Functions ts3Functions;

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src) { strncpy(dest, src, destSize-1); (dest)[destSize-1] = '\0'; }
#endif

#define PLUGIN_API_VERSION 20

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

static char* pluginID = NULL;

#ifdef _WIN32
/* Helper function to convert wchar_T to Utf-8 encoded strings on Windows */
static int wcharToUtf8(const wchar_t* str, char** result) {
	int outlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
	*result = (char*)malloc(outlen);
	if(WideCharToMultiByte(CP_UTF8, 0, str, -1, *result, outlen, 0, 0) == 0) {
		*result = NULL;
		return -1;
	}
	return 0;
}
#endif

/*********************************** Required functions ************************************/
/*
 * If any of these required functions is not implemented, TS3 will refuse to load the plugin
 */

/* Unique name identifying this plugin */
const char* ts3plugin_name() {
#ifdef _WIN32
	/* TeamSpeak expects UTF-8 encoded characters. Following demonstrates a possibility how to convert UTF-16 wchar_t into UTF-8. */
	static char* result = NULL;  /* Static variable so it's allocated only once */
	if(!result) {
		const wchar_t* name = L"Nyushitsu Plugin";
		if(wcharToUtf8(name, &result) == -1) {  /* Convert name into UTF-8 encoded result */
			result = PLUGIN_NAME;  /* Conversion failed, fallback here */
		}
	}
	return result;
#else
	return PLUGIN_NAME;
#endif
}

/* Plugin version */
const char* ts3plugin_version() {
    return PLUGIN_VERSION;
}

/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */
int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}

/* Plugin author */
const char* ts3plugin_author() {
	/* If you want to use wchar_t, see ts3plugin_name() on how to use */
    return PLUGIN_AUTHOR;
}

/* Plugin description */
const char* ts3plugin_description() {
	/* If you want to use wchar_t, see ts3plugin_name() on how to use */
    return PLUGIN_DESCRIPTION;
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}

/*
 * Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
 * If the function returns 1 on failure, the plugin will be unloaded again.
 */
int ts3plugin_init() {
    char appPath[PATH_BUFSIZE];
    char resourcesPath[PATH_BUFSIZE];
    char configPath[PATH_BUFSIZE];
	char pluginPath[PATH_BUFSIZE];

    /* Your plugin init code here */
    printf("PLUGIN: init\n");

    /* Example on how to query application, resources and configuration paths from client */
    /* Note: Console client returns empty string for app and resources path */
    ts3Functions.getAppPath(appPath, PATH_BUFSIZE);
    ts3Functions.getResourcesPath(resourcesPath, PATH_BUFSIZE);
    ts3Functions.getConfigPath(configPath, PATH_BUFSIZE);
	ts3Functions.getPluginPath(pluginPath, PATH_BUFSIZE);

	printf("PLUGIN: App path: %s\nResources path: %s\nConfig path: %s\nPlugin path: %s\n", appPath, resourcesPath, configPath, pluginPath);

    return 0;
	/* 0 = success, 1 = failure, -2 = failure but client will not show a "failed to load" warning */
	/* -2 is a very special case and should only be used if a plugin displays a dialog (e.g. overlay) asking the user to disable
	 * the plugin again, avoiding the show another dialog by the client telling the user the plugin failed to load.
	 * For normal case, if a plugin really failed to load because of an error, the correct return value is 1. */
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown() {
    /* Your plugin cleanup code here */
    printf("PLUGIN: shutdown\n");

	/*
	 * Note:
	 * If your plugin implements a settings dialog, it must be closed and deleted here, else the
	 * TeamSpeak client will most likely crash (DLL removed but dialog from DLL code still open).
	 */

	/* Free pluginID if we registered it */
	if(pluginID) {
		free(pluginID);
		pluginID = NULL;
	}
}

/****************************** Optional functions ********************************/
/*
 * Following functions are optional, if not needed you don't need to implement them.
 */

/************************** TeamSpeak callbacks ***************************/
/*
 * Following functions are optional, feel free to remove unused callbacks.
 * See the clientlib documentation for details on each function.
 */

/* Clientlib */

void ts3plugin_onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage) {
	char* nickname;
	char msg[BUFSIZ];
	anyID myID;
	uint64 myChannelID;

	snprintf(msg, sizeof(msg), "ClientMoveEvent: clientID=%d, oldChannelID=%lld, newChannelID=%lld, visibility=%d", clientID, oldChannelID, newChannelID, visibility);
	ts3Functions.logMessage(msg, LogLevel_INFO, PLUGIN_NAME, 0);

    // 自分の ID を取得
	if (ts3Functions.getClientID(serverConnectionHandlerID, &myID) != ERROR_ok) {
		ts3Functions.logMessage("fail to getClientID", LogLevel_INFO, PLUGIN_NAME, 0);
		return;
	}
    // 現在のチャンネル ID を取得
	if (ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &myChannelID) != ERROR_ok) {
		ts3Functions.logMessage("fail to getChannelOfClient", LogLevel_INFO, PLUGIN_NAME, 0);
		return;
	}
    
    // 自分に関するイベントは読み上げない
    if (clientID == myID) {
        return;
    }

	if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &nickname) == ERROR_ok) {
		nyushitsu_sendMessage(oldChannelID, newChannelID, myChannelID, nickname);
		ts3Functions.freeMemory(nickname);
	}
	else {
		ts3Functions.logMessage("fail to getClientVariableAsString", LogLevel_INFO, PLUGIN_NAME, 0);
	}
}

void ts3plugin_onClientMoveMovedEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, anyID moverID, const char* moverName, const char* moverUniqueIdentifier, const char* moveMessage) {
	char* nickname;
	char msg[BUFSIZ];
	anyID myID;
	uint64 myChannelID;

	snprintf(msg, sizeof(msg), "ClientMoveMovedEvent: clientID=%d, oldChannelID=%lld, newChannelID=%lld, visibility=%d", clientID, oldChannelID, newChannelID, visibility);
	ts3Functions.logMessage(msg, LogLevel_INFO, PLUGIN_NAME, 0);

	// 自分の ID を取得
	if (ts3Functions.getClientID(serverConnectionHandlerID, &myID) != ERROR_ok) {
		ts3Functions.logMessage("fail to getClientID", LogLevel_INFO, PLUGIN_NAME, 0);
		return;
	}
	// 現在のチャンネル ID を取得
	if (ts3Functions.getChannelOfClient(serverConnectionHandlerID, myID, &myChannelID) != ERROR_ok) {
		ts3Functions.logMessage("fail to getChannelOfClient", LogLevel_INFO, PLUGIN_NAME, 0);
		return;
	}

	// 自分に関するイベントは読み上げない
	if (clientID == myID) {
		return;
	}

	if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &nickname) == ERROR_ok) {
		nyushitsu_sendMessage(oldChannelID, newChannelID, myChannelID, nickname);
		ts3Functions.freeMemory(nickname);
	}
	else {
		ts3Functions.logMessage("fail to getClientVariableAsString", LogLevel_INFO, PLUGIN_NAME, 0);
	}
}
