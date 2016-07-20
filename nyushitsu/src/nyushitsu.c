/*
 * Nyushitsu Plugin
 *
 * Copyright (c) 2016 Chirimen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nyushitsu.h"
#include "storage.h"
#include "bouyomi.h"


/* 読み上げメッセージを選択して送信 */
void nyushitsu_sendMessage(UINT64 oldChannelID, UINT64 newChannelID, UINT64 myChannelID, const char* nickname)
{
	char nickname_filtered[MAX_NICKNAME];
	char* template;

	if (config.avoidOtherRoom == 1) {
		if (oldChannelID != myChannelID && newChannelID != myChannelID) {
			logMessage(u8"自分のチャンネルに関係ないイベントです: old=%lld, new=%lld, name=%s", oldChannelID, newChannelID, nickname);
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
    else {											// 入室、他チャンネル間移動、他チャンネルからの接続断
        if (newChannelID == myChannelID) {
            template = u8"%s が入室しました";		// 別のチャンネルから現在のチャンネルに入室
        }
        else if (newChannelID == 0) {
            template = u8"%s が切断しました";		// 別のチャンネルから切断
        }
        else {
            return;									// 他チャンネル間の移動は読み上げない
        }        
    }

	if (config.filterStrip) {
		filter_strip(nickname, nickname_filtered, sizeof(nickname_filtered));
		logMessage("replace nickname: %s -> %s", nickname, nickname_filtered);
		nickname = nickname_filtered;
	}

	char msg[MAX_BOUYOMIMSG];

	snprintf(msg, sizeof(msg), template, nickname);
	bouyomi_sendMessage(msg);
}

void nyushitsu_sendChatMessage(const char *fromName, const char *message)
{
	char fromName_filtered[MAX_NICKNAME];
		
	if (config.filterStrip) {
		filter_strip(fromName, fromName_filtered, sizeof(fromName_filtered));
		logMessage("replace nickname: %s -> %s", fromName, fromName_filtered);
		fromName = fromName_filtered;
	}

	char msg[MAX_BOUYOMIMSG];

	snprintf(msg, sizeof(msg), "%s %s", fromName, message);
	bouyomi_sendMessage(msg);
}

/* 設定内容保存ファイルの読み込み */
void nyushitsu_readConfig(char *configPath)
{
	config_init(configPath);
	config_read();
}

/* 設定を ini ファイルへ書き込む */
void nyushitsu_writeConfig(void)
{
	config_write();
}


/* 設定ウィンドウの初期化?  ts3plugin_configure() から呼び出されるが、たぶん呼ばれない */
void nyushitsu_configDialog(void* qParentWidget)
{
	adapter_configure(qParentWidget);
}

/* 設定ウィンドウの表示 */
void nyushitsu_showDialog(void)
{
	adapter_show();
}
