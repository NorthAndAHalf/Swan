#pragma once

#ifdef SF_DEBUG
#include <cassert>
#define SF_ASSERT(x, msg) if (!(x)) { assert(false && msg); }
#else
#define SF_ASSERT(x, msg)
#endif

// Key Code Macros - Values from GLFW
#define SF_KEY_UNKNOWN            -1

/* Printable keys */
#define SF_KEY_SPACE              32
#define SF_KEY_APOSTROPHE         39  /* ' */
#define SF_KEY_COMMA              44  /* , */
#define SF_KEY_MINUS              45  /* - */
#define SF_KEY_PERIOD             46  /* . */
#define SF_KEY_SLASH              47  /* / */
#define SF_KEY_0                  48
#define SF_KEY_1                  49
#define SF_KEY_2                  50
#define SF_KEY_3                  51
#define SF_KEY_4                  52
#define SF_KEY_5                  53
#define SF_KEY_6                  54
#define SF_KEY_7                  55
#define SF_KEY_8                  56
#define SF_KEY_9                  57
#define SF_KEY_SEMICOLON          59  /* ; */
#define SF_KEY_EQUAL              61  /* = */
#define SF_KEY_A                  65
#define SF_KEY_B                  66
#define SF_KEY_C                  67
#define SF_KEY_D                  68
#define SF_KEY_E                  69
#define SF_KEY_F                  70
#define SF_KEY_G                  71
#define SF_KEY_H                  72
#define SF_KEY_I                  73
#define SF_KEY_J                  74
#define SF_KEY_K                  75
#define SF_KEY_L                  76
#define SF_KEY_M                  77
#define SF_KEY_N                  78
#define SF_KEY_O                  79
#define SF_KEY_P                  80
#define SF_KEY_Q                  81
#define SF_KEY_R                  82
#define SF_KEY_S                  83
#define SF_KEY_T                  84
#define SF_KEY_U                  85
#define SF_KEY_V                  86
#define SF_KEY_W                  87
#define SF_KEY_X                  88
#define SF_KEY_Y                  89
#define SF_KEY_Z                  90
#define SF_KEY_LEFT_BRACKET       91  /* [ */
#define SF_KEY_BACKSLASH          92  /* \ */
#define SF_KEY_RIGHT_BRACKET      93  /* ] */
#define SF_KEY_GRAVE_ACCENT       96  /* ` */
#define SF_KEY_WORLD_1            161 /* non-US #1 */
#define SF_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define SF_KEY_ESCAPE             256
#define SF_KEY_ENTER              257
#define SF_KEY_TAB                258
#define SF_KEY_BACKSPACE          259
#define SF_KEY_INSERT             260
#define SF_KEY_DELETE             261
#define SF_KEY_RIGHT              262
#define SF_KEY_LEFT               263
#define SF_KEY_DOWN               264
#define SF_KEY_UP                 265
#define SF_KEY_PAGE_UP            266
#define SF_KEY_PAGE_DOWN          267
#define SF_KEY_HOME               268
#define SF_KEY_END                269
#define SF_KEY_CAPS_LOCK          280
#define SF_KEY_SCROLL_LOCK        281
#define SF_KEY_NUM_LOCK           282
#define SF_KEY_PRINT_SCREEN       283
#define SF_KEY_PAUSE              284
#define SF_KEY_F1                 290
#define SF_KEY_F2                 291
#define SF_KEY_F3                 292
#define SF_KEY_F4                 293
#define SF_KEY_F5                 294
#define SF_KEY_F6                 295
#define SF_KEY_F7                 296
#define SF_KEY_F8                 297
#define SF_KEY_F9                 298
#define SF_KEY_F10                299
#define SF_KEY_F11                300
#define SF_KEY_F12                301
#define SF_KEY_F13                302
#define SF_KEY_F14                303
#define SF_KEY_F15                304
#define SF_KEY_F16                305
#define SF_KEY_F17                306
#define SF_KEY_F18                307
#define SF_KEY_F19                308
#define SF_KEY_F20                309
#define SF_KEY_F21                310
#define SF_KEY_F22                311
#define SF_KEY_F23                312
#define SF_KEY_F24                313
#define SF_KEY_F25                314
#define SF_KEY_KP_0               320
#define SF_KEY_KP_1               321
#define SF_KEY_KP_2               322
#define SF_KEY_KP_3               323
#define SF_KEY_KP_4               324
#define SF_KEY_KP_5               325
#define SF_KEY_KP_6               326
#define SF_KEY_KP_7               327
#define SF_KEY_KP_8               328
#define SF_KEY_KP_9               329
#define SF_KEY_KP_DECIMAL         330
#define SF_KEY_KP_DIVIDE          331
#define SF_KEY_KP_MULTIPLY        332
#define SF_KEY_KP_SUBTRACT        333
#define SF_KEY_KP_ADD             334
#define SF_KEY_KP_ENTER           335
#define SF_KEY_KP_EQUAL           336
#define SF_KEY_LEFT_SHIFT         340
#define SF_KEY_LEFT_CONTROL       341
#define SF_KEY_LEFT_ALT           342
#define SF_KEY_LEFT_SUPER         343
#define SF_KEY_RIGHT_SHIFT        344
#define SF_KEY_RIGHT_CONTROL      345
#define SF_KEY_RIGHT_ALT          346
#define SF_KEY_RIGHT_SUPER        347
#define SF_KEY_MENU               348

#define SF_KEY_LAST               SF_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

 /*! @brief If this bit is set one or more Shift keys were held down.
  *
  *  If this bit is set one or more Shift keys were held down.
  */
