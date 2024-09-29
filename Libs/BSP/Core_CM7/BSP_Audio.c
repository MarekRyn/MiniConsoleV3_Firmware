/*******************************************************************
 * MiniConsole V3 - Board Support Package - Audio Libs
 *******************************************************************/

#include "BSP_Audio.h"

// Type definitions
typedef struct {
	uint8_t			volume_L;
	uint8_t			volume_R;
	uint8_t			chvolume_L[AUDIO_CFG_CHANNELS];
	uint8_t			chvolume_R[AUDIO_CFG_CHANNELS];
	uint32_t		command;
	uint32_t		status;
	uint32_t		c_params[16];	// additional parameters for commands (written by application)
	uint32_t		s_params[16];	// additional parameters for status (written by audio system)
} AUDIO_REG_TypeDef;

// Global variables
__IO SH0_RAM static AUDIO_REG_TypeDef		AUDIO_regs = {0};
__IO static void* 							AUDIO_callbacks[AUDIO_STATUS_COUNT] = {0};


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
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	AUDIO_regs.chvolume_L[chno] = volume;
	AUDIO_regs.chvolume_R[chno] = volume;
	return BSP_OK;
}

uint8_t BSP_Audio_SetChannelVolumeLR(uint8_t chno, uint8_t volume_L, uint8_t volume_R) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	AUDIO_regs.chvolume_L[chno] = volume_L;
	AUDIO_regs.chvolume_R[chno] = volume_R;
	return BSP_OK;
}

uint8_t BSP_Audio_IncChannelVolume(uint8_t chno, uint8_t delta) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	uint16_t volume = AUDIO_regs.chvolume_L[chno];
	volume+=delta;
	if (volume > 255) volume = 255;
	AUDIO_regs.chvolume_L[chno] = (uint8_t)volume;
	AUDIO_regs.chvolume_R[chno] = (uint8_t)volume;
	return BSP_OK;
}

uint8_t BSP_Audio_DecChannelVolume(uint8_t chno, uint8_t delta) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	uint16_t volume = AUDIO_regs.chvolume_L[chno];
	volume-=delta;
	if (volume > 255) volume = 0;
	AUDIO_regs.chvolume_L[chno] = (uint8_t)volume;
	AUDIO_regs.chvolume_R[chno] = (uint8_t)volume;
	return BSP_OK;
}


uint8_t BSP_Audio_Init(void) {
	// Waiting for AUDIO readiness
	while (AUDIO_regs.status != AUDIO_STATUS_READY)	{};
	memset((AUDIO_REG_TypeDef *)&AUDIO_regs.s_params, 0, sizeof(AUDIO_regs.s_params));
	AUDIO_regs.status = AUDIO_STATUS_NONE;

	// Set default volumes
	for (uint8_t i = 0; i < AUDIO_CFG_CHANNELS; i++) BSP_Audio_SetChannelVolume(i, 128);
	BSP_Audio_SetMasterVolume(100);
	return BSP_OK;
}

uint8_t BSP_Audio_LinkStartupSound(uint8_t chno) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_SND_LOGO;
	AUDIO_regs.c_params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkTestSound(uint8_t chno) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_SND_TEST;
	AUDIO_regs.c_params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkSourceMP3(uint8_t chno, void * addr, uint32_t size) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_MP3;
	AUDIO_regs.c_params[0] = chno;
	AUDIO_regs.c_params[1] = (uint32_t)addr;
	AUDIO_regs.c_params[2] = size;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkSourceSMP3(uint8_t chno) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_SMP3;
	AUDIO_regs.c_params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkSourceMOD(uint8_t chno, void * addr, uint32_t size) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_MOD;
	AUDIO_regs.c_params[0] = chno;
	AUDIO_regs.c_params[1] = (uint32_t)addr;
	AUDIO_regs.c_params[2] = size;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkSourceRAW(uint8_t chno, void * addr, uint32_t size, uint8_t chn, uint8_t bitformat, uint16_t freq) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_RAW;
	AUDIO_regs.c_params[0] = chno;
	AUDIO_regs.c_params[1] = (uint32_t)addr;
	AUDIO_regs.c_params[2] = size;
	AUDIO_regs.c_params[3] = chn;
	AUDIO_regs.c_params[4] = bitformat;
	AUDIO_regs.c_params[5] = freq;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_LinkSourceMID(uint8_t chno, void * sfaddr, uint32_t sfsize, void * addr, uint32_t size) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_LINK_MID;
	AUDIO_regs.c_params[0] = chno;
	AUDIO_regs.c_params[1] = (uint32_t)sfaddr;
	AUDIO_regs.c_params[2] = sfsize;
	AUDIO_regs.c_params[3] = (uint32_t)addr;
	AUDIO_regs.c_params[4] = size;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}


uint8_t BSP_Audio_ChannelPLay(uint8_t chno, uint8_t repeat) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// repeat = 255 -> loop forever

	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_PLAY;
	AUDIO_regs.c_params[0] = chno;
	AUDIO_regs.c_params[1] = repeat;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_ChannelStop(uint8_t chno) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_STOP;
	AUDIO_regs.c_params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}

uint8_t BSP_Audio_ChannelPause(uint8_t chno) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_PAUSE;
	AUDIO_regs.c_params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}


uint8_t BSP_Audio_GetFreeChannel(void) {
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_GETCHANNEL;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	// Wait until data processed;
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Return value
	return AUDIO_regs.s_params[0];
}

void * BSP_Audio_GetBufAddr(uint8_t chno) {
	if (chno >= AUDIO_CFG_CHANNELS) return NULL;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_GETBUFADDR;
	AUDIO_regs.c_params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	// Wait until data processed;
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Return value
	return (void *)AUDIO_regs.s_params[0];
}

uint8_t BSP_Audio_BufUpdateCompleted(uint8_t chno) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
	// Wait until command register is ready
	while (AUDIO_regs.command != AUDIO_CMD_NONE) {};
	// Setup registers
	AUDIO_regs.command = AUDIO_CMD_BUFUPDCMPL;
	AUDIO_regs.c_params[0] = chno;
	// Activate command by sending SEV to CM4 core;
	__SEV();
	return BSP_OK;
}



uint8_t BSP_Audio_RegisterStatusCallback(uint8_t status, void* callback) {
	AUDIO_callbacks[status] = callback;
	return BSP_OK;
}

uint32_t BSP_Audio_GetStatusParam(uint8_t index) {
	return AUDIO_regs.s_params[index];
}


// Interrupt handlers
void CM4_SEV_IRQHandler(void) {
	if (AUDIO_regs.status == AUDIO_STATUS_NONE) return;
	if (AUDIO_regs.status == AUDIO_STATUS_READY) return;
	if (AUDIO_regs.status >= AUDIO_STATUS_COUNT) return;

	if (AUDIO_callbacks[AUDIO_regs.status] != NULL) {
		void (*pcallback)(void) = AUDIO_callbacks[AUDIO_regs.status];
		pcallback();
	}

	memset((AUDIO_REG_TypeDef *)&AUDIO_regs.s_params, 0, sizeof(AUDIO_regs.s_params));
	AUDIO_regs.status = AUDIO_STATUS_NONE;
}
