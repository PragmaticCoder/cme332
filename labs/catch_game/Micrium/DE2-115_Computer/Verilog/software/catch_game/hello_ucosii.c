#include "globals.h"
#include "utils.h"

/* global variables */

extern volatile int* video_resolution;
extern volatile int* rgb_status;

extern int screen_x;
extern int screen_y;

extern int res_offset;
extern int col_offset;

extern int pos_x;
extern int pos_y;

extern int score;

extern unsigned KEY_val;
extern int KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag;

extern char text_disp[2] = "C\0";
extern char clear_text[2] = " \0";
extern char clear_row_text[81] =
  "                                                  "
  "                              \0";

short background_color;

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048
OS_STK task_key_press_stk[TASK_STACKSIZE];
OS_STK task_disp_vga_char_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */
#define TASK_KEY_PRESS_PRIORITY 1
#define TASK_VGA_CHAR_PRIORITY 2

/* Prints "Hello World" and sleeps for three seconds */
void
Task_read_KEYs(void* pdata)
{
  debug("Started: Task_read_KEYs");
  KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag = 0, 0, 0, 0;

  for (;;) {
    OSSemPend(SEM_read_KEYs, 0, &err);

    debug("%u: \tHello from Task_read_KEYs", OSTime);
    Check_KEYs(&KEY0_flag, &KEY1_flag, &KEY2_flag, &KEY3_flag);

    if (KEY0_flag) {
      debug("MOVE RIGHT");
      ++pos_x;
      KEY0_flag = 0;
    }

    if (KEY1_flag) {
      debug("MOVE DOWN");
      ++pos_y;
      KEY1_flag = 0;
    }

    if (KEY2_flag) {
      debug("MOVE UP");
      --pos_y;
      KEY2_flag = 0;
    }

    if (KEY3_flag) {
      debug("MOVE LEFT");
      --pos_x;
      KEY3_flag = 0;
    }

    OSSemPost(SEM_read_KEYs);
    OSTimeDly(1);
  }
}

/* Initial Display Setup */
static void
Task_VGA_init(void)
{
  debug("Initializing VGA Display");
  debug("Clearing the screen initially");

  VGA_box(0, 0, STANDARD_X, STANDARD_Y, 0); /* clear the screen */

  /* clearing all characters from the screen */
  for (int i = 0; i < 60; i++)
    VGA_text(0, i, clear_row_text);

  VGA_info_right(70, 0, "          ", background_color);
  VGA_info_right(70, 1, " CATCH    ", background_color);
  VGA_info_right(70, 2, " THE      ", background_color);
  VGA_info_right(70, 3, " NUMBERS! ", background_color);
  VGA_info_right(70, 4, "          ", background_color);
  VGA_info_right(70, 5, " SCORE:   ", background_color);
  VGA_info_right(70, 6, "        0 ", background_color);
  VGA_info_right(70, 7, "          ", background_color);
  VGA_info_right(70, 8, " TIME:    ", background_color);
  VGA_info_right(70, 9, " 00:00:00 ", background_color);
  VGA_info_right(70, 10, "          ", background_color);
  VGA_info_right(70, 11, "          ", background_color);
  VGA_info_right(70, 12, "          ", background_color);
  VGA_info_right(70, 13, "          ", background_color);
  VGA_info_right(70, 14, "          ", background_color);
  VGA_info_right(70, 15, "          ", background_color);
  VGA_info_right(70, 16, "          ", background_color);
  VGA_info_right(70, 17, "          ", background_color);
  VGA_info_right(70, 18, "          ", background_color);
  VGA_info_right(70, 19, "          ", background_color);
  VGA_info_right(70, 20, "          ", background_color);
  VGA_info_right(70, 21, "          ", background_color);
  VGA_info_right(70, 22, "          ", background_color);
  VGA_info_right(70, 23, "          ", background_color);
  VGA_info_right(70, 24, "          ", background_color);
  VGA_info_right(70, 25, "          ", background_color);
  VGA_info_right(70, 26, "          ", background_color);
  VGA_info_right(70, 27, "          ", background_color);
  VGA_info_right(70, 28, "          ", background_color);
  VGA_info_right(70, 29, "          ", background_color);
  VGA_info_right(70, 30, "          ", background_color);
  VGA_info_right(70, 31, "          ", background_color);
  VGA_info_right(70, 32, "          ", background_color);
  VGA_info_right(70, 33, "          ", background_color);
  VGA_info_right(70, 34, "          ", background_color);
  VGA_info_right(70, 35, "          ", background_color);
  VGA_info_right(70, 36, "          ", background_color);
  VGA_info_right(70, 37, "          ", background_color);
  VGA_info_right(70, 38, "          ", background_color);
  VGA_info_right(70, 39, "          ", background_color);
  VGA_info_right(70, 40, "          ", background_color);
  VGA_info_right(70, 41, "          ", background_color);
  VGA_info_right(70, 42, "          ", background_color);
  VGA_info_right(70, 43, "          ", background_color);
  VGA_info_right(70, 44, "          ", background_color);
  VGA_info_right(70, 45, "          ", background_color);
  VGA_info_right(70, 46, "          ", background_color);
  VGA_info_right(70, 47, "          ", background_color);
  VGA_info_right(70, 48, "          ", background_color);
  VGA_info_right(70, 49, "          ", background_color);
  VGA_info_right(70, 50, "          ", background_color);
  VGA_info_right(70, 51, "          ", background_color);
  VGA_info_right(70, 52, "          ", background_color);
  VGA_info_right(70, 53, "          ", background_color);
  VGA_info_right(70, 54, "          ", background_color);
  VGA_info_right(70, 55, "          ", background_color);
  VGA_info_right(70, 56, " PRESS    ", background_color);
  VGA_info_right(70, 57, "  ECE     ", background_color);
  VGA_info_right(70, 58, " TO QUIT  ", background_color);
  VGA_info_right(70, 59, "          ", background_color);
}

