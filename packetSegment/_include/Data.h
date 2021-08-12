
#include <map>
#include <string>
#include "Segment.h"


typedef struct MeteringData
{
    string admin_code;                             //行政区
    int concentrate_no;                            //集中器编号
    int measured_point;                            //测量点号 = 电表编号
    unsigned char meter_addr[6];                   //电表地址
    unsigned char metering_time[6];                //抄表时间
    unsigned char ice_type[4];                     //冻结类型： Td_d, Td_m, Td_h, Td_c, 
    unsigned char metering_icedate[4];             //冻结日期
    unsigned char variable[255];                   //数据名称
    unsigned char unit[10];                        //单位
    int datatype;                                  //数据类型   0: 整形， 1：浮点， 2：字符类型 3：布尔
    unsigned char value[30];                       //数据值
    unsigned char occTime[3];                      //发生时间
}METERINGDATA;   



//日冻结正向有功/无功总电量
typedef struct DailyForwardWattMetering
{     
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icedate[4];           //冻结日期

    float forwardwatt;                           //正向有功总电量
    float forwardnowatt;                         //正向无功总电量

}DAILYICEWATTMETERING;


//日冻结反向有功/无功总电量
typedef struct DailyOppositeWattMetering
{     
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icedate[4];           //冻结日期

    float oppositewatt;                           //正向有功总电量
    float oppositenowatt;                         //正向无功总电量

}DAILYOPPOSITEWATTMETERING;


//日冻结正向有功/无功总最大需量
typedef struct DailyForwardMaxDemandWattMetering
{
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icedate[4];           //冻结日期

    float max_demand_forwardwatt;                //正向有功总最大需量
    float max_demand_forwardnowatt;              //正向无功总最大需量

}DAILYFORWARDMAXDEMANDWATTMETERING;


//日冻结反向有功/无功总最大需量
typedef struct DailyOppositeMaxDemandWattMetering
{
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icedate[4];           //冻结日期

    float max_demand_oppositewatt;               //反向有功总最大需量
    float max_demand_oppositenowatt;             //反向无功总最大需量

}DAILYOPPOSITEMAXDEMANDWATTMETERING;


//日冻结日不平衡度越限累计时间
typedef struct DailyNonBalanceTime
{
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icedate[4];           //冻结日期

    int currency_not_balance_minutes;           // 电流不平衡度越限日累计时间
    int voltage_not_balance_minutes;            // 电压不平衡度越限日累计时间
    float max_unbalanced_current;               // 电流不平衡最大值
    unsigned char occTimeCurrent[3];            // 电流不平衡最大值发生时间
    float max_unbalanced_voltage;               // 电压不平衡最大值
    unsigned char occTimeMaxVoltage[3];         // 电压不平衡最大值发生时间

}DAILYNONBALANCETIME;

//日负载率统计
typedef struct DailyLoadMaxMin
{

    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icedate[4];           //冻结日期

    float max_load_percentage;                   //最大负载率
    unsigned char occTimeMax[3];                 //发生时间
    float min_load_percentage;                   //最小负载率
    unsigned char occTimeMin[3];                 //发生时间

}DAILYLOADMAXMIN;


// 日冻结三相断相以及A相， B相， C相 断相次数和断相时间
typedef struct ABCPhaseFailureTimes
{
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icedate[4];           //冻结日期

    int totalfailedtimes;                        // 总失败次数
    int APhasefailedtimes;                       //"A相断相总次数";
    int BPhasefailedtimes;                       //"B相断相总次数";
    int CPhasefailedtimes;                       //"C相断相总次数";

    int totalfailedminutes;                      // 总失败分钟
    int APhasefailedminutes;                     // A相断累计时间
    int BPhasefailedminutes;                     // B相断累计时间
    int CPhasefailedminutes;                     // C相断累计时间

}ABCPHASEFAILURETIMES;

//日冻结电压统计
typedef struct DailyVoltageStatistics
{
    int AphaseUpUpLimitTime;                      //A相电压越上上限日累计时间
    int AphaseDownDownLimitTime;                  //A相电压越下下限日累计时间
    int AphaseUpLimitTime;                        //A相电压越上限日累计时间
    int AphaseDownLimitTime;                      //A相电压越下限日累计时间
    int AphaseQualifiedTime;                      //A相电压合格日累计时间

    int BphaseUpUpLimitTime;                      //B相电压越上上限日累计时间
    int BphaseDownDownLimitTime;                  //B相电压越下下限日累计时间
    int BphaseUpLimitTime;                        //B相电压越上限日累计时间
    int BphaseDownLimitTime;                      //B相电压越下限日累计时间
    int BphaseQualifiedTime;                      //B相电压合格日累计时间

    int CphaseUpUpLimitTime;                      //C相电压越上上限日累计时间
    int CphaseDownDownLimitTime;                  //C相电压越下下限日累计时间
    int CphaseUpLimitTime;                        //C相电压越上限日累计时间
    int CphaseDownLimitTime;                      //C相电压越下限日累计时间
    int CphaseQualifiedTime;                      //C相电压合格日累计时间

    float AphaseVoltageMax;                         //A相电压最大值
    unsigned char AoccTimeVoltageMax[3];            //A相电压最大值发生时间
    float AphaseVoltageMin;                         //A相电压最小值
    unsigned char AoccTimeVoltageMin[3];            //A相电压最小值发生时间
    
    float BphaseVoltageMax;                         //B相电压最大值
    unsigned char BoccTimeVoltageMax[3];            //B相电压最大值发生时间
    float BphaseVoltageMin;                         //B相电压最小值
    unsigned char BoccTimeVoltageMin[3];            //B相电压最小值发生时间

    float CphaseVoltageMax;                         //C相电压最大值
    unsigned char CoccTimeVoltageMax[3];            //C相电压最大值发生时间
    float CphaseVoltageMin;                         //C相电压最小值
    unsigned char CoccTimeVoltageMin[3];            //C相电压最小值发生时间

    float AphaseVoltageAverage;                     //A相电压平均值
    float BphaseVoltageAverage;                     //B相电压平均值
    float CphaseVoltageAverage;                     //C相电压平均值
}DAILYVOLTAGESTATISTICS;


