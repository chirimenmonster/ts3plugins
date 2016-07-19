#include <iostream>
#include <regex>
#include <string>

#include <Windows.h>

#include "nyushitsu.h"
#include "filters.h"


static int regex_replace(wchar_t *src, const wchar_t *regex, const wchar_t *fmt, wchar_t *dst, size_t size)
{
	std::wregex re(regex);
	std::wstring result = std::regex_replace(src, re, fmt);
	wcscpy_s(dst, size, result.c_str());
	return 0;
}

int filter_strip(const char *string, char *result, size_t size)
{
	int len;
	wchar_t wcsbuf[MAX_NICKNAME];

	len = MultiByteToWideChar(CP_UTF8, 0, string, -1, wcsbuf, MAX_NICKNAME);
	if (len == 0) {
		logMessage(u8"ワイド文字に変換できませんでした: '%s'", string);
		return -1;
	}

	// 0: ハイフンとアンダースコアを空白に変換
	regex_replace(wcsbuf, L"[-_－＿]", L" ", wcsbuf, MAX_NICKNAME);

	// 1: 先頭の記号と名前文字列の後に続く記号以下を削除
	regex_replace(wcsbuf, L"[^[:alpha:]]*([[:alnum:][:blank:]]*).*", L"$1", wcsbuf, MAX_NICKNAME);

	// 2: 数字を空白に変換
	regex_replace(wcsbuf, L"[[:digit:]][[:digit:]]*", L" ", wcsbuf, MAX_NICKNAME);


	len = WideCharToMultiByte(CP_UTF8, 0, wcsbuf, -1, result, size, NULL, NULL);
	if (len == 0) {
		logMessage(u8"フィルタ後の文字列をUTF-8に変換できませんでした: 元の文字列 '%s'", string);
		return -1;
	}

	return 0;
}
