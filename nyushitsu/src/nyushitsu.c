/*
 * Nyushitsu Plugin
 *
 * Copyright (c) 2016 Chirimen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "nyushitsu.h"
#include "utils.h"
#include "storage.h"

static SOCKET sock;


/* 棒読みちゃんへ接続する */
int bouyomi_connect(void) {
	// 棒読みちゃんのTCPサーバへ接続
	char* sHost = "127.0.0.1";	// 棒読みちゃんが動いているホスト
	int iPort = 50001;			// 棒読みちゃんのTCPサーバのポート番号(デフォルト値)

	WSADATA wsaData;
	int err;
	char *msg;

	err = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (err != 0) {
		switch (err) {
		case WSASYSNOTREADY:
			msg = "WSASYSNOTREADY";
			break;
		case WSAVERNOTSUPPORTED:
			msg = "WSAVERNOTSUPPORTED";
			break;
		case WSAEINPROGRESS:
			msg = "WSAEINPROGRESS";
			break;
		case WSAEPROCLIM:
			msg = "WSAEPROCLIM";
			break;
		case WSAEFAULT:
			msg = "WSAEFAULT";
			break;
		}
		logMessage("Error WSAStartup: %s", msg);
		return 1;
	}

	struct sockaddr_in server;

	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		logMessage("socket : %d\n", WSAGetLastError());
		return 1;
	}

	// 接続先指定用構造体の準備
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.S_un.S_addr = inet_addr(sHost);

	// サーバに接続
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0) {
		logMessage("connect : %d\n", WSAGetLastError());
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

	if (bouyomi_connect() != 0) {
		return 1;
	}

	iLength = (int)strlen(bMessage);
	logMessage(u8"send to BoyomiChan (%d): %s\n", iLength, bMessage);

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
void nyushitsu_sendMessage(UINT64 oldChannelID, UINT64 newChannelID, UINT64 myChannelID, const char* nickname) {
	char nickname_filtered[BUFSIZ];
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

	char msg[1024];

	if (config.filterNumber) {
		filter_number(nickname, nickname_filtered, BUFSIZ);
		logMessage("replace nickname: %s -> %s", nickname, nickname_filtered);
		snprintf(msg, sizeof(msg), template, nickname_filtered);
	}
	else {
		snprintf(msg, sizeof(msg), template, nickname);
	}
	logMessage(msg);
	
	bouyomi_sendMessage(msg);
}
