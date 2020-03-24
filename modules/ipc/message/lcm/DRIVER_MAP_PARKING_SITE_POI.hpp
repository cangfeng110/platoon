/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <lcm/lcm_coretypes.h>

#ifndef __DRIVER_MAP_PARKING_SITE_POI_hpp__
#define __DRIVER_MAP_PARKING_SITE_POI_hpp__

#include "DRIVER_MAP_LANE_LINK_KEY.hpp"
#include "DRIVER_MAP_POINT.hpp"
#include "DRIVER_MAP_POINT.hpp"


class DRIVER_MAP_PARKING_SITE_POI
{
    public:
        int8_t     iMajorType;

        int8_t     iMinorType;

        int8_t     iPlaceType;

        int8_t     iAttachedRoadCount;

        DRIVER_MAP_LANE_LINK_KEY stAttachedRoad[4];

        int32_t    iValue;

        DRIVER_MAP_POINT stPoint2VehicleCor;

        int64_t    iSID;

        int16_t    iLength;

        int16_t    iWidth;

        int8_t     iAttribute;

        int8_t     istyle;

        int8_t     iShapeType;

        int16_t    iShapeNum;

        DRIVER_MAP_POINT stShapePoint2VehicleCor[4];

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
         * Returns "DRIVER_MAP_PARKING_SITE_POI"
         */
        inline static const char* getTypeName();

        // LCM support functions. Users should not call these
        inline int _encodeNoHash(void *buf, int offset, int maxlen) const;
        inline int _getEncodedSizeNoHash() const;
        inline int _decodeNoHash(const void *buf, int offset, int maxlen);
        inline static uint64_t _computeHash(const __lcm_hash_ptr *p);
};

int DRIVER_MAP_PARKING_SITE_POI::encode(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;
    int64_t hash = (int64_t)getHash();

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->_encodeNoHash(buf, offset + pos, maxlen - pos);
    if (tlen < 0) return tlen; else pos += tlen;

    return pos;
}

int DRIVER_MAP_PARKING_SITE_POI::decode(const void *buf, int offset, int maxlen)
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

int DRIVER_MAP_PARKING_SITE_POI::getEncodedSize() const
{
    return 8 + _getEncodedSizeNoHash();
}

int64_t DRIVER_MAP_PARKING_SITE_POI::getHash()
{
    static int64_t hash = _computeHash(NULL);
    return hash;
}

const char* DRIVER_MAP_PARKING_SITE_POI::getTypeName()
{
    return "DRIVER_MAP_PARKING_SITE_POI";
}

int DRIVER_MAP_PARKING_SITE_POI::_encodeNoHash(void *buf, int offset, int maxlen) const
{
    int pos = 0, tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->iMajorType, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->iMinorType, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->iPlaceType, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->iAttachedRoadCount, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    for (int a0 = 0; a0 < 4; a0++) {
        tlen = this->stAttachedRoad[a0]._encodeNoHash(buf, offset + pos, maxlen - pos);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    tlen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &this->iValue, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->stPoint2VehicleCor._encodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &this->iSID, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_encode_array(buf, offset + pos, maxlen - pos, &this->iLength, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_encode_array(buf, offset + pos, maxlen - pos, &this->iWidth, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->iAttribute, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->istyle, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_encode_array(buf, offset + pos, maxlen - pos, &this->iShapeType, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_encode_array(buf, offset + pos, maxlen - pos, &this->iShapeNum, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    for (int a0 = 0; a0 < 4; a0++) {
        tlen = this->stShapePoint2VehicleCor[a0]._encodeNoHash(buf, offset + pos, maxlen - pos);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int DRIVER_MAP_PARKING_SITE_POI::_decodeNoHash(const void *buf, int offset, int maxlen)
{
    int pos = 0, tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->iMajorType, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->iMinorType, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->iPlaceType, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->iAttachedRoadCount, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    for (int a0 = 0; a0 < 4; a0++) {
        tlen = this->stAttachedRoad[a0]._decodeNoHash(buf, offset + pos, maxlen - pos);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    tlen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &this->iValue, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = this->stPoint2VehicleCor._decodeNoHash(buf, offset + pos, maxlen - pos);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this->iSID, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_decode_array(buf, offset + pos, maxlen - pos, &this->iLength, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_decode_array(buf, offset + pos, maxlen - pos, &this->iWidth, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->iAttribute, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->istyle, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int8_t_decode_array(buf, offset + pos, maxlen - pos, &this->iShapeType, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    tlen = __int16_t_decode_array(buf, offset + pos, maxlen - pos, &this->iShapeNum, 1);
    if(tlen < 0) return tlen; else pos += tlen;

    for (int a0 = 0; a0 < 4; a0++) {
        tlen = this->stShapePoint2VehicleCor[a0]._decodeNoHash(buf, offset + pos, maxlen - pos);
        if(tlen < 0) return tlen; else pos += tlen;
    }

    return pos;
}

int DRIVER_MAP_PARKING_SITE_POI::_getEncodedSizeNoHash() const
{
    int enc_size = 0;
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    for (int a0 = 0; a0 < 4; a0++) {
        enc_size += this->stAttachedRoad[a0]._getEncodedSizeNoHash();
    }
    enc_size += __int32_t_encoded_array_size(NULL, 1);
    enc_size += this->stPoint2VehicleCor._getEncodedSizeNoHash();
    enc_size += __int64_t_encoded_array_size(NULL, 1);
    enc_size += __int16_t_encoded_array_size(NULL, 1);
    enc_size += __int16_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int8_t_encoded_array_size(NULL, 1);
    enc_size += __int16_t_encoded_array_size(NULL, 1);
    for (int a0 = 0; a0 < 4; a0++) {
        enc_size += this->stShapePoint2VehicleCor[a0]._getEncodedSizeNoHash();
    }
    return enc_size;
}

uint64_t DRIVER_MAP_PARKING_SITE_POI::_computeHash(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for(fp = p; fp != NULL; fp = fp->parent)
        if(fp->v == DRIVER_MAP_PARKING_SITE_POI::getHash)
            return 0;
    const __lcm_hash_ptr cp = { p, (void*)DRIVER_MAP_PARKING_SITE_POI::getHash };

    uint64_t hash = 0x83380895d2dcb467LL +
         DRIVER_MAP_LANE_LINK_KEY::_computeHash(&cp) +
         DRIVER_MAP_POINT::_computeHash(&cp) +
         DRIVER_MAP_POINT::_computeHash(&cp);

    return (hash<<1) + ((hash>>63)&1);
}

#endif
