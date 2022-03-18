#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "Motorctl.h"
#include "Wallkey.h"
#include "Json_parse.h"
#include "Http.h"
#include "Led.h"

#define MOTOR_CTL1   26
#define MOTOR_CTL2   25

//#define GJ5620              //big motor
#define GJ5606K             //
//define GJ5606K_exb           //zhan jia 


#ifdef GJ5620

#define ALLDOWNTIME         62*1000
#define ALLUPTIME           62*1000

#define ANGLE_N0TO0         50
#define ANGLE_0TO10         154
#define ANGLE_10TO20        65
#define ANGLE_20TO30        50
#define ANGLE_30TO40        42
#define ANGLE_40TO50        37
#define ANGLE_50TO60        34
#define ANGLE_60TO70        31
#define ANGLE_70TO80        200//29


#define ANGLE_0TON0         50
#define ANGLE_10TO0         82
#define ANGLE_20TO10        73
#define ANGLE_30TO20        66
#define ANGLE_40TO30        61
#define ANGLE_50TO40        57
#define ANGLE_60TO50        54
#define ANGLE_70TO60        50
#define ANGLE_80TO70        350
#define ANGLE_80TO60        400
#define ANGLE_80TO50        454
#define ANGLE_80TO40        511
#define ANGLE_80TO30        572
#define ANGLE_80TO20        638
#define ANGLE_80TO10        711
#define ANGLE_80TO0         850//793


//下降
//#define HEIGHT_TOTAL     58000
#define HEIGHT_100TO90     4740
#define HEIGHT_90TO80      4930
#define HEIGHT_80TO70      5109
#define HEIGHT_70TO60      5310
#define HEIGHT_60TO50      5536
#define HEIGHT_50TO40      5795
#define HEIGHT_40TO30      6092
#define HEIGHT_30TO20      6441
#define HEIGHT_20TO10      6858
#define HEIGHT_10TO0       8000  //7189

//上升
#define HEIGHT_0TO10       8696
#define HEIGHT_10TO20      6371
#define HEIGHT_20TO30      6049
#define HEIGHT_30TO40      5771
#define HEIGHT_40TO50      5528
#define HEIGHT_50TO60      5314
#define HEIGHT_60TO70      5123
#define HEIGHT_70TO80      4950
#define HEIGHT_80TO90      4795
#define HEIGHT_90TO100     4900 //4702  

#endif


#ifdef GJ5606K
#define ALLDOWNTIME         43*1000
#define ALLUPTIME           43*1000

#define ANGLE_N0TO0         70
#define ANGLE_0TO10         139
#define ANGLE_10TO20        101
#define ANGLE_20TO30        85
#define ANGLE_30TO40        76
#define ANGLE_40TO50        70
#define ANGLE_50TO60        66
#define ANGLE_60TO70        80
#define ANGLE_70TO80        500//29

#define ANGLE_0TO30         325
#define ANGLE_30TO50        146
#define ANGLE_50TO80        200


#define ANGLE_0TON0         50
#define ANGLE_10TO0         75
#define ANGLE_20TO10        75
#define ANGLE_30TO20        75
#define ANGLE_40TO30        75
#define ANGLE_50TO40        75
#define ANGLE_60TO50        75
#define ANGLE_70TO60        75

#define ANGLE_80TO70        300
#define ANGLE_80TO60        375
#define ANGLE_80TO50        450
#define ANGLE_80TO40        525
#define ANGLE_80TO30        600
#define ANGLE_80TO20        675
#define ANGLE_80TO10        750
#define ANGLE_80TO0         1000//950


#define ANGLE_30TO0         225
#define ANGLE_50TO30        150


//下降
//#define HEIGHT_TOTAL    58000

// #define HEIGHT_100TO90     3269
// #define HEIGHT_90TO80      3400
// #define HEIGHT_80TO70      3523
// #define HEIGHT_70TO60      3662
// #define HEIGHT_60TO50      3818
// #define HEIGHT_50TO40      3996
// #define HEIGHT_40TO30      4201
// #define HEIGHT_30TO20      4442
// #define HEIGHT_20TO10      4730
// #define HEIGHT_10TO0       6000  //4958

#define HEIGHT_100TO90     4100
#define HEIGHT_90TO80      4100
#define HEIGHT_80TO70      4100
#define HEIGHT_70TO60      4100
#define HEIGHT_60TO50      4100
#define HEIGHT_50TO40      4100
#define HEIGHT_40TO30      4100
#define HEIGHT_30TO20      4100
#define HEIGHT_20TO10      4100
#define HEIGHT_10TO0       6000  //4958

#define HEIGHT_100TO0      43000

//上升
// #define HEIGHT_0TO10       4843
// #define HEIGHT_10TO20      4608
// #define HEIGHT_20TO30      4375
// #define HEIGHT_30TO40      4175
// #define HEIGHT_40TO50      3999
// #define HEIGHT_50TO60      3844
// #define HEIGHT_60TO70      3706
// #define HEIGHT_70TO80      3581
// #define HEIGHT_80TO90      3468
// #define HEIGHT_90TO100     4000  //3401

#define HEIGHT_0TO10       4100
#define HEIGHT_10TO20      4100
#define HEIGHT_20TO30      4100
#define HEIGHT_30TO40      4100
#define HEIGHT_40TO50      4100
#define HEIGHT_50TO60      4100
#define HEIGHT_60TO70      4100
#define HEIGHT_70TO80      4100
#define HEIGHT_80TO90      4100
#define HEIGHT_90TO100     6000  //3401

#define HEIGHT_0TO100      43000

// #define HEIGHT_100TO90    2252
// #define HEIGHT_90TO80      2334
// #define HEIGHT_80TO70      2412
// #define HEIGHT_70TO60      2499
// #define HEIGHT_60TO50      2597
// #define HEIGHT_50TO40      2709
// #define HEIGHT_40TO30      2837
// #define HEIGHT_30TO20      2989
// #define HEIGHT_20TO10      3170
// #define HEIGHT_10TO0        4000  //4958

// //上升
// #define HEIGHT_0TO10       3240
// #define HEIGHT_10TO20      3093
// #define HEIGHT_20TO30      2947
// #define HEIGHT_30TO40      2821
// #define HEIGHT_40TO50      2711
// #define HEIGHT_50TO60      2613
// #define HEIGHT_60TO70      2527
// #define HEIGHT_70TO80      2448
// #define HEIGHT_80TO90      2377
// #define HEIGHT_90TO100     3000  //3401

#endif

#ifdef GJ5606K_exb
#define ALLDOWNTIME         5*1000
#define ALLUPTIME           5*1000

#define ANGLE_N0TO0         70
#define ANGLE_0TO10         139
#define ANGLE_10TO20        101
#define ANGLE_20TO30        85
#define ANGLE_30TO40        76
#define ANGLE_40TO50        70
#define ANGLE_50TO60        66
#define ANGLE_60TO70        80
#define ANGLE_70TO80        500

#define ANGLE_0TO30         325
#define ANGLE_30TO50        146
#define ANGLE_50TO80        646


#define ANGLE_0TON0         50
#define ANGLE_10TO0         75
#define ANGLE_20TO10        75
#define ANGLE_30TO20        75
#define ANGLE_40TO30        75
#define ANGLE_50TO40        75
#define ANGLE_60TO50        75
#define ANGLE_70TO60        75
#define ANGLE_80TO70        300

#define ANGLE_80TO60        375
#define ANGLE_80TO50        450
#define ANGLE_80TO40        525
#define ANGLE_80TO30        600
#define ANGLE_80TO20        675
#define ANGLE_80TO10        750
#define ANGLE_80TO0         1000//950


#define ANGLE_30TO0         225
#define ANGLE_50TO30        225


//下降
//#define HEIGHT_TOTAL    58000

#define HEIGHT_100TO90     2600
#define HEIGHT_90TO80      2600
#define HEIGHT_80TO70      2600
#define HEIGHT_70TO60      2600
#define HEIGHT_60TO50      2600
#define HEIGHT_50TO40      2600
#define HEIGHT_40TO30      2600
#define HEIGHT_30TO20      2600
#define HEIGHT_20TO10      2600
#define HEIGHT_10TO0       4000  

#define HEIGHT_100TO0      23800

//上升
#define HEIGHT_0TO10       2600
#define HEIGHT_10TO20      2600
#define HEIGHT_20TO30      2600
#define HEIGHT_30TO40      2600
#define HEIGHT_40TO50      2600
#define HEIGHT_50TO60      2600
#define HEIGHT_60TO70      2600
#define HEIGHT_70TO80      2600
#define HEIGHT_80TO90      2600
#define HEIGHT_90TO100     4000  //3401

#define HEIGHT_0TO100      23800


#endif

#define MOTORSTATUS_RUN      0X01
#define MOTORSTATUS_STOP     0X02

#define KEYCTLANGLETIME      1000

struct Motor_Value_Struct
{
    int8_t height;
    int8_t angle;
};


struct Motor_Value_Struct Motor_Value;

uint8_t Motor_Status=MOTORSTATUS_STOP;

void timer_once_cb(void *arg);//函数声明
esp_timer_handle_t timer_once_handle = 0;//定义单次定时器句柄

//定义一个单次运行的定时器结构体
esp_timer_create_args_t timer_once_arg = { .callback = &timer_once_cb, //设置回调函数
        .arg = NULL, //不携带参数
        .name = "OnceTimer" //定时器名字
        };


void timer_once_cb(void *arg) 
{
    Motor_Status=MOTORSTATUS_STOP;
    //int64_t tick = esp_timer_get_time();
    //printf("方法回调名字: %s , 距离定时器开启时间间隔 = %lld \r\n", __func__, tick);
}

void Motor_Up(void)
{
    gpio_set_level(MOTOR_CTL1, 1);
    gpio_set_level(MOTOR_CTL2, 1);//上升

}


void Motor_Down(void)
{
    gpio_set_level(MOTOR_CTL1, 0);
    gpio_set_level(MOTOR_CTL2, 1);//下降    
    
}


void Motor_Stop(void)
{
    gpio_set_level(MOTOR_CTL1, 0);
    gpio_set_level(MOTOR_CTL2, 0);//停止   
}

static void Motor_Height_sub1(void)//高度-1（范围0-100）
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, (HEIGHT_100TO0/100) * 1000); //HEIGHT_100TO0定义为全程运行时间，除以100分为100份，每一份运行一次
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=Motor_Value.height-1;
    printf("sub1_height=%d\n",Motor_Value.height);
}

static void Motor_Height_100to90(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_100TO90 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=90;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_90to80(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_90TO80 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=80;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_80to70(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_80TO70 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=70;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_70to60(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_70TO60 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=60;
    printf("height=%d\n",Motor_Value.height);
}


static void Motor_Height_60to50(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_60TO50 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=50;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_50to40(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_50TO40 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=40;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_40to30(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_40TO30 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=30;
    printf("height=%d\n",Motor_Value.height);
}


static void Motor_Height_30to20(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_30TO20 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=20;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_20to10(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_20TO10 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=10;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_10to0(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, HEIGHT_10TO0 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=0;
    Motor_Value.angle=0;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_add1(void)  //HEIGHT_0TO100定义为全程运行时间，除以100分为100份，每一份运行一次
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, (HEIGHT_0TO100/100) * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=Motor_Value.height+1;
    printf("add1_height=%d\n",Motor_Value.height);
}

static void Motor_Height_0to10(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_0TO10 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=10;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_10to20(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_10TO20 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=20;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_20to30(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_20TO30 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=30;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_30to40(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_30TO40 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=40;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_40to50(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_40TO50 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=50;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_50to60(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_50TO60 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=60;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_60to70(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_60TO70 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=70;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_70to80(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_70TO80 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=80;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Height_80to90(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_80TO90 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=90;
    printf("height=%d\n",Motor_Value.height);
}


static void Motor_Height_90to100(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, HEIGHT_90TO100 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.height=100;
    printf("height=%d\n",Motor_Value.height);
}

static void Motor_Angle_80to60(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_80TO60 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=60;
    printf("angle=%d\n",Motor_Value.angle);
}



static void Motor_Angle_80to40(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_80TO40 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=40;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_80to30(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_80TO30 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=30;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_80to20(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_80TO20 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=20;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_80to10(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_80TO10 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=10;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_80to0(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_80TO0 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=0;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_80to70(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_80TO70 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=70;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_80to50(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_80TO50 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=50;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_70to60(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_70TO60 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=60;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_60to50(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_60TO50 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=50;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_50to40(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_50TO40 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=40;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_50to30(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_50TO30 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=30;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_40to30(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_40TO30 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=30;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_30to20(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_30TO20 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=20;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_30to0(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_30TO0 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=0;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_20to10(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_20TO10 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=10;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_10to0(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_10TO0 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=0;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_0toN0(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Up();
    esp_timer_start_once(timer_once_handle, ANGLE_0TON0 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=-10;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_N0to0(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_N0TO0 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=0;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_0to10(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_0TO10 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=10;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_0to30(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_0TO30 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=30;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_10to20(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_10TO20 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=20;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_20to30(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_20TO30 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=30;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_30to40(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_30TO40 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=40;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_30to50(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_30TO50 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=50;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_40to50(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_40TO50 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=50;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_50to60(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_50TO60 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=60;
    printf("angle=%d\n",Motor_Value.angle);
}

static void Motor_Angle_50to80(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_50TO80 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=80;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_60to70(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_60TO70 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=70;
    printf("angle=%d\n",Motor_Value.angle);
}


static void Motor_Angle_70to80(void)
{
    Motor_Status=MOTORSTATUS_RUN;
    Motor_Down();
    esp_timer_start_once(timer_once_handle, ANGLE_70TO80 * 1000); //启动单次定时器
    while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
    Motor_Stop();       
    Motor_Value.angle=80;
    printf("angle=%d\n",Motor_Value.angle);
}



void Motor_Init(void)
{
    gpio_config_t io_conf;

    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO16
    io_conf.pin_bit_mask = (1<<MOTOR_CTL1);
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 1;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    io_conf.pin_bit_mask = (1<<MOTOR_CTL2);
    gpio_config(&io_conf);

    Motor_Value.height=-1;
    Motor_Value.angle=-1;


    esp_err_t err  = esp_timer_create(&timer_once_arg, &timer_once_handle);
    if(err != ESP_OK)
    {
        printf("create once timer errcode:%d\n", err);
    }
}


int Motor_SetAllDown(void)
{
    if(Motor_Status!=MOTORSTATUS_RUN)
    {
        printf("set down start,height=%d,angle=%d\n",Motor_Value.height,Motor_Value.angle);
        Motor_Value.height=100;
        Motor_Value.angle=80;
        Motor_Status=MOTORSTATUS_RUN;
        Motor_Down();
        esp_timer_start_once(timer_once_handle, ALLDOWNTIME * 1000); //启动单次定时器
        while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
        Motor_Stop();       
        Motor_Value.height=100;
        Motor_Value.angle=80;
        printf("set down over,height=%d,angle=%d\n",Motor_Value.height,Motor_Value.angle);
        return MOTOROK;
    }
    else
    {
        return MOTORERR;
    }
}


int Motor_SetAllUp(void)
{
    if(Motor_Status!=MOTORSTATUS_RUN)
    {
        printf("set up start,now height=%d,angle=%d\n",Motor_Value.height,Motor_Value.angle);
        Motor_Value.height=0;
        Motor_Value.angle=0;
        Motor_Status=MOTORSTATUS_RUN;
        Motor_Up();
        esp_timer_start_once(timer_once_handle, ALLUPTIME * 1000); //启动单次定
        while(Motor_Status==MOTORSTATUS_RUN){vTaskDelay(10 / portTICK_RATE_MS);}
        Motor_Stop();       
        Motor_Value.height=0;
        Motor_Value.angle=0;
        printf("set up over,height=%d,angle=%d\n",Motor_Value.height,Motor_Value.angle);
        return MOTOROK;
    }
    else
    {
        return MOTORERR;
    }
}

void Motor_Value_Query(int8_t* height,int8_t* angle)
{
    *height=Motor_Value.height;
    *angle=Motor_Value.angle;
}


int Motor_HandCtl_Angle(uint8_t change)
{
    if(change==ADD)
    {
        if(Motor_Status!=MOTORSTATUS_RUN)
        {
            switch(Motor_Value.angle)
            {
                case 80:
                    return MOTORERR;
                    break;
                case 50:
                    Motor_Angle_50to80();
                    break;
                case 30:
                    Motor_Angle_30to50();
                    break;
                case 0:
                    Motor_Angle_0to30();
                    break;
                case -10:
                    Motor_Angle_N0to0();
                default:
                    return MOTORERR;
                    break;

            }
            return MOTOROK;
        }
        else
        {
            return MOTORERR;
        }

    }
    else if(change==SUB)
    {
        if(Motor_Status!=MOTORSTATUS_RUN)
        {
            switch(Motor_Value.angle)
            {
                case 0:
                    return MOTORERR;
                    break;
                case 30:
                    Motor_Angle_30to0();
                    break;
                case 50:
                    Motor_Angle_50to30();
                    break;
                case 80:
                    Motor_Angle_80to50();
                    break;
                default:
                    return MOTORERR;
                    break;

            }
            return MOTOROK;
        }
        else
        {
            return MOTORERR;
        }
    }
    else
    {
        return MOTORERR;
    }
}



int Motor_HandCtl_Height(uint8_t change)
{
    if(change==SUB)//上升
    {
        if(Motor_Status!=MOTORSTATUS_RUN)
        {
            switch(Motor_Value.angle)//先将角度清到-10度
            {
                case 80:
                    Motor_Angle_80to70();
                    Motor_Angle_70to60();
                    Motor_Angle_60to50();
                    Motor_Angle_50to40();
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 70:
                    Motor_Angle_70to60();
                    Motor_Angle_60to50();
                    Motor_Angle_50to40();
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 60:
                    Motor_Angle_60to50();
                    Motor_Angle_50to40();
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 50:
                    Motor_Angle_50to40();
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 40:
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 30:
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 20:
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 10:
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 0:
                    Motor_Angle_0toN0();
                    break;
                case -10:
                    return MOTORERR;
                    break;
                default:
                    break;
            }
            switch(Motor_Value.height)//再调整高度
            {
                case 100:
                    Motor_Height_100to90();
                    Motor_Angle_N0to0();    //角度变为0
                    break;
                case 90:
                    Motor_Height_90to80();
                    Motor_Angle_N0to0();  
                    break;
                case 80:
                    Motor_Height_80to70();
                    Motor_Angle_N0to0();  
                    break;
                case 70:
                    Motor_Height_70to60();
                    Motor_Angle_N0to0();  
                    break;
                case 60:
                    Motor_Height_60to50();
                    Motor_Angle_N0to0();  
                    break;
                case 50:
                    Motor_Height_50to40();
                    Motor_Angle_N0to0();  
                    break;
                case 40:
                    Motor_Height_40to30();
                    Motor_Angle_N0to0();  
                    break;
                case 30:
                    Motor_Height_30to20();
                    Motor_Angle_N0to0();  
                    break;
                case 20:
                    Motor_Height_20to10();
                    Motor_Angle_N0to0();  
                    break;
                case 10:
                    Motor_Height_10to0();
                    break;
                case 0:
                    return MOTORERR;
                    break;
                default:
                    return MOTORERR;
                    break;
            }
            return MOTOROK;
        }
        else
        {
            return MOTORERR;
        }

    }
    else if(change==ADD)//下降
    {
        if(Motor_Status!=MOTORSTATUS_RUN)
        {
            if(Motor_Value.height!=0)//高度为0时，不转动角度，直接下降
            {
                switch(Motor_Value.angle)//先将角度变为80
                {
                    case -10:
                        Motor_Angle_N0to0();               
                        Motor_Angle_0to10();  
                        Motor_Angle_10to20();  
                        Motor_Angle_20to30();  
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80();                       
                    case 0:
                        Motor_Angle_0to10();  
                        Motor_Angle_10to20();  
                        Motor_Angle_20to30();  
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 10:
                        Motor_Angle_10to20();  
                        Motor_Angle_20to30();  
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 20:
                        Motor_Angle_20to30();  
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 30:
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 40:
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 50:
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 60:
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 70:
                        Motor_Angle_70to80(); 
                        break;
                    case 80:
                        break;
                    default:
                        break;
                }
            }
            switch(Motor_Value.height)//再调整高度
            {
                case 0:
                    Motor_Height_0to10(); 
                    Motor_Value.angle=80;
                    break;
                case 10:
                    Motor_Height_10to20(); 
                    break;
                case 20:
                    Motor_Height_20to30(); 
                    break;
                case 30:
                    Motor_Height_30to40(); 
                    break;
                case 40:
                    Motor_Height_40to50(); 
                    break;
                case 50:
                    Motor_Height_50to60(); 
                    break;
                case 60:
                    Motor_Height_60to70(); 
                    break;
                case 70:
                    Motor_Height_70to80(); 
                    break;
                case 80:
                    Motor_Height_80to90(); 
                    break;
                case 90:
                    Motor_Height_90to100(); 
                    break;
                case 100:
                    return MOTORERR;
                    break;
                default:
                    return MOTORERR;
                    break;

            }
            return MOTOROK;
        }
        else
        {
            return MOTORERR;
        }
    }
    else
    {
        return MOTORERR;
    }
}




static int Motor_KeyCtl_Height(uint8_t change)
{
    if(change==SUB)//上升
    {
        if(Motor_Status!=MOTORSTATUS_RUN)
        {
            switch(Motor_Value.angle)//先将角度清到-10度
            {
                case 80:
                    Motor_Angle_80to70();
                    Motor_Angle_70to60();
                    Motor_Angle_60to50();
                    Motor_Angle_50to40();
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 70:
                    Motor_Angle_70to60();
                    Motor_Angle_60to50();
                    Motor_Angle_50to40();
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 60:
                    Motor_Angle_60to50();
                    Motor_Angle_50to40();
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 50:
                    Motor_Angle_50to40();
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 40:
                    Motor_Angle_40to30();
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 30:
                    Motor_Angle_30to20();
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 20:
                    Motor_Angle_20to10();
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 10:
                    Motor_Angle_10to0();
                    Motor_Angle_0toN0();
                    break;
                case 0:
                    Motor_Angle_0toN0();
                    break;
                case -10:
                    //return MOTORERR;
                    break;
                default:
                    break;
            }
            //再调整高度
            if(Motor_Value.height == 0)
            {
                return MOTORERR;
            }
            else
            {
                Motor_Height_sub1();
                return MOTOROK;
            }
        }
        else
        {
            return MOTORERR;
        }

    }
    else if(change==ADD)//下降
    {
        if(Motor_Status!=MOTORSTATUS_RUN)
        {
            if(Motor_Value.height!=0)//高度为0时，不转动角度，直接下降
            {
                switch(Motor_Value.angle)//先将角度变为80
                {
                    case -10:
                        Motor_Angle_N0to0();               
                        Motor_Angle_0to10();  
                        Motor_Angle_10to20();  
                        Motor_Angle_20to30();  
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80();                       
                    case 0:
                        Motor_Angle_0to10();  
                        Motor_Angle_10to20();  
                        Motor_Angle_20to30();  
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 10:
                        Motor_Angle_10to20();  
                        Motor_Angle_20to30();  
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 20:
                        Motor_Angle_20to30();  
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 30:
                        Motor_Angle_30to40();  
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 40:
                        Motor_Angle_40to50();  
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 50:
                        Motor_Angle_50to60();  
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 60:
                        Motor_Angle_60to70();
                        Motor_Angle_70to80(); 
                        break;
                    case 70:
                        Motor_Angle_70to80(); 
                        break;
                    case 80:
                        break;
                    default:
                        break;
                }
            }

            if(Motor_Value.height==100)
            {
                return MOTORERR;
            }
            else
            {
                Motor_Height_add1(); 
                return MOTOROK;
            }
        }
        else
        {
            return MOTORERR;
        }
    }
    else
    {
        return MOTORERR;
    }
}

int Motor_AutoCtl(int8_t auto_height, int8_t auto_angle)//自动控制函数，给目标高角值
{
    if (!((auto_angle == 0) || (auto_angle == 30) || (auto_angle == 50) || (auto_angle == 80)))
    {
        ESP_LOGI("motor", "auto_angle error");
        return MOTORERR;
    }
    /*  目标高度 > 当前高度  窗帘下降
        Anow -> A80
        Hnow -> Htar
        A80  -> Atar
    */
    if (auto_height > Motor_Value.height)  
    {
        while(80 != Motor_Value.angle)
        {
            Motor_HandCtl_Angle(ADD);
            vTaskDelay(50 / portTICK_RATE_MS);
        }

        while(auto_height != Motor_Value.height)
        {
            Motor_Height_add1();
            vTaskDelay(5 / portTICK_RATE_MS);
        }

        while(auto_angle != Motor_Value.angle)
        {
            Motor_HandCtl_Angle(SUB);
            vTaskDelay(50 / portTICK_RATE_MS);
        }
    }
    /*  目标高度 < 当前高度  窗帘上升
        Anow -> A-10
        Hnow -> Htar
        A-10 -> Atar
    */
    else if (auto_height < Motor_Value.height)  
    {
        while(0 != Motor_Value.angle)
        {
            Motor_HandCtl_Angle(SUB);
            vTaskDelay(50 / portTICK_RATE_MS);
        }
        Motor_Angle_0toN0();

        while(auto_height != Motor_Value.height)
        {
            Motor_Height_sub1();
            vTaskDelay(5 / portTICK_RATE_MS);
        }

        while(auto_angle != Motor_Value.angle)
        {
            Motor_HandCtl_Angle(ADD);
            vTaskDelay(50 / portTICK_RATE_MS);
        }
    }
    /*  目标高度 = 当前高度  只执行角度
        Anow -> Atar
    */
    else
    {
        while(auto_angle != Motor_Value.angle)
        {
            if(auto_angle > Motor_Value.angle)
            {
                Motor_HandCtl_Angle(ADD);
                vTaskDelay(50 / portTICK_RATE_MS);
            }    
            else if(auto_angle < Motor_Value.angle)
            {
                Motor_HandCtl_Angle(SUB);
                vTaskDelay(50 / portTICK_RATE_MS);
            }
        }
    }
    return MOTOROK;
}

// int Motor_AutoCtl(int8_t auto_height,int8_t auto_angle)//自动控制函数，给高角值
// {
//     if((auto_height==0)&&(auto_angle==0))//目标为0，0  则判断当前状态不是0，x的情况情况下，执行全收指令
//     {
        
//         if(Motor_Value.height!=0)
//         {
//             return Motor_SetAllUp();
//         }
//         else
//         {
//             return MOTORERR;
//         }
//     }
//     else if((auto_height==100)&&(Motor_Value.height!=100))//目标为100，x  当前高度不是100，执行全放
//     {
//         Motor_SetAllDown();
//     }
//     else if((auto_height==100)&&(auto_angle==80)&&(Motor_Value.angle<70))//目标为100，80 ，当前角度<70 执行全放
//     {
//         Motor_SetAllDown();
//     }
    
//     if((auto_height==100)&&(Motor_Value.height==100))//目标为100，x  当前是100，x，则转角到位
//     {       
//         if(Motor_Value.angle==80)//在100，80时给角度值，则直接到位
//         {
//             switch(auto_angle)//目标角度
//             {
//                 case 80:
//                     return MOTOROK;
//                     break;
//                 case 70:
//                     Motor_Angle_80to70();
//                     break;
//                 case 60:
//                     Motor_Angle_80to60();
//                     break;
//                 case 50:
//                     Motor_Angle_80to50();
//                     break;
//                 case 40:
//                     Motor_Angle_80to40();
//                     break;
//                 case 30:
//                     Motor_Angle_80to30();
//                     break;
//                 case 20:
//                     Motor_Angle_80to20();
//                     break;
//                 case 10:
//                     Motor_Angle_80to10();
//                     break;
//                 case 0:
//                     Motor_Angle_80to0();
//                     break;
//                 default:
//                     return MOTORERR;
//                     break;
//             }
//             return MOTOROK;
//         }
//         else//在100，非80时给目标角度值
//         {
//             if((auto_angle>=(-10))&&(auto_angle<=80))
//             {
//                 while(auto_angle!=Motor_Value.angle)
//                 {
//                     if(auto_angle>Motor_Value.angle)
//                     {
//                         Motor_HandCtl_Angle(ADD);
//                         vTaskDelay(50 / portTICK_RATE_MS);
//                     }    
//                     else if(auto_angle<Motor_Value.angle)
//                     {
//                         Motor_HandCtl_Angle(SUB);
//                         vTaskDelay(50 / portTICK_RATE_MS);
//                     }
//                 }
//                 return MOTOROK;
//             }
//             else
//             {
//                 return MOTORERR;
//             }
//         }
//     }
//     else
//     {
//         return MOTORERR;
//     }

// }



int Motor_KeyCtl(uint8_t change)
{
    int ret;
    if(change==ADD)//上升
    {
        if(Motor_Status!=MOTORSTATUS_RUN)
        {
            if((Motor_Value.angle==0)&&(Motor_Value.height==0))//当前是00时，直接返回error
            {
                return MOTORERR;
            }

            else if(Motor_Value.angle==30)//当角度=30时，执行角度--后，=角度=0，返回，停止
            {
                Motor_HandCtl_Angle(SUB); 
                return MOTORERR;
            }
            else if((Motor_Value.angle!=0)&&(Motor_Value.angle!=-10))  //当前角度在非0状态时，角度先变为0
            {
                ret=Motor_HandCtl_Angle(SUB); 
                if(ret==MOTOROK)
                {
                    vTaskDelay(KEYCTLANGLETIME / portTICK_RATE_MS);//每个角度控制改变间隔
                    return ret;
                }
                else
                {
                    return ret;
                }
            }
            else//当前角度=0状态时，高度直接减少
            {
                //高度每次减小10，减小后返回MOTOROK，继续减小，直到高度=0停止
                //
                ret=Motor_KeyCtl_Height(SUB);
                if(ret==MOTOROK)
                {
                    return ret; 
                }
                else
                {
                    return ret;
                }
            }
        }
        else
        {
            return MOTORERR;
        }
    }

    else if(change==SUB)//下降
    {
        if(Motor_Status!=MOTORSTATUS_RUN)
        {
            
            if((Motor_Value.angle==0)&&(Motor_Value.height==0))   //当前为0，0状态，直接执行转角度0to80后立即直接+高度
            {
                Motor_Value.angle = 80;
                return Motor_KeyCtl_Height(ADD);
            }
            else if(Motor_Value.angle==50)                       //当角度为50时，执行angle++后角度=80，返回停止
            {
                Motor_HandCtl_Angle(ADD);
                return MOTORERR;
            }
            else
            {
                if(Motor_Value.angle!=80)       //当前角度在非80状态时，角度++,达到80
                {
                    ret= Motor_HandCtl_Angle(ADD);
                    if(ret==MOTOROK)
                    {
                        vTaskDelay(KEYCTLANGLETIME / portTICK_RATE_MS);
                        return ret;
                    }
                    else
                    {
                        return ret;
                    }
                }
                else            //当前角度在80状态时，高度++
                {
                    return Motor_KeyCtl_Height(ADD);
                }
            }
        }
        else
        {
            return MOTORERR;
        }
    }
    else if(change==STOP)
    {
        change=WAIT;
        if(Motor_Value.angle==-10)  //当前正在上升，停止后角度为-10，则执行-10～10
        {
            Motor_Angle_N0to0();   //正常上升结束后执行一次-10～10
        }
    }
    return MOTORERR;

}


void Motor_Ctl_App(void)
{
    switch(WallKeyCtl_Status)
    {
        case WallKeyUpStart:
            work_status=WORK_WALLKEY;
            Led_Status=LED_STA_HAND;
            mqtt_json_s.mqtt_last=MAX_WALLKEY_TIME;
            if(Motor_KeyCtl(ADD)==MOTORERR)
            {
                WallKeyCtl_Status=WallKeyUpStop;
                printf("WallKeyCtl_Status=stop\n");
            }
            break;

        case WallKeyDownStart:
            work_status=WORK_WALLKEY;
            Led_Status=LED_STA_HAND;
            mqtt_json_s.mqtt_last=MAX_WALLKEY_TIME;
            if(Motor_KeyCtl(SUB)==MOTORERR)
            {
                WallKeyCtl_Status=WallKeyDownStop;
                printf("WallKeyCtl_Status=stop\n");
            }
            break;

        case WallKeyUpStop: //上升停止，高度停止时执行-10～10，角度停止时不执行                  
            Motor_KeyCtl(STOP);
            WallKeyCtl_Status=WallKeyWait;
            strcpy(mqtt_json_s.mqtt_mode, "0");//手动模式
            if(WifiStatus==WIFISTATUS_CONNET)
            {
                http_send_mes(POST_NORMAL);
            }
            break;

        case WallKeyDownStop://下降停止
            WallKeyCtl_Status=WallKeyWait;
            strcpy(mqtt_json_s.mqtt_mode, "0");//手动模式
            if(WifiStatus==WIFISTATUS_CONNET)
            {
                http_send_mes(POST_NORMAL);
            }
            break;

        default:
            break;
    }

}



