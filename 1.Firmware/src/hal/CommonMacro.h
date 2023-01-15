#ifndef __COMMONMACRO_H
#define __COMMONMACRO_H

/*********************************/
/*    Designed by _VIFEXTech     */
/*********************************/

//Finish  2019-03-21 v1.0 ����ע��
//Upgrade 2019-03-21 v1.1 ����__ValueCloseTo
//Upgrade 2019-05-16 v1.2 ����__ExecuteFuncWithTimeout
//Upgrade 2019-05-16 v1.3 ����__ValueStep
//Upgrade 2019-09-25 v1.4 ����__ExecuteOnce
//Upgrade 2020-01-27 v1.5 ����__SemaphoreTake
//Upgrade 2020-03-10 v1.6 ����__ValuePlus

#define __ValueMonitor(now, func) \
do{\
    static int last=(now);\
    if(last!=(now))func,last=(now);\
}while(0)
#define __EventMonitor(now, func) __ValueMonitor((now),func)//���ݾɴ���


#define __ValueCloseTo(src, dest, step) \
do{\
    if((src)<(dest))(src)+=(step);\
    else if((src)>(dest))(src)-=(step);\
}while(0)


#define __ValueStep(src, step, max) ((src)=(((step)>=0)?(((src)+(step))%(max)):(((src)+(max)+(step))%(max))))

#define __ValuePlus(src, plus, min, max)\
do{\
    int __value_temp = (src);\
    __value_temp += (plus);\
    if(__value_temp<(min))__value_temp=(max);\
    else if(__value_temp>(max))__value_temp=(min);\
    (src) = __value_temp;\
}while(0)


#define __IntervalExecute(func, time) \
do{\
    static unsigned long lasttime=0;\
    if(millis()-lasttime>=(time))func,lasttime=millis();\
}while(0)

#define __LoopExecute(func, n) for(unsigned long i=0;i<(n);i++)func

#ifndef constrain
#   define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif

#define __LimitValue(x, min, max) ((x)=constrain((x),(min),(max)))


#define __Map(x, in_min, in_max, out_min, out_max) \
    (((x)-(in_min))*((out_max)-(out_min))/((in_max)-(in_min))+(out_min))


#define __Sizeof(arr) (sizeof(arr)/sizeof(arr[0]))


#define __TypeExplain(type, data) (*((type*)(&(data))))


#define __ExecuteFuncWithTimeout(func, n, timeout, flag)\
do{\
    volatile unsigned long start=millis();\
    (flag)=false;\
    while(millis()-start<(timeout)){\
        if(func==(n)){(flag)=true;break;}\
    }\
}while(0)


#define __ExecuteOnce(func)\
do{\
    static bool isInit = false;\
    if(!isInit){func,isInit=true;}\
}while(0)

#define __SemaphoreTake(sem, func)\
do{\
    if((sem)){func,(sem)=false;}\
}while(0)

#endif
