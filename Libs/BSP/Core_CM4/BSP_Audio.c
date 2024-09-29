/*******************************************************************
 * MiniConsole V3 - Board Support Package - Audio Libs
 *******************************************************************/



#include "BSP_Audio.h"

// Codecs
#include "hxcmod.h"
#include "rawaudio.h"
#include "midiaudio.h"
#define DR_MP3_IMPLEMENTATION
#define DR_MP3_ONLY_MP3
#define DR_MP3_NO_STDIO
#include "dr_mp3.h"


// Macros
#define CLAMP(X, MIN, MAX)		( (X) < (MIN) ? (MIN) : ((X) > (MAX) ? (MAX) : (X)) )


// Tables
static const uint8_t TAB_VOLUME[256] =
	{
	   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,
	   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   4,   4,
	   4,   4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   6,   7,   7,   7,   8,
	   8,   8,   8,   9,   9,   9,  10,  10,  10,  11,  11,  12,  12,  12,  13,  13,
	  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  21,
	  22,  22,  23,  23,  24,  25,  25,  26,  27,  27,  28,  29,  29,  30,  31,  32,
	  32,  33,  34,  35,  35,  36,  37,  38,  39,  40,  40,  41,  42,  43,  44,  45,
	  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  60,  61,  62,
	  63,  64,  65,  67,  68,  69,  70,  72,  73,  74,  76,  77,  78,  80,  81,  83,
	  84,  85,  87,  88,  90,  91,  93,  94,  96,  98,  99, 101, 102, 104, 106, 107,
	 109, 111, 112, 114, 116, 118, 120, 121, 123, 125, 127, 129, 131, 133, 135, 137,
	 139, 141, 143, 145, 147, 149, 151, 153, 155, 157, 160, 162, 164, 166, 168, 171,
	 173, 175, 178, 180, 183, 185, 187, 190, 192, 195, 197, 200, 202, 205, 208, 210,
	 213, 216, 218, 221, 224, 226, 229, 232, 235, 238, 241, 244, 246, 249, 252, 255
	};

static const uint32_t TAB_BITRATE[16] = {0, 32000, 40000, 48000, 56000, 64000, 80000, 96000, 112000, 128000, 160000, 192000, 224000, 256000, 320000, 0};


// Type definitions
typedef struct {
	uint32_t	type;
	void*		pctx;
	uint32_t	state;
	uint8_t		gain_L;
	uint8_t		gain_R;
	uint8_t		repeat;
	uint32_t	tmp0;
} AUDIO_CHANNEL_TypeDef;

typedef struct {
	AUDIO_CHANNEL_TypeDef	channels[AUDIO_CFG_CHANNELS];
	uint8_t					newcmd;
	uint8_t					gain_L;
	uint8_t					gain_R;
	uint8_t					buf_edit;
	uint8_t					buf_ready;
	uint8_t					buf_busy;
} AUDIO_CTX_TypeDef;

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


typedef struct {
	uint8_t * 				streambuf;	// Buffer in shared memory
	uint32_t				buf_modulo;
	uint32_t				buf_frames;
	uint32_t				i_uploaded;
	uint32_t				i_consumed;
	drmp3dec_frame_info		mp3_info;
	drmp3dec				mp3_dec;
	uint32_t				framesize;
	uint8_t					bufflag;
} AUDIO_STREAM_TypeDef;

// Variables
__IO static						AUDIO_CTX_TypeDef		AUDIO_ctx = {0};
__IO SH0_RAM static 			AUDIO_REG_TypeDef		AUDIO_regs = {0};
__IO static						TxRxContext_TypeDef		BSP_hdma_ctx = {0};

__IO static int16_t 			OutputBuf[AUDIO_CFG_BUF_SIZE] __attribute__ ((aligned (4)));	// Output buffer
__IO static int16_t				ChannelBuf[AUDIO_CFG_CHANNELS][AUDIO_CFG_BUF_SIZE >> 1] __attribute__ ((aligned (4)));	// Channel buffers
__IO static int32_t				MixBuf[AUDIO_CFG_BUF_SIZE >> 1]; // Mixing buffer


// Variable for simple buffer allocation
static void * sh0_bufs[8] = {0};


// ********** IRQ Handlers and Callbacks ****************

