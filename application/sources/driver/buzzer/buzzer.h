#ifndef __BUZZER_H__
#define __BUZZER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "io_cfg.h"

// Single tone definition
typedef struct {
	uint16_t frequency;
	uint8_t  duration;
} Tone_TypeDef;

static const Tone_TypeDef tones_startup[] = {
	{2000,3},
	{   0,3},
	{3000,3},
	{   0,3},
	{4000,3},
	{   0,3},
	{1200,4},
	{   0,6},
	{4500,6},
	{   0,0}     // <-- tones end
};
static const Tone_TypeDef tones_snake_move[] = {
    {3500, 1}, {0, 1}, {0, 0}  // bíp rất ngắn, cao tần
};
static const Tone_TypeDef tones_snake_eat[] = {
    {800, 2}, {1200, 3}, {0, 0}  // âm tăng tần
};
static const Tone_TypeDef tones_snake_crash[] = {
    {1000, 3}, {0, 1}, {700, 4}, {0, 1}, {400, 6}, {0, 0}
};
static const Tone_TypeDef tones_menu_click[] = {
    {3000, 2}, {0, 2}, {0, 0} // bíp ngắn
};
static const Tone_TypeDef tones_snake_turn[] = {
    {2800, 1}, {0, 1}, {2200, 1}, {0, 0}
};
static const Tone_TypeDef tones_menu_select[] = {
    {1000, 2}, {0, 1}, {2000, 3}, {0, 0} // bíp đôi khác nhau (chọn OK)
};
static const Tone_TypeDef tones_snake_init[] = {
    {1200, 3},
    {0, 2},
    {1600, 3},
    {0, 2},
    {2000, 5},
    {0, 0}
};
static const Tone_TypeDef tones_3beep[] = {
	{4000, 3},
	{   0,10},
	{1000, 6},
	{   0,10},
	{4000, 3},
	{   0, 0}
};

// "Super Mario bros." =)
static const Tone_TypeDef tones_SMB[] = {
	{2637,18}, // E7 x2
	{   0, 9}, // x3
	{2637, 9}, // E7
	{   0, 9}, // x3
	{2093, 9}, // C7
	{2637, 9}, // E7
	{   0, 9}, // x3
	{3136, 9}, // G7
	{   0,27}, // x3
	{1586, 9}, // G6
	{   0,27}, // x3

	{2093, 9}, // C7
	{   0,18}, // x2
	{1586, 9}, // G6
	{   0,18}, // x2
	{1319, 9}, // E6
	{   0,18}, // x2
	{1760, 9}, // A6
	{   0, 9}, // x1
	{1976, 9}, // B6
	{   0, 9}, // x1
	{1865, 9}, // AS6
	{1760, 9}, // A6
	{   0, 9}, // x1

	{1586,12}, // G6
	{2637,12}, // E7
	{3136,12}, // G7
	{3520, 9}, // A7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{3136, 9}, // G7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2093, 9}, // C7
	{2349, 9}, // D7
	{1976, 9}, // B6
	{   0,18}, // x2

	{2093, 9}, // C7
	{   0,18}, // x2
	{1586, 9}, // G6
	{   0,18}, // x2
	{1319, 9}, // E6
	{   0,18}, // x2
	{1760, 9}, // A6
	{   0, 9}, // x1
	{1976, 9}, // B6
	{   0, 9}, // x1
	{1865, 9}, // AS6
	{1760, 9}, // A6
	{   0, 9}, // x1

	{1586,12}, // G6
	{2637,12}, // E7
	{3136,12}, // G7
	{3520, 9}, // A7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{3136, 9}, // G7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2093, 9}, // C7
	{2349, 9}, // D7
	{1976, 9}, // B6

	{   0, 0}
};

// Merry Christmas
static const Tone_TypeDef tones_merryChrismast[] = {
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0,18}, // x2

	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0,18}, // x2

	{2637, 9}, // E7
	{   0, 9}, // x1
	{3136, 9}, // G7
	{   0, 9}, // x1
	{2093, 9}, // C7
	{   0, 9}, // x1
	{2349, 9}, // D7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0,24}, // x2

	{2794, 9}, // F7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{   0, 9}, // x1
	{2794, 9}, // F7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2349, 9}, // D7
	{   0, 9}, // x1
	{2349, 9}, // D7
	{   0, 9}, // x1
	{2637, 9}, // E7
	{   0, 9}, // x1
	{2349, 9}, // D7
	{   0, 9}, // x1
	{3136, 9}, // G7
	{   0, 0}  // <-- tones end
};

// Function prototypes
void BUZZER_Init(void);
void BUZZER_Enable(uint16_t freq, uint32_t duration);
void BUZZER_Disable(void);
void BUZZER_PlayTones(const Tone_TypeDef * melody);

extern void buzzer_irq( void );

#ifdef __cplusplus
}
#endif

#endif // __BUZZER_H__
