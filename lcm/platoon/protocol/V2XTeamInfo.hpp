/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>

#ifndef __platoon_protocol_V2XTeamInfo_hpp__
#define __platoon_protocol_V2XTeamInfo_hpp__

#include <vector>

namespace platoon
{
namespace protocol
{

class V2XTeamInfo
{
    public:
        int8_t     head_vehicle;

        int64_t    team_id;

        int32_t    mem_number;

        std::vector< int64_t > mem_id;

        std::vector< int64_t > mem_status;

    public:
        /**
         * Encode a message into binary form.
         *
         * @param buf The output buffer.
         * @param offset Encoding starts at thie byte offset into @p buf.
         * @param maxlen Maximum number of bytes to write.  This should generally be
         *  equal to getEncodedSize().
         * @return The number of bytes encoded, or <0 on error.
         */
        inline int encode(void *buf, int offset, int maxlen) const;

        /**
         * Check how many bytes are required to encode this message.
         */
        inline int getEncodedSize() const;

        /**
         * Decode a message from binary form into this instance.
         *
         * @param buf The buffer containing the encoded message.
         * @param offset The byte offset into @p buf where the encoded message starts.
         * @param maxlen The maximum number of bytes to reqad while decoding.
         * @return The number of bytes decoded, or <0 if an error occured.
         */
        inline int decode(const void *buf, int offset, int maxlen);

        /**
         * Retrieve the 64-bit fingerprint identifying the structure of the message.
         * Note that the fingerprint is the same for all instances of the same
         * message type, and is a fingerprint on the message type definition, not on
         * the message contents.
         */
        inline static int64_t getHash();

        /**
         * Returns "V2XTeamInfo"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int V2XTeamInfo::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int V2XTeamInfo::decode(const void *buf, int offset, int maxlen)
{
    int pos = 0, thislen;

    int64_t msg_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &msg_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (msg_hash != getHash()) return -1;

    thislen = this->_decodeNoHash(buf, offset + pos, maxlen - pos);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int V2XTeamInfo::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t V2XTeamInfo::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* V2XTeamInfo::getTypeName()
{
    return "V2XTeamInfo";
}

int V2XTeamInfo::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = __boolean_encode_array(buf, offset + pos, maxlen - pos, &this->head_vehicle, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &this->team_id, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->mem_number, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    if(this->mem_number > 0) {
        tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &this->mem_id[0], this->mem_number);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    if(this->mem_number > 0) {
        tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &this->mem_status[0], this->mem_number);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int V2XTeamInfo::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = __boolean_decode_array(buf, offset + pos, maxlen - pos, &this->head_vehicle, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this->team_id, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->mem_number, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    if(this->mem_number) {
        this->mem_id.resize(this->mem_number);
        tlen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this->mem_id[0], this->mem_number);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    if(this->mem_number) {
        this->mem_status.resize(this->mem_number);
        tlen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this->mem_status[0], this->mem_number);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int V2XTeamInfo::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += __boolean_encoded_array_size(NULL, 1);
    enc_size += __int64_t_encoded_array_size(NULL, 1);
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += __int64_t_encoded_array_size(NULL, this->mem_number);
    enc_size += __int64_t_encoded_array_size(NULL, this->mem_number);
    return enc_size;
}

uint64_t V2XTeamInfo::_computeHash(const __lcm_hash_ptr *)
{
    uint64_t hash = 0xd1ac31a98e7a0d6cLL;
    return (hash<<1) + ((hash>>63)&1);
}

}

}

#endif