// I2S3 IRQ Handler
void SPI3_IRQHandler(void) {
	BSP_STM32_I2S_IRQHandler(AUDIO_CFG_I2S,(TxRxContext_TypeDef* )&BSP_hdma_ctx);
}

// IRQ from Core CM7 - activated when new command is available to process
void CM7_SEV_IRQHandler(void) {
	AUDIO_ctx.newcmd = 1;
}


// Callback when DMA reach end of buffer
static void _AudioCallbackTC(TxRxContext_TypeDef * ctx) {
	if (AUDIO_ctx.buf_ready != 0) return;
	AUDIO_ctx.buf_ready = 255;
	AUDIO_ctx.buf_edit = 1;
	AUDIO_ctx.buf_busy = 0;

	UNUSED(ctx);
}

// Callback when DMA reach half of buffer
static void _AudioCallbackHT(TxRxContext_TypeDef * ctx) {
	if (AUDIO_ctx.buf_ready != 1) return;
	AUDIO_ctx.buf_ready = 255;
	AUDIO_ctx.buf_edit = 0;
	AUDIO_ctx.buf_busy = 1;

	UNUSED(ctx);
}

// Callback on DMA error
static void _AudioCallbackTE(TxRxContext_TypeDef * ctx) {
	UNUSED(ctx);
}



// ********** Private functions ****************

static void * _SMP3_BufAlloc(void) {
	for (uint8_t i = 0; i < 8; i++) {
		if (sh0_bufs[i] == NULL) if ((AUDIO_SH0_PROTECTED + AUDIO_CFG_STREAMBUF_SIZE * (i + 1)) < AUDIO_SH0_SIZE) {
			sh0_bufs[i] = (void *)(AUDIO_SH0_PROTECTED + AUDIO_CFG_STREAMBUF_SIZE * i);
			return sh0_bufs[i];
		}
	}
	return NULL;
}

static void _SMP3_BufFree(void * addr) {
	for (uint8_t i = 0; i < 8; i++) if (sh0_bufs[i] == addr) {
			sh0_bufs[i] = NULL;
			return;
	}
}


static uint8_t _SMP3_Init(AUDIO_STREAM_TypeDef * ctx) {
	// Allocating stream buffer for mp3 frames;
	ctx->streambuf = _SMP3_BufAlloc();
	if (ctx->streambuf == NULL) return BSP_ERROR;

	// Initialize offsets for uploaded and consumed data
	ctx->i_consumed = 0;
	ctx->i_uploaded = 0;
	ctx->buf_modulo = AUDIO_CFG_STREAMBUF_SIZE;
	ctx->buf_frames = 0;
	ctx->framesize = 0;

	// Initialize mp3 decoder
	memset(&ctx->mp3_info, 0 , sizeof(drmp3dec_frame_info));
	memset(&ctx->mp3_dec, 0, sizeof(drmp3dec));
	drmp3dec_init(&ctx->mp3_dec);

	return BSP_OK;
}


static uint8_t _SMP3_Read(AUDIO_STREAM_TypeDef * ctx, uint32_t len, int16_t * dst_buf) {
	uint32_t offset_consumed;
	uint32_t framesize;
	uint32_t req_frames;
	uint8_t flag = 0;
	uint32_t l;
	uint8_t * frame;

	// Streamed data checks. If any fails, than function will fill the dst_buf with 0s.
	do {
		// Checking if required length is multiply of 1152
		if (len % 1152) break;

		// Checking if there are enough uploaded frames to fill buffer
		req_frames = (len / 1152);
		if (ctx->buf_frames < req_frames) ctx->bufflag = 1;
		// Buf flag can be reset only when loaded minimum number of frames in stream buffer
		if (ctx->bufflag) break;

		// All is ok
		flag = 1;
	} while (0);

	// Updating PCM buffer

	switch (flag) {
	case 0:
		// Tests failed. Filling up with 0s.
		memset(dst_buf, 0, len * 4);
		break;
	case 1:
		// All ok - decoding frames
		while (req_frames > 0) {
			offset_consumed = ctx->i_consumed % ctx->buf_modulo;
			frame = (void *)ctx->streambuf + offset_consumed + AUDIO_SH0_CM4_ADDR;
			l = drmp3dec_decode_frame_ex(&ctx->mp3_dec, frame, dst_buf, &ctx->mp3_info);
			if (l != 1152) {
				// Error in decoding -> Flush stream buffer
				ctx->i_consumed = 0;
				ctx->i_uploaded = 0;
				ctx->buf_frames = 0;
				memset(dst_buf, 0, len * 4);
				break;
			}
			framesize = ctx->mp3_info.frame_bytes;
			framesize = (framesize & 0xFFFFFFFC) + 4;
			ctx->i_consumed += framesize;
			dst_buf += 2 * l;
			ctx->buf_frames--;
			req_frames--;
		}
		break;
	}
	return BSP_OK;
}

