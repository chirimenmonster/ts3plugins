/*
 * Nyushitsu Plugin
 *
 * Copyright (c) 2016 Chirimen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <shlwapi.h>

#include "ts3_functions.h"
#include "nyushitsu.h"

#define CONFIG_AVOID_OTHER_ROOM     "avoid_other_room"

static SOCKET sock;
static LPSTR wConfigFile[MAX_PATH];

static struct {
	TCHAR wConfigFile[MAX_PATH];
	int avoidOtherRoom;
} config;

/* 設定ファイルルーチン群の初期設定 */
int config_init(void) {
	char configPath[MAX_PATH];
	TCHAR wConfigPath[MAX_PATH];
	size_t s;
	char msg[1024];

	/* 設定ファイル名の初期化 */
	ts3Functions.getConfigPath(configPath, MAX_PATH);
	
	mbstowcs_s(&s, wConfigPath, sizeof(wConfigPath), configPath, strlen(configPath));
	PathCombine(config.wConfigFile, wConfigPath, TEXT(PLUGIN_DLLNAME));
	PathAddExtension(config.wConfigFile, TEXT(".ini"));

	snprintf(msg, sizeof(msg), "Config File: %ws", config.wConfigFile);
	ts3Functions.logMessage(msg, LogLevel_INFO, PLUGIN_NAME, 0);

	return 0;
}

/* 設定ファイルの読み込み　*/
int config_read(void) {
	char msg[1024];

	config.avoidOtherRoom = GetPrivateProfileInt(TEXT(PLUGIN_DLLNAME), TEXT(CONFIG_AVOID_OTHER_ROOM), 0, config.wConfigFile);

	snprintf(msg, sizeof(msg), "read config: %s = %d", CONFIG_AVOID_OTHER_ROOM, config.avoidOtherRoom);
	ts3Functions.logMessage(msg, LogLevel_INFO, PLUGIN_NAME, 0);

	return 0;
}

/* 設定ファイルへの保存 */
int config_write(void) {
	TCHAR value[256];
	char msg[1024];

	swprintf(value, sizeof(value), TEXT("%d"), config.avoidOtherRoom);
	WritePrivateProfileString(TEXT(PLUGIN_DLLNAME), TEXT(CONFIG_AVOID_OTHER_ROOM), value, config.wConfigFile);

	snprintf(msg, sizeof(msg), "write config: %s = %ls", CONFIG_AVOID_OTHER_ROOM, value);
	ts3Functions.logMessage(msg, LogLevel_INFO, PLUGIN_NAME, 0);

	return 0;
}

/* メニュー状態のスイッチ */
int config_applymenu(char* pluginID, int menuID, int enable) {
	if (enable >= 0) {
		config.avoidOtherRoom = enable;
	}
    if (config.avoidOtherRoom == 0) {
        ts3Functions.setPluginMenuEnabled(pluginID, menuID,     1);
        ts3Functions.setPluginMenuEnabled(pluginID, menuID + 1, 0);
    } else {
        ts3Functions.setPluginMenuEnabled(pluginID, menuID,     0);
        ts3Functions.setPluginMenuEnabled(pluginID, menuID + 1, 1);
    }
    return 0;
}


/* 棒読みちゃんへ接続する */
int bouyomi_connect(void) {
	// 棒読みちゃんのTCPサーバへ接続
	char* sHost = "127.0.0.1";	// 棒読みちゃんが動いているホスト
	int iPort = 50001;			// 棒読みちゃんのTCPサーバのポート番号(デフォルト値)

	WSADATA wsaData;
	int err;
	char msg[1024];

	err = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (err != 0) {
		switch (err) {
		case WSASYSNOTREADY:
			printf("WSASYSNOTREADY\n");
			break;
		case WSAVERNOTSUPPORTED:
			printf("WSAVERNOTSUPPORTED\n");
			break;
		case WSAEINPROGRESS:
			printf("WSAEINPROGRESS\n");
			break;
		case WSAEPROCLIM:
			printf("WSAEPROCLIM\n");
			break;
		case WSAEFAULT:
			printf("WSAEFAULT\n");
			break;
		}
		ts3Functions.logMessage("Error WSAStartup", LogLevel_ERROR, PLUGIN_NAME, 0);
		return 1;
	}

	struct sockaddr_in server;

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		snprintf(msg, sizeof(msg), "socket : %d\n", WSAGetLastError());
		ts3Functions.logMessage(msg, LogLevel_ERROR, PLUGIN_NAME, 0);
		return 1;
	}

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.S_un.S_addr = inet_addr(sHost);

	// サーバに接続
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0) {
		snprintf(msg, sizeof(msg), "connect : %d\n", WSAGetLastError());
		ts3Functions.logMessage(msg, LogLevel_ERROR, PLUGIN_NAME, 0);
		return 1;
	}
	return 0;
}

