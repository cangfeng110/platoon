#ifndef DATASTREAMCONTAINER_H
#define DATASTREAMCONTAINER_H
#include <mutex>
#include <deque>
#include <memory>
#include <vector>
#include "modules/common/include/time/time.h"
#include "modules/common/include/time/time_tool.h"
#include "modules/common/include/time/time_util.h"
//#include <iostream>
namespace atd
{
namespace ipc
{
/**
 * @brief The IecuTimeStampUnion union :Time conversion in iecu
 */
union IecuTimeStampUnion
{
    std::int32_t lowHighArray[2];
    std::int64_t timeStampValue;
};

using namespace atd::common::time;
/**
 * @brief Base class for all data streams,virtual class cannot be instantiated
 */
class  BaseData
{
public:
    BaseData() {}
    virtual ~BaseData() = default;
    /**
     * @brief setTimeStamp set a custom time
     * @param rVal
     */
    void setTimeStamp(const Timestamp& rVal);

    template<typename PrecisionDuration>
    void setTimeStamp(const int32_t &rHigh, const int32_t &rLow)
    {
        timeStampUnion.lowHighArray[0] = std::move(rLow);
        timeStampUnion.lowHighArray[1] = std::move(rHigh);
        setTimeStamp(timeStampUnion.timeStampValue);
    }

//    template<typename PrecisionDuration>
    bool setTimeStamp(const int64_t &rVal)
    {
        auto timestamp_=FromInt64<micros>(rVal);
        if(timestamp_!=timestamp)
        {
            timestamp = FromInt64<micros>(rVal);
            return true;
        }
        return false;
    }

    /**
     * @brief setTimeStamp set the current time : unit:nonosecs 1e-9 s
     */
    void setTimeStamp();

    bool isFresher(const BaseData& rVal);

    Timestamp getTimeStamp();
private:

    IecuTimeStampUnion timeStampUnion;

    Timestamp   timestamp;

    bool valid;
};


template<typename T>
class  DataStream : public BaseData
{
public:
    DataStream();
    ~DataStream() {}
    /**
     * @brief setData
     * @param rVal
     */
    void setData(const std::shared_ptr<T> rVal);

    /**
     * @brief resetData reset data to default value
     */
    void resetData();

    /**
     * @brief getData get the lVal
     * @param lVal
     * @return true get data successfully
     */
    bool getData(T& lVal);

    std::shared_ptr<T> getData()
    {
        return data;
    }

    void setDataValid(bool Valid);

    bool IsValid();
    /**
     * @brief data template type defined data
     */

private:

    std::shared_ptr<T> data;
    /**
     * @brief dataValid
     */
    bool dataValid;
};


template<typename T>
void DataStream<T>::setData(const std::shared_ptr<T> rVal)
{
//    data.reset(rVal.get());
    data = rVal;
}


template<typename T>
bool DataStream<T>::getData(T &lVal)
{
    if(dataValid)
    {
        lVal = std::move(*data);
        return true;
    }
    else
    {
        return false;
    }
}

//template<typename T>
//std::shared_ptr<T> DataStream::getData()
//{
//    if(dataValid)
//    {
//        return data;
//    }
//    else
//    {
//        return nullptr;
//    }
//}



template <typename T>
/**
 * @brief Serialized storage data stream
 */
class DataStreamContainer
{
public:
    DataStreamContainer();
    ~DataStreamContainer() {}
    /**
     * @brief Set the amount of data stored in the queue
     * @param n  if n==1 Ensure that the queue is always up to date
     */
    void setSequenceNum(int n);

    /**
     * @brief update
     * @param newArrivalData stuff new arrival data into the queue
     * @return true if newArrivalData is new than any other data in the queue
     */
    bool update(const DataStream<T>& newArrivalData);
    /**
     * @brief insertionData insertion one data that may not be the newest data
     * @param data
     */
    bool insertData(const DataStream<T>& data);

    /**
     * @brief clearAll clear all the Data in dataQueue;
     */
    void clearAll();

    /**
     * @brief getData get data before theTime
     * @param out  put the data in the vector the newest in the front
     * @param theTime
     * @return
     */
    bool getData(std::vector<DataStream<T>>& out, Timestamp theTime);

    /**
     * @brief getOldest get the oldest data;
     * @param out
     * @return
     */
    bool getOldest(DataStream<T>& out);

    /**
     * @brief getNewest get the
     * @param out
     * @return
     */
    bool getNewest(DataStream<T>& out);

    int sequenceNum;
private:
    std::mutex m_mutex;
    std::deque<DataStream<T>> dataQueue;
    std::shared_ptr<DataStream<T>> freshData;
    void pushBack(const DataStream<T>& data);
};

template<typename T>
bool DataStreamContainer<T>::getNewest(DataStream<T> &out)
{
    if(dataQueue.size() > 0)
    {
        out = std::move(dataQueue.back());
        return true;
    }
    return false;
}

template<typename T>
bool DataStreamContainer<T>::getOldest(DataStream<T> &out)
{
    if(dataQueue.size() > 0)
    {
        out = std::move(dataQueue.front());
        return true;
    }
    return false;
}

template<typename T>
bool DataStreamContainer<T>::getData(std::vector<DataStream<T> > &out, Timestamp theTime)
{
    DataStream<T> temp;
    temp.setTimeStamp(theTime);
    for(auto iter = dataQueue.begin(); iter != dataQueue.end(); ++iter)
    {
        if(iter->isFresher(temp))
        {
            for ( ; iter != dataQueue.end(); ++iter)
            {
                auto tempObj = std::move(*iter);
                out.emplace_back(tempObj);
            }
            break;
            return true;
        }
    }
    return false;
}


template<typename T>
bool DataStreamContainer<T>::update(const DataStream<T> &newArrivalData)
{
    insertData(newArrivalData);
}

template<typename T>
bool DataStreamContainer<T>::insertData(const DataStream<T> &data)
{
    for(auto it = dataQueue.begin(); it != dataQueue.end(); it++)
    {
        if(!it->isFresher(data)) //newArrivalData is fresher
        {
            dataQueue.insert(it, data);
            while(dataQueue.size() > sequenceNum)
            {
                dataQueue.pop_front();
            }
            return true;
        }
    }
    dataQueue.emplace_front(data);
    while(dataQueue.size() > sequenceNum)
    {
        dataQueue.pop_front();
    }
    return false;
}


template<typename T>
void DataStreamContainer<T>::pushBack(const DataStream<T> &data)
{
    dataQueue.emplace_back(data);
    while (dataQueue.size() > sequenceNum)
    {
        dataQueue.pop_front();
    }
}

template<typename T>
DataStreamContainer<T>::DataStreamContainer()
    : freshData(nullptr), sequenceNum(1)
{

}
template<typename T>
void DataStreamContainer<T>::setSequenceNum(int n)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    sequenceNum = n;
}
template<typename T>
void DataStreamContainer<T>::clearAll()
{
    dataQueue.clear();
}

template<typename T>
DataStream<T>::DataStream(): BaseData(), dataValid(false),data(nullptr)
{
    resetData();
}

template<typename T>
void DataStream<T>::resetData()
{
    data = nullptr;
    dataValid = false;
}

template<typename T>
void DataStream<T>::setDataValid(bool Valid)
{
    dataValid = Valid;
}

template<typename T>
bool DataStream<T>::IsValid()
{
    return dataValid;
}





}
}
#endif // DATASTREAMCONTAINER_H
