#ifndef INBOUND_COMMUNICATION_HEADER
#define INBOUND_COMMUNICATION_HEADER

#include <stdint.h>
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

typedef struct THB_Info
{
    uint8_t THBon;
    uint8_t THBtype;
    uint16_t reserved1;
    uint32_t reseverd2;
} __attribute__ ((packed)) THB_Info_t;

typedef struct Transparent_Transmission_Data_Header
{
    uint8_t ttdID;
    uint8_t transtype;
    uint16_t reseverd1;
    uint32_t dataSN;
    uint16_t informationage;
    uint16_t secmark;
    uint16_t discardage;
    uint16_t reserved2;
    THB_Info_t THBinfo;

} __attribute__ ((packed)) Transparent_Transmission_Data_Header_t;

// typedef struct Transparent_Transmission_Data_Header
// {
//     uint8_t ttdID;
//     uint8_t reseverd;
//     uint16_t reseverd1;
//     uint32_t dataSN;
//     uint16_t informationage;
//     uint16_t secmark;
//     uint16_t discardage;
//     uint16_t maxIOsize;
//     uint32_t reserved2;
//     uint32_t reserved3;
// } __attribute__ ((packed)) Transparent_Transmission_Data_Header_t;

#endif // INBOUND_COMMUNICATION_HEADER