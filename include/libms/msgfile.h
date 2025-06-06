#ifndef LIBMS_MSGFILE_H
#define LIBMS_MSGFILE_H

#include "common.h"
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

struct MsbtInfo;

struct MsbtAttrInfo {
    char c_0x00;
    char c_0x01;
};

struct MsbtInfo *LMS_InitMessage(void *data);
void LMS_CloseMessage(struct MsbtInfo *info);
int LMS_GetTextIndexByLabel(struct MsbtInfo *info, const char *label);
const wchar_t *LMS_GetText(struct MsbtInfo *info, int index);
const wchar_t *LMS_GetTextByLabel(struct MsbtInfo *info, const char *label);
int LMS_GetLabelByTextIndex(struct MsbtInfo *info, int index, char *outLabel);
struct MsbtAttrInfo *LMS_GetAttribute(struct MsbtInfo *info, int index);

#ifdef __cplusplus
}
#endif

#endif
