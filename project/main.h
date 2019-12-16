#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// #define DEBUG

#define println(fmt, args) ({\
	printf(fmt,args);\
	printf("\n");\
})

#ifdef DEBUG
#define printTEST(fmt, args) ({\
		println(fmt,args);\
})
#else
#define printTEST(fmt, args)
#endif

enum SettingTypes
{
	SETTING_TEST,
	SETTING_EXPORTING,
	SETTING_LOAD_FILE,
	SETTING_MAKE_FILE,
	SETTING_PRINT_AVAIBLE_PIECES,
	SETTING_PRINT_AVAIBLE_MOVE,
	SETTING_AUTO_PLAY_1_MOMENT,
	SETTING_REMAIN_TIME,
	SETTING_MAX,
};

extern bool g_TESTMODE;
extern bool g_PRINTPIECES;
extern bool g_PRINTMOVES;
extern bool g_AUTOPLAY;
char* g_exportingFile;

enum SettingTypes GetSettingType(const char* buf);

void LoadSettings(const char* fileName);

#endif