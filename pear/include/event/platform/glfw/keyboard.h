#ifdef PEAR_PLATFORM_GLFW

#ifndef PEAR_EVENT_GLFW_KEYBOARD_H_
#define PEAR_EVENT_GLFW_KEYBOARD_H_

// stolen from glfw3.h

#define PEAR_PLATFORM_KEY_SPACE              32
#define PEAR_PLATFORM_KEY_APOSTROPHE         39  /* ' */
#define PEAR_PLATFORM_KEY_COMMA              44  /* , */
#define PEAR_PLATFORM_KEY_MINUS              45  /* - */
#define PEAR_PLATFORM_KEY_PERIOD             46  /* . */
#define PEAR_PLATFORM_KEY_SLASH              47  /* / */
#define PEAR_PLATFORM_KEY_0                  48
#define PEAR_PLATFORM_KEY_1                  49
#define PEAR_PLATFORM_KEY_2                  50
#define PEAR_PLATFORM_KEY_3                  51
#define PEAR_PLATFORM_KEY_4                  52
#define PEAR_PLATFORM_KEY_5                  53
#define PEAR_PLATFORM_KEY_6                  54
#define PEAR_PLATFORM_KEY_7                  55
#define PEAR_PLATFORM_KEY_8                  56
#define PEAR_PLATFORM_KEY_9                  57
#define PEAR_PLATFORM_KEY_SEMICOLON          59  /* ; */
#define PEAR_PLATFORM_KEY_EQUAL              61  /* = */
#define PEAR_PLATFORM_KEY_A                  65
#define PEAR_PLATFORM_KEY_B                  66
#define PEAR_PLATFORM_KEY_C                  67
#define PEAR_PLATFORM_KEY_D                  68
#define PEAR_PLATFORM_KEY_E                  69
#define PEAR_PLATFORM_KEY_F                  70
#define PEAR_PLATFORM_KEY_G                  71
#define PEAR_PLATFORM_KEY_H                  72
#define PEAR_PLATFORM_KEY_I                  73
#define PEAR_PLATFORM_KEY_J                  74
#define PEAR_PLATFORM_KEY_K                  75
#define PEAR_PLATFORM_KEY_L                  76
#define PEAR_PLATFORM_KEY_M                  77
#define PEAR_PLATFORM_KEY_N                  78
#define PEAR_PLATFORM_KEY_O                  79
#define PEAR_PLATFORM_KEY_P                  80
#define PEAR_PLATFORM_KEY_Q                  81
#define PEAR_PLATFORM_KEY_R                  82
#define PEAR_PLATFORM_KEY_S                  83
#define PEAR_PLATFORM_KEY_T                  84
#define PEAR_PLATFORM_KEY_U                  85
#define PEAR_PLATFORM_KEY_V                  86
#define PEAR_PLATFORM_KEY_W                  87
#define PEAR_PLATFORM_KEY_X                  88
#define PEAR_PLATFORM_KEY_Y                  89
#define PEAR_PLATFORM_KEY_Z                  90
#define PEAR_PLATFORM_KEY_LEFT_BRACKET       91  /* [ */
#define PEAR_PLATFORM_KEY_BACKSLASH          92  /* \ */
#define PEAR_PLATFORM_KEY_RIGHT_BRACKET      93  /* ] */
#define PEAR_PLATFORM_KEY_GRAVE_ACCENT       96  /* ` */
#define PEAR_PLATFORM_KEY_WORLD_1            161 /* non-US #1 */
#define PEAR_PLATFORM_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define PEAR_PLATFORM_KEY_ESCAPE             256
#define PEAR_PLATFORM_KEY_ENTER              257
#define PEAR_PLATFORM_KEY_TAB                258
#define PEAR_PLATFORM_KEY_BACKSPACE          259
#define PEAR_PLATFORM_KEY_INSERT             260
#define PEAR_PLATFORM_KEY_DELETE             261
#define PEAR_PLATFORM_KEY_RIGHT              262
#define PEAR_PLATFORM_KEY_LEFT               263
#define PEAR_PLATFORM_KEY_DOWN               264
#define PEAR_PLATFORM_KEY_UP                 265
#define PEAR_PLATFORM_KEY_PAGE_UP            266
#define PEAR_PLATFORM_KEY_PAGE_DOWN          267
#define PEAR_PLATFORM_KEY_HOME               268
#define PEAR_PLATFORM_KEY_END                269
#define PEAR_PLATFORM_KEY_CAPS_LOCK          280
#define PEAR_PLATFORM_KEY_SCROLL_LOCK        281
#define PEAR_PLATFORM_KEY_NUM_LOCK           282
#define PEAR_PLATFORM_KEY_PRINT_SCREEN       283
#define PEAR_PLATFORM_KEY_PAUSE              284
#define PEAR_PLATFORM_KEY_F1                 290
#define PEAR_PLATFORM_KEY_F2                 291
#define PEAR_PLATFORM_KEY_F3                 292
#define PEAR_PLATFORM_KEY_F4                 293
#define PEAR_PLATFORM_KEY_F5                 294
#define PEAR_PLATFORM_KEY_F6                 295
#define PEAR_PLATFORM_KEY_F7                 296
#define PEAR_PLATFORM_KEY_F8                 297
#define PEAR_PLATFORM_KEY_F9                 298
#define PEAR_PLATFORM_KEY_F10                299
#define PEAR_PLATFORM_KEY_F11                300
#define PEAR_PLATFORM_KEY_F12                301
#define PEAR_PLATFORM_KEY_F13                302
#define PEAR_PLATFORM_KEY_F14                303
#define PEAR_PLATFORM_KEY_F15                304
#define PEAR_PLATFORM_KEY_F16                305
#define PEAR_PLATFORM_KEY_F17                306
#define PEAR_PLATFORM_KEY_F18                307
#define PEAR_PLATFORM_KEY_F19                308
#define PEAR_PLATFORM_KEY_F20                309
#define PEAR_PLATFORM_KEY_F21                310
#define PEAR_PLATFORM_KEY_F22                311
#define PEAR_PLATFORM_KEY_F23                312
#define PEAR_PLATFORM_KEY_F24                313
#define PEAR_PLATFORM_KEY_F25                314
#define PEAR_PLATFORM_KEY_KP_0               320
#define PEAR_PLATFORM_KEY_KP_1               321
#define PEAR_PLATFORM_KEY_KP_2               322
#define PEAR_PLATFORM_KEY_KP_3               323
#define PEAR_PLATFORM_KEY_KP_4               324
#define PEAR_PLATFORM_KEY_KP_5               325
#define PEAR_PLATFORM_KEY_KP_6               326
#define PEAR_PLATFORM_KEY_KP_7               327
#define PEAR_PLATFORM_KEY_KP_8               328
#define PEAR_PLATFORM_KEY_KP_9               329
#define PEAR_PLATFORM_KEY_KP_DECIMAL         330
#define PEAR_PLATFORM_KEY_KP_DIVIDE          331
#define PEAR_PLATFORM_KEY_KP_MULTIPLY        332
#define PEAR_PLATFORM_KEY_KP_SUBTRACT        333
#define PEAR_PLATFORM_KEY_KP_ADD             334
#define PEAR_PLATFORM_KEY_KP_ENTER           335
#define PEAR_PLATFORM_KEY_KP_EQUAL           336
#define PEAR_PLATFORM_KEY_LEFT_SHIFT         340
#define PEAR_PLATFORM_KEY_LEFT_CONTROL       341
#define PEAR_PLATFORM_KEY_LEFT_ALT           342
#define PEAR_PLATFORM_KEY_LEFT_SUPER         343
#define PEAR_PLATFORM_KEY_RIGHT_SHIFT        344
#define PEAR_PLATFORM_KEY_RIGHT_CONTROL      345
#define PEAR_PLATFORM_KEY_RIGHT_ALT          346
#define PEAR_PLATFORM_KEY_RIGHT_SUPER        347
#define PEAR_PLATFORM_KEY_MENU               348

#endif

#endif
