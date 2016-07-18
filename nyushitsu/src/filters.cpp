#include <iostream>
#include <regex>
#include <string>

#include <Windows.h>

#include "nyushitsu.h"
#include "filters.h"


int filter_strip(const char *string, char *result, size_t size)
{
	int len;
	wchar_t wcsbuf[MAX_NICKNAME];

	len = MultiByteToWideChar(CP_UTF8, 0, string, -1, wcsbuf, sizeof(wcsbuf)/sizeof(wcsbuf[0]));
	if (len == 0) {
		logMessage(u8"ワイド文字に変換できませんでした: '%s'", string);
		return -1;
	}

	// 0: ハイフンとアンダースコアを空白に変換
	std::wregex re0(L"[-_－＿]");
	wchar_t fmt0[] = L" ";
	std::wstring wresult0 = std::regex_replace(wcsbuf, re0, fmt0);

	// 1: 先頭の記号と名前文字列の後に続く記号以下を削除
	std::wregex re1(L"[^[:alpha:]]*([[:alnum:][:blank:]]*).*");
	wchar_t fmt1[] = L"$1";
	std::wstring wresult1 = std::regex_replace(wresult0, re1, fmt1);

	// 2: 数字を空白に変換
	std::wregex re2(L"[[:digit:]][[:digit:]]*");
	wchar_t fmt2[] = L" ";
	std::wstring wresult2 = std::regex_replace(wresult1, re2, fmt2);


	len = WideCharToMultiByte(CP_UTF8, 0, wresult2.c_str(), -1, result, size, NULL, NULL);
	if (len == 0) {
		logMessage(u8"フィルタ後の文字列をUTF-8に変換できませんでした: 元の文字列 '%s'", string);
		return -1;
	}

	return 0;
}
