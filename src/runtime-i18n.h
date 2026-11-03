#ifndef RUNTIME_I18N_H
#define RUNTIME_I18N_H
#include "w.h"
enum PlayerLanguage {PlayerSpanish,PlayerEnglish,PlayerFrench,PlayerCatalan,PLAYER_LANGUAGE_COUNT};
enum PlayerText {
  PT_FILE,
  PT_ACTIVITY,
  PT_TOOLS,
  PT_HELP_MENU,
  PT_OPEN_MENU,
  PT_ASSOC_MENU,
  PT_MINIMIZE,
  PT_EXIT_MENU,
  PT_RESTART_MENU,
  PT_ACT_HELP_MENU,
  PT_PUZZLE_MENU,
  PT_ACT_INFO_MENU,
  PT_PRINT_MENU,
  PT_APPEARANCE,
  PT_CLASSIC_SKIN,
  PT_MODERN_SKIN,
  PT_ABOUT_MENU,
  PT_LANGUAGE,
  PT_SAVE_DEFAULT,
  PT_DEFAULT_FMT,
  PT_OPEN,
  PT_EXIT,
  PT_RESTART,
  PT_HELP,
  PT_PREVIOUS,
  PT_NEXT,
  PT_PRINT,
  PT_PUZZLE,
  PT_INFO,
  PT_HINT_EXIT,
  PT_HINT_OPEN,
  PT_HINT_PRINT,
  PT_HINT_RESTART,
  PT_HINT_HELP,
  PT_HINT_PUZZLE,
  PT_HINT_INFO,
  PT_HINT_NEXT,
  PT_HINT_PREVIOUS,
  PT_READY,
  PT_SCORE,
  PT_TRIES,
  PT_TIME,
  PT_CLASSIC_SCORE,
  PT_CLASSIC_TRIES,
  PT_CLASSIC_TIME,
  PT_ACTIVITY_FMT,
  PT_SINGLE_ACTIVITY,
  PT_OPEN_TO_START,
  PT_INTERACTIVE,
  PT_SELECT_TO_START,
  PT_PICK_TITLE,
  PT_FILTER_PACKAGES,
  PT_FILTER_ALL,
  PT_ABOUT_TITLE,
  PT_ABOUT_TEXT,
  PT_ASSOC_TITLE,
  PT_ASSOC_HEADING,
  PT_ASSOC_DESC,
  PT_PLAYER,
  PT_ASSOC_STEPS,
  PT_ASSOC_REGISTER,
  PT_CANCEL,
  PT_ASSOC_ERROR_FMT,
  PT_ASSOC_DONE_TITLE,
  PT_ASSOC_DONE_TEXT,
  PT_ASSOC_WINDOW_ERROR_FMT,
  PT_SAVE_LANG_ERROR,
  PT_APP_DESCRIPTION,
  PT_PACKAGE_TYPE,
  PT_COUNT
};
extern int playerLanguage;
extern int playerDefaultLanguage;
const char *tr(PlayerText text);
const char *playerLanguageName(int language);
void initPlayerLanguage(const char *iniFile);
void setPlayerLanguage(int language);
BOOL savePlayerLanguage(const char *iniFile);
HMENU createPlayerMenu();
const char *legacyPlayerText(UINT id);
void localizePlayerDialog(HWND dialog);
#endif