static uint8_t _SMP3_Deinit(AUDIO_STREAM_TypeDef * ctx) {
	// Free stream buffer
	if (ctx->streambuf) _SMP3_BufFree(ctx->streambuf);
	// Free ctx
	free(ctx);

	return BSP_OK;
}


static void * _SMP3_GetBufAddr(AUDIO_STREAM_TypeDef * ctx) {

	if (!ctx->streambuf) return NULL;

	uint32_t offset = ctx->i_uploaded % ctx->buf_modulo;
	return (void *)ctx->streambuf + offset + AUDIO_SH0_CM7_ADDR;

}

static uint8_t _SMP3_BufUpdateComplete(AUDIO_STREAM_TypeDef * ctx) {

	uint32_t framesize;
	uint32_t buf_modulo;
	uint32_t offset_uploaded = ctx->i_uploaded % ctx->buf_modulo;
	uint32_t offset_consumed = ctx->i_consumed % ctx->buf_modulo;
	uint8_t * hdr = ctx->streambuf + offset_uploaded + AUDIO_SH0_CM4_ADDR;

	// Validating data in the buffer
	if (hdr[0] != 0xFF) return BSP_ERROR;			// MP3 frame indicator
	if ((hdr[1] & 0xFA) != 0xFA) return BSP_ERROR; 	// Only MPEG1 Layer 3 are valid
	if ((hdr[2] & 0x0C) != 0x00) return BSP_ERROR;	// Only 44100 sampling is valid
	if ((hdr[3] & 0x30) == 0x30) return BSP_ERROR; 	// Mono is not supported
	if ((hdr[2] >> 4) == 0x0F) return BSP_ERROR; 	// Illegal bitrate

	// Calculating frame size
	framesize = (144 * (TAB_BITRATE[hdr[2] >> 4])) / 44100;
	framesize = (framesize & 0xFFFFFFFC) + 4;

	// For free bitrate we will use previous frame size
	if (framesize == 0) framesize = ctx->framesize;
	if (framesize > 0) ctx->framesize = framesize;

	// Updating buffer modulo (including 16 bytes of safety margin)
	buf_modulo = ((AUDIO_CFG_STREAMBUF_SIZE - 16) / framesize) * framesize;
	if (buf_modulo != ctx->buf_modulo) {
		ctx->i_uploaded = offset_uploaded;
		ctx->i_consumed = offset_consumed;
		ctx->buf_modulo = buf_modulo;
	}

	// Checking if buffer overflowed with data. If yes than override last available slot
	if ((ctx->i_uploaded + framesize) <= ctx->i_consumed) return BSP_BUSY;
	if ((ctx->i_uploaded + framesize) > (ctx->i_consumed + ctx->buf_modulo - framesize)) return BSP_BUSY;

	// Updating stored frame count
	ctx->buf_frames++;
	if (ctx->buf_frames >= 6) ctx->bufflag = 0;

	// Updating buffer uploaded index (head index)
	ctx->i_uploaded += framesize;

	return BSP_OK;
}

//static inline void _status_ready(void) {
//	// Wait until command register is ready
//	while (AUDIO_regs.status != AUDIO_STATUS_NONE) {};
//	// Setup registers
//	AUDIO_regs.status = AUDIO_STATUS_READY;
//	// Activate command by sending SEV to CM7 core;
//	__SEV();
//}
//
//static inline void _status_buf_underrun(void) {
//	// Wait until command register is ready
//	while (AUDIO_regs.status != AUDIO_STATUS_NONE) {};
//	// Setup registers
//	AUDIO_regs.status = AUDIO_STATUS_BUF_UNDERRUN;
//	// Activate command by sending SEV to CM7 core;
//	__SEV();
//}

