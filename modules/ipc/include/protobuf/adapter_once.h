#ifndef MODULES_IPC_ADAPTERS_ADAPTERONCE_H_
#define MODULES_IPC_ADAPTERS_ADAPTERONCE_H_

#include <string>
#include <type_traits>
#include <vector>

//#include "google/protobuf/descriptor.h"
//#include "google/protobuf/message_lite.h"

#include "modules/ipc/include/protobuf/cprotosubscriber.h"
#include "modules/ipc/conf/channel_config.pb.h"
#include "modules/common/include/util/string_util.h"

/**
 * @namespace atd::ipc::adapter
 * @brief atd::ipc::adapter
 */
namespace atd {
namespace ipc {
namespace adapter {

using atd::common::util::StrCat;

class AdapterOnceBase {
public:
    virtual ~AdapterOnceBase() = default;

    /**
   * @brief returns the topic name that this adapter listens to.
   */
    virtual const std::string& topic_name() const = 0;

};

template <class T>
class AdapterOnce : public AdapterOnceBase {
public:

    AdapterOnce(const std::string &topic_name,const ChannelConfig &config,const int fix_size)
    {
        if(config.mode() == ChannelConfig::PUBLISH_ONLY){
            Subscriber_=new CPROTOSubscriber_M<T>(topic_name);
        }
        else if(config.mode() == ChannelConfig::RECEIVE_ONLY){
            Subscriber_=new CPROTOSubscriber_M<T>(false,30,topic_name,NULL);
            data_= new T();
        }
        else if(config.mode() == ChannelConfig::RTE_RECEIVE_ONLY){
            Subscriber_=new CPROTOSubscriber_M<T>(false,30,topic_name,NULL);
            data_= new T();
        }

        topic_name_=topic_name;
        last_receive_time_=0;
        count=0;
        is_valid_= false;
        is_correct_ = false;
        config_.CopyFrom(config);
        fix_size_=fix_size;
    }

    /**
   * @brief returns the topic name that this adapter listens to.
   */
    const std::string& topic_name() const override { return topic_name_; }

    /**
   * @brief fill the data header
   */
    void FillHeader(T* data,double timestamp)
    {
        auto* header = data->mutable_header();
        //double timestamp = atd::common::time::Clock::NowInSeconds();
        header->set_module_name(topic_name_);
        header->set_timestamp_sec(timestamp);
        count=( static_cast<unsigned int> ( count ) +1 ) %65535;
        header->set_sequence_num(count);
    }

    T* GetData(){return data_;}

    CPROTOSubscriber_M<T> * GetSubscriber() { return Subscriber_;}

    int GetFixsize(){return fix_size_;}

    bool GetValid(){return is_valid_;}

    void SetValid(bool is_valid)
    {
        is_valid_=is_valid;
    }


    Status PROTO_TO_ARRAY(T* data,uint8_t inputmsg[])
    {
        data->clear_reservedmsg();
        int sizegap=Subscriber_->findsizegap(data,fix_size_);
        if(sizegap==-1)
        {
            return Status(atd::common::ErrorCode::HTTP_LOGIC_ERROR,StrCat(topic_name_," msg size is out of range"));
        }
        else if(sizegap==-2)
        {
            return Status(atd::common::ErrorCode::HTTP_LOGIC_ERROR,StrCat(topic_name_," msg size is HUGE"));
        }
        const std::string reservedstring(sizegap, 'x');
        int bw=0;
        if(sizegap>=16379 && sizegap<=16383){bw=1;}
        else if(sizegap>=128 && sizegap<=16378){bw=2;}
        else if(sizegap==126 || sizegap==127){bw=3;}
        else if(sizegap>=0 && sizegap<=125){bw=4;}
        const std::string reservedbit(bw, 'b');
        auto* tmpstr=data->mutable_reservedmsg();
        tmpstr->set_reserved_msg(reservedstring);
        tmpstr->set_reserved_bit(reservedbit);

        int cur_size = Subscriber_->getMsgSize(data);
        if(cur_size!=fix_size_)
        {
            return Status(atd::common::ErrorCode::HTTP_LOGIC_ERROR,StrCat(topic_name_," fix_size not the same as regu_size"));
        }
        Subscriber_->sendProtoMsg(data,fix_size_,inputmsg);
        return Status(atd::common::ErrorCode::OK,"none");
    }


    Status ARRAY_TO_PROTO(const uint8_t inputmsg[])
    {
        Status msg_proc=Subscriber_->getProtoMsg(inputmsg,fix_size_,data_);
        if(msg_proc.ok())
        {
            is_correct_=true;
            if(data_->header().timestamp_sec()!=last_receive_time_)
            {
                last_receive_time_=data_->header().timestamp_sec();
                is_valid_=true;
            }
            else
            {
                is_valid_=false;
            }
            return Status(atd::common::ErrorCode::OK,"none");
        }
        else
        {
            is_correct_=false;
            is_valid_=false;
            return Status(atd::common::ErrorCode::HTTP_LOGIC_ERROR,msg_proc.ToString() );
        }
    }

private:



    /// The topic name that the adapter listens to.
    std::string topic_name_;

    double last_receive_time_;

    //the message is updated
    bool is_valid_;

    //the message is translated correctly
    bool is_correct_;

    //the fix size of the send/recv array translated by protofile
    int  fix_size_;

    unsigned int count;

    //the message config
    ChannelConfig config_;

    //the proto trans class
    CPROTOSubscriber_M<T> * Subscriber_;

    T* data_;
};

}  // namespace adapter
}  // namespace ipc
}  // namespace atd

#endif  // MODULES_IPC_ADAPTERS_ADAPTERONCE_H_
