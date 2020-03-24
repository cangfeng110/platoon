/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>

#ifndef __DRIVER_MAP_TOTAL_hpp__
#define __DRIVER_MAP_TOTAL_hpp__

#include "HEADER.hpp"
#include "DRIVER_MAP_POI_LIST.hpp"
#include "DRIVER_MAP_LANE_LINE_PROFILE_LIST.hpp"
#include "DRIVER_MAP_LANE_SECTION_LIST.hpp"


class DRIVER_MAP_TOTAL
{
    public:
        HEADER     header;

        DRIVER_MAP_POI_LIST m_poi_list;

        DRIVER_MAP_LANE_LINE_PROFILE_LIST m_lane_line_profile_list;

        DRIVER_MAP_LANE_SECTION_LIST m_lane_section_list;

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
         * Returns "DRIVER_MAP_TOTAL"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int DRIVER_MAP_TOTAL::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int DRIVER_MAP_TOTAL::decode(const void *buf, int offset, int maxlen)
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

int DRIVER_MAP_TOTAL::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t DRIVER_MAP_TOTAL::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* DRIVER_MAP_TOTAL::getTypeName()
{
    return "DRIVER_MAP_TOTAL";
}

int DRIVER_MAP_TOTAL::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = this->header._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->m_poi_list._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->m_lane_line_profile_list._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->m_lane_section_list._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int DRIVER_MAP_TOTAL::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = this->header._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->m_poi_list._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->m_lane_line_profile_list._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->m_lane_section_list._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int DRIVER_MAP_TOTAL::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += this->header._getEncodedSizeNoHash();
    enc_size += this->m_poi_list._getEncodedSizeNoHash();
    enc_size += this->m_lane_line_profile_list._getEncodedSizeNoHash();
    enc_size += this->m_lane_section_list._getEncodedSizeNoHash();
    return enc_size;
}

uint64_t DRIVER_MAP_TOTAL::_computeHash(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for(fp = p; fp != NULL; fp = fp->parent)
        if(fp->v == DRIVER_MAP_TOTAL::getHash)
            return 0;
    const __lcm_hash_ptr cp = { p, (void*)DRIVER_MAP_TOTAL::getHash };

    uint64_t hash = 0xaacfbea0d7ebfafeLL +
         HEADER::_computeHash(&cp) +
         DRIVER_MAP_POI_LIST::_computeHash(&cp) +
         DRIVER_MAP_LANE_LINE_PROFILE_LIST::_computeHash(&cp) +
         DRIVER_MAP_LANE_SECTION_LIST::_computeHash(&cp);

    return (hash<<1) + ((hash>>63)&1);
}

#endif
