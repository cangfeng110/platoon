#ifndef INBOUND_GNSS_DATA
#define INBOUND_GNSS_DATA

#endif // INBOUND_GNSS_DATA

typedef struct inbound_gnss_data
{
    /**
     * Range -1800000000..1800000000 (-180..180) Units: 1/10 micro degree
     * 0 shall be used when unavailable
     */
    int32_t lon;
    /**
     * Range: -900000000..900000000 (-90..90) Units: 1/10 micro degree
     * 0 shall be used when unavailable
     */
    int32_t lat;
    /** Altitude in meters units: 0.01 meter */
    int32_t                 alt;
    /** Longitude position uncertainty, units: 0.01meter */
    int32_t                 epx;
    /** Latitude position uncertainty, units: 0.01meter */
    int32_t                 epy;
    /** Track uncertainty, units: 0.01degree */
    int32_t                 epd;
    /** Seconds since the Epoch */
    uint32_t                timestamp_seconds;
    /** Miliseconds since the last Second */
    uint32_t                timestamp_miliseconds;
    /** Range: 0..3600, 3600 shall be used when unavailable. Unit: 1/10 degree */
    uint16_t                heading;
    /** 0.001 m/s */
    uint16_t                speed;
    /** acc_x:  X-axis acceleration units: 0.001m/s^2 */
    int32_t                 acc_x;
    /** acc_y:  Y-axis acceleration units: 0.001m/s^2 */
    int32_t                 acc_y;
    /** acc_z:  Z-axis acceleration units: 0.001m/s^2 */
    int32_t                 acc_z;
    /** gyro_x: pitch   units: 0.001rad/s */
    int32_t                 gyro_x;
    /** gyro_y: roll    units: 0.001rad/s */
    int32_t                 gyro_y;
    /** gyro_z: yaw     units: 0.001rad/s */
    int32_t                 gyro_z;
    /** reserved1 */
    uint32_t                rsvd1;
    /** reserved2 */
    uint32_t                rsvd2;
    /** reserved3 */
    uint32_t                rsvd3;
    /** reserved4 */
    uint32_t                rsvd4; 
    /** reserved5 */
    uint32_t                rsvd5;     
}__attribute__ ((packed))  inbound_gnss_data_t;
