#include "handle_can.h"
#include <servo.h>
#include <led.h>
#include <motor.h>
#include <adc.h>
#include <encoder.h>
#include "timers.h"
#include "uv.h"
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
	uint8_t target_group = GetSenderDeviceGroupCode(packet);
	uint8_t target_serial = GetSenderDeviceSerialNumber(packet);
	switch(type = DecodeTelemetryType(packet)){
		case CAN_SCIENCE_SENSOR_GAS:
			sensor_val = read_gas_sensor();
			break;
		case CAN_SCIENCE_SENSOR_UV:
			sensor_val = read_uv_sensor();
			break;
		case CAN_SCIENCE_SENSOR_MOISTURE:
			sensor_val = mars_moisture();
			break;
		case CAN_SCIENCE_SENSOR_AIR_QUALITY:
			//sensor_val = read_aq_sensor();
			break;
		case PACKET_TELEMETRY_MOTOR1_ENC:
			sensor_val = get_encoder_ticks(0);
			break;
		case PACKET_TELEMETRY_MOTOR2_ENC:
			sensor_val = get_encoder_ticks(1);
			break;
		default:
			set_LED(LED_ERR, 3);
			update_LEDS(get_mS()/40);
			set_LED(LED_ERR, 0);
	}
	AssembleTelemetryReportPacket(&new_packet, target_group, target_serial, type, sensor_val);
	if(SendCANPacket(&new_packet)){
		set_LED(LED_ERR, 3);
		update_LEDS(get_mS()/40);
		set_LED(LED_ERR, 0);
	}
}

void handle_CAN_packet(CANPacket *packet){
	switch(GetPacketID(packet)){
		case ID_MOTOR_UNIT_PWM_DIR_SET: ;
			uint8_t motor = GetScienceMotorIDFromPacket(packet);
			int16_t pwm = GetScienceMotorPWMFromPacket(packet);
			set_motor_power(motor, pwm);
			break;
		case ID_SCIENCE_SERVO_SET: ;
			uint8_t servo = GetScienceServoIDFromPacket(packet);
			uint8_t angle = GetScienceServoAngleFromPacket(packet);
			set_servo_position(servo, angle);
			break;
		case ID_LED_COLOR: ;
			uint8_t r = packet->data[1];
			uint8_t g = packet->data[2];
			uint8_t b = packet->data[3];
			uint8_t l = packet->data[4];
			set_motor_leds(0);
			set_RGB_LED(l, r, g, b);
			break;
		case ID_TELEMETRY_PULL:
			handle_telemetry_packet(packet);
			break;
		default:
			set_LED(LED_ERR, 3);
			update_LEDS(get_mS()/40);
	}
}

