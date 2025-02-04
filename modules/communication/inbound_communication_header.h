#ifndef INBOUND_COMMUNICATION_HEADER
#define INBOUND_COMMUNICATION_HEADER

#endif // INBOUND_COMMUNICATION_HEADER
/** inboud communication header */
typedef struct inbound_communication_header
{
    /** 0xADFF246C */
    uint32_t proto_id; 
    /** see communication op_type 4bit */
    uint8_t  op_type   :4;
    /**  current ver = 0 4 bit*/
    uint8_t  ver       :4;
    /** see  communication op_code include data, event, control */
    uint8_t  op_code; 
    /** command sequence number */
    uint16_t op_sn;
    /** reserved1 */
    uint32_t rsvd1;
    /** reserved2 */
    uint16_t rsvd2;
    /** length of PDU */
    uint16_t msg_len;
    /** reserved3 */
    uint32_t rsvd3;
    /** reserved4 */
    uint32_t rsvd4;
} __attribute__ ((packed)) inbound_communication_header_t;
