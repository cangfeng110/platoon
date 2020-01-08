#ifndef INBOUND_CAN_DATA_H_
#define INBOUND_CAN_DATA_H_



// #ifndef PACKED_STRUCT
// #define PACKED_STRUCT
// __attribute__ ((packed))
// #endif  // PACKED_STRUCT

#include <stdint.h>
/** inboud communication header */
// typedef struct inbound_communication_header
// {
//     /** 0xADFF246C */
//     uint32_t proto_id; 
//     /** see communication op_type 4bit */
//     uint8_t  op_type   :4;
//     /**  current ver 4 bit  ver = 5 */
//     uint8_t  ver       :4;
//     /** see  communication op_code include data, event, control */
//     uint8_t  op_code; 
//     /** command sequence number */
//     uint16_t op_sn;
//     /** reserved1 */
//     uint32_t rsvd1;
//     /** reserved2 */
//     uint16_t rsvd2;
//     /** length of PDU */
//     uint16_t msg_len;
//     /** reserved3 */
//     uint32_t rsvd3;
//     /** reserved4 */
//     uint32_t rsvd4;
// } __attribute__ ((packed)) inbound_communication_header_t;


typedef struct inbound_can_data
{

    /** 车速, unavailable(65535) 0.001 m/s */
    uint16_t                speed; 
	/** 后轮转数，unavailable(65535) */
	uint16_t                rear_wheel_tick;
    /** range:-5000..5000, 单位 0.1degree， unavailable:5001 方向盘转角，右转为正值. */
    int16_t                steering_wheel_angle;
    /** range: 0..3600, 3600 shall be used when unavailable. 单位0.1degree */
    uint16_t                heading;
    /** neutral (0), — 空挡
        park (1), — 驻车挡
        forwardGears (2), — 前进挡
        reverseGears (3), — 倒挡
        reserved1 (4),
        reserved2 (5),
        reserved3 (6),
        unavailable (7) — 无效或未装配 【事件类数据】 */
    uint8_t                 transmisn;
    /** 车灯状态，可以多种灯同时ON.
        0:ALL LIGHTS OFF 
        1:hazarlight on(车辆警示灯亮起,【汽车双跳灯闪】) 
        2:brake lighton(刹车灯) 
        4: left turn light on 
        8: right turn light on 
        16:fault light on(故障灯：发动机故障、机油灯等，提示驾驶员需要维修车辆)
        32:parking light on(停车灯)
        64:Backup Light on(倒车灯)
        128:reserved 256:reserved, 
        unavailable(65535).【事件类数据】*/
    uint16_t                light;
    /** 车辆ESP状态，0: unavailable,1: off, 2: On but notEngaged, 3: Engaged.【事件类数据】 */
    uint8_t                 ESP_status;
    /** 车辆TCS状态，0: unavailable,1: off, 2: On but notEngaged, 3: Engaged.【事件类数据】 */
    uint8_t                 TCS_status;
    /** 车辆ABS状态，0: unavailable,1: off, 2: On but notEngaged, 3: Engaged.【事件类数据】 */
    uint8_t                 ABS_status;
    /** 车辆LDW状态，0: unavailable,1: off, 2: On but notEngaged, 3: Engaged.【事件类数据】 */
    uint8_t                 LDW_status; 
    /** 车辆刹车踏板状态，0: unavailable,1: off, 2: On but notEngaged, 3: Engaged.【事件类数据】 */
    uint8_t                 brake_pedal; 
    /** 车辆刹车踏板百分比，0-100(0%-100%), 101:unavailable.【事件类数据】 */ 
    uint8_t                 pedal_status;
    /** 车辆失控状态，unavailable, 1: off, 2: On【事件类数据】 */
    uint8_t                 lose_contorl;
    /** 车辆轮胎气压状态，0:unavailable， 1: normal, 2:The vehicle has determined that at least one tire hasrun flat【事件类数据】*/
    uint8_t                 tire_pressure;
    /** reserved1 */
    uint8_t                 rsvd1;
    /** 车辆X轴方向加速度，加速为正值，unavailable(32767) 单位：0.001m/s^2 */
    int32_t                acc_x;
    /** 车辆Y轴方向加速度，加速为正值，unavailable(32767) 单位：0.001m/s^2 */
    int32_t                acc_y;
    /** 车辆Z轴方向加速度，加速为正值，unavailable(32767) 单位：0.001m/s^2 */
    int32_t                acc_z;
    /** 车辆俯仰角角加速度，加速为正值，unavailable(32767) 单位：0.001rad/s */
    int32_t                gyro_x;
    /** 车辆翻滚角角加速度，加速为正值，unavailable(32767) 单位：0.001rad/s */
    int32_t                gyro_y;
    /** 车辆偏航角角加速度，加速为正值，unavailable(32767) 单位：0.001rad/s */
    int32_t                gyro_z;
    /** reserved2 */
    uint32_t                rsvd2;
    /** reserved3 */
    uint32_t                rsvd3;
    /** reserved4 */
    uint32_t                rsvd4;
    /** reserved5 */
    uint32_t                rsvd5;
    /** reserved6 */
    uint32_t                rsvd6;
} __attribute__ ((packed)) inbound_can_data_t;

#endif  // INBOUND_CAN_DATA_H
