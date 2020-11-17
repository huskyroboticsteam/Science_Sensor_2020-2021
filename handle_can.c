#include "handle_can.h"
#include "CANPacket.h"
#include "CANScience.h"
#include "CANCommon.h"

void handle_CAN_packet(CANPacket *packet){
	switch(GetPacketID(packet)){
		case ID_MOTOR_UNIT_PWM_DIR_SET:
			uint8_t motor = GetScienceMotorIDFromPacket(packet);
			int16_t pwm = GetScienceMotorPWMFromPacket(packet);
			break;
		case ID_MOTOR_UNIT_SERVO_SET:
			break;
		//case
	}
}