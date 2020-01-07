#ifndef OUTBOUND_COMMUNICATION_HEADER
#define OUTBOUND_COMMUNICATION_HEADER

#endif // OUTBOUND_COMMUNICATION_HEADER
typedef struct outbound_communication_header
{
    /** 0xAFDD2468 */
    uint32_t proto_id;
    /** see communication op_type 4bit */
    uint8_t  op_type   :4;
    /**  current ver = 0 4 bit*/
    uint8_t  ver       :4;
    /** see  communication op_code */
    uint8_t  op_code;
    /** range 0-255 use for events */
    uint8_t  msg_priority;
    /** reserved1 */
    uint8_t  rsvd1;
    /** Seconds since the Epoch*/
    uint32_t timestamp_seconds;
    /** Miliseconds since the last Second */
    uint32_t timestamp_miliseconds;
    /** Local OBU/RSU ID */
    uint32_t local_id;
    /** reserved2 */
    uint32_t rsvd2;
    /** reserved3 */
    uint16_t op_sn;
    /** length of PDU */
    uint16_t msg_len;
    /** reserved4 */
    uint32_t rsvd3;
    /** reserved5 */
    uint32_t rsvd4;
} __attribute__ ((packed))  outbound_communication_header_t;

