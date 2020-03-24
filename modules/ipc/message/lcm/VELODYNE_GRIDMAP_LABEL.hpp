/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>

#ifndef __VELODYNE_GRIDMAP_LABEL_hpp__
#define __VELODYNE_GRIDMAP_LABEL_hpp__

#include "HEADER.hpp"


class VELODYNE_GRIDMAP_LABEL
{
    public:
        HEADER     stHeader;

        int16_t    nMaxRowNum;

        int16_t    nMaxColNum;

        int8_t     gnGridMapR[300][800];

        int8_t     gnGridMapG[300][800];

        int8_t     gnGridMapB[300][800];

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
         * Returns "VELODYNE_GRIDMAP_LABEL"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int VELODYNE_GRIDMAP_LABEL::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int VELODYNE_GRIDMAP_LABEL::decode(const void *buf, int offset, int maxlen)
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

int VELODYNE_GRIDMAP_LABEL::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t VELODYNE_GRIDMAP_LABEL::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* VELODYNE_GRIDMAP_LABEL::getTypeName()
{
    return "VELODYNE_GRIDMAP_LABEL";
}

int VELODYNE_GRIDMAP_LABEL::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = this->stHeader._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_encode_array(buf, offset + pos, maxlen - pos, &this->nMaxRowNum, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_encode_array(buf, offset + pos, maxlen - pos, &this->nMaxColNum, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    for (int a0 = 0; a0 < 300; a0++) {
        tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->gnGridMapR[a0][0], 800);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    for (int a0 = 0; a0 < 300; a0++) {
        tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->gnGridMapG[a0][0], 800);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    for (int a0 = 0; a0 < 300; a0++) {
        tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->gnGridMapB[a0][0], 800);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int VELODYNE_GRIDMAP_LABEL::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = this->stHeader._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_decode_array(buf, offset + pos, maxlen - pos, &this->nMaxRowNum, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_decode_array(buf, offset + pos, maxlen - pos, &this->nMaxColNum, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    for (int a0 = 0; a0 < 300; a0++) {
        tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->gnGridMapR[a0][0], 800);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    for (int a0 = 0; a0 < 300; a0++) {
        tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->gnGridMapG[a0][0], 800);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    for (int a0 = 0; a0 < 300; a0++) {
        tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->gnGridMapB[a0][0], 800);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int VELODYNE_GRIDMAP_LABEL::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += this->stHeader._getEncodedSizeNoHash();
    enc_size += __int16_t_encoded_array_size(NULL, 1);
    enc_size += __int16_t_encoded_array_size(NULL, 1);
    enc_size += 300 * __int8_t_encoded_array_size(NULL, 800);
    enc_size += 300 * __int8_t_encoded_array_size(NULL, 800);
    enc_size += 300 * __int8_t_encoded_array_size(NULL, 800);
    return enc_size;
}

uint64_t VELODYNE_GRIDMAP_LABEL::_computeHash(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for(fp = p; fp != NULL; fp = fp->parent)
        if(fp->v == VELODYNE_GRIDMAP_LABEL::getHash)
            return 0;
    const __lcm_hash_ptr cp = { p, (void*)VELODYNE_GRIDMAP_LABEL::getHash };

    uint64_t hash = 0x8e65cfba2ef6b8c8LL +
         HEADER::_computeHash(&cp);

    return (hash<<1) + ((hash>>63)&1);
}

#endif
