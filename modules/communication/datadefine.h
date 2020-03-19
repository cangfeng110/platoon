#ifndef _DATA_DEFINE_H
#define _DATA_DEFINE_H
namespace platoon
{
    #define ACCURACY_10 1E10
    #define ACCURACY_7 1E7
    #define INVALID_FLOAT 1.0E10
    #define Epslion 1e-8
    const double PI = 3.1415926;
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
    };

    enum FmsOrder
    {
        F_Invalid = 0,
        F_Leader = 1,
        F_Enqueue = 2,
        F_Dequeue = 3,
        F_DisBand = 4,
    };
}

#endif