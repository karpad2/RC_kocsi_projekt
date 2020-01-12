//
// Created by KArpi on 2019. 10. 24..
//

#ifndef RC_KOCSI_FILESYSTEM_H
#define RC_KOCSI_FILESYSTEM_H

#define FILESYSTEM SPIFFS
#define FORMAT_FILESYSTEM false

#include "String.h"
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif
File fsUploadFile;





#endif //RC_KOCSI_FILESYSTEM_H
