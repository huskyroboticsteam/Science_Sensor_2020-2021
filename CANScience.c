#include "CANPacket.h"
#include "CANCommon.h"
#include "CANScience.h"

void AssembleScienceSensorPullPacket(CANPacket *packetToAssemble,
	uint8_t targetGroup,
	uint8_t targetSerial,
	uint8_t sensorCode)
{
	AssembleTelemetryPullPacket(packetToAssemble, targetGroup, targetSerial, sensorCode);
}

void AssembleScienceMotorControlPacket(CANPacket *packetToAssemble,
	uint8_t targetGroup,
	uint8_t targetSerial,
	uint8_t motor,
	int16_t PWMSet)
{
	packetToAssemble->id = ConstructCANID(PRIO_MOTOR_UNIT_PWM_DIR_SET, targetGroup, targetSerial);
	packetToAssemble->dlc = 4;
	int nextByte = WritePacketIDOnly(packetToAssemble->data, DLC_MOTOR_UNIT_PWM_DIR_SET);
	PackShortIntoDataMSBFirst(packetToAssemble->data, PWMSet, nextByte);
	packetToAssemble->data[nextByte + 2] = motor;
}

void GetScienceMotorPWMFromPacket(CANPacket *packet){
	
}

void GetScienceMotorIDFromPacket(CANPacket *packet){
	return packet->data[3];
}