/* 棒読みちゃんへの接続を閉じる */
int bouyomi_close(void) {
	closesocket(sock);
	WSACleanup();
	return 0;
}

/* 棒読みちゃんへテキストを送る */
int bouyomi_sendMessage(const char *bMessage) {
	INT16 iVoice = 1;
	INT16 iVolume = -1;
	INT16 iSpeed = -1;
	INT16 iTone = -1;
	INT16 iCommand = 0x0001;
	INT32 iLength;
	char bCode = 0;
	char buf[BUFSIZ];
	char msg[1024];

	if (bouyomi_connect() != 0) {
		return 1;
	}

	iLength = (int)strlen(bMessage);
	snprintf(msg, sizeof(msg), u8"send to BoyomiChan (%d): %s\n", iLength, bMessage);
	ts3Functions.logMessage(msg, LogLevel_INFO, PLUGIN_NAME, 0);

	memset(buf, 0, sizeof(buf));
	*(u_short *)&buf[0] = iCommand; //コマンド（ 0:メッセージ読み上げ）
	*(u_short *)&buf[2] = iSpeed;   //速度    （-1:棒読みちゃん画面上の設定）
	*(u_short *)&buf[4] = iTone;    //音程    （-1:棒読みちゃん画面上の設定）
	*(u_short *)&buf[6] = iVolume;  //音量    （-1:棒読みちゃん画面上の設定）
	*(u_short *)&buf[8] = iVoice;   //声質    （ 0:棒読みちゃん画面上の設定、1:女性1、2:女性2、3:男性1、4:男性2、5:中性、6:ロボット、7:機械1、8:機械2、10001～:SAPI5）
	buf[10] = bCode;    //文字列のbyte配列の文字コード(0:UTF-8, 1:Unicode, 2:Shift-JIS)
	*(u_long *)&buf[11] = iLength;	//文字列のbyte配列の長さ
	strncpy_s(buf + 15, sizeof(buf) - 16, bMessage, iLength);	//文字列のbyte配列
	send(sock, buf, 15 + iLength, 0);

	bouyomi_close();

	return 0;
}

/* 読み上げメッセージを選択して送信 */
void nyushitsu_sendMessage(uint64 oldChannelID, uint64 newChannelID, uint64 myChannelID, const char* nickname) {
	char msg[BUFSIZ];
	char* template;

	if (config.avoidOtherRoom == 1) {
		if (oldChannelID != myChannelID && newChannelID != myChannelID) {
			snprintf(msg, sizeof(msg), u8"自分のチャンネルに関係ないイベントです: old=%lld, new=%lld, name=%s", oldChannelID, newChannelID, nickname);
			ts3Functions.logMessage(msg, LogLevel_INFO, PLUGIN_NAME, 0);
			return;
		}
	}

    if (oldChannelID == 0) {						// 接続
        if (newChannelID == myChannelID) {
            template = u8"%s が入室しました";		// TS3に接続して現在のチャンネルに入室            
        }
        else {
			template = u8"%s が接続しました";		// TS3に接続して別のチャンネルに入室
        }
    }
    else if (oldChannelID == myChannelID) {			// 退室
		if (newChannelID == 0) {
            template = u8"%s が切断しました";		// 現在のチャンネルから切断
        }
        else {
            template = u8"%s が移動しました";		// 現在のチャンネルから別のチャンネルに移動
        }
    }
    else {											// 入室、他チャンネル間移動、他チャンネルからの接続段
        if (newChannelID == myChannelID) {
            template = u8"%s が入室しました";		// 別のチャンネルから現在のチャンネルに入室
        }
        else if (newChannelID == 0) {
            template = u8"%s が切断しました";		// 別のチャンネルから切断
        }
        else {
            return;								// 他チャンネル間の移動は読み上げない
        }        
    }

	snprintf(msg, sizeof(msg), template, nickname);
	ts3Functions.logMessage(msg, LogLevel_INFO, PLUGIN_NAME, 0);

	bouyomi_sendMessage(msg);
}
