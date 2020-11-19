#include "CANPacket.h"
#include "CANCommon.h"
#include "CANScience.h"
#include "CANMotorUnit.h"

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
	int nextByte = WritePacketIDOnly(packetToAssemble->data, ID_MOTOR_UNIT_PWM_DIR_SET);
	PackShortIntoDataMSBFirst(packetToAssemble->data, PWMSet, nextByte);
	packetToAssemble->data[nextByte + 2] = motor;
}

void AssembleScienceServoPacket(CANPacket *packetToAssemble,
	uint8_t targetGroup,
	uint8_t targetSerial,
	uint8_t servo,
	uint8_t degrees)
{
	packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetGroup, targetSerial);
	packetToAssemble->dlc = 3;
	WritePacketIDOnly(packetToAssemble->data, ID_MOTOR_UNIT_SERVO_SET);
	packetToAssemble->data[1] = servo;
	packetToAssemble->data[2] = degrees;
}

int16_t GetScienceMotorPWMFromPacket(CANPacket *packet){
	return DecodeBytesToIntMSBFirst(packet->data, 1, 2);
}

uint8_t GetScienceMotorIDFromPacket(CANPacket *packet){
	return packet->data[3];
}