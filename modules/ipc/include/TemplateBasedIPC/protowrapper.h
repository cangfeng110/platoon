#ifndef PROTOWRAPPER_H
#define PROTOWRAPPER_H

#include <iostream>
#include <cstring>
#include "modules/common/include/status/status.h"
#include "modules/common/include/time/time.h"
#include "modules/common/include/easylogging++.h"

namespace  atd {
namespace ipc {
union uint8ArrayToInt
{
    uint8_t array[4];
    int32_t size;
};

class DecodeAndEncodeFromArray
{
public:
    DecodeAndEncodeFromArray(){}


    static inline int getMaxlen(const void *buf ,int offset=0)
    {
        uint8ArrayToInt tempObj;
        memcpy(tempObj.array,static_cast<const uint8_t*>(buf)+offset,4);
        return tempObj.size;
    }

    template<typename T>
    static inline void setMaxlen(const T& msg,void* buf,int offset=0)
    {
        uint8ArrayToInt tempObj;
        tempObj.size=msg.getEncodedSize();
        memcpy((static_cast<uint8_t*>(buf)+offset),tempObj.array,4);
    }

    template<typename T>
    static bool dumpToArray(const T& msg,void *buf, int size, int offset=0)
    {
        int msgEncodeSize=msg.getEncodedSize();
        if(msgEncodeSize+4>size)
        {
            std::cout<<"buff size is too small for "<<msg.getTypeName()<<std::endl;
            return false;
        }
        setMaxlen(msg,buf,offset);
        msg.encode(buf,4,msgEncodeSize);
        return true;
    }

    template<typename T>
    static bool dumpToArray(const T& msg,void *buf)
    {
        if(dumpToArray(msg,buf,msg.getEncodedSize()+4))
        {
            return true;
        }
        return false;
    }

    template<typename T>
    static bool decodeFromArray(T& msg, const void *buf, int offset=0)
    {
        int dataLength=getMaxlen(buf,offset);
        if(-1==msg.decode(buf,4,dataLength))
        {
            std::cout<<"errot decodeing "<< msg.getTypeName()<<"from array"<<std::endl;
            return false;
        }
        return true;
    }

    template<typename T>
    static bool fillHeader(T& data,const uint64_t& timeStamp,int& seq_num_)
    {
        auto* header = data.mutable_header();
        header->set_timestamp_sec(timeStamp);
        header->set_sequence_num((static_cast<int>(++seq_num_))%65535);
    }

    template<typename T>
    static bool fillHeader(T& data,int& seq_num)
{
    auto* header = data.mutable_header();
    double timestamp=atd::common::time::Clock::NowInMicroSeconds();
    header->set_timestamp_sec(timestamp);
    header->set_sequence_num((static_cast<int>(++seq_num))%65535);
}

};


class ProtoWrapperBase
{
public:
    ProtoWrapperBase();
    virtual ~ProtoWrapperBase()=default;
    /**
     * Encode a message into binary form.
     *
     * @param buf The output buffer.
     * @param offset Encoding starts at thie byte offset into @p buf.
     * @param maxlen Maximum number of bytes to write.  This should generally be
     *  equal to getEncodedSize().
     * @return The number of bytes encoded, or <0 on error.
     */
    virtual inline int encode(void *buf, int offset, int maxlen) const = 0;

    /**
     * Check how many bytes are required to encode this message.
     */
    virtual inline int getEncodedSize() const = 0 ;


    /**
     * Decode a message from binary form into this instance.
     *
     * @param buf The buffer containing the encoded message.
     * @param offset The byte offset into @p buf where the encoded message starts.
     * @param maxlen The maximum number of bytes to reqad while decoding.
     * @return The number of bytes decoded, or <0 if an error occured.
     */
    virtual inline int decode(const void *buf, int offset, int maxlen) = 0;


    virtual  std::string getTypeName() const {}

};


template<typename T>
class ProtoWrapper:public ProtoWrapperBase
{
public:
    ProtoWrapper():ProtoWrapperBase(),m_dataLenth(0),data(std::make_shared<T>(T{})){}
    inline int encode(void *buf, int offset, int maxlen) const override;
    inline int decode(const void *buf, int offset, int maxlen) override;
    inline int getEncodedSize() const override;
    std::string getTypeName() const override;
    inline int getMaxlen(const void *buf ,int offset=0);

    inline void setMaxlen(void* buf,int offset=0);

    bool dumpToArray(void *buf, int size, int offset=0);

    bool dumpToArray(void *buf);

    bool decodeFromArray(const void *buf, int offset=0);

    virtual atd::common::Status  InitFromXml(const std::string &channel_config_filename){}

    virtual std::string ShortDebugString(){}

    inline T getData();

    inline std::shared_ptr<T> getDataPtr()
    {
        return data;
    }