static inline void _status_ch_repeat(uint8_t chno) {
	// Wait until command register is ready
	while (AUDIO_regs.status != AUDIO_STATUS_NONE) {};
	// Setup registers
	AUDIO_regs.s_params[0] = chno;
	AUDIO_regs.status = AUDIO_STATUS_CH_REPEAT;
	// Activate command by sending SEV to CM7 core;
	__SEV();
}

static inline void _status_ch_stop(uint8_t chno) {
	// Wait until command register is ready
	while (AUDIO_regs.status != AUDIO_STATUS_NONE) {};
	// Setup registers
	AUDIO_regs.s_params[0] = chno;
	AUDIO_regs.status = AUDIO_STATUS_CH_STOP;
	// Activate command by sending SEV to CM7 core;
	__SEV();
}

static uint8_t _process_cmd(void) {
	uint8_t chno, repeat;
	uint32_t addr, sfaddr, size, sfsize, chn, bitformat, freq;

	AUDIO_ctx.newcmd = 0;

	switch (AUDIO_regs.command) {

	case AUDIO_CMD_LINK_SND_LOGO:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		BSP_Audio_ChannelLinkSource(chno, AUDIO_CH_SOURCE_MP3);
		drmp3_init_memory(AUDIO_ctx.channels[chno].pctx, (void *)SND_Logo, sizeof(SND_Logo), NULL);
		break;

	case AUDIO_CMD_LINK_SND_TEST:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		BSP_Audio_ChannelLinkSource(chno, AUDIO_CH_SOURCE_MP3);
		drmp3_init_memory(AUDIO_ctx.channels[chno].pctx, (void *)SND_Test, sizeof(SND_Test), NULL);
		break;

	case AUDIO_CMD_LINK_MP3:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		addr = AUDIO_regs.c_params[1];
		size = AUDIO_regs.c_params[2];
		BSP_Audio_ChannelLinkSource(chno, AUDIO_CH_SOURCE_MP3);
		drmp3_init_memory(AUDIO_ctx.channels[chno].pctx, (void *)addr, size, NULL);
		break;

	case AUDIO_CMD_LINK_SMP3:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		BSP_Audio_ChannelLinkSource(chno, AUDIO_CH_SOURCE_SMP3);
		_SMP3_Init(AUDIO_ctx.channels[chno].pctx);
		break;

	case AUDIO_CMD_LINK_MOD:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		addr = AUDIO_regs.c_params[1];
		size = AUDIO_regs.c_params[2];
		BSP_Audio_ChannelLinkSource(chno, AUDIO_CH_SOURCE_MOD);
		hxcmod_init(AUDIO_ctx.channels[chno].pctx);
		hxcmod_setcfg(AUDIO_ctx.channels[chno].pctx, 44100, 0, 0);
		hxcmod_load(AUDIO_ctx.channels[chno].pctx, (void *)addr, size);
		break;

	case AUDIO_CMD_LINK_RAW:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		addr = AUDIO_regs.c_params[1];
		size = AUDIO_regs.c_params[2];
		chn = AUDIO_regs.c_params[3];
		bitformat = AUDIO_regs.c_params[4];
		freq = AUDIO_regs.c_params[5];
		BSP_Audio_ChannelLinkSource(chno, AUDIO_CH_SOURCE_RAW);
		RA_Init(AUDIO_ctx.channels[chno].pctx, (int16_t *)addr, size, chn, bitformat, freq);
		break;

	case AUDIO_CMD_LINK_MID:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		sfaddr = AUDIO_regs.c_params[1];
		sfsize = AUDIO_regs.c_params[2];
		addr = AUDIO_regs.c_params[3];
		size = AUDIO_regs.c_params[4];
		BSP_Audio_ChannelLinkSource(chno, AUDIO_CH_SOURCE_MID);
		MID_Init(AUDIO_ctx.channels[chno].pctx, (void *)sfaddr, sfsize, (void *)addr, size);
		break;

	case AUDIO_CMD_PLAY:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		repeat = (uint8_t)AUDIO_regs.c_params[1];
		AUDIO_ctx.channels[chno].state = AUDIO_CH_STATE_PLAY;
		AUDIO_ctx.channels[chno].repeat = repeat;
		break;

	case AUDIO_CMD_STOP:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		AUDIO_ctx.channels[chno].state = AUDIO_CH_STATE_STOP;
		break;

	case AUDIO_CMD_PAUSE:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;
		AUDIO_ctx.channels[chno].state = AUDIO_CH_STATE_PAUSE;
		break;

	case AUDIO_CMD_GETCHANNEL:
		chno = BSP_Audio_GetChannel();
		AUDIO_regs.s_params[0] = chno;
		break;

	case AUDIO_CMD_GETBUFADDR:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		AUDIO_regs.s_params[0] = (uint32_t)_SMP3_GetBufAddr(AUDIO_ctx.channels[chno].pctx);
		break;

	case AUDIO_CMD_BUFUPDCMPL:
		chno = (uint8_t)AUDIO_regs.c_params[0];
		_SMP3_BufUpdateComplete(AUDIO_ctx.channels[chno].pctx);
		break;
	}

	// Zero registers
	memset((AUDIO_REG_TypeDef *)&AUDIO_regs.c_params, 0, sizeof(AUDIO_regs.c_params));
	// Informing CM7 core that command execution is completed
	AUDIO_regs.command = AUDIO_CMD_NONE;
	return BSP_OK;
}


// ********** Public functions ****************
uint8_t BSP_Audio_ChannelFree(uint8_t chno) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;

	// Free resources
	switch (AUDIO_ctx.channels[chno].type) {
	case AUDIO_CH_SOURCE_NONE:
		break;
	case AUDIO_CH_SOURCE_MP3:
		if (AUDIO_ctx.channels[chno].pctx != NULL) drmp3_uninit(AUDIO_ctx.channels[chno].pctx);
		break;
	case AUDIO_CH_SOURCE_SMP3:
		if (AUDIO_ctx.channels[chno].pctx != NULL) _SMP3_Deinit(AUDIO_ctx.channels[chno].pctx);
		break;
	case AUDIO_CH_SOURCE_MOD:
		if (AUDIO_ctx.channels[chno].pctx != NULL) hxcmod_unload(AUDIO_ctx.channels[chno].pctx);
		break;
	case AUDIO_CH_SOURCE_RAW:
		if (AUDIO_ctx.channels[chno].pctx != NULL) RA_Deinit(AUDIO_ctx.channels[chno].pctx);
		break;
	case AUDIO_CH_SOURCE_MID:
		break;
	}

	if (AUDIO_ctx.channels[chno].pctx != NULL) free(AUDIO_ctx.channels[chno].pctx);
	AUDIO_ctx.channels[chno].pctx = NULL;
	AUDIO_ctx.channels[chno].type = AUDIO_CH_SOURCE_NONE;
	AUDIO_ctx.channels[chno].state = AUDIO_CH_STATE_DISABLED;

	return BSP_OK;
}

