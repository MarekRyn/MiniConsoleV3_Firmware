/*******************************************************************
 * MiniConsole V3 - Board Support Package - Resources
 *******************************************************************/

#include "BSP_Resources.h"


typedef struct {
	uint32_t	addr;
	uint32_t	size;
} ResObj_TypeDef;


typedef struct {
	ResObj_TypeDef		objects[RES_OBJECTS_COUNT];
	uint32_t			resmap[RES_OBJECTS_COUNT * 2 + 2];
	uint32_t			resAddr;
	uint32_t			resSize;
	uint32_t			resFree;
} ResCtx_TypeDef;


static ResCtx_TypeDef ResCtx = {0};


static int _compare(const void * a, const void * b) {
	if ((*(uint32_t*)a) > (*(uint32_t*)b)) return 1;
	if ((*(uint32_t*)a) < (*(uint32_t*)b)) return -1;
	return 0;
}


uint8_t BSP_Res_Init(void * resAddr, uint32_t resSize) {
	// resAddr - memory address for resource storage
	// resSize - size of memory available for resource storage [bytes]

	// Initiating variables
	ResCtx.resAddr = (uint32_t)resAddr;
	ResCtx.resSize = resSize;
	ResCtx.resFree = resSize;

	// Cleaning object list
	for (uint32_t i = 0; i < RES_OBJECTS_COUNT; i++) {
		ResCtx.objects[i].addr = 0;
		ResCtx.objects[i].size = 0;
	}

	return BSP_OK;
}


void* BSP_Res_Alloc(uint32_t objSize) {
	if ((objSize + 12) > ResCtx.resFree) return NULL;

	uint32_t i = 0;
	uint32_t rmi = 0;
	uint32_t index = 0xFFFFFFFF; // Array index for storing new object
	uint32_t alloc_start = 0;
	uint32_t alloc_end = 0;

	// Create memory map
	// Adding dummy object of size 0 and start and first address
	ResCtx.resmap[rmi++] = ResCtx.resAddr;
	ResCtx.resmap[rmi++] = ResCtx.resAddr;

	// Adding objects to memory map
	i = 0;
	while (i < RES_OBJECTS_COUNT) {
		if (ResCtx.objects[i].size) {
			ResCtx.resmap[rmi++] = ResCtx.objects[i].addr - 8; // Start address
			ResCtx.resmap[rmi++] = ResCtx.objects[i].addr + ResCtx.objects[i].size; // End address
		} else {
			if (index == 0xFFFFFFFF) index = i; 	// index contains empty slot for future object
			ResCtx.resmap[rmi++] = 0xFFFFFFFF;
			ResCtx.resmap[rmi++] = 0xFFFFFFFF;
		}
		i++;
	}

	// Sorting memory map
	qsort(ResCtx.resmap, rmi, sizeof(uint32_t), _compare);

	// Searching for free space
	i = 1;
	while (i <= (RES_OBJECTS_COUNT)) {
		alloc_start = (ResCtx.resmap[2*i-1] < 0xFFFFFFFF) ? ResCtx.resmap[2*i-1] : ResCtx.resAddr;
		alloc_end = (ResCtx.resmap[2*i] < 0xFFFFFFFF) ? ResCtx.resmap[2*i] : (ResCtx.resAddr + ResCtx.resSize);
		if ((alloc_end - alloc_start) >= (objSize + 12)) break;
		i++;
	}
	if (i > RES_OBJECTS_COUNT) return NULL;

	// Allocating required space
	if (alloc_start & 0x00000003) alloc_start = (alloc_start + 4) & 0xFFFFFFFC; // Start address must be dividing by 4
	*(uint32_t *)alloc_start = index;
	alloc_start += 4;
	*(uint32_t *)alloc_start = objSize;
	alloc_start += 4;
	ResCtx.objects[index].addr = alloc_start;
	ResCtx.objects[index].size = objSize;
	ResCtx.resFree -= (objSize + 12);

	return (void *)alloc_start;
}


uint8_t BSP_Res_Free(void * objAddr) {
	if (objAddr == NULL) return BSP_ERROR;
	uint32_t * objIndex = (uint32_t *)objAddr - 2;
	if (*objIndex > RES_OBJECTS_COUNT) return BSP_ERROR;
	if (ResCtx.objects[*objIndex].addr != (uint32_t)objAddr) return BSP_ERROR;

	ResCtx.resFree += (ResCtx.objects[*objIndex].size + 12);
	ResCtx.objects[*objIndex].addr = 0;
	ResCtx.objects[*objIndex].size = 0;

	return BSP_OK;
}


void* BSP_Res_Load(char *filename) {

	FIL	file = {0};
	FRESULT res = {0};

	// Opening file

	res = f_open(&file, filename, FA_READ);
	if (res) return NULL;

	uint32_t fsize = f_size(&file);
	uint32_t findex = 0;
	uint32_t fread = 0;

	// Allocating Memory
	void* objAddr = BSP_Res_Alloc(fsize);
	if (objAddr == NULL) return NULL;

	// Reading file into memory
	while (findex < fsize) {
		res = f_read(&file, (void *)((uint32_t)objAddr + findex), 4096, &fread);
		if (res) return NULL;
		findex += fread;
	}

	// Closing file
	f_close(&file);

	return objAddr;
}


uint32_t BSP_Res_GetSize(void * objAddr) {
	uint32_t * objSize = (uint32_t *)objAddr - 1;
	return *objSize;
}
