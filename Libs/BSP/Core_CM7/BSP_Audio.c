/*******************************************************************
 * MiniConsole V3 - Board Support Package - Audio Libs
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_Audio.h"

// Type definitions
typedef struct {
	uint8_t			volume_L;
	uint8_t			volume_R;
	uint8_t			chvolume_L[AUDIO_CFG_CHANNELS];
	uint8_t			chvolume_R[AUDIO_CFG_CHANNELS];
	uint32_t		command;
	uint32_t		params[128];
} AUDIO_REG_TypeDef;

enum AUDIO_CMD {
	AUDIO_CMD_NONE,
	AUDIO_CMD_LINK_SND_LOGO,
	AUDIO_CMD_LINK_SND_TEST,
	AUDIO_CMD_LINK_MP3,
	AUDIO_CMD_LINK_MOD,
	AUDIO_CMD_LINK_RAW,
	AUDIO_CMD_PLAY,
	AUDIO_CMD_STOP,
	AUDIO_CMD_PAUSE
};

enum AUDIO_CH_STATE {
	AUDIO_CH_STATE_DISABLED,
	AUDIO_CH_STATE_PLAY,
	AUDIO_CH_STATE_STOP,
	AUDIO_CH_STATE_PAUSE
};

// Global variables
__IO SH0_RAM static AUDIO_REG_TypeDef		AUDIO_regs = {0};


uint8_t BSP_Audio_LoadMasterVolume(void) {
	uint8_t volume = (uint8_t)((BSP_STM32_RTC_GetBackupReg(RTC, 10) & 0x000000FF) >> 0);
	AUDIO_regs.volume_L = volume;
	AUDIO_regs.volume_R = volume;
	return BSP_OK;
}

uint8_t BSP_Audio_SaveMasterVolume(void) {
	uint32_t tmp = BSP_STM32_RTC_GetBackupReg(RTC, 10);
	tmp &= 0xFFFFFF00;
	tmp |= AUDIO_regs.volume_L;
	BSP_STM32_RTC_SetBackupReg(RTC, 10, tmp);
	return BSP_OK;
}

uint8_t BSP_Audio_SetMasterVolume(uint8_t volume) {
	AUDIO_regs.volume_L = volume;
	AUDIO_regs.volume_R = volume;
	return BSP_OK;
}

uint8_t BSP_Audio_GetMasterVolume(void) {
	return AUDIO_regs.volume_L;
}

uint8_t BSP_Audio_GetMasterVolumeL(void) {
	return AUDIO_regs.volume_L;
}

uint8_t BSP_Audio_GetMasterVolumeR(void) {
	return AUDIO_regs.volume_R;
}

uint8_t BSP_Audio_SetMasterVolumeLR(uint8_t volume_L, uint8_t volume_R) {
	AUDIO_regs.volume_L = volume_L;
	AUDIO_regs.volume_R = volume_R;
	return BSP_OK;
}

uint8_t BSP_Audio_IncMasterVolume(uint8_t delta) {
	uint16_t volume = AUDIO_regs.volume_L;
	volume+=delta;
	if (volume > AUDIO_CFG_MAXMASTERVOL) volume = AUDIO_CFG_MAXMASTERVOL;
	AUDIO_regs.volume_L = (uint8_t)volume;
	AUDIO_regs.volume_R = (uint8_t)volume;
	return BSP_OK;
}

uint8_t BSP_Audio_DecMasterVolume(uint8_t delta) {
	uint16_t volume = AUDIO_regs.volume_L;
	volume-=delta;
	if (volume > 255) volume = 0;
	AUDIO_regs.volume_L = (uint8_t)volume;
	AUDIO_regs.volume_R = (uint8_t)volume;
	return BSP_OK;
}

uint8_t BSP_Audio_SetChannelVolume(uint8_t chno, uint8_t volume) {
	AUDIO_regs.chvolume_L[chno] = volume;
	AUDIO_regs.chvolume_R[chno] = volume;
	return BSP_OK;
}

uint8_t BSP_Audio_SetChannelVolumeLR(uint8_t chno, uint8_t volume_L, uint8_t volume_R) {
	AUDIO_regs.chvolume_L[chno] = volume_L;
	AUDIO_regs.chvolume_R[chno] = volume_R;
	return BSP_OK;
}

uint8_t BSP_Audio_IncChannelVolume(uint8_t chno, uint8_t delta) {
	uint16_t volume = AUDIO_regs.chvolume_L[chno] = volume;
	volume+=delta;
	if (volume > 255) volume = 255;
	AUDIO_regs.chvolume_L[chno] = (uint8_t)volume;
	AUDIO_regs.chvolume_R[chno] = (uint8_t)volume;
	return BSP_OK;
}

uint8_t BSP_Audio_DecChannelVolume(uint8_t chno, uint8_t delta) {
	uint16_t volume = AUDIO_regs.chvolume_L[chno] = volume;
	volume-=delta;
	if (volume > 255) volume = 0;
	AUDIO_regs.chvolume_L[chno] = (uint8_t)volume;
	AUDIO_regs.chvolume_R[chno] = (uint8_t)volume;
	return BSP_OK;
}


uint8_t BSP_Audio_Init(void) {
	// Set default volumes
	for (uint8_t i = 0; i < AUDIO_CFG_CHANNELS; i++) BSP_Audio_SetChannelVolume(i, 128);
	BSP_Audio_SetMasterVolume(100);
	return BSP_OK;
}

uint8_t BSP_Audio_LinkStartupSound(uint8_t chno) {
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_SND_LOGO;
	AUDIO_regs.params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkTestSound(uint8_t chno) {
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_SND_TEST;
	AUDIO_regs.params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkSourceMP3(uint8_t chno, uint32_t addr, uint32_t size) {
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_MP3;
	AUDIO_regs.params[0] = chno;
	AUDIO_regs.params[1] = addr;
	AUDIO_regs.params[2] = size;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkSourceMOD(uint8_t chno, uint32_t addr, uint32_t size) {
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_MOD;
	AUDIO_regs.params[0] = chno;
	AUDIO_regs.params[1] = addr;
	AUDIO_regs.params[2] = size;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkSourceRAW(uint8_t chno, uint32_t addr, uint32_t size) {
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_RAW;
	AUDIO_regs.params[0] = chno;
	AUDIO_regs.params[1] = addr;
	AUDIO_regs.params[2] = size;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_ChannelPLay(uint8_t chno, uint8_t repeat) {
	// repeat = 255 -> loop forever

	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_PLAY;
	AUDIO_regs.params[0] = chno;
	AUDIO_regs.params[1] = repeat;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_ChannelStop(uint8_t chno) {
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_STOP;
	AUDIO_regs.params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BPS_Audio_ChannelPause(uint8_t chno) {
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_PAUSE;
	AUDIO_regs.params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

