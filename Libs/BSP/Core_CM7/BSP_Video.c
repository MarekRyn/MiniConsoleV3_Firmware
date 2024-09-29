/*******************************************************************
 * MiniConsole V3 - Board Support Package - Video Library
 *******************************************************************/

#include "BSP_Video.h"
#include "BSP_Common.h"
#include "BSP_Audio.h"
#include <stdlib.h>

// Private definitions
#define AVI_RIFF_ID      	0x46464952
#define AVI_AVI_ID      	0x20495641
#define AVI_LIST_ID      	0x5453494C
#define AVI_HDRL_ID      	0x6C726468
#define AVI_MOVI_ID      	0x69766F6D
#define AVI_STRL_ID      	0x6C727473

#define AVI_AVIH_ID      	0x68697661
#define AVI_STRH_ID      	0x68727473
#define AVI_STRF_ID      	0x66727473
#define AVI_STRD_ID      	0x64727473

#define AVI_VIDS_STREAM  	0x73646976
#define AVI_AUDS_STREAM   	0x73647561

#define AVI_VIDS_FLAG     	0x6463
#define AVI_AUDS_FLAG      	0x7762

#define AVI_FORMAT_MJPG   	0x47504A4D

#define  AVI_END_FILE     	0
#define  AVI_VIDEO_FRAME  	1
#define  AVI_AUDIO_FRAME  	2
#define  AVI_ERROR        	3

#define   AVI_MAKEWORD(ptr)  	(uint16_t)(((uint16_t)*((uint8_t*)(ptr))<<8)|(uint16_t)*(uint8_t*)((ptr)+1))
#define  AVI_MAKEDWORD(ptr)  	(uint32_t)(((uint16_t)*(uint8_t*)(ptr)|(((uint16_t)*(uint8_t*)(ptr+1))<<8)|\
            					(((uint16_t)*(uint8_t*)(ptr+2))<<16)|(((uint16_t)*(uint8_t*)(ptr+3))<<24)))

// Private types
typedef enum {
  AVI_OK   =0,
  AVI_RIFF_ERR,
  AVI_AVI_ERR,
  AVI_LIST_ERR,
  AVI_HDRL_ERR,
  AVI_AVIH_ERR,
  AVI_STRL_ERR,
  AVI_STRH_ERR,
  AVI_STRF_ERR,
  AVI_MOVI_ERR,
  AVI_FORMAT_ERR,
  AVI_STREAM_ERR,
}AVISTATUS;


// Private structures
typedef struct {
  uint32_t RiffID;
  uint32_t FileSize;
  uint32_t AviID;
} AVI_HEADER;

typedef struct {
  uint32_t FrameID;
  uint32_t FrameSize;
} FRAME_HEADER;

typedef struct {
  uint32_t ListID;
  uint32_t BlockSize;
  uint32_t ListType;
} LIST_HEADER;

typedef struct {
  uint32_t BlockID;
  uint32_t BlockSize;
  uint32_t SecPerFrame;
  uint32_t MaxByteSec;
  uint32_t PaddingGranularity;
  uint32_t Flags;
  uint32_t TotalFrame;
  uint32_t InitFrames;
  uint32_t Streams;
  uint32_t RefBufSize;
  uint32_t Width;
  uint32_t Height;
  uint32_t Reserved[4];
} AVIH_HEADER;


typedef struct {
  uint32_t BlockID;
  uint32_t BlockSize;
  uint32_t StreamType;
  uint32_t Handler;
  uint32_t Flags;
  uint16_t Priority;
  uint16_t Language;
  uint32_t InitFrames;
  uint32_t Scale;
  uint32_t Rate;
  uint32_t Start;
  uint32_t Length;
  uint32_t RefBufSize;
  uint32_t Quality;
  uint32_t SampleSize;
  struct {
    uint16_t  Left;
    uint16_t  Top;
    uint16_t  Right;
    uint16_t  Bottom;
  } Frame;
} STRH_HEADER;


typedef struct {
  uint32_t     BmpSize;
  uint32_t     Width;
  uint32_t     Height;
  uint16_t     Planes;
  uint16_t     BitCount;
  uint32_t     Compression;
  uint32_t     SizeImage;
  uint32_t     XpixPerMeter;
  uint32_t     YpixPerMeter;
  uint32_t     ClrUsed;
  uint32_t     ClrImportant;
} BMP_HEADER;


