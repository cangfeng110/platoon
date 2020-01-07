#ifndef REMOTE_INFO
#define REMOTE_INFO

#endif // REMOTE_INFO
typedef struct remote_info
{
    /** Remote OBU ID */
    uint32_t remote_id;
    /** 0-65536 Engineering mode 20bits*/
    uint32_t Orientation:20;
    /** Range: 0..3600, 3600 shall be used when unavailable. Unit: 1/10 degree 12bits*/
    uint32_t heading    :12;
    /** remote vehicle distance Units:0.1m */
    uint16_t distance;
    /** remote vehicle x direction distance Units:0.1m */
    int16_t  x10;
    /** remote vehicle x direction distance Units:0.1m */
    int16_t  x01;
    /** reserved1 */
    uint16_t rsvd1;
    /** Range: -1800000000..1800000000, 0 shall be used when unavailable */
    int32_t  lon;
    /** Range:-900000000..900000000, 0 shall be used when unavailable */
    int32_t  lat;
    /** remote speed, unit:0.001m/s */
    uint16_t speed;
    /** remote type, 1:Normal Vehicle, 2:Emergency Vehicle, 3:Motorcycle */
    uint8_t  type;
    /** 判断是否修证 */
    uint8_t  is_correct;
    /**
      * Range: -4096..61439.
      * Represents the geographic position above or below the reference ellipsoid(typically WGS-84).
      * The number has a resolution of 1 decimeter and represents an asymmetric range of positive and negative
      * values. Any elevation higher than +6143.9 meters is represented as +61439.
      * Any elevation lower than -409.5 meters is represented as -4095. If the sending device does not know its elevation,
      * it shall encode the Elevation data element with -4096.
      */
    int32_t elevation;
    /** vehicle width , Range: 0..1023 ,  unit:  1cm  */
    uint16_t width;
    /** vehicle length, Range: 0.. 4095 ,  unit:  1cm  */
    uint16_t length;
    /** the timestamp, unit:  seconds  */
    uint32_t  tv_sec;
    /** the rest milliseconds,  unit:  milliseconds   */
    uint32_t  tv_millisec;
    /** 0：ALL LIGHTS OFF 4：LEFTTURN SIGNAL ON 8：RIGHT TURNSIGNAL ON */
    uint8_t   lights;
    /** 0：NONE 128：Hard Braking 2048：Disabled Vehicle */
    uint16_t  events;
    /** neutral (0), — 空挡 park (1), — 驻车挡 forwardGears (2), — 前进挡
        reverseGears (3), — 倒挡 reserved1 (4), reserved2 (5), reserved3 (6), unavailable (7) — 无效或未装配. */
    uint8_t  transmisn_state;
    /** Represent X-axis acceleration. Negative values indicate
        deceleration, and possible braking action. Range:
        -2000..2001, the value 2000 shall be used for values greater than 2000, the value
        -2000 shall be used for values less than -2000, avalue of 2001 shall be used
        for Unavailable. units: 0.01m/s^2 */
    int16_t acc_x;
    /** Represent Y-axis acceleration. Lateral acceleration is the
        acceleration along the Y axis or perpendicular to the
        vehicle’s general direction of travel in parallel with
        a left-to right centerline. Range: -2000..2001, the value 2000 shall be used for
        values greater than 2000, the value -2000 shall be used for values less than
        -2000, a value of 2001 shall be used for Unavailable. units: 0.01m/s^2 */
    int16_t acc_y;
    /** A data element representing the signed vertical
        acceleration of the vehicle along the vertical axis
        in units of 0.02 G (where 9.80665 meters per second
        squared is one G, i.e., 0.02 G = 0.1962 meters per second
        squared). Range: -127..127 (-2.52 to +2.54 G), for
        ranges >= 2.54 G(+127), for ranges < = 2.52 G(-126),unavailable(-127).
    */
    int8_t  acc_z;
    /** range：-32767..32767, unavailable(32767). yaw rate，represent a vehicle’s rotation about its vertical axis within。右转为正值。*/
    int16_t yaw_rate;
    /** range：-126..127 （-189度到189度）, -189度或超出（-126），189度或超出（126），unavailable(127)。
                方向盘转角，右转为正值。
    */
    int8_t  SteeringWheel;
    /** Represent the accuracy which can be expected from a GNSS system in 1cm steps.
        Range: 0-4095, any value equal or greater than 40.94 meter(4094),unavailable(4095), no
        error(0). units: 1cm */
    uint16_t PositionAccuracy;
    /** Represent the accuracy which can be expected from a GNSS system in 1cm steps.
        Range: 0-4095, any value equal or greater than 40.94 meter(4094),unavailable(4095), no
        error(0). units: 1cm */
    uint16_t ElevationAccuracy;
    /** reserved2 */
    uint32_t rsvd2;
    /** reserved3 */
    uint32_t rsvd3;
    /** reserved4 */
    uint32_t rsvd4;
} __attribute__ ((packed)) remote_info_t;

