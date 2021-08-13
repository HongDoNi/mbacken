#include "DataUnwrapper.h"


int DataUnwrapper::AFN_EVENT     = 0x0E;   //读取终端 3 类数据
int DataUnwrapper::AFN_DATA2     = 0x0D;   //读取终端 2 类数据
int DataUnwrapper::AFN_DATA1     = 0x0C;   //读取终端 1 类数据
int DataUnwrapper::AFN_SETTING   = 0x09;   //读取终端配置参数


DataUnwrapper::DataUnwrapper()
{    
   
}

unsigned char* DataUnwrapper:: DAencode(string value)
    {
        unsigned char* pout = new unsigned char[2];
        int pn = std::stoi(value);

        if(pn==0){
            pout[0]=0;
            pout[1]=0;
        }else{
            pout[0]=(unsigned char)(1 << (pn-1) % 8);
            pout[1]=(unsigned char)((pn) / 8+1);
        }
        return pout;
    }


int DataUnwrapper::DAdecode(unsigned char byteBuffer1, unsigned char byteBuffer2) 
    {
        unsigned char value[2];
        value[0] = byteBuffer1;
        value[1] = byteBuffer2;
        

        if(value[0]==0 && value[1]==0){
            return 0;
        }else{
            int pn=(value[1]-1)*8;
            for(int i=0;i<8;i++){
                if((value[0]>>i==1)){
                    pn+=i+1;
                }
            }
            return pn;
        }
    }

unsigned char* DataUnwrapper::DTencode(string value)
{
        unsigned char out[2];
        int fn= std::stoi(value));
        out[0]=(unsigned char)(1 << (fn-1) % 8);      
        out[1]=(unsigned char)((fn-1) / 8);
        return out;
}


int DataUnwrapper::DTdecode(unsigned char byteBuffer1, unsigned char byteBuffer2)
{
      
    int fn = byteBuffer2*8;

    for(int i=0;i<8;i++)
    {
        if(((byteBuffer1&0xff)>>i)==1)
        {
            fn+=i+1;
        }
    }
    
    return fn;
}

void DataUnwrapper::decode(unsigned char in[], int nLen, PacketSegmentContext* packetSegmentContext)
{
        Data dataSeg =(Data)packetSegmentContext->getSegment(SegmentEnum::DATA);
        Control control =(Control)packetSegmentContext->getSegment(SegmentEnum::CONTROL);

        if(!control.isDecodeCompleted())
        {
            return; 
        }

        // dataSeg.setPn(da.decode(in[14], in[15]));
        // dataSeg.setFn(dt.decode(in[16], in[17]));
        dataSeg.setPn(DAdecode(in[14], in[15]));
        dataSeg.setFn(DTdecode(in[16], in[17]));
        dataSeg.setAFN(control.getAfn()); 

        //固定剩余数据长度
        int tailLeft=4;

        if(control.getFcbOrAcd()==1){
            tailLeft+=2;
        }

        if(control.getTpV()==1){
            tailLeft+=6;
        }

        if(control.getAfn() == AFN_EVENT) 
        {
            decodeEvent(in, nLen, &dataSeg, &control);            
            
        }
        else if(control.getAfn() == AFN_DATA2) 
        {
            decodeData2(in, nLen, &dataSeg, &control);
        }            
        else if(control.getAfn() == AFN_DATA1) 
        {
            decodeData1(in, nLen, &dataSeg, &control);
        
        }
        else if(control.getAfn() == AFN_SETTING) 
        {   
            decodeSetting(in, nLen, &dataSeg, &control);       
        
        }
      
        dataSeg.setDecodeCompleted();

        if(next!=null){
            next.decode(in,nLen, packetSegmentContext);
        }
}


    
    
string DataUnwrapper::decodeData2(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{
    
    switch(dataSeg.getFn())
    {
    case 1:
        DailyForwardWattMetering(in, nLen, dataSeg, control);  //日冻结正向有功无功
        break;
    case 2:
        DailyOppositeWattMetering(in, nLen, dataSeg, control);    //日冻结反向有功无功
        break;
    case 3:
        DailyForwardMaxDemandWattMetering(in, nLen, dataSeg, control);   //日冻结正向最大需求有功无功
        break;
    case 4:
        DailyOppositeMaxDemandWattMetering(in, nLen, dataSeg, control);  //日冻结反向最大需求有功无功
        break;
    case 17：
        MonthForwardWattMetering(in, nLen, dataSeg, control);    //月冻结正向有功无功
        break;
    case 18:
        MonthOppositeWattMetering(in, nLen, dataSeg, control);     //月冻结反向有功无功
        break;
    case 19:
        MonthForwardMaxDemandWattMetering(in, nLen, dataSeg, control);  //月冻结正向最大需求有功无功
        break;
    case 20:
        MonthOppositeMaxDemandWattMetering(in, nLen, dataSeg, control);   //月冻结反向最大需求有功无功
        break;
    case 27：
        DailyVoltageStatistics(in, nLen, dataSeg, control);      //日冻结日电压数据统计
        break;
    case 28：
        DailyNonBalanceTime(in, nLen, dataSeg, control);          //日冻结日不平衡度越限累计时间 
        break;
    case 29：
        DailyCurrencyStatistics(in, nLen, dataSeg, control);      //日冻结日电流数据统计
        break;
    case 31：
        DailyLoadMaxMin(in, nLen, dataSeg, control);              //日负载率统计
        break;
    case 32：
        DailyABCPhaseFailureTimes(in, nLen, dataSeg, control);   // 日冻结三相断相以及A相， B相， C相 断相次数和断相时间
        break;
    }
    
}


void DataUnwrapper::decodeData1(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{
    switch(dataSeg.getFn())
    {
    case 1:
        DailyForwardWattMetering(in, nLen, dataSeg, control);  //日冻结正向有功无功
        break;
    case 25:
        CurrentMetering(in, nLen, dataSeg, control); //当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
        break;



    }
}


void DataUnwrapper::decodeSetting(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{

    switch(dataSeg.getFn())
    {
    case 1:
        ReadConcenstrateInfo(in, nLen, dataSeg, control);
        break;
    }
}


    
 void DataUnwrapper::DailyForwardWattMetering(unsigned char in[], int nLen, Data* dataSeg, Control* control)
 {
    DAILYICEWATTMETERING  meteringdata;

    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();

    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = in[pos+2];
    
    meteringdata.metering_time[0]       = 20;
    meteringdata.metering_time[1]       = in[pos+7];
    meteringdata.metering_time[2]       = in[pos+6];
    meteringdata.metering_time[3]       = in[pos+5];
    meteringdata.metering_time[4]       = in[pos+4];
    meteringdata.metering_time[5]       = in[pos+3];

    int m = in[pos+8];   // 费率数

    // "正向有功总电量" : 浮点型, kWh//  
    meteringdata.forwardwatt      =  (in[pos+13]>>4)*100000 + (in[pos+13]<<4)*10000 + (in[pos+12]>>4)*1000 + (in[pos+12]<<4)*100 + (in[pos+11]>>4)*10 + (in[pos+11]<<4) + (in[pos+10]>>4)*0.1 + (in[pos+10]<<4)*0.01 + (in[pos+9]>>4)*0.001 + (in[pos+9]<<4)*0.0001;
    pos += 13;
    // "正向无功总电量" : 浮点型,  kWh
    meteringdata.forwardnowatt    =  (in[pos+m*5+3]>>4)*1000 + (in[pos+m*5+3]<<4)*100 + (in[pos+m*5+2]>>4)*10 + (in[pos+m*5+2]<<4) + (in[pos+m*5+1]>>4)*0.1 + (in[pos+m*5+1]<<4)*0.01 + (in[pos+m*5]>>4)*0.001 + (in[pos+m*5]<<4)*0.0001;
    dataSeg->addDailyForwardWattMetering(meteringdata);

 }


 void DataUnwrapper::DailyOppositeWattMetering(unsigned char in[], int nLen, Data* dataSeg, Control* control)
 {
    DAILYOPPOSITEWATTMETERING  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();

    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = in[pos+2];
  

    meteringdata.metering_time[0]      = 20;
    meteringdata.metering_time[1]      = in[pos+7];
    meteringdata.metering_time[2]      = in[pos+6];
    meteringdata.metering_time[3]      = in[pos+5];
    meteringdata.metering_time[4]      = in[pos+4];
    meteringdata.metering_time[5]      = in[pos+3];

    int m = in[pos+8];   // 费率数

    //"反向有功总电量": 浮点型, kWh//  
    meteringdata.oppositewatt    =  (in[pos+13]>>4)*100000 + (in[pos+13]<<4)*10000 + (in[pos+12]>>4)*1000 + (in[pos+12]<<4)*100 + (in[pos+11]>>4)*10 + (in[pos+11]<<4) + (in[pos+10]>>4)*0.1 + (in[pos+10]<<4)*0.01 + (in[pos+9]>>4)*0.001 + (in[pos+9]<<4)*0.0001;
        
    pos += 13;
    
    //"反向无功总电量": 浮点型, kWh //  
  
    meteringdata.oppositenowatt    =  (in[pos+m*5+3]>>4)*1000 + (in[pos+m*5+3]<<4)*100 + (in[pos+m*5+2]>>4)*10 + (in[pos+m*5+2]<<4) + (in[pos+m*5+1]>>4)*0.1 + (in[pos+m*5+1]<<4)*0.01 + (in[pos+m*5]>>4)*0.001 + (in[pos+m*5]<<4)*0.0001;
    dataSeg->addDailyOppositeWattMetering(meteringdata);
 }


void DataUnwrapper::DailyForwardMaxDemandWattMetering(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{
    
    DAILYFORWARDMAXDEMANDWATTMETERING  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = in[pos+2];

    meteringdata.metering_time[0]      = 20;
    meteringdata.metering_time[1]      = in[pos+7];
    meteringdata.metering_time[2]      = in[pos+6];
    meteringdata.metering_time[3]      = in[pos+5];
    meteringdata.metering_time[4]      = in[pos+4];
    meteringdata.metering_time[5]      = in[pos+3];

    int m = in[pos+8];   // 费率数

    //"正向有功总最大需量": 浮点型, kW //  
    meteringdata.max_demand_forwardwatt      =  (in[pos+11]>>4)*10 + (in[pos+11]<<4) + (in[pos+10]>>4)*0.1 + (in[pos+10]<<4)*0.01 + (in[pos+9]>>4)*0.001 + (in[pos+9]<<4)*0.0001;
 
    pos += 11;
    //"正向无功总最大需量"：浮点型, kw //     
    meteringdata.max_demand_forwardnowatt    =  (in[pos+(m+1)*4+2]>>4)*10 + (in[pos+(m+1)*4+2]<<4) + (in[pos+(m+1)*4+1]>>4)*0.1 + (in[pos+(m+1)*4+1]<<4)*0.01 + (in[pos+(m+1)*4]>>4)*0.001 + (in[pos+(m+1)*4]<<4)*0.0001;
    dataSeg->addDailyForwardMaxDemandWattMetering(meteringdata);

}

void DataUnwrapper::DailyOppositeMaxDemandWattMetering(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{
    
    DAILYOPPOSITEMAXDEMANDWATTMETERING  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = in[pos+2];

    meteringdata.metering_time[0]      = 20;
    meteringdata.metering_time[1]      = in[pos+7];
    meteringdata.metering_time[2]      = in[pos+6];
    meteringdata.metering_time[3]      = in[pos+5];
    meteringdata.metering_time[4]      = in[pos+4];
    meteringdata.metering_time[5]      = in[pos+3];

    int m = in[pos+8];   // 费率数
    // "反向有功总最大需量": 浮点型, kW//  
    meteringdata.max_demand_oppositewatt    =  (in[pos+11]>>4)*10 + (in[pos+11]<<4) + (in[pos+10]>>4)*0.1 + (in[pos+10]<<4)*0.01 + (in[pos+9]>>4)*0.001 + (in[pos+9]<<4)*0.0001;
        
    pos += 11;
    //"正反向无功总最大需量"：浮点型, kW //  
    meteringdata.max_demand_oppositenowatt    =  (in[pos+(m+1)*4+2]>>4)*10 + (in[pos+(m+1)*4+2]<<4) + (in[pos+(m+1)*4+1]>>4)*0.1 + (in[pos+(m+1)*4+1]<<4)*0.01 + (in[pos+(m+1)*4]>>4)*0.001 + (in[pos+(m+1)*4]<<4)*0.0001;
    dataSeg->addDailyOppositeMaxDemandWattMetering(meteringdata);

}

///////////////////////////monthly metering //////////////////////////////////////////////

void DataUnwrapper::MonthForwardWattMetering(unsigned char in[], int nLen, Data* dataSeg, Control* control)
 {
    MONTHFORWARDWATTMETERING  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icemonth[0]   = 20;
    meteringdata.metering_icemonth[1]   = in[pos];
    meteringdata.metering_icemonth[2]   = in[pos+1];
    
    
    meteringdata.metering_time[0]       = 20;
    meteringdata.metering_time[1]       = in[pos+6];
    meteringdata.metering_time[2]       = in[pos+5];
    meteringdata.metering_time[3]       = in[pos+4];
    meteringdata.metering_time[4]       = in[pos+3];
    meteringdata.metering_time[5]       = in[pos+2];

    int m = in[pos+7];   // 费率数

    //"月正向有功总电量": 浮点型, kWh//  
    meteringdata.monthforwardwatt      =  (in[pos+12]>>4)*100000 + (in[pos+12]<<4)*10000 + (in[pos+11]>>4)*1000 + (in[pos+11]<<4)*100 + (in[pos+10]>>4)*10 + (in[pos+10]<<4) + (in[pos+9]>>4)*0.1 + (in[pos+9]<<4)*0.01 + (in[pos+8]>>4)*0.001 + (in[pos+8]<<4)*0.0001;
    
    pos += 12;

    //"月正向无功总电量": 浮点型, kWh// 
    meteringdata.monthforwardnowatt    =  (in[pos+m*5+3]>>4)*1000 + (in[pos+m*5+3]<<4)*100 + (in[pos+m*5+2]>>4)*10 + (in[pos+m*5+2]<<4) + (in[pos+m*5+1]>>4)*0.1 + (in[pos+m*5+1]<<4)*0.01 + (in[pos+m*5]>>4)*0.001 + (in[pos+m*5]<<4)*0.0001;
    dataSeg->addMonthForwardWattMetering(meteringdata);

 }


 void DataUnwrapper::MonthOppositeWattMetering(unsigned char in[], int nLen, Data* dataSeg, Control* control)
 {
    MONTHOPPOSITEWATTMETERING  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = 0;
 

    meteringdata.metering_time[0]      = 20;
    meteringdata.metering_time[1]      = in[pos+6];
    meteringdata.metering_time[2]      = in[pos+5];
    meteringdata.metering_time[3]      = in[pos+4];
    meteringdata.metering_time[4]      = in[pos+3];
    meteringdata.metering_time[5]      = in[pos+2];

    int m = in[pos+7];   // 费率数

    // "月反向有功总电量": 浮点型, kWh//  
    meteringdata.monthoppositewatt      =  (in[pos+12]>>4)*100000 + (in[pos+12]<<4)*10000 + (in[pos+11]>>4)*1000 + (in[pos+11]<<4)*100 + (in[pos+10]>>4)*10 + (in[pos+10]<<4) + (in[pos+9]>>4)*0.1 + (in[pos+9]<<4)*0.01 + (in[pos+8]>>4)*0.001 + (in[pos+8]<<4)*0.0001;

    pos += 12;
    
    //"月反向无功总电量": 浮点型, kWh//  
    meteringdata.monthoppositenowatt    =  (in[pos+m*5+3]>>4)*1000 + (in[pos+m*5+3]<<4)*100 + (in[pos+m*5+2]>>4)*10 + (in[pos+m*5+2]<<4) + (in[pos+m*5+1]>>4)*0.1 + (in[pos+m*5+1]<<4)*0.01 + (in[pos+m*5]>>4)*0.001 + (in[pos+m*5]<<4)*0.0001;
    dataSeg->addMonthOppositeWattMetering(meteringdata);

 }


void DataUnwrapper::MonthForwardMaxDemandWattMetering(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{
    
    MONTHFORWARDMAXDEMANDWATTMETERING  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = 0
   
    meteringdata.metering_time[0]       = 20;
    meteringdata.metering_time[1]       = in[pos+6];
    meteringdata.metering_time[2]       = in[pos+5];
    meteringdata.metering_time[3]       = in[pos+4];
    meteringdata.metering_time[4]       = in[pos+3];
    meteringdata.metering_time[5]       = in[pos+2];

    int m = in[pos+7];   // 费率数

    // "月正向有功总最大需量"：浮点型, kW //   
    meteringdata.max_demand_forwardwatt    =  (in[pos+10]>>4)*10 + (in[pos+10]<<4) + (in[pos+9]>>4)*0.1 + (in[pos+9]<<4)*0.01 + (in[pos+8]>>4)*0.001 + (in[pos+8]<<4)*0.0001;
        
    pos += 10;
    
    //"月正向无功总最大需量": 浮点型, kW//  
    meteringdata.max_demand_forwardnowatt    =  (in[pos+(m+1)*4+2]>>4)*10 + (in[pos+(m+1)*4+2]<<4) + (in[pos+(m+1)*4+1]>>4)*0.1 + (in[pos+(m+1)*4+1]<<4)*0.01 + (in[pos+(m+1)*4]>>4)*0.001 + (in[pos+(m+1)*4]<<4)*0.0001;
    dataSeg->addMonthForwardMaxDemandWattMetering(meteringdata);



}

void DataUnwrapper::MonthOppositeMaxDemandWattMetering(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{
    
    MONTHOPPOSITEMAXDEMANDWATTMETERING  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = 0;

    meteringdata.metering_time[0]      = 20;
    meteringdata.metering_time[1]      = in[pos+6];
    meteringdata.metering_time[2]      = in[pos+5];
    meteringdata.metering_time[3]      = in[pos+4];
    meteringdata.metering_time[4]      = in[pos+3];
    meteringdata.metering_time[5]      = in[pos+2];

    int m = in[pos+7];   // 费率数

    // "月反向有功总最大需量": 浮点型 , kW //  
    meteringdata.max_demand_oppositewatt    =  (in[pos+10]>>4)*10 + (in[pos+10]<<4) + (in[pos+9]>>4)*0.1 + (in[pos+9]<<4)*0.01 + (in[pos+8]>>4)*0.001 + (in[pos+8]<<4)*0.0001;

    pos += 10;    
    
    // "月正反向无功总最大需量": 浮点型,  kW//  
    meteringdata.max_demand_oppositenowatt    =  (in[pos+(m+1)*4+2]>>4)*10 + (in[pos+(m+1)*4+2]<<4) + (in[pos+(m+1)*4+1]>>4)*0.1 + (in[pos+(m+1)*4+1]<<4)*0.01 + (in[pos+(m+1)*4]>>4)*0.001 + (in[pos+(m+1)*4]<<4)*0.0001;
    dataSeg->addMonthOppositeMaxDemandWattMetering(meteringdata);

}


void DataUnwrapper::DailyLoadMaxMin(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{

    DAILYLOADMAXMIN  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = in[pos+2];
    
    // "负载率最大值": 浮点型, % //
  
    meteringdata.max_load_percentage    =  ((in[pos+3]>>4)<<1) *10 + (in[pos+3]<<4) + (in[pos+4]>>4)*0.1 + (in[pos+4]<<4)*0.01;
    meteringdata.occTimeMax[0]          =  (in[pos+7]>>4) *10 + in[pos+7]<<4 ;
    meteringdata.occTimeMax[1]          =  (in[pos+6]>>4) *10 + in[pos+6]<<4 ;
    meteringdata.occTimeMax[2]          =  (in[pos+5]>>4) *10 + in[pos+5]<<4 ;

    
    // "负载率最小值"; 浮点型, % //
    meteringdata.min_load_percentage    =  ((in[pos+8]>>4)<<1) *10 + (in[pos+8]<<4) + (in[pos+9]>>4)*0.1 + (in[pos+9]<<4)*0.01;
    meteringdata.occTime[0]             =  (in[pos+12]>>4) *10 + in[pos+12]<<4 ;
    meteringdata.occTime[1]             =  (in[pos+11]>>4) *10 + in[pos+11]<<4 ;
    meteringdata.occTime[2]             =  (in[pos+10]>>4) *10 + in[pos+10]<<4 ;
    
    dataSeg->addDailyLoadMaxMin(meteringdata);
    
    
}


void DataUnwrapper::DailyABCPhaseFailureTimes(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{

    ABCPHASEFAILURETIMES  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = in[pos+2];
    
    meteringdata.metering_time[0]      = 20;
    meteringdata.metering_time[1]      = in[pos+6];
    meteringdata.metering_time[2]      = in[pos+5];
    meteringdata.metering_time[3]      = in[pos+4];
    meteringdata.metering_time[4]      = in[pos+3];
    meteringdata.metering_time[5]      = in[pos+2];

    //"断相总次数"： 整型, 次数//  
    meteringdata.totalfailedtimes     =  (in[pos+7]>>4) *10 + in[pos+7]<<4 + (in[pos+8]>>4)*1000 + (in[pos+8]<<4)*100 + (in[pos+9]>>4)*100000 + (in[pos+9]<<4)*10000;
    //"A相断相总次数"： 整型, 次数//   
    meteringdata.APhasefailedtimes    =  (in[pos+10]>>4) *10 + in[pos+10]<<4 + (in[pos+10]>>4)*1000 + (in[pos+10]<<4)*100 + (in[pos+11]>>4)*100000 + (in[pos+11]<<4)*10000;
    //"B相断相总次数"： 整型, 次数//
    meteringdata.BPhasefailedtimes    =  (in[pos+12]>>4) *10 + in[pos+12]<<4 + (in[pos+13]>>4)*1000 + (in[pos+13]<<4)*100 + (in[pos+14]>>4)*100000 + (in[pos+14]<<4)*10000;
    //"C相断相总次数"： 整型, 次数//
    meteringdata.CPhasefailedtimes    =  (in[pos+15]>>4) *10 + in[pos+15]<<4 + (in[pos+16]>>4)*1000 + (in[pos+16]<<4)*100 + (in[pos+17]>>4)*100000 + (in[pos+17]<<4)*10000;
    
    // "相断累计时间": 整型, 分钟 //    
    meteringdata.totalfailedminutes   =  (in[pos+18]>>4) *10 + in[pos+18]<<4 + (in[pos+19]>>4)*1000 + (in[pos+19]<<4)*100 + (in[pos+20]>>4)*100000 + (in[pos+20]<<4)*10000;
    // "A相断累计时间": 整型, 分钟 // 
    meteringdata.APhasefailedminutes  =  (in[pos+21]>>4) *10 + in[pos+21]<<4 + (in[pos+22]>>4)*1000 + (in[pos+22]<<4)*100 + (in[pos+23]>>4)*100000 + (in[pos+23]<<4)*10000;
    // "B相断累计时间": 整型, 分钟 // 
    meteringdata.BPhasefailedminutes  =  (in[pos+24]>>4) *10 + in[pos+24]<<4 + (in[pos+25]>>4)*1000 + (in[pos+25]<<4)*100 + (in[pos+26]>>4)*100000 + (in[pos+26]<<4)*10000;
    // "C相断累计时间": 整型, 分钟 // 
    meteringdata.CPhasefailedminutes  =  (in[pos+27]>>4) *10 + in[pos+27]<<4 + (in[pos+28]>>4)*1000 + (in[pos+28]<<4)*100 + (in[pos+29]>>4)*100000 + (in[pos+29]<<4)*10000;
    dataSeg->addDailyABCPhaseFailureTimes(meteringdata);

}


void DataUnwrapper::DailyVoltageStatistics(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{

    DAILYVOLTAGESTATISTICS  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = in[pos+2];
   
    // "A相电压越上上限日累计时间"： 整型, 分钟 //  
    meteringdata.AphaseUpUpLimitTime        =  (in[pos+3]>>4) *10 + in[pos+3]<<4 + (in[pos+4]>>4)*1000 + (in[pos+4]<<4)*100;
    // "A相电压越下下限日累计时间"： 整型, 分钟 //
    meteringdata.AphaseDownDownLimitTime    =  (in[pos+5]>>4) *10 + in[pos+5]<<4 + (in[pos+6]>>4)*1000 + (in[pos+6]<<4)*100;
    // "A相电压越上限日累计时间"： 整型, 分钟 //
    meteringdata.AphaseUpLimitTime          =  (in[pos+7]>>4) *10 + in[pos+7]<<4 + (in[pos+8]>>4)*1000 + (in[pos+8]<<4)*100;
    // "A相电压越下限日累计时间"： 整型, 分钟 //
    meteringdata.AphaseDownLimitTime        =  (in[pos+9]>>4) *10 + in[pos+9]<<4 + (in[pos+10]>>4)*1000 + (in[pos+10]<<4)*100;
     // "A相电压合格日累计时间": 整型, 分钟 //  
    meteringdata.AphaseQualifiedTime       =  (in[pos+11]>>4) *10 + in[pos+11]<<4 + (in[pos+12]>>4)*1000 + (in[pos+12]<<4)*100;
  
    // "B相电压越上上限日累计时间"： 整型, 分钟 // 
    meteringdata.BphaseUpUpLimitTime       =  (in[pos+13]>>4) *10 + in[pos+13]<<4 + (in[pos+14]>>4)*1000 + (in[pos+14]<<4)*100; 
    // "B相电压越下下限日累计时间"： 整型, 分钟 // 
    meteringdata.BphaseDownDownLimitTime    =  (in[pos+15]>>4) *10 + in[pos+15]<<4 + (in[pos+16]>>4)*1000 + (in[pos+16]<<4)*100;
    // "B相电压越上限日累计时间"： 整型, 分钟 //
    meteringdata.BphaseUpLimitTime          =  (in[pos+17]>>4) *10 + in[pos+17]<<4 + (in[pos+18]>>4)*1000 + (in[pos+18]<<4)*100;
    // "B相电压越下限日累计时间"： 整型, 分钟 //
    meteringdata.BphaseDownLimitTime        =  (in[pos+19]>>4) *10 + in[pos+19]<<4 + (in[pos+20]>>4)*1000 + (in[pos+20]<<4)*100;
    // "B相电压合格日累计时间": 整型, 分钟 //
    meteringdata.BphaseQualifiedTime        =  (in[pos+21]>>4) *10 + in[pos+21]<<4 + (in[pos+22]>>4)*1000 + (in[pos+22]<<4)*100;
    

    // "C相电压越上上限日累计时间"： 整型, 分钟 // 
    meteringdata.BphaseUpUpLimitTime    =  (in[pos+23]>>4) *10 + in[pos+23]<<4 + (in[pos+24]>>4)*1000 + (in[pos+24]<<4)*100;
    // "C相电压越下下限日累计时间"： 整型, 分钟 // 
    meteringdata.BphaseDownDownLimitTime    =  (in[pos+25]>>4) *10 + in[pos+25]<<4 + (in[pos+26]>>4)*1000 + (in[pos+26]<<4)*100;   
    // "C相电压越上限日累计时间"： 整型, 分钟 // 
    meteringdata.BphaseUpLimitTime    =  (in[pos+27]>>4) *10 + in[pos+27]<<4 + (in[pos+28]>>4)*1000 + (in[pos+28]<<4)*100;   
     //"C相电压越下限日累计时间"： 整型, 分钟 // 
    meteringdata.BphaseDownLimitTime    =  (in[pos+29]>>4) *10 + in[pos+29]<<4 + (in[pos+30]>>4)*1000 + (in[pos+30]<<4)*100;    
    // "C相电压合格日累计时间": 整型, 分钟 //
    meteringdata.BphaseQualifiedTime    =  (in[pos+31]>>4) *10 + in[pos+31]<<4 + (in[pos+32]>>4)*1000 + (in[pos+32]<<4)*100;
   
    ///////////////////////////////////////////////////////////////////////////////////
    // "A相电压最大值": 浮点型, V //  
    meteringdata.AphaseVoltageMax        =  in[pos+33]>>4 + (in[pos+33]<<4)*0.1 + (in[pos+34]>>4)*100 + (in[pos+34]<<4)*10;
    meteringdata.AoccTimeVoltageMax[0]   =  (in[pos+37]>>4) *10 + in[pos+37]<<4 ;
    meteringdata.AoccTimeVoltageMax[1]   =  (in[pos+36]>>4) *10 + in[pos+36]<<4 ;
    meteringdata.AoccTimeVoltageMax[2]   =  (in[pos+35]>>4) *10 + in[pos+35]<<4 ;
   
   // "A相电压最小值": 浮点型, V // 
    meteringdata.AphaseVoltageMin        =  in[pos+38]>>4 + (in[pos+38]<<4)*0.1 + (in[pos+39]>>4)*100 + (in[pos+39]<<4)*10;
    meteringdata.AoccTimeVoltageMin[0]   =  (in[pos+42]>>4) *10 + in[pos+42]<<4 ;
    meteringdata.AoccTimeVoltageMin[1]   =  (in[pos+41]>>4) *10 + in[pos+41]<<4 ;
    meteringdata.AoccTimeVoltageMin[2]   =  (in[pos+40]>>4) *10 + in[pos+40]<<4 ;   
   
    // "B相电压最大值": 浮点型, V // 
    meteringdata.BphaseVoltageMax        =  in[pos+43]>>4 + (in[pos+43]<<4)*0.1 + (in[pos+44]>>4)*100 + (in[pos+44]<<4)*10;
    meteringdata.BoccTimeVoltageMax[0]   =  (in[pos+47]>>4) *10 + in[pos+47]<<4 ;
    meteringdata.BoccTimeVoltageMax[1]   =  (in[pos+46]>>4) *10 + in[pos+46]<<4 ;
    meteringdata.BoccTimeVoltageMax[2]   =  (in[pos+45]>>4) *10 + in[pos+45]<<4 ;
  
    // "B相电压最小值": 浮点型, V // 
    meteringdata.BphaseVoltageMin        =  in[pos+48]>>4 + (in[pos+48]<<4)*0.1 + (in[pos+49]>>4)*100 + (in[pos+49]<<4)*10;
    meteringdata.BoccTimeVoltageMin[0]   =  (in[pos+52]>>4) *10 + in[pos+52]<<4 ;
    meteringdata.BoccTimeVoltageMin[1]   =  (in[pos+51]>>4) *10 + in[pos+51]<<4 ;
    meteringdata.BoccTimeVoltageMin[2]   =  (in[pos+50]>>4) *10 + in[pos+50]<<4 ;
   
   
    // "C相电压最大值": 浮点型, V //
    meteringdata.CphaseVoltageMax        =  in[pos+53]>>4 + (in[pos+53]<<4)*0.1 + (in[pos+54]>>4)*100 + (in[pos+54]<<4)*10;
    meteringdata.CoccTimeVoltageMax[0]   =  (in[pos+57]>>4) *10 + in[pos+57]<<4 ;
    meteringdata.CoccTimeVoltageMax[1]   =  (in[pos+56]>>4) *10 + in[pos+56]<<4 ;
    meteringdata.CoccTimeVoltageMax[2]   =  (in[pos+55]>>4) *10 + in[pos+55]<<4 ;
    
    // "C相电压最小值": 浮点型, V // 
    meteringdata.CphaseVoltageMin        =  in[pos+58]>>4 + (in[pos+58]<<4)*0.1 + (in[pos+59]>>4)*100 + (in[pos+59]<<4)*10;
    meteringdata.CoccTimeVoltageMin[0]   =  (in[pos+62]>>4) *10 + in[pos+62]<<4 ;
    meteringdata.CoccTimeVoltageMin[1]   =  (in[pos+61]>>4) *10 + in[pos+61]<<4 ;
    meteringdata.CoccTimeVoltageMin[2]   =  (in[pos+60]>>4) *10 + in[pos+60]<<4 ;
   
    //"A相电压平均值"： 浮点型, V//  
    meteringdata.AphaseVoltageAverage    =  in[pos+63]>>4 + (in[pos+63]<<4)*0.1 + (in[pos+64]>>4)*100 + (in[pos+64]<<4)*10;
    
    //"B相电压平均值": 浮点型, V //  
    meteringdata.BphaseVoltageAverage    =  in[pos+65]>>4 + (in[pos+65]<<4)*0.1 + (in[pos+66]>>4)*100 + (in[pos+66]<<4)*10;
    
    //"C相电压平均值": 浮点型, V // 
    meteringdata.CphaseVoltageAverage    =  in[pos+67]>>4 + (in[pos+67]<<4)*0.1 + (in[pos+68]>>4)*100 + (in[pos+68]<<4)*10;
    dataSeg->addDailyVoltageStatistics(meteringdata);

}


void DataUnwrapper::DailyNonBalanceTime(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{

    DAILYNONBALANCETIME  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = in[pos+2];
   
    //"电流不平衡度越限日累计时间": 整型, 分钟 //  
    meteringdata.currency_not_balance_minutes    =  (in[pos+3]>>4)*10 + (in[pos+3]<<4) + (in[pos+4]>>4)*10 + (in[pos+4]<<4)*100;
    //"电压不平衡度越限日累计时间": 整型, 分钟//  
    meteringdata.voltage_not_balance_minutes     =  (in[pos+5]>>4)*10 + (in[pos+5]<<4) + (in[pos+6]>>4)*10 + (in[pos+6]<<4)*100;
   

    // "电流不平衡最大值": 浮点型, %//   
    meteringdata.max_unbalanced_currency    =  (in[pos+7]>>4) + (in[pos+7]<<4)*0.1 + ((in[pos+8]>>4)<< 1)*100 + (in[pos+8]<<4)*10;
    meteringdata.occTimeCurrent[0]          =  (in[pos+11]>>4) *10 + in[pos+11]<<4 ;
    meteringdata.occTimeCurrent[1]          =  (in[pos+10]>>4) *10 + in[pos+10]<<4 ;
    meteringdata.occTimeCurrent[2]          =  (in[pos+9]>>4) *10 + in[pos+9]<<4 ;

    //"电压不平衡最大值": 浮点型, %//  
    meteringdata.max_unbalanced_voltage     =  (in[pos+9]>>4) + (in[pos+9]<<4)*0.1 + ((in[pos+10]>>4)<< 1)*100 + (in[pos+10]<<4)*10;
    meteringdata.occTimeVoltage[0]          =  (in[pos+13]>>4) *10 + in[pos+13]<<4 ;
    meteringdata.occTimeVoltage[1]          =  (in[pos+12]>>4) *10 + in[pos+12]<<4 ;
    meteringdata.occTimeVoltage[2]          =  (in[pos+11]>>4) *10 + in[pos+11]<<4 ;

    dataSeg->addDailyNonBalanceTime(meteringdata);    

}



void DataUnwrapper::DailyCurrentStatistics(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{

    METERINGDATA  meteringdata;
    meteringdata.admin_code             = control->getAdmin_code();
    meteringdata.concentrate_no         = control->getconcentrate_no(); 
    meteringdata.measured_point         = dataSeg->getPn();
    int pos = 18;

    meteringdata.metering_icedate[0]    = 20;
    meteringdata.metering_icedate[1]    = in[pos];
    meteringdata.metering_icedate[2]    = in[pos+1];
    meteringdata.metering_icedate[3]    = in[pos+2];
    
    // "A相电流越上上限日累计时间": 整型,  分钟 //  
    meteringdata.AphaseUpUpLimitTime    =  (in[pos+3]>>4) *10 + in[pos+3]<<4 + (in[pos+4]>>4)*1000 + (in[pos+4]<<4)*100;
   
    // "A相电流越上限日累计时间":整型,分钟 //  
    meteringdata.AphaseUpLimitTime      =  (in[pos+5]>>4) *10 + in[pos+5]<<4 + (in[pos+6]>>4)*1000 + (in[pos+6]<<4)*100;
   

    // "B相电流越上上限日累计时间": 整型,  分钟 //  
    meteringdata.BphaseUpUpLimitTime    =  (in[pos+7]>>4) *10 + in[pos+7]<<4 + (in[pos+8]>>4)*1000 + (in[pos+8]<<4)*100;
   
    // "B相电流越上限日累计时间":整型,分钟 //
    meteringdata.BphaseUpLimitTime      =  (in[pos+9]>>4) *10 + in[pos+9]<<4 + (in[pos+10]>>4)*1000 + (in[pos+10]<<4)*100;
  
    // "C相电流越上上限日累计时间": 整型,  分钟 //  
    meteringdata.CphaseUpUpLimitTime    =  (in[pos+11]>>4) *10 + in[pos+11]<<4 + (in[pos+12]>>4)*1000 + (in[pos+12]<<4)*100;

    // "C相电流越上限日累计时间":整型,分钟 //
    meteringdata.CphaseUpLimitTime      =  (in[pos+13]>>4) *10 + in[pos+13]<<4 + (in[pos+14]>>4)*1000 + (in[pos+14]<<4)*100;
  

    // "零序电流越上限累计时间": 整型, 分钟//    
    meteringdata.ZeroSeqCurrentTime     =  (in[pos+15]>>4) *10 + in[pos+15]<<4 + (in[pos+16]>>4)*1000 + (in[pos+16]<<4)*100;


    ///////////////////////////////////////////////////////////////////////////////////
    // "A相电流最大值"：浮点型, A //  
   
    meteringdata.AphaseCurrentMax          =  in[pos+17]>>4 + (in[pos+17]<<4)*0.1 + (in[pos+18]>>4)*100 + (in[pos+18]<<4)*10;
    meteringdata.AoccTime[0]               =  (in[pos+21]>>4) *10 + in[pos+21]<<4 ;
    meteringdata.AoccTime[1]               =  (in[pos+20]>>4) *10 + in[pos+20]<<4 ;
    meteringdata.AoccTime[2]               =  (in[pos+19]>>4) *10 + in[pos+19]<<4 ;
    
    // "B相电流最大值": 浮点型, A // 
    
    meteringdata.BphaseCurrentMax          =  in[pos+22]>>4 + (in[pos+22]<<4)*0.1 + (in[pos+23]>>4)*100 + (in[pos+23]<<4)*10;
    meteringdata.BoccTime[0]               =  (in[pos+26]>>4) *10 + in[pos+26]<<4 ;
    meteringdata.BoccTime[1]               =  (in[pos+25]>>4) *10 + in[pos+25]<<4 ;
    meteringdata.BoccTime[2]               =  (in[pos+24]>>4) *10 + in[pos+24]<<4 ;    
   

     // "C相电流最大值": 浮点型, A //
    meteringdata.CphaseCurrentMax          =  in[pos+27]>>4 + (in[pos+27]<<4)*0.1 + (in[pos+28]>>4)*100 + (in[pos+28]<<4)*10;
    meteringdata.CoccTime[0]               =  (in[pos+31]>>4) *10 + in[pos+31]<<4 ;
    meteringdata.CoccTime[1]               =  (in[pos+30]>>4) *10 + in[pos+30]<<4 ;
    meteringdata.CoccTime[2]               =  (in[pos+29]>>4) *10 + in[pos+29]<<4 ;   
    

    // "零序电流最大值": 浮点型, A  //  
    meteringdata.ZeroSeqCurrentMax         =  in[pos+32]>>4 + (in[pos+32]<<4)*0.1 + (in[pos+33]>>4)*100 + (in[pos+33]<<4)*10;
    meteringdata.ZeroSeqoccTime[0]         =  (in[pos+36]>>4) *10 + in[pos+36]<<4 ;
    meteringdata.ZeroSeqoccTime[1]         =  (in[pos+35]>>4) *10 + in[pos+35]<<4 ;
    meteringdata.ZeroSeqoccTime[2]         =  (in[pos+34]>>4) *10 + in[pos+34]<<4 ;    
    dataSeg->addDailyCurrentStatistics(meteringdata);

}

void DataUnwrapper::ReadConcenstrateInfo(unsigned char in[], int nLen, Data* dataSeg, Control* control)
{
    CONCENTRATEINFO concentrate;
    int pos = 18;

    concentrate.admin_code       = control->getAdmin_code();       //行政区   
    concentrate.concentrate_no   = control->getconcentrate_no();   //终端编号
                             
    concentrate.producer_code[0] = in[pos];                        //厂商代号
    concentrate.producer_code[1] = in[pos+1]; 
    concentrate.producer_code[2] = in[pos+2];  
    concentrate.producer_code[3] = in[pos+3]; 

    for(int i=0; i<8; i++)
        concentrate.device_no[i]    = in[pos+4+i];            //设备编号

    concentrate.software_edition[0] = in[pos+12];             //终端软件版本           
    concentrate.software_edition[1] = in[pos+13];             //终端软件版本           
    concentrate.software_edition[2] = in[pos+14];             //终端软件版本           
    concentrate.software_edition[3] = in[pos+15];             //终端软件版本           
    
    
    concentrate.issued_date[0]= 20;                           //终端软件发布日期：日月年; 
    concentrate.issued_date[1]= in[pos+18]; 
    concentrate.issued_date[2]= in[pos+17]; 
    concentrate.issued_date[3]= in[pos+16]; 

    for(int i=0; i<11; i++)
        concentrate.containersizeInfo[i]    = in[pos+19+i];   // 终端配置容量信息码  

                
    concentrate.protocol[0] = in[pos+30];                    // 通信协议
    concentrate.protocol[1] = in[pos+31];
    concentrate.protocol[2] = in[pos+32];
    concentrate.protocol[3] = in[pos+33];

    concentrate.hardware_edition[0] = in[pos+34];         // 终端硬件版本号 ; 
    concentrate.hardware_edition[1] = in[pos+35];
    concentrate.hardware_edition[2] = in[pos+36];
    concentrate.hardware_edition[3] = in[pos+37];

    concentrate.hardware_issued_date[0] = 20;         // 硬件发布日期    
    concentrate.hardware_issued_date[1] = in[pos+40];
    concentrate.hardware_issued_date[2] = in[pos+39];
    concentrate.hardware_issued_date[3] = in[pos+38];

    dataSeg->addConcentrates(concentrate);
}


void DataUnwrapper::decodeEvent(unsigned char in[], int nLen, Data* dataSeg, Control* control) 
{
    int pos = 18;
      

    if((dataSeg->getFn() == 1 || dataSeg->getFn() == 2) && dataSeg->getPn() == 0)
    {
        dataSeg->SetEC1(in[pos]);
        dataSeg->SetEC2(in[pos+1]);
        dataSeg->SetPm(in[pos+2]);
        dataSeg->SetPn(in[pos+3]);
        pos += 4;
        EVENTDATA evData;
        int EventNum                = dataSeg->getPn() - dataSeg->getPm(); 
        evData.admin_code           = control->getAdmin_code();        //行政区
        evData.concentrate_no       = control->getconcentrate_no();    //集中器编号
        evData.measured_point       = -1;    

        for(int i=0; i<EventNum; i++ )
        {
            evData.event       =  in[pos];
            evData.event_len   =  in[pos+1]; 

            evData.event_datetime[5] =  in[pos+2];
            evData.event_datetime[4] =  in[pos+3];
            evData.event_datetime[3] =  in[pos+4];
            evData.event_datetime[2] =  in[pos+5];
            evData.event_datetime[1] =  in[pos+6];
            evData.event_datetime[0] =  20;

            if(evData.event == EVENT::ERC8 || evData.event == ERC9 || evData.event == ERC10 || evData.event == ERC11 || evData.event == ERC12||  evData.event == ERC13 || evData.event == ERC15 || evData.event == ERC17 || evData.event == ERC18) 
                 evData.measured_point = in[pos+7] << 4;
            else if(evData.event >= EVENT::ERC24 && evData.event <= EVENT::ERC31)
                 evData.measured_point = in[pos+7] << 4;
            else if(evData.event == EVENT::ERC33 || evData.event == EVENT::ERC34 || evData.event == EVENT::ERC38 || evData.event == EVENT::ERC39 || evData.event == EVENT::ERC40 || evData.event == EVENT::ERC41)
                 evData.measured_point = in[pos+7] << 4;

            int j=0;
            for(j=0; j< evData.event_len -5; j++)
            {
                evData.details[j] =  in[pos + 7 + j];
            }

            evData.details[j] = '\0';
            dataSeg->addEventData(evData);
            pos += evData.event_len + 2 ;                        

        }
    }
}




