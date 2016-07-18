#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int config_init(char *configPath);
int config_read(void);
int config_write(void);

typedef struct {
	int avoidOtherRoom;
	int filterStrip;
} config_t;

extern config_t config;

#ifdef __cplusplus
}
#endif
