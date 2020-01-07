#ifndef _TEMPLATE_DATA_CONTAINER_H_
#define _TEMPLATE_DATA_CONTAINER_H_

#include <map>
#include <assert.h>
#include <mutex>

const int TTL_COUNT = 5; // 5 * 200ms

//################################################
//
// template container for data with a living-time
//
//################################################

// single thread
template<typename T>
class templateDataContainer
{
public:
    templateDataContainer()
        : timeToLive(-1)
    { }

    void setData(const T &message)
    {
        _data = message;
        timeToLive = TTL_COUNT;
    }

    const T &getData() const
    {
        return _data;
    }

    void decreaseTTL()
    {
        if(timeToLive >= 0)
        {
            timeToLive --;
        }
    }

    bool isUpToDate()
    {
        return (timeToLive >= 0) ? true : false;
    }

private:

    int timeToLive;
    T _data;
};

// multi threads
template<typename T>
class templateDataContainer_Mutex
{
public:
    templateDataContainer_Mutex()
        : timeToLive(-1)
    { }

    void setData(const T &message)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _data = message;
        timeToLive = TTL_COUNT;
    }

    T getData()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _data;
    }

    void decreaseTTL()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(timeToLive >= 0)
        {
            timeToLive --;
        }
    }

    bool isUpToDate()
    {
        return (timeToLive >= 0) ? true : false;
    }

private:
    std::mutex _mutex;
    int timeToLive;

    T _data;
};

//#####################################################
//
// template container for multi data with a living-time
//
//#####################################################

// single thread
template<typename T>
class templateDataContainer_Map
{
public:
    void setData(const int key, const T &message)
    {
        _data[key].setData(message);
    }

    std::map<int, templateDataContainer<T> > &getData()
    {
        return _data;
    }

    void decreaseTTL()
    {
        typename std::map<int, templateDataContainer<T> >::iterator it =
                _data.begin();
        while(it != _data.end())
        {
            it->second.decreaseTTL();
            if(!it->second.isUpToDate())
            {
                it = _data.erase(it);
            }
            else
                it ++;
        }
    }

    bool isUpToDate()
    {
        return _data.size() > 0 ? true : false;
    }

private:
    std::map<int, templateDataContainer<T> > _data;
};

// multi threads
template<typename T>
class templateDataContainer_Map_Mutex
{
public:

    void setData(const int key, const T &message)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _data[key].setData(message);
    }

    std::map<int, templateDataContainer<T> > getData()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _data;
    }

    void decreaseTTL()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        typename std::map<int, templateDataContainer<T> >::iterator it =
                _data.begin();
        while(it != _data.end())
        {
            it->second.decreaseTTL();
            if(!it->second.isUpToDate())
            {
                it = _data.erase(it);
            }
            else
                it ++;
        }
    }

    bool isUpToDate()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _data.size() > 0 ? true : false;
    }

private:
    std::mutex _mutex;

    std::map<int, templateDataContainer<T> > _data;
};

//############################################
//
// template container for data used with lock
//
//############################################
template<typename T>
class templateDataContainer_Lock
{
public:
    templateDataContainer_Lock()
        : _lock(true)
    { }

    void setDataAndUnlock(const T &message)
    {
        _lock = false;
        _data = message;
    }

    const T &getDataAndLock()
    {
        assert(_lock == false);
        _lock = true;
        return _data;
    }

    bool isLocked()
    {
        return _lock;
    }

private:
    bool _lock;

    T _data;
};

#endif
