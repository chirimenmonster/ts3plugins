#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int filter_strip(const char *string, char *result, size_t size);
int filter_textchat(const char *string, char *result, size_t size);

#ifdef __cplusplus
}
#endif