uint8_t BSP_Audio_ChannelLinkSource(uint8_t chno, uint32_t source) {
	if (chno >= AUDIO_CFG_CHANNELS) return BSP_ERROR;

	// Free previous resources
	BSP_Audio_ChannelFree(chno);

	// Linking new source
	switch (source) {

	case AUDIO_CH_SOURCE_NONE:
		break;

	case AUDIO_CH_SOURCE_MP3:
		AUDIO_ctx.channels[chno].pctx = malloc(sizeof(drmp3));
		if (AUDIO_ctx.channels[chno].pctx == NULL) return BSP_ERROR;
		AUDIO_ctx.channels[chno].type = AUDIO_CH_SOURCE_MP3;
		AUDIO_ctx.channels[chno].state = AUDIO_CH_STATE_DISABLED;
		AUDIO_ctx.channels[chno].gain_L = TAB_VOLUME[AUDIO_regs.chvolume_L[chno]];
		AUDIO_ctx.channels[chno].gain_R = TAB_VOLUME[AUDIO_regs.chvolume_R[chno]];
		break;

	case AUDIO_CH_SOURCE_SMP3:
		AUDIO_ctx.channels[chno].pctx = malloc(sizeof(AUDIO_STREAM_TypeDef));
		if (AUDIO_ctx.channels[chno].pctx == NULL) return BSP_ERROR;
		AUDIO_ctx.channels[chno].type = AUDIO_CH_SOURCE_SMP3;
		AUDIO_ctx.channels[chno].state = AUDIO_CH_STATE_DISABLED;
		AUDIO_ctx.channels[chno].gain_L = TAB_VOLUME[AUDIO_regs.chvolume_L[chno]];
		AUDIO_ctx.channels[chno].gain_R = TAB_VOLUME[AUDIO_regs.chvolume_R[chno]];
		break;

	case AUDIO_CH_SOURCE_MOD:
		AUDIO_ctx.channels[chno].pctx = malloc(sizeof(modcontext));
		if (AUDIO_ctx.channels[chno].pctx == NULL) return BSP_ERROR;
		AUDIO_ctx.channels[chno].type = AUDIO_CH_SOURCE_MOD;
		AUDIO_ctx.channels[chno].state = AUDIO_CH_STATE_DISABLED;
		AUDIO_ctx.channels[chno].gain_L = TAB_VOLUME[AUDIO_regs.chvolume_L[chno]];
		AUDIO_ctx.channels[chno].gain_R = TAB_VOLUME[AUDIO_regs.chvolume_R[chno]];
		break;

	case AUDIO_CH_SOURCE_RAW:
		AUDIO_ctx.channels[chno].pctx = malloc(sizeof(rawaudioctx_TypeDef));
		if (AUDIO_ctx.channels[chno].pctx == NULL) return BSP_ERROR;
		AUDIO_ctx.channels[chno].type = AUDIO_CH_SOURCE_RAW;
		AUDIO_ctx.channels[chno].state = AUDIO_CH_STATE_DISABLED;
		AUDIO_ctx.channels[chno].gain_L = TAB_VOLUME[AUDIO_regs.chvolume_L[chno]];
		AUDIO_ctx.channels[chno].gain_R = TAB_VOLUME[AUDIO_regs.chvolume_R[chno]];
		break;

	case AUDIO_CH_SOURCE_MID:
		AUDIO_ctx.channels[chno].pctx = malloc(sizeof(midctx_TypeDef));
		if (AUDIO_ctx.channels[chno].pctx == NULL) return BSP_ERROR;
		AUDIO_ctx.channels[chno].type = AUDIO_CH_SOURCE_MID;
		AUDIO_ctx.channels[chno].state = AUDIO_CH_STATE_DISABLED;
		AUDIO_ctx.channels[chno].gain_L = TAB_VOLUME[AUDIO_regs.chvolume_L[chno]];
		AUDIO_ctx.channels[chno].gain_R = TAB_VOLUME[AUDIO_regs.chvolume_R[chno]];
		break;

	default:
		return BSP_ERROR;

	}

	return BSP_OK;
}


uint8_t BSP_Audio_GetChannel(void) {
	// Returns number of first channel that is not playing sound at this moment
	uint8_t ch = 0;
	while (ch < AUDIO_CFG_CHANNELS) {
		if (AUDIO_ctx.channels[ch].type == AUDIO_CH_SOURCE_NONE) return ch;
		if (AUDIO_ctx.channels[ch].state == AUDIO_CH_STATE_DISABLED) return ch;
		if (AUDIO_ctx.channels[ch].state == AUDIO_CH_STATE_STOP) return ch;
		ch++;
	}
	// Returns 255 if no free channel found
	return 255;
}

uint8_t BSP_Audio_Init(void) {

	// Configuring I2S Interface
	BSP_STM32_I2S_Init(AUDIO_CFG_I2S, (TxRxContext_TypeDef* )&BSP_hdma_ctx, I2S_AUDIOFREQ_44K);

	// Linking DMA stream to I2C context
	BSP_STM32_DMA_Init(AUDIO_CFG_DMA, DMA_MEMORY_TO_PERIPH, DMA_PINC_DISABLE, DMA_MINC_ENABLE, DMA_PDATAALIGN_HALFWORD, DMA_MDATAALIGN_HALFWORD, DMA_CIRCULAR, DMA_PRIORITY_LOW);

	BSP_hdma_ctx.callback_HT = (uint32_t)_AudioCallbackHT;
	BSP_hdma_ctx.callback_TC = (uint32_t)_AudioCallbackTC;
	BSP_hdma_ctx.callback_TE = (uint32_t)_AudioCallbackTE;

	if (BSP_STM32_DMA_CtxLink(AUDIO_CFG_DMA, (TxRxContext_TypeDef* )&BSP_hdma_ctx, DMA_REQUEST_SPI3_TX)) return BSP_ERROR;

	// Enable Audio Amplifiers
	BSP_DRV_Audio_Enable();

	// Initialization of Audio Engine

	AUDIO_ctx.newcmd = 0;
	AUDIO_ctx.buf_ready = 255;
	AUDIO_ctx.buf_edit = 1;
	AUDIO_ctx.buf_busy = 0;
	AUDIO_ctx.gain_L = TAB_VOLUME[AUDIO_regs.volume_L];
	AUDIO_ctx.gain_R = TAB_VOLUME[AUDIO_regs.volume_R];

	AUDIO_regs.command = AUDIO_CMD_NONE;
	memset((AUDIO_REG_TypeDef *)&AUDIO_regs.c_params, 0, sizeof(AUDIO_regs.c_params));
	memset((AUDIO_REG_TypeDef *)&AUDIO_regs.s_params, 0, sizeof(AUDIO_regs.s_params));

	// Start DMA
	BSP_STM32_I2S_TransmitDMA(AUDIO_CFG_I2S, (TxRxContext_TypeDef* )&BSP_hdma_ctx, (int16_t *)OutputBuf, AUDIO_CFG_BUF_SIZE);

	// Reporting that AUDIO system is ready
	AUDIO_regs.status = AUDIO_STATUS_READY;
	__SEV();

	return BSP_OK;
}


