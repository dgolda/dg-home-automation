/*
 * DgNodes.h
 *
 *  Created on: 11 pa� 2015
 *      Author: Damian
 */

#ifndef NODE2_SLOTS_H_
#define NODE2_SLOTS_H_

const int NODE2_PULSE_INPUT_SLOT = 0;
const int NODE2_TOTAL_SLOT = NODE2_PULSE_INPUT_SLOT + 1;
const int NODE2_DAILY_SLOT = NODE2_TOTAL_SLOT + 2;
const int NODE2_SPEED_SLOT = NODE2_DAILY_SLOT + 2;

const int NODE2_TEMP1_SLOT = NODE2_SPEED_SLOT + 2; //2 sloty
const int NODE2_CHIP_TEMP_SLOT = NODE2_TEMP1_SLOT + 2; //2 sloty
const int NODE2_CHIP_VOLTAGE_SLOT = NODE2_CHIP_TEMP_SLOT + 2; //2 sloty

#endif /* NODE2_SLOTS_H_ */
