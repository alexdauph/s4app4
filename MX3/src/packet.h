#ifndef _PKT_H /* Guard against multiple inclusion */
#define _PKT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "UDP_app.h"
#include "system_config.h"
#include "system_definitions.h"

#define PKT_LEN 484
#define PKT_X_OFFSET 4
#define PKT_Y_OFFSET 164
#define PKT_Z_OFFSET 324

void PKT_Init(void);
void PKT_Tasks(void);
void PKT_Add(int val_x, int val_y, int val_z);

#endif /* _PKT_H */