    inline void setData(std::shared_ptr<T>& data_);
private:
    std::shared_ptr<T> data;
    int m_dataLenth;
};

template<typename T>
int ProtoWrapper<T>::encode(void *buf, int offset, int maxlen) const
{
    std::string encoded_msg_by_protobuf;
    if(!data->SerializeToString(&encoded_msg_by_protobuf))
    {
        return -1;
    }
    encoded_msg_by_protobuf.copy(static_cast<char*>(buf)+offset,encoded_msg_by_protobuf.size());
    return encoded_msg_by_protobuf.size();
}

template<typename T>
int ProtoWrapper<T>::decode(const void *buf, int offset, int maxlen)
{
    if(!data)
    {
        LOG(INFO)<<"set data Null in decode";
        data=std::make_shared<T>(T{});
    }
    if(!data->ParseFromArray(static_cast<const uint8_t*>(buf)+offset,maxlen))
    {
        if(!data->ParsePartialFromArray(static_cast<const uint8_t*>(buf)+offset,maxlen))
        {
            return -1;
        }
    }
//    m_dataLenth=data.ByteSize();
    return data->ByteSize();
}

template<typename T>
int ProtoWrapper<T>::getEncodedSize() const
{
    return data->ByteSize();
}

template<typename T>
std::string ProtoWrapper<T>::getTypeName() const
{
    return data->GetTypeName();
}

template<typename T>
T ProtoWrapper<T>::getData()
{
    if(!data)
    {
        LOG(INFO)<<"set Null for the data!!!";
        data=std::make_shared<T>(T{});
    }
    return std::move(*data);
}


template<typename T>
void ProtoWrapper<T>::setData(std::shared_ptr<T> &data_)
{
//    data.reset(data_,[=](T* ){});
    data=data_;
}

template<typename T>
int ProtoWrapper<T>::getMaxlen(const void *buf, int offset)
{
    uint8ArrayToInt tempObj;
    memcpy(tempObj.array,static_cast<const uint8_t*>(buf)+offset,4);
    return tempObj.size;
}

template<typename T>
void ProtoWrapper<T>::setMaxlen(void *buf, int offset)
{
    uint8ArrayToInt tempObj;
    tempObj.size=getEncodedSize();
    memcpy((static_cast<uint8_t*>(buf)+offset),tempObj.array,4);
}

template<typename T>
bool ProtoWrapper<T>::dumpToArray(void *buf, int size,int offset)
{
    if(getEncodedSize()+4>size)
    {
        std::cout<<"buff size is too small for "<<getTypeName()<<std::endl;
        return false;
    }
    setMaxlen(buf,offset);
    encode(buf,4,getEncodedSize());
    return true;
}

template<typename T>
bool ProtoWrapper<T>::dumpToArray(void *buf)
{
    if(dumpToArray(buf,getEncodedSize()+4))
    {
        return true;
    }
    return false;
}

template<typename T>
bool ProtoWrapper<T>::decodeFromArray(const void *buf, int offset)
{
    int dataLength=getMaxlen(buf,offset);
    if(-1==decode(buf,4,dataLength))
    {
        std::cout<<"errot decodeing "<< getTypeName()<<"from array"<<std::endl;
        return false;
    }
    return true;
}


template <typename T>
class CStyleStructWrapper: public ProtoWrapperBase
{
public:
    CStyleStructWrapper();
    inline int encode(void *buf, int offset, int maxlen) const override;
    inline int decode(const void *buf, int offset, int maxlen) override;
    inline int getEncodedSize() const override;
    std::string getTypeName() const override;
//    inline int getMaxlen(const void *buf ,int offset=0);

//    inline void setMaxlen(void* buf,int offset=0);

//    bool dumpToArray(void *buf, int size, int offset=0);

//    bool dumpToArray(void *buf);

//    bool decodeFromArray(const void *buf, int offset=0);

//    virtual atd::common::Status  InitFromXml(const std::string &channel_config_filename){}

//    virtual std::string ShortDebugString(){}

    inline T getData();
    inline void setData(const T & data_);
    inline void setbuf(const uint8_t *buf, int offset=0);
private:
    T* Msg;
    std::unique_ptr<uint8_t []> m_ptrBuf;
};

template<typename T>
CStyleStructWrapper<T>::CStyleStructWrapper():m_ptrBuf(new uint8_t[sizeof(T)])
{
    Msg=reinterpret_cast<T*>(m_ptrBuf.get());
}

template<typename T>
int CStyleStructWrapper<T>::encode(void *buf, int offset, int maxlen) const
{
    std::memcpy(buf+offset,m_ptrBuf.get(),sizeof(T));
    return sizeof(T);
}

template<typename T>
int CStyleStructWrapper<T>::decode(const void *buf, int offset, int maxlen)
{
    std::memcpy(m_ptrBuf.get(),buf+offset,sizeof(T));
    return sizeof(T);
}

template<typename T>
int CStyleStructWrapper<T>::getEncodedSize() const
{
    return sizeof(T);
}

template<typename T>
std::string CStyleStructWrapper<T>::getTypeName() const
{
    return "This is a c style struct ";
}

template<typename T>
T CStyleStructWrapper<T>::getData()
{
    return *Msg;
}

template<typename T>
void CStyleStructWrapper<T>::setData(const T &data_)
{
    *Msg=data_;
}

template<typename T>
void CStyleStructWrapper<T>::setbuf(const uint8_t *buf,int offset)
{
    std::memcpy(m_ptrBuf.get(),buf+offset,sizeof(T));
}

}
}


#endif // PROTOWRAPPER_H
