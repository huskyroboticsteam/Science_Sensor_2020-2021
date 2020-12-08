#include "handle_can.h"
#include <servo.h>
#include <led.h>
#include <motor.h>
#include <adc.h>
#include "CANPacket.h"
#include "CANScience.h"
#include "CANCommon.h"
#include "CANMotorUnit.h"
#include "CANSerialNumbers.h"
#include "Port.h"

void handle_telemetry_packet(CANPacket *packet){
	int32_t sensor_val = 0;
	CANPacket new_packet;
	uint8_t type;
	switch(type = DecodeTelemetryType(packet)){
		case CAN_SCIENCE_SENSOR_GAS:
			sensor_val = read_gas_sensor();
			break;
		case CAN_SCIENCE_SENSOR_UV:
			//sensor_val = read_uv_sensor();
			break;
		case CAN_SCIENCE_SENSOR_MOISTURE:
			sensor_val = mars_moisture();
			break;
		case CAN_SCIENCE_SENSOR_AIR_QUALITY:
			//sensor_val = read_aq_sensor();
			break;
	}
	AssembleTelemetryReportPacket(&new_packet, DEVICE_GROUP_MASTER, DEVICE_SERIAL_JETSON, type, sensor_val);
	SendCANPacket(&new_packet);
}

void handle_CAN_packet(CANPacket *packet){
	switch(GetPacketID(packet)){
		case ID_MOTOR_UNIT_PWM_DIR_SET: ;
			uint8_t motor = GetScienceMotorIDFromPacket(packet);
			int16_t pwm = GetScienceMotorPWMFromPacket(packet);
			set_motor_power(motor, pwm);
			break;
		case ID_MOTOR_UNIT_SERVO_SET: ;
			uint8_t servo = GetScienceServoIDFromPacket(packet);
			int8_t angle = GetScienceServoAngleFromPacket(packet);
			set_servo_position(servo, angle);
			break;
		case ID_LED_COLOR: ;
			uint8_t r = packet->data[1];
			uint8_t g = packet->data[2];
			uint8_t b = packet->data[3];
			uint8_t R[3] = {r, r, r};
			uint8_t G[3] = {g, g, b};
			uint8_t B[3] = {b, b, b};
			set_RGB(R, G, B);
			break;
		case ID_TELEMETRY_PULL:
			handle_telemetry_packet(packet);
			break;
	}
}