//日冻结电流统计
typedef struct DailyCurrentStatistics
{
    int AphaseUpUpLimitTime;                      //A相电流越上上限日累计时间
    int AphaseUpLimitTime;                        //A相电流越上限日累计时间  

    int BphaseUpUpLimitTime;                      //B相电流越上上限日累计时间
    int BphaseUpLimitTime;                        //B相电流越上限日累计时间   
    
    int CphaseUpUpLimitTime;                      //C相电流越上上限日累计时间   
    int CphaseUpLimitTime;                        //C相电流越上限日累计时间  

    int ZeroSeqCurrentTime;                       //零序电流越上限累计时间

    float AphaseCurrentMax;                       //A相电流最大值
    unsigned char AoccTime[3];                    //A相电流最大值发生时间
    float BphaseCurrentMax;                       //B相电流最大值 
    unsigned char BoccTime[3];                    //B相电流最大值发生时间
    float CphaseCurrentMax;                       //C相电流最大值
    unsigned char CoccTime[3];                    //C相电流最大值发生时间

    float ZeroSeqCurrentMax;                      //零序电流最大值
    unsigned char ZeroSeqoccTime[3];              //零序电流最大值发生时间
}DAILYCURRENTSTATISTICS;


//月冻结正向有功无功总电量
typedef struct MonthForwardWattMetering
{     
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icemonth[3];           //冻结月

    float monthforwardwatt;                           //正向有功总电量
    float monthforwardnowatt;                         //正向无功总电量

}MONTHFORWARDWATTMETERING;


//月冻结正向有功/无功总最大需量
typedef struct MonthForwardMaxDemandWattMetering
{
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icemonth[3];           //冻结月

    float max_demand_forwardwatt;                //正向有功总最大需量
    float max_demand_forwardnowatt;              //正向无功总最大需量
}MONTHFORWARDMAXDEMANDWATTMETERING;

//月冻结反向有功无功总电量
typedef struct MonthOppositeWattMetering
{     
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icemonth[3];          //冻结月

    float monthoppositewatt;                     //反向有功总电量
    float monthoppositenowatt;                   //反向无功总电量

}MONTHOPPOSITEWATTMETERING;

//月冻结反向有功/无功总最大需量
typedef struct MonthOppositeMaxDemandWattMetering
{
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    unsigned char metering_time[6];              //抄表时间    
    unsigned char metering_icemonth[3];           //冻结月

    float max_demand_oppositewatt;                //正向有功总最大需量
    float max_demand_oppositenowatt;              //正向无功总最大需量
}MONTHOPPOSITEMAXDEMANDWATTMETERING;



typedef struct MeterParameter                    //电表信息
{
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    int measured_point;                          // 测量点号
    int protocol;                                // 通信协议
    long int address;                            // 通信地址
    long int collector_addr;                     //所属采集器地址
    int big_user_no;                             //大用户编号
    int little_user_no;                          //小用户编号
    
}METERPARAMETER;

typedef struct ConcentrateInfo                   //集中器信息
{
    string admin_code;                           //行政区
    int concentrate_no;                          //集中器编号
    unsigned char longitude[5];                  //经度
    unsigned char latitude[5];                   //纬度
    unsigned char VendorID[2];                   //生产厂商代码
    int DeviceNo;                                //设备代码
    int softwareEdition;                         //软件版本
    unsigned char softwareIssueDate[8];          //软件发布日期
    unsigned char Protocol[5];                   //通信协议
    unsigned char hardwareEdition;               //硬件版本号
    unsigned char hardwareIssueDate[8];          //硬件发布日期
}CONCENTRATEINFO;

typedef enum event
{
    ERC0 = 0, // not in use
    ERC1,
    ERC2,
    ERC3,
    ERC4,
    ERC5,
    ERC6,
    ERC7,
    ERC8,
    ERC9,
    ERC10,
    ERC11,
    ERC12,
    ERC13,
    ERC14,
    ERC15,
    ERC16,
    ERC17,
    ERC18,
    ERC19,
    ERC20,
    ERC21,
    ERC22,
    ERC23,
    ERC24,
    ERC25,
    ERC26,
    ERC27,
    ERC28,
    ERC29,
    ERC30,
    ERC31,
    ERC32,
    ERC33,
    ERC34,
    ERC35,
    ERC36,
    ERC37,
    ERC38,
    ERC39,
    ERC40,
    ERC41
    
}EVENT; 

typedef struct EventData
{
   string admin_code;                           //行政区
   int concentrate_no;                          //集中器编号
   int measured_point;                          // 测量点号
   EVENT event;
   int event_len;
   unsigned char event_datetime[6];
   unsigned char details[255];                 //事件详情
   
}EVENTDATA;


class Data : public Segment
{

public:

Data(SegmentEnum segmentID);
~Data();

string getCommand() {
    return command;
}

void setCommand(string command) {
    this->command = command;
}

int getAFN()
{
    return afn;
}

void setAFN(int afn)
{
    this->afn = afn;
}

int getPn() {
    return pn;
}

void setPn(int pn) {
    this->pn = pn;
}

int getFn() {
    return fn;
}

void setFn(int fn) 
{
    this->fn = fn;
}

void setPn(int Pn) {
    this->Pn = Pn;
}

void setPm(int Pm) {
    this->Pm = Pm;
}

void setEC1(int EC1) {
    this->EC1 = EC1;
}

void setEC2(int EC2) {
    this->EC2 = EC2;
}

void setrates(int rates) {
    this->rates = rates;
}


void addDailyForwardWattMetering(DAILYICEWATTMETERING  meteringdata);
void addDailyOppositeWattMetering(DAILYOPPOSITEWATTMETERING  meteringdata);
void addDailyForwardMaxDemandWattMetering(DAILYFORWARDMAXDEMANDWATTMETERING  meteringdata);
void addDailyOppositeMaxDemandWattMetering(DAILYOPPOSITEMAXDEMANDWATTMETERING  meteringdata);
void addMonthForwardWattMetering(MONTHFORWARDWATTMETERING  meteringdata);
void addMonthOppositeWattMetering(MONTHOPPOSITEWATTMETERING  meteringdata);
void addMonthForwardMaxDemandWattMetering(MONTHFORWARDMAXDEMANDWATTMETERING  meteringdata);
void addMonthOppositeMaxDemandWattMetering(MONTHOPPOSITEMAXDEMANDWATTMETERING  meteringdata);
void addDailyLoadMaxMin(DAILYLOADMAXMIN  meteringdata);
void addDailyABCPhaseFailureTimes(ABCPHASEFAILURETIMES  meteringdata);
void addDailyVoltageStatistics(DAILYVOLTAGESTATISTICS  meteringdata);
void addDailyNonBalanceTime(DAILYNONBALANCETIME  meteringdata);
void addDailyCurrentStatistics(DAILYCURRENTSTATISTICS  meteringdata);
void addEventData(EVENTDATA evData);
void addConcentrates(CONCENTRATEINFO concentrate);

void reset();

private:
/**
 * 功能码
 */
int afn;

/**
* 信息点pn
*/
int pn;
/**
* 信息类fn
*/
int fn;

/**
* 抄表时间
*/
string meteringtime;

/**
* 费率数
*/
int rates;

/**
* 表对象数
*/
int metres;

/**
* 是否消息认证
*/
bool PW; 

/**
* 重要事件和非重要事件数
*/
int EC1;
int EC2;

/**
* 事件起始和结束指针
*/
int Pm;  //本帧报文传送的事件记录起始指针 Pm
int Pn;  //本帧报文传送的事件记录结束指针 Pn

/**
* 数据
*/
string command;

vector <string> poweroffon_meteraddress;
vector <EVENTDATA> event_data;
vector <CONCENTRATEINFO> concentrates;

vector <DAILYICEWATTMETERING> dailyice_metering_data;
vector <DAILYOPPOSITEWATTMETERING> dailyice_opposite_metering_data;
vector <DAILYFORWARDMAXDEMANDWATTMETERING> dailyice_maxdemand_metering_data;
vector <DAILYOPPOSITEMAXDEMANDWATTMETERING>dailyice_oppositemaxdemand_metering_data; 
vector <MONTHFORWARDWATTMETERING> month_forwardwatt_metering_data;
vector <MONTHOPPOSITEWATTMETERING>month_oppositewatt_metering_data;
vector <MONTHFORWARDMAXDEMANDWATTMETERING> month_maxdemand_metering_data;
vector <MONTHOPPOSITEMAXDEMANDWATTMETERING> month_opposite_maxdemand_metering_data;
vector <DAILYLOADMAXMIN> dailyice_loadmaxmin;
vector <ABCPHASEFAILURETIMES>dailyicce_ABCphase_failure_times;
vector <DAILYNONBALANCETIME>dailyice_ABCphase_nobalance_times;
vector <DAILYVOLTAGESTATISTICS>dailyice_voltage_statistics;
vector <DAILYCURRENTSTATISTICS>dailyice_current_statistics;


};