#define SF_MOD_SHIFT           0x0001
  /*! @brief If this bit is set one or more Control keys were held down.
   *
   *  If this bit is set one or more Control keys were held down.
   */
#define SF_MOD_CONTROL         0x0002
   /*! @brief If this bit is set one or more Alt keys were held down.
	*
	*  If this bit is set one or more Alt keys were held down.
	*/
#define SF_MOD_ALT             0x0004
	/*! @brief If this bit is set one or more Super keys were held down.
	 *
	 *  If this bit is set one or more Super keys were held down.
	 */
#define SF_MOD_SUPER           0x0008
	 /*! @brief If this bit is set the Caps Lock key is enabled.
	  *
	  *  If this bit is set the Caps Lock key is enabled and the @ref
	  *  SF_LOCK_KEY_MODS input mode is set.
	  */
#define SF_MOD_CAPS_LOCK       0x0010
	  /*! @brief If this bit is set the Num Lock key is enabled.
	   *
	   *  If this bit is set the Num Lock key is enabled and the @ref
	   *  SF_LOCK_KEY_MODS input mode is set.
	   */
#define SF_MOD_NUM_LOCK        0x0020

	   /*! @} */

	   /*! @defgroup buttons Mouse buttons
		*  @brief Mouse button IDs.
		*
		*  See [mouse button input](@ref input_mouse_button) for how these are used.
		*
		*  @ingroup input
		*  @{ */
#define SF_MOUSE_BUTTON_1         0
#define SF_MOUSE_BUTTON_2         1
#define SF_MOUSE_BUTTON_3         2
#define SF_MOUSE_BUTTON_4         3
#define SF_MOUSE_BUTTON_5         4
#define SF_MOUSE_BUTTON_6         5
#define SF_MOUSE_BUTTON_7         6
#define SF_MOUSE_BUTTON_8         7
#define SF_MOUSE_BUTTON_LAST      SF_MOUSE_BUTTON_8
#define SF_MOUSE_BUTTON_LEFT      SF_MOUSE_BUTTON_1
#define SF_MOUSE_BUTTON_RIGHT     SF_MOUSE_BUTTON_2
#define SF_MOUSE_BUTTON_MIDDLE    SF_MOUSE_BUTTON_3
		/*! @} */

		/*! @defgroup joysticks Joysticks
		 *  @brief Joystick IDs.
		 *
		 *  See [joystick input](@ref joystick) for how these are used.
		 *
		 *  @ingroup input
		 *  @{ */
#define SF_JOYSTICK_1             0
#define SF_JOYSTICK_2             1
#define SF_JOYSTICK_3             2
#define SF_JOYSTICK_4             3
#define SF_JOYSTICK_5             4
#define SF_JOYSTICK_6             5
#define SF_JOYSTICK_7             6
#define SF_JOYSTICK_8             7
#define SF_JOYSTICK_9             8
#define SF_JOYSTICK_10            9
#define SF_JOYSTICK_11            10
#define SF_JOYSTICK_12            11
#define SF_JOYSTICK_13            12
#define SF_JOYSTICK_14            13
#define SF_JOYSTICK_15            14
#define SF_JOYSTICK_16            15
#define SF_JOYSTICK_LAST          SF_JOYSTICK_16
		 /*! @} */

		 /*! @defgroup gamepad_buttons Gamepad buttons
		  *  @brief Gamepad buttons.
		  *
		  *  See @ref gamepad for how these are used.
		  *
		  *  @ingroup input
		  *  @{ */
#define SF_GAMEPAD_BUTTON_A               0
#define SF_GAMEPAD_BUTTON_B               1
#define SF_GAMEPAD_BUTTON_X               2
#define SF_GAMEPAD_BUTTON_Y               3
#define SF_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define SF_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define SF_GAMEPAD_BUTTON_BACK            6
#define SF_GAMEPAD_BUTTON_START           7
#define SF_GAMEPAD_BUTTON_GUIDE           8
#define SF_GAMEPAD_BUTTON_LEFT_THUMB      9
#define SF_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define SF_GAMEPAD_BUTTON_DPAD_UP         11
#define SF_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define SF_GAMEPAD_BUTTON_DPAD_DOWN       13
#define SF_GAMEPAD_BUTTON_DPAD_LEFT       14
#define SF_GAMEPAD_BUTTON_LAST            SF_GAMEPAD_BUTTON_DPAD_LEFT

#define SF_GAMEPAD_BUTTON_CROSS       SF_GAMEPAD_BUTTON_A
#define SF_GAMEPAD_BUTTON_CIRCLE      SF_GAMEPAD_BUTTON_B
#define SF_GAMEPAD_BUTTON_SQUARE      SF_GAMEPAD_BUTTON_X
#define SF_GAMEPAD_BUTTON_TRIANGLE    SF_GAMEPAD_BUTTON_Y
		  /*! @} */

		  /*! @defgroup gamepad_axes Gamepad axes
		   *  @brief Gamepad axes.
		   *
		   *  See @ref gamepad for how these are used.
		   *
		   *  @ingroup input
		   *  @{ */
#define SF_GAMEPAD_AXIS_LEFT_X        0
#define SF_GAMEPAD_AXIS_LEFT_Y        1
#define SF_GAMEPAD_AXIS_RIGHT_X       2
#define SF_GAMEPAD_AXIS_RIGHT_Y       3
#define SF_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define SF_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define SF_GAMEPAD_AXIS_LAST          SF_GAMEPAD_AXIS_RIGHT_TRIGGER
