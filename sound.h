#ifndef   SOUND
#define   SOUND

#include <stdconst.h>

#define USE_SOUND_INTERUPTS 1

extern UWORD _duration;
extern ULONG *_pattern;
extern UBYTE _length;
extern UBYTE _rate;
extern ULONG _i;

void sound_handler(void);
void SoundInit(void);
void SoundSync(ULONG *pattern, UBYTE length, UBYTE rate, UWORD duration, unsigned int v);
void SoundAsync(ULONG *pattern, UBYTE length, UBYTE rate, UWORD duration, unsigned int v);
void SoundExit(void);
void SoundIntDisable(void);
void SoundIntEnable(void (*handler)(void));
void SoundDisable(void);
void SoundEnable(void);
#endif
