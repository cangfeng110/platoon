#ifndef _DATA_DEFINE_H
#define _DATA_DEFINE_H
namespace platoon
{
    #define ACCURACY_10 1E10
    #define ACCURACY_7 1E7
    #define INVALID_FLOAT 1.0E10
    #define Epslion 1e-8
    const double PI = 3.1415926;

    /**
     * the vehicle status changed as bleow:
     * first vehicle in platoon : Auto -----> Wait -------> Leader,can't to Abnormal
     * other vehicle in platoon : Auto ------> Enqueue -------> KeepQueue -------> Dequeue
     * Abnormal has the highest priority,  
     * the other vehicle in platoon can be transed to Abnormal in any status.
    */
    enum DriveMode
    {
        Manual = 0,
        Auto = 1,
        Leader = 2,
        Enqueue = 3,
        KeepQueue = 4,
        Dequeue = 5,
        Abnormal = 6,
        Notset = 7,
        LeaderWait = 8,
        CutIN = 9,
        SubLeader = 10,
    };

    enum FmsOrder
    {
        F_Invalid = 0,
        F_Leader = 1,
        F_Enqueue = 2,
        F_Dequeue = 3,
        F_DisBand = 4,
        F_Reset = 5,
    };

    enum ApplyOrder
    {
        NoApply = 0,
        BeJoiner = 1,
        BeLeader = 2,
    };

    enum ApplyResult
    {   
        NoAnser = 0,
        RefuseJoiner = 1,
        RefuseLeader = 2,
        AllowJoin = 3,
        AllowLeader = 4,
    };
}

#endif