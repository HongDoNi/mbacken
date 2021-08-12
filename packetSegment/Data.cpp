#include "Segment.h"
    
    
Data::Data(SegmentEnum segmentID): Segment(segmentID)
{
    reset();
}

void Data::reset() 
{
    pn=0;
    fn=0;
    command="";
}


void Data::addDailyForwardWattMetering(DAILYICEWATTMETERING  meteringdata)
{
    dailyice_metering_data。push_back(meteringdata);

}


void Data::addDailyOppositeWattMetering(DAILYOPPOSITEWATTMETERING  meteringdata)
{

    dailyice_opposite_metering_data.push_back(meteringdata);
}


void Data::addDailyForwardMaxDemandWattMetering(DAILYFORWARDMAXDEMANDWATTMETERING  meteringdata)
{

    dailyice_maxdemand_metering_data.push_back(meteringdata);

}


void Data::addDailyOppositeMaxDemandWattMetering(DAILYOPPOSITEMAXDEMANDWATTMETERING  meteringdata)
{

    dailyice_oppositemaxdemand_metering_data.push_back(meteringdata);


}


void Data::addMonthForwardWattMetering(MONTHFORWARDWATTMETERING  meteringdata)
{

    month_forwardwatt_metering_data.push_back(meteringdata);

}


void Data::addMonthOppositeWattMetering(MONTHOPPOSITEWATTMETERING  meteringdata)
{

    month_oppositewatt_metering_data.push_back(meteringdata);

}


void Data::addMonthForwardMaxDemandWattMetering(MONTHFORWARDMAXDEMANDWATTMETERING  meteringdata)
{

    month_maxdemand_metering_data.push_back(meteringdata);

}



void Data::addMonthOppositeMaxDemandWattMetering(MONTHOPPOSITEMAXDEMANDWATTMETERING  meteringdata)
{

    month_opposite_maxdemand_metering_data.push_back(meteringdata);


}


void Data::addDailyLoadMaxMin(DAILYLOADMAXMIN  meteringdata)
{

    dailyice_loadmaxmin.push_back(meteringdata);

}


void Data::addDailyABCPhaseFailureTimes(ABCPHASEFAILURETIMES  meteringdata)
{

    dailyicce_ABCphase_failure_times.push_back(meteringdata);


}


void Data::addDailyVoltageStatistics(DAILYVOLTAGESTATISTICS  meteringdata)
{

    dailyice_ABCphase_nobalance_times.push_back(meteringdata);

}


void Data::addDailyNonBalanceTime(DAILYNONBALANCETIME  meteringdata);
{


    dailyice_voltage_statistics.push_back(meteringdata);
}


void Data::addDailyCurrentStatistics(DAILYCURRENTSTATISTICS  meteringdata)
{

    dailyice_current_statistics.push_back(meteringdata);

}

void Data::addEventData(EVENTDATA evData);
{

    event_data.push_back(evData);

}


void Data::addConcentrates(CONCENTRATEINFO concentrate)
{
    concentrates.push_back(concentrate);

}