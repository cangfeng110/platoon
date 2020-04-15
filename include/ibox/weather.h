#ifndef _WEATHER_DATA_
#define _WEATHER_DATA_

#include <stdint.h>

struct Weather
{
    //降雨量，单位mm
    double rainfall;

    //能见度， 单位km;
    double visibility;

    //最大风力等级
    unsigned int wind;

    //温度
    double temperature;

    //湿度
    double humidity;
    
    //天气状态, 不同的数字代表不同的天气，这个需要再定义
    int  weather;

} __attribute__ ((packed));
#endif