typedef struct {
  uint8_t  rgbBlue;
  uint8_t  rgbGreen;
  uint8_t  rgbRed;
  uint8_t  rgbReserved;
} AVIRGBQUAD;

typedef struct {
  uint32_t BlockID;
  uint32_t BlockSize;
  BMP_HEADER bmiHeader;
  AVIRGBQUAD bmColors[1];
} STRF_BMPHEADER;


typedef struct {
  uint32_t BlockID;
  uint32_t BlockSize;
  uint16_t FormatTag;
  uint16_t Channels;
  uint32_t SampleRate;
  uint32_t BaudRate;
  uint16_t BlockAlign;
  uint16_t Size;
} STRF_WAVHEADER;


typedef struct {
  uint32_t  SecPerFrame;
  uint32_t  TotalFrame;
  uint32_t  FrameRate;
  uint32_t  Width;
  uint32_t  Height;
  uint32_t  SampleRate;
  uint16_t  Channels;
  uint16_t  AudioType;
  uint16_t  StreamID;
  uint32_t  StreamSize;
  uint32_t	StreamSizePadded;
  uint8_t*  VideoFLAG;
  uint8_t*  AudioFLAG;
} AVI_INFO;

typedef struct {
  AVI_INFO  aviInfo;
  FIL		file;
  uint8_t	status;
  uint32_t  CurrentFrame;
  uint32_t  FrameSize;
  uint32_t	FrameSizePadded;
  uint8_t   *pVideoBuffer;
  uint32_t  VideoBufferSize;
  uint32_t  FileSize;
  uint32_t	Synch0;
  uint32_t 	Synch1;
  uint8_t	AudioChannel;
  uint32_t  OffsetFrame0;
  uint32_t	OffsetIdx1;
  uint32_t* pIndex;
} AVI_CONTEXT;

static AVI_CONTEXT		vidctx;
const uint8_t* AVI_VIDS_FLAG_TBL[2] = {(uint8_t*)"00dc", (uint8_t*)"01dc"};
const uint8_t* AVI_AUDS_FLAG_TBL[2] = {(uint8_t*)"00wb", (uint8_t*)"01wb"};

volatile uint32_t x = 0;

// Private functions

OPT_O2 static uint32_t _AVI_SearchID(uint8_t* buf,  uint32_t size, uint8_t *id) {
	uint32_t i;

	for(i=0; i<(size - 4); i++) {

		if (buf[i + 0] != id[0]) continue;
		if (buf[i + 1] != id[1]) continue;
		if (buf[i + 2] != id[2]) continue;
		if (buf[i + 3] != id[3]) continue;
		return i;

	}

	return 0;
}


OPT_O1 static void _AVI_GetStreamInfo(uint8_t* buf) {

	vidctx.aviInfo.StreamID = AVI_MAKEWORD (buf + 2);
	vidctx.aviInfo.StreamSize = AVI_MAKEDWORD (buf + 4);
	vidctx.aviInfo.StreamSizePadded = vidctx.aviInfo.StreamSize;
	if (vidctx.aviInfo.StreamSizePadded & 1) vidctx.aviInfo.StreamSizePadded++;

}


