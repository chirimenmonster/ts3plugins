
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "nyushitsu.h"
#include "bouyomi.h"

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
	char buf[MAX_BOUYOMICMD];

	if (bouyomi_connect() != 0) {
		return 1;
	}

	iLength = strlen(bMessage);
	logMessage(u8"send to BoyomiChan (%d): %s\n", iLength, bMessage);

	memset(buf, 0, sizeof(buf));
	*(INT16 *)&buf[0] = iCommand; //コマンド（ 0:メッセージ読み上げ）
	*(INT16 *)&buf[2] = iSpeed;   //速度    （-1:棒読みちゃん画面上の設定）
	*(INT16 *)&buf[4] = iTone;    //音程    （-1:棒読みちゃん画面上の設定）
	*(INT16 *)&buf[6] = iVolume;  //音量    （-1:棒読みちゃん画面上の設定）
	*(INT16 *)&buf[8] = iVoice;   //声質    （ 0:棒読みちゃん画面上の設定、1:女性1、2:女性2、3:男性1、4:男性2、5:中性、6:ロボット、7:機械1、8:機械2、10001～:SAPI5）
	buf[10] = bCode;    //文字列のbyte配列の文字コード(0:UTF-8, 1:Unicode, 2:Shift-JIS)
	*(INT32 *)&buf[11] = iLength;	//文字列のbyte配列の長さ
	strncpy_s(buf + BOUYOMI_HEADERSIZE, sizeof(buf) - BOUYOMI_HEADERSIZE, bMessage, iLength);	//文字列のbyte配列
	send(sock, buf, BOUYOMI_HEADERSIZE + iLength, 0);

	bouyomi_close();

	return 0;
}