uint8_t BSP_Audio_Loop(void) {

	while(1) {

		if (AUDIO_ctx.newcmd) _process_cmd();

		if (AUDIO_ctx.buf_edit == 255) continue;

		// Update master volume
		AUDIO_ctx.gain_L = TAB_VOLUME[AUDIO_regs.volume_L];
		AUDIO_ctx.gain_R = TAB_VOLUME[AUDIO_regs.volume_R];

		// Processing PCM frames for each channel
		for (uint8_t ch = 0; ch < (AUDIO_CFG_CHANNELS); ch++) {
			if (AUDIO_ctx.channels[ch].type == AUDIO_CH_SOURCE_NONE) continue;
			if (AUDIO_ctx.channels[ch].state != AUDIO_CH_STATE_PLAY) continue;

			AUDIO_ctx.channels[ch].gain_L = TAB_VOLUME[AUDIO_regs.chvolume_L[ch]];
			AUDIO_ctx.channels[ch].gain_R = TAB_VOLUME[AUDIO_regs.chvolume_R[ch]];

			switch (AUDIO_ctx.channels[ch].type) {

			case AUDIO_CH_SOURCE_MP3:
				drmp3 * pmp3 = (drmp3*)AUDIO_ctx.channels[ch].pctx;
				drmp3_read_pcm_frames_s16(pmp3, AUDIO_CFG_BUF_SIZE >> 2, (int16_t *)ChannelBuf[ch]);
				if (pmp3->memory.currentReadPos == pmp3->memory.dataSize) {
					if (AUDIO_ctx.channels[ch].repeat != 0) drmp3_seek_to_start_of_stream(pmp3);
					if ((AUDIO_ctx.channels[ch].repeat > 0) && (AUDIO_ctx.channels[ch].repeat < 255)) AUDIO_ctx.channels[ch].repeat--;
					if (AUDIO_ctx.channels[ch].repeat == 0) { AUDIO_ctx.channels[ch].state = AUDIO_CH_STATE_STOP; _status_ch_stop(ch); }
						else _status_ch_repeat(ch);
				}
				break;

			case AUDIO_CH_SOURCE_SMP3:
				AUDIO_STREAM_TypeDef * smp3ctx = (AUDIO_STREAM_TypeDef *)AUDIO_ctx.channels[ch].pctx;
				_SMP3_Read(smp3ctx, AUDIO_CFG_BUF_SIZE >> 2, (int16_t *)ChannelBuf[ch]);
				break;

			case AUDIO_CH_SOURCE_MOD:
				modcontext * pmod = (modcontext*)AUDIO_ctx.channels[ch].pctx;
				hxcmod_fillbuffer(AUDIO_ctx.channels[ch].pctx, (int16_t *)ChannelBuf[ch], AUDIO_CFG_BUF_SIZE >> 2, NULL);
				if ((AUDIO_ctx.channels[ch].tmp0 == (pmod->song.length - 1)) && (pmod->tablepos == 0)) {
					if ((AUDIO_ctx.channels[ch].repeat > 0) && (AUDIO_ctx.channels[ch].repeat < 255)) AUDIO_ctx.channels[ch].repeat--;
					if (AUDIO_ctx.channels[ch].repeat == 0) { AUDIO_ctx.channels[ch].state = AUDIO_CH_STATE_STOP; _status_ch_stop(ch); }
						else _status_ch_repeat(ch);
				}
				AUDIO_ctx.channels[ch].tmp0 = pmod->tablepos;
				break;

			case AUDIO_CH_SOURCE_RAW:
				rawaudioctx_TypeDef * praw = (rawaudioctx_TypeDef *)AUDIO_ctx.channels[ch].pctx;
				RA_Read(praw, AUDIO_CFG_BUF_SIZE >> 2, (int16_t *)ChannelBuf[ch]);
				if (praw->index == praw->length) {
					if (AUDIO_ctx.channels[ch].repeat != 0) RA_Seek(praw, 0);
					if ((AUDIO_ctx.channels[ch].repeat > 0) && (AUDIO_ctx.channels[ch].repeat < 255)) AUDIO_ctx.channels[ch].repeat--;
					if (AUDIO_ctx.channels[ch].repeat == 0) { AUDIO_ctx.channels[ch].state = AUDIO_CH_STATE_STOP; _status_ch_stop(ch); }
						else _status_ch_repeat(ch);
				}
				break;

			case AUDIO_CH_SOURCE_MID:
				midctx_TypeDef * pmid = (midctx_TypeDef *)AUDIO_ctx.channels[ch].pctx;
				MID_Read(pmid, AUDIO_CFG_BUF_SIZE >> 2, (int16_t *)ChannelBuf[ch]);
				if (AUDIO_ctx.channels[ch].repeat != 0) MID_Reset(pmid);
				if ((AUDIO_ctx.channels[ch].repeat > 0) && (AUDIO_ctx.channels[ch].repeat < 255)) AUDIO_ctx.channels[ch].repeat--;
				if (AUDIO_ctx.channels[ch].repeat == 0) { AUDIO_ctx.channels[ch].state = AUDIO_CH_STATE_STOP; _status_ch_stop(ch); }
					else _status_ch_repeat(ch);
				break;
			}
		}

		// Mixing channels
		int16_t * outbuf = AUDIO_ctx.buf_edit ? (int16_t*)&OutputBuf[AUDIO_CFG_BUF_SIZE >> 1] : (int16_t*)&OutputBuf[0];

		for (uint8_t ch = 0; ch < (AUDIO_CFG_CHANNELS); ch++) {
			if (AUDIO_ctx.channels[ch].type == AUDIO_CH_SOURCE_NONE) continue;
			if (AUDIO_ctx.channels[ch].state != AUDIO_CH_STATE_PLAY) continue;

			for (uint32_t i = 0; i < (AUDIO_CFG_BUF_SIZE >> 1); i+=2) {
				MixBuf[i+0] += (int32_t)ChannelBuf[ch][i+0] * AUDIO_ctx.channels[ch].gain_L;
				MixBuf[i+1] += (int32_t)ChannelBuf[ch][i+1] * AUDIO_ctx.channels[ch].gain_R;
			}
		}

		for (uint32_t i = 0; i < (AUDIO_CFG_BUF_SIZE >> 1); i+=2) {
			outbuf[i+0] = (int16_t)CLAMP( ((MixBuf[i+0] * AUDIO_ctx.gain_L) >> (AUDIO_CFG_MIXDIV + 5 + 5)), -32768, 32767 );
			MixBuf[i+0] = 0;
			outbuf[i+1] = (int16_t)CLAMP( ((MixBuf[i+1] * AUDIO_ctx.gain_R) >> (AUDIO_CFG_MIXDIV + 5 + 5)), -32768, 32767 );
			MixBuf[i+1] = 0;
		}

		// Switching buf flags
		AUDIO_ctx.buf_ready = AUDIO_ctx.buf_edit;
		AUDIO_ctx.buf_edit = 255;
	}

	return BSP_OK;
}



