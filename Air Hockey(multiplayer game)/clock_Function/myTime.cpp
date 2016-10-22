#include"myTime.h"

#include<time.h>
#include<iostream>

namespace clockFunction
{
	/*First 6 signifcant figures are hours.mins.sec no space betweem numbers*/
	void  MyTime::timeFill(unsigned short* hour, unsigned short* minute, unsigned short* seconds)
	{
		time_t tock;
		struct tm * timeinfo;
		char tempTime[26] = "\0";

		time(&tock);
		timeinfo = localtime(&tock);
		sprintf(tempTime,"%s",asctime(timeinfo));

		*hour = ((short)tempTime[11]- 48)*10 +
			((short)tempTime[12]- 48);
		*minute = ((short)tempTime[14]- 48)*10 +
			((short)tempTime[15]- 48);
		*seconds = ((short)tempTime[17]- 48)*10 +
			((short)tempTime[18]- 48);
	}

	/*plues value must be less than 1440,to large response 2, response 1 is the same day, response 22 is next day*/
	unsigned short MyTime::plusTimeMinutes(unsigned short plusValue,unsigned short* fHour, unsigned short* fMinute)
	{
	
		unsigned short minutesInHours = *fHour*60;
		unsigned short allTheMinutes = minutesInHours+*fMinute;
		if(plusValue < 1440)
		{
			allTheMinutes += plusValue;
			*fHour = allTheMinutes/60;
			*fMinute = allTheMinutes-*fHour*60;
			if(*fHour > 23)
			{
				//plus 1 day response
				*fHour-=24;
				return 2;
			}
			else
			{
				return 1;
			}

		}
		else
		{
			return 22;
		}
	}

	/*plues value must be less than 3600,to large response 22, response 1 is the same hour, response 2 is next hour*/
	unsigned short MyTime::plusTimeSeconds(unsigned short plusValue,unsigned short* min, unsigned short* sec)
	{
		unsigned short secondsInMinutes = *min*60;
		unsigned short allTheSec = secondsInMinutes+*sec;
		if(plusValue < 3600)
		{
			allTheSec += plusValue;
			*min = allTheSec/60;
			*sec = allTheSec-*min*60;
			if(*min > 59)
			{
				//plus 1 day response
				*min-=60;
				return 2;
			}
			else
			{
				return 1;
			}

		}
		else
		{
			return 22;
		}
	}

	/*amount to minus return 1 is good, 22 is to large minus value*/
	unsigned short MyTime::minusTime(unsigned short minusValue,unsigned short* minute, unsigned short* seconds)
	{
		unsigned short secondsInMinutes = *minute*60;
		unsigned short allTheSec = secondsInMinutes+*seconds;
		if(minusValue <= allTheSec)
		{
			allTheSec -= minusValue;
			*minute = allTheSec/60;
			*seconds = allTheSec-*minute*60;
			return 1;
		}
		else
		{
			return 22;
		}
	}
	/*for positive number larger values first*/
	long MyTime::differnceInSeconds(unsigned short hourA,unsigned short minuteA, unsigned short secondsA,
									  unsigned short hourB,unsigned short minuteB,unsigned short secondsB)
	{
		if(hourA > 23 || hourB >23)
		{return -86401;}
		if(minuteA > 59 || minuteB > 59)
		{return -86402;}
		if( secondsA > 59 || secondsB > 59)
		{return -86403;}

		long secondsFromHours = (hourA-hourB)*60*60;		
		short secondsFromMinutes = (minuteA - minuteB)*60;
		short sec = secondsA-secondsB;
		secondsFromHours += secondsFromMinutes+ sec;
		return secondsFromHours;
	}
	/*for positive number larger values first*/
	short MyTime::differnceInMinutes(unsigned short hourA, unsigned short minuteA,
									  unsigned short hourB,unsigned short minuteB)
	{
		if(hourA > 23 ||hourB > 23)
		{return 1441;}
		else if(minuteA > 59 || minuteB > 59)
		{return 1442;}
		
		short minutesFromHours = (hourA - hourB)*60;
		short min = minuteA-minuteB;
		return (minutesFromHours+min);
	}
}