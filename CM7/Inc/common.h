/*
 * common.h
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#ifndef COMMON_H_
#define COMMON_H_

enum ENUM_State0{
	STATE0_PWR_UP,				// After powering up
	STATE0_PWR_CONFIRMED,		// PWR button hold minimum 2 seconds
	STATE0_RESTARTED,			// After restarting
	STATE0_FAULT_RECOVERY,		// After restarting due to hard fault
	STATE0_INITIATED,			// Board and all peripherals initiated
	STATE0_BOOTLOADER_INIT,		// Menu button hold during start-up, entering bootloader
	STATE0_BOOTLOADER_MAIN,		// Bootloader menu
	STATE0_APPLICATION_INIT,	// Entering application lodaed into QSPI flash
	STATE0_APPLICATION_MAIN,	// Application
	STATE0_RESTARTING,			// Restart requested
	STATE0_PWR_DOWN,			// Power down requested
	STATE0_FAULT				// Fault
};

enum ENUM_State1{
	STATE1_APPS,			// Managing applications
	STATE1_INPUTS,			// Test / Configuration of keyboard and joystick
	STATE1_IMU,				// Test / Configuration of IMU
	STATE1_AUDIO,			// Test / Configuration of Audio
	STATE1_SCREEN,			// Test / Configuration of Screen
	STATE1_BT,				// Test / Configuration of BT
	STATE1_DATETIME,		// Setting Date and Time
	STATE1_POWER,			// Restart / Shut-Down console
	STATE1_INFO				// Various information about unit
};


extern __IO uint32_t state0;
extern __IO uint32_t state1;



#endif /* COMMON_H_ */