OPT_O2 static AVISTATUS _AVI_Init(uint8_t *buf, uint32_t size) {

	uint32_t offset;
	uint8_t *tbuf;
	AVI_HEADER *aviheader;
	LIST_HEADER *listheader;
	AVIH_HEADER *avihheader;
	STRH_HEADER *strhheader;
	STRF_BMPHEADER *bmpheader;
	STRF_WAVHEADER *wavheader;

	tbuf = buf;

	aviheader = (AVI_HEADER*)(buf);
	if (aviheader->RiffID != AVI_RIFF_ID) return AVI_RIFF_ERR;
	if (aviheader->AviID != AVI_AVI_ID) return AVI_AVI_ERR;

	buf += sizeof(AVI_HEADER);
	listheader = (LIST_HEADER*)(buf);

	if (listheader->ListID != AVI_LIST_ID) return AVI_LIST_ERR;
	if (listheader->ListType != AVI_HDRL_ID) return AVI_HDRL_ERR;

	buf += sizeof(LIST_HEADER);
	avihheader = (AVIH_HEADER*)(buf);
	if (avihheader->BlockID != AVI_AVIH_ID) return AVI_AVIH_ERR;
	vidctx.aviInfo.SecPerFrame = avihheader->SecPerFrame;
	vidctx.aviInfo.TotalFrame = avihheader->TotalFrame;
	vidctx.aviInfo.FrameRate = 1000000 / avihheader->SecPerFrame;

	buf += avihheader->BlockSize + 8;
	listheader = (LIST_HEADER*)(buf);
	if (listheader->ListID != AVI_LIST_ID) return AVI_LIST_ERR;
	if (listheader->ListType != AVI_STRL_ID) return AVI_STRL_ERR;

	strhheader = (STRH_HEADER*)(buf + 12);
	if (strhheader->BlockID != AVI_STRH_ID) return AVI_STRH_ERR;

	switch (strhheader->StreamType) {

	case AVI_VIDS_STREAM:

		if (strhheader->Handler != AVI_FORMAT_MJPG) return AVI_FORMAT_ERR;
		vidctx.aviInfo.VideoFLAG = (uint8_t*)AVI_VIDS_FLAG_TBL[0];
		vidctx.aviInfo.AudioFLAG = (uint8_t*)AVI_AUDS_FLAG_TBL[1];

		bmpheader = (STRF_BMPHEADER*)(buf + 12 + strhheader->BlockSize + 8);
		if (bmpheader->BlockID != AVI_STRF_ID) return AVI_STRF_ERR;
		vidctx.aviInfo.Width = bmpheader->bmiHeader.Width;
		vidctx.aviInfo.Height = bmpheader->bmiHeader.Height;

		buf += listheader->BlockSize + 8;
		listheader = (LIST_HEADER*)(buf);
		if (listheader->ListID != AVI_LIST_ID) {

			vidctx.aviInfo.SampleRate = 0;
			vidctx.aviInfo.Channels = 0;
			vidctx.aviInfo.AudioType = 0;

		} else {

			if (listheader->ListType != AVI_STRL_ID) return AVI_STRL_ERR;

			strhheader = (STRH_HEADER*)(buf + 12);
			if (strhheader->BlockID != AVI_STRH_ID) return AVI_STRH_ERR;
			if (strhheader->StreamType != AVI_AUDS_STREAM) return AVI_FORMAT_ERR;

			wavheader = (STRF_WAVHEADER*)(buf + 12 + strhheader->BlockSize + 8);
			if (wavheader->BlockID != AVI_STRF_ID) return AVI_STRF_ERR;
			vidctx.aviInfo.SampleRate = wavheader->SampleRate;
			vidctx.aviInfo.Channels = wavheader->Channels;
			vidctx.aviInfo.AudioType = wavheader->FormatTag;

		}

		break;

	case AVI_AUDS_STREAM:

		vidctx.aviInfo.VideoFLAG = (uint8_t*)AVI_VIDS_FLAG_TBL[1];
		vidctx.aviInfo.AudioFLAG = (uint8_t*)AVI_AUDS_FLAG_TBL[0];

		wavheader = (STRF_WAVHEADER*)(buf + 12 + strhheader->BlockSize + 8);
		if(wavheader->BlockID != AVI_STRF_ID) return AVI_STRF_ERR;
		vidctx.aviInfo.SampleRate = wavheader->SampleRate;
		vidctx.aviInfo.Channels = wavheader->Channels;
		vidctx.aviInfo.AudioType = wavheader->FormatTag;
		if ( wavheader->SampleRate && (wavheader->FormatTag != 85)) return AVI_STREAM_ERR;

		buf += listheader->BlockSize + 8;
		listheader = (LIST_HEADER*)(buf);
		if (listheader->ListID != AVI_LIST_ID) return AVI_LIST_ERR;
		if (listheader->ListType != AVI_STRL_ID) return AVI_STRL_ERR;

		strhheader = (STRH_HEADER*)(buf + 12);
		if (strhheader->BlockID != AVI_STRH_ID) return AVI_STRH_ERR;
		if (strhheader->StreamType != AVI_VIDS_STREAM) return AVI_FORMAT_ERR;

		bmpheader = (STRF_BMPHEADER*)(buf + 12 + strhheader->BlockSize + 8);
		if (bmpheader->BlockID != AVI_STRF_ID) return AVI_STRF_ERR;
		if (bmpheader->bmiHeader.Compression != AVI_FORMAT_MJPG) return AVI_FORMAT_ERR;
		vidctx.aviInfo.Width = bmpheader->bmiHeader.Width;
		vidctx.aviInfo.Height = bmpheader->bmiHeader.Height;

		break;

	}

	offset = _AVI_SearchID(tbuf, size, (uint8_t*)"movi");
	if (offset == 0) return AVI_MOVI_ERR;

	// Index offset
	vidctx.OffsetIdx1 = AVI_MAKEDWORD(tbuf + offset - 4) + offset;
	vidctx.OffsetFrame0 = offset + 4;

	return AVI_OK;
}


static AVISTATUS _AVI_Index(void) {
	uint8_t ibuf[16];
	uint32_t r;
	uint32_t size;
	uint32_t * pOffset;

	vidctx.pIndex = NULL;

	if (f_lseek(&vidctx.file, vidctx.OffsetIdx1)) return AVI_AVI_ERR;
	if (f_read(&vidctx.file, ibuf, 8, &r)) return AVI_AVI_ERR;

	if (ibuf[0] != 'i') return AVI_AVI_ERR;
	if (ibuf[1] != 'd') return AVI_AVI_ERR;
	if (ibuf[2] != 'x') return AVI_AVI_ERR;
	if (ibuf[3] != '1') return AVI_AVI_ERR;

	size = AVI_MAKEDWORD(ibuf + 4) / 16;

	if (size == 0) return AVI_AVI_ERR;

	vidctx.aviInfo.TotalFrame = 0;
	vidctx.pIndex = (uint32_t *)vidctx.pVideoBuffer;
	pOffset = vidctx.pIndex;

	while (size > 0) {
		if ((vidctx.aviInfo.TotalFrame * 4) > vidctx.VideoBufferSize) return AVI_AVI_ERR;
		if (f_read(&vidctx.file, ibuf, 16, &r)) return AVI_AVI_ERR;
		size--;
		if (ibuf[2] != 'd') continue;
		if (ibuf[3] != 'c') continue;
		*pOffset = AVI_MAKEDWORD(ibuf+8) - 4;
		pOffset++;
		vidctx.aviInfo.TotalFrame++;
	}

	vidctx.pVideoBuffer = (uint8_t *)pOffset;
	vidctx.VideoBufferSize -= (vidctx.aviInfo.TotalFrame * 4);
	vidctx.aviInfo.TotalFrame-=2; // Safety margin for fast forward

	return AVI_OK;
}


// Public functions

uint8_t BSP_Video_Init(char * filename, uint8_t * pVideoBuffer, uint32_t VideoBufferSize) {
	uint32_t r = 0;
	uint8_t buf[8];

	// Deinit previous session
	if (vidctx.aviInfo.AudioType == 85) BSP_Audio_ChannelStop(vidctx.AudioChannel);
	f_close(&vidctx.file);

	// Zero context
	memset(&vidctx, 0, sizeof(AVI_CONTEXT));

	// Open file for reading
	if (f_open(&vidctx.file, filename, FA_READ) != FR_OK) return BSP_ERROR;

	// Initiate AVI parser
	vidctx.pVideoBuffer = pVideoBuffer;
	vidctx.VideoBufferSize = VideoBufferSize;
	vidctx.FileSize = f_size(&vidctx.file);
	vidctx.CurrentFrame = 0;

	if (f_lseek(&vidctx.file, 0)) return BSP_ERROR;
	if (f_read(&vidctx.file, vidctx.pVideoBuffer, VideoBufferSize,(UINT*) &r )) return BSP_ERROR;
	if(r != VideoBufferSize) return BSP_ERROR;

	// Parsing AVI
	if (_AVI_Init(vidctx.pVideoBuffer, VideoBufferSize)) return BSP_ERROR;

	// Indexing AVI
	if (_AVI_Index()) return BSP_ERROR;

	// Going to first frame
	if (f_lseek(&vidctx.file, vidctx.OffsetFrame0)) return BSP_ERROR;
	if (f_read(&vidctx.file, buf, 8, &r)) return BSP_ERROR;

	_AVI_GetStreamInfo(buf);

	// Initiating audio
	if (vidctx.aviInfo.AudioType == 85) {
		vidctx.AudioChannel = BSP_Audio_GetFreeChannel();
		BSP_Audio_LinkSourceSMP3(vidctx.AudioChannel);
		BSP_Audio_SetChannelVolume(vidctx.AudioChannel, 100);
		BSP_Audio_ChannelPLay(vidctx.AudioChannel, 1);
	}

	return BSP_OK;
}


uint8_t BSP_Video_GetFrame(void) {

	uint32_t readSize = 0;
	uint32_t tick;

	do {

		if (!vidctx.status) return BSP_OK;

		// Reading frame
		// Get the current frame size
		vidctx.FrameSize = vidctx.aviInfo.StreamSize;
		vidctx.FrameSizePadded = vidctx.aviInfo.StreamSizePadded;

		switch (vidctx.aviInfo.StreamID) {

		case AVI_VIDS_FLAG:
			// Video Frame

			// Frame rate timing
			tick = BSP_GetTick();
			vidctx.Synch1 += ((tick - vidctx.Synch0) * 1000);
			vidctx.Synch0 = tick;
			if (vidctx.Synch1 < vidctx.aviInfo.SecPerFrame) break;

			// Read current frame + header of next frame
			if ((vidctx.FrameSizePadded + 8) > vidctx.VideoBufferSize) return BSP_ERROR;
			if (f_read(&vidctx.file, vidctx.pVideoBuffer, vidctx.FrameSizePadded + 8, (UINT*)&readSize)) return BSP_ERROR;
			if (readSize != (vidctx.FrameSizePadded + 8)) return BSP_ERROR;

			// Get info for next frame
			_AVI_GetStreamInfo(vidctx.pVideoBuffer + vidctx.FrameSizePadded);

			// Increase video frame counter. Stop if last frame.
			vidctx.CurrentFrame++;
			if (vidctx.CurrentFrame >= vidctx.aviInfo.TotalFrame) vidctx.status = 0;

			// Skip frame if size is 0;
			if (vidctx.FrameSizePadded == 0) break;

			// Skip frame if size > VideoBufferSize
			if (vidctx.FrameSizePadded > vidctx.VideoBufferSize) break;

			// Update synch variable
			vidctx.Synch1 %= vidctx.aviInfo.SecPerFrame;

			// If single frame refresh only, than update status
			if (vidctx.status == 2) vidctx.status = 0;

			// DECODING FRAME
			BSP_LCD_DecodeJPEG(vidctx.pVideoBuffer, vidctx.FrameSizePadded);

			break;

		case AVI_AUDS_FLAG:
			// Audio Frame

			// Get stream buffer address
			void * mp3frame = BSP_Audio_GetBufAddr(vidctx.AudioChannel);

			// Read current frame + header of next frame
			if ((vidctx.FrameSizePadded + 8) > vidctx.VideoBufferSize) return BSP_ERROR;
			if (f_read(&vidctx.file, mp3frame, vidctx.FrameSizePadded + 8, (UINT*)&readSize)) return BSP_ERROR;
			if (readSize != (vidctx.FrameSizePadded + 8)) return BSP_ERROR;

			// Get info for next frame
			_AVI_GetStreamInfo(mp3frame + vidctx.FrameSizePadded);

			// Confirm completion of reading frame
			BSP_Audio_BufUpdateCompleted(vidctx.AudioChannel);

			break;

		default:
			// All other frames - just read and ignore content

			// Read current frame + header of next frame
			if ((vidctx.FrameSizePadded + 8) > vidctx.VideoBufferSize) return BSP_ERROR;
			if (f_read(&vidctx.file, vidctx.pVideoBuffer, vidctx.FrameSizePadded + 8, (UINT*)&readSize)) return BSP_ERROR;
			if (readSize != (vidctx.FrameSizePadded + 8)) return BSP_ERROR;

			// Get info for next frame
			_AVI_GetStreamInfo(vidctx.pVideoBuffer + vidctx.FrameSizePadded);
		}
	} while (vidctx.aviInfo.StreamID != AVI_VIDS_FLAG);

	return BSP_OK;
}


uint8_t BSP_Video_DeInit(void) {
	if (vidctx.aviInfo.AudioType == 85) BSP_Audio_ChannelStop(vidctx.AudioChannel);
	f_close(&vidctx.file);
	return BSP_OK;
}

uint8_t BSP_Video_Seek(uint32_t frame) {
	uint8_t hdr[8];
	uint32_t r;

	// Without idx1 available, seek operation cannot be performed
	if (vidctx.pIndex == NULL) return BSP_ERROR;
	// Checking frame range
	if (frame > (vidctx.aviInfo.TotalFrame - 1)) return BSP_ERROR;

	// Jumping to selected frame
	if (f_lseek(&vidctx.file, vidctx.OffsetFrame0 + vidctx.pIndex[frame])) return BSP_ERROR;
	// Reading header;
	if (f_read(&vidctx.file, hdr, 8, &r)) return BSP_ERROR;
	// Parsing header;
	_AVI_GetStreamInfo(hdr);
	// Updating current frame number
	vidctx.CurrentFrame = frame;
	// Update status
	if (vidctx.status == 0) vidctx.status = 2;

	return BSP_OK;
}


uint8_t BSP_Video_Rev(uint32_t delta) {
	uint8_t hdr[8];
	uint32_t r;
	uint32_t frame = vidctx.CurrentFrame - delta;

	// Without idx1 available, seek operation cannot be performed
	if (vidctx.pIndex == NULL) return BSP_ERROR;
	// Checking frame range
	if (frame > (vidctx.aviInfo.TotalFrame - 1)) frame = 0;

	// Jumping to selected frame
	if (f_lseek(&vidctx.file, vidctx.OffsetFrame0 + vidctx.pIndex[frame])) return BSP_ERROR;
	// Reading header;
	if (f_read(&vidctx.file, hdr, 8, &r)) return BSP_ERROR;
	// Parsing header;
	_AVI_GetStreamInfo(hdr);
	// Updating current frame number
	vidctx.CurrentFrame = frame;
	// Update status
	if (vidctx.status == 0) vidctx.status = 2;

	return BSP_OK;
}

uint8_t BSP_Video_Fwd(uint32_t delta) {
	uint8_t hdr[8];
	uint32_t r;
	uint32_t frame = vidctx.CurrentFrame + delta;

	// Without idx1 available, seek operation cannot be performed
	if (vidctx.pIndex == NULL) return BSP_ERROR;
	// Checking frame range
	if (frame > (vidctx.aviInfo.TotalFrame - 1)) frame = vidctx.aviInfo.TotalFrame - 1;

	// Jumping to selected frame
	if (f_lseek(&vidctx.file, vidctx.OffsetFrame0 + vidctx.pIndex[frame])) return BSP_ERROR;
	// Reading header;
	if (f_read(&vidctx.file, hdr, 8, &r)) return BSP_ERROR;
	// Parsing header;
	_AVI_GetStreamInfo(hdr);
	// Updating current frame number
	vidctx.CurrentFrame = frame;
	// Update status
	if (vidctx.status == 0) vidctx.status = 2;

	return BSP_OK;
}

uint8_t BSP_Video_Play(void) {
	vidctx.status = 1;
	return BSP_OK;
}

uint8_t BSP_Video_Stop(void) {
	vidctx.status = 0;
	// Rewind to frame 0
	BSP_Video_Seek(0);
	return BSP_OK;
}

uint8_t BSP_Video_Pause(void) {
	vidctx.status = 0;
	return BSP_OK;
}

uint8_t BSP_Video_SetVolume(uint8_t vol) {
	if (vidctx.aviInfo.AudioType != 85) return BSP_ERROR;
	BSP_Audio_SetChannelVolume(vidctx.AudioChannel, vol);
	return BSP_OK;
}

uint8_t BSP_Video_DrawFrame(int16_t x, int16_t y) {
	if (vidctx.CurrentFrame == 0) return BSP_OK;
	BSP_LCD_CopyBufJPEG((uint16_t)x, (uint16_t)y);
	return BSP_OK;
}

uint8_t BSP_Video_DrawFrameC(int16_t x, int16_t y) {
	if (vidctx.CurrentFrame == 0) return BSP_OK;
	x -= BSP_STM32_JPEG_GetWidth(JPEG) >> 1;
	y -= BSP_STM32_JPEG_GetHeight(JPEG) >> 1;
	BSP_LCD_CopyBufJPEG((uint16_t)x, (uint16_t)y);
	return BSP_OK;
}

uint32_t BSP_Video_GetTotalFrames(void) {
	return vidctx.aviInfo.TotalFrame;
}

uint32_t BSP_Video_GetCurrentFrame(void) {
	return vidctx.CurrentFrame;
}

uint32_t BSP_Video_GetWidth(void) {
	return vidctx.aviInfo.Width;
}

uint32_t BSP_Video_GetHeight(void) {
	return vidctx.aviInfo.Height;
}

uint32_t BSP_Video_GetFrameRate(void) {
	return vidctx.aviInfo.FrameRate;
}
