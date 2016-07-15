#pragma once

int config_init(char *configPath);
int config_read(void);
int config_write(void);

typedef struct {
	int avoidOtherRoom;
	int filterNumber;
} config_t;

#ifdef __cplusplus
extern "C" {
#endif

extern config_t config;

#ifdef __cplusplus
}
#endif
