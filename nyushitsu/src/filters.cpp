#include <iostream>
#include <regex>
#include <string>

#include <Windows.h>

#include "nyushitsu.h"
#include "filters.h"


static int mbstowcs(const char *mbs, wchar_t *wcs, size_t wsize)
{
	int len;

	len = MultiByteToWideChar(CP_UTF8, 0, mbs, -1, wcs, wsize);
	if (len == 0) {
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			return 0;
		}
		return -1;
	}
	return len;
}

static int wcstombs(const wchar_t *wcs, char *mbs, size_t msize)
{
	int len;

	len = WideCharToMultiByte(CP_UTF8, 0, wcs, -1, mbs, msize, NULL, NULL);
	if (len == 0) {
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			return 0;
		}
		return -1;
	}
	return len;
}


static int regex_replace(wchar_t *src, const wchar_t *regex, const wchar_t *fmt, wchar_t *dst, size_t size)
{
	std::wregex re(regex);
	std::wstring result = std::regex_replace(src, re, fmt);
	int r = wcscpy_s(dst, size, result.c_str());
	if (r == ERANGE) {
		logMessage(u8"文字列が長すぎです: len=%d", result.length());
	}
	return 0;
}

// ニックネーム用のフィルタ
int filter_strip(const char *string, char *result, size_t size)
{
	int len, r;
	wchar_t wcsbuf[MAX_NICKNAME];

	result[0] = '\0';

	r = mbstowcs(string, wcsbuf, MAX_NICKNAME);
	if (r == 0) {
		logMessage(u8"文字列が長過ぎます: '%s'", string);
		strncpy(result, u8"誰か", size);
		return 0;
	}
	if (r < 0) {
		logMessage(u8"UTF-8文字列をUTF-16に変換できませんでした: 元の文字列 '%s'", string);
		strncpy(result, u8"誰か", size);
		return -1;
	}

	// 0: ハイフンとアンダースコアを空白に変換
	regex_replace(wcsbuf, L"[-_－＿]", L" ", wcsbuf, MAX_NICKNAME);

	// 1: 先頭の記号と名前文字列の後に続く記号以下を削除
	regex_replace(wcsbuf, L"[^[:alpha:]]*([[:alnum:][:blank:]]*).*", L"$1", wcsbuf, MAX_NICKNAME);

	// 2: 数字を空白に変換
	regex_replace(wcsbuf, L"[[:digit:]][[:digit:]]*", L" ", wcsbuf, MAX_NICKNAME);


	r = wcstombs(wcsbuf, result, size);
	if (r == 0) {
		logMessage(u8"フィルタ後の文字列をUTF-8に変換できませんでした。文字列が長過ぎます: 元の文字列 '%s'", string);
		strncpy(result, u8"誰か", size);
		return 0;
	}
	if (r < 0) {
		logMessage(u8"フィルタ後の文字列をUTF-8に変換できませんでした: 元の文字列 '%s'", string);
		strncpy(result, u8"誰か", size);
		return -1;
	}

	return 0;
}

// テキストチャット用のフィルタ
int filter_textchat(const char *string, char *result, size_t size)
{
	int r;
	wchar_t wcsbuf[BUFSIZ];

	result[0] = '\0';

	r = mbstowcs(string, wcsbuf, BUFSIZ);
	if (r == 0) {
		logMessage(u8"文字列が長過ぎます: '%s'", string);
		strncpy(result, u8"長文省略", size);
		return 0;
	}
	if (r < 0) {
		logMessage(u8"UTF-8文字列をUTF-16に変換できませんでした: 元の文字列 '%s'", string);
		strncpy(result, u8"変換失敗", size);
		return -1;
	}

	regex_replace(wcsbuf, L"\\[URL\\].*\\[/URL\\]", L" URL ", wcsbuf, BUFSIZ);

	wcsbuf[MAX_TEXTCHAT + 1] = L'\0';

	r = wcstombs(wcsbuf, result, size);
	if (r == 0) {
		logMessage(u8"フィルタ後の文字列をUTF-8に変換できませんでした。文字列が長過ぎます: 元の文字列 '%s'", string);
		strncpy(result, u8"長文省略", size);
		return 0;
	}
	if (r < 0) {
		logMessage(u8"フィルタ後の文字列をUTF-8に変換できませんでした: 元の文字列 '%s'", string);
		strncpy(result, u8"変換失敗", size);
		return -1;
	}

	logMessage("filter_textchat: %s", result);

	return 0;
}