/* Display Character using VGA Output */
void
Task_VGA_char(void* pdata)
{
  debug("Started: Task_VGA_char");

  for (;;) {
    debug(
      "%u: \tTask_VGA_char: (pos_x, pos_y) = (%d, %d)", OSTime, pos_x, pos_y);

    VGA_animated_char(pos_x, pos_y, text_disp, background_color);

    OSTimeDly(1);
  }
}

/* The main function creates two task and starts multi-tasking */
int
main(void)
{

  /* ***************************** Initialization *****************************
   */

  score = 0;

  KEY0_flag, KEY1_flag, KEY2_flag, KEY3_flag = 0, 0, 0, 0;

  /* ************************ Semaphores Initialization ***********************
   */

  SEM_read_KEYs = OSSemCreate(1);

  /* **************************** VGA Display Setup ***************************
   */

  video_resolution = (int*)(PIXEL_BUF_CTRL_BASE + 0x8);
  rgb_status = (int*)(RGB_RESAMPLER_BASE);

  screen_x = *video_resolution & 0xFFFF;
  screen_y = (*video_resolution >> 16) & 0xFFFF;

  /* letter initially positioned at the centre of screen */
  pos_x = 40;
  pos_y = 30;

  int db = get_data_bits(*rgb_status & 0x3F);

  /* check if resolution is smaller than the standard 320 x 240 */
  res_offset = (screen_x == 160) ? 1 : 0;

  /* check if number of data bits is less than the standard 16-bits */
  col_offset = (db == 8) ? 1 : 0;

  /* update color */
  background_color = resample_rgb(db, INTEL_RED);
  VGA_animated_char(pos_x, pos_y, text_disp, background_color);

  /* **************************************************************************
   */

  Task_VGA_init(); /* Initial Display Layout Setup */

  OSTaskCreateExt(Task_read_KEYs,
                  NULL,
                  (void*)&task_key_press_stk[TASK_STACKSIZE - 1],
                  TASK_KEY_PRESS_PRIORITY,
                  TASK_KEY_PRESS_PRIORITY,
                  task_key_press_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(Task_VGA_char,
                  NULL,
                  (void*)&task_disp_vga_char_stk[TASK_STACKSIZE - 1],
                  TASK_VGA_CHAR_PRIORITY,
                  TASK_VGA_CHAR_PRIORITY,
                  task_disp_vga_char_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSStart();
  return 0;
}