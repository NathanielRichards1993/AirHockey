#include<gtest\gtest.h>
#include<iostream>
#include<clock_Function\myTime.h>
using clockFunction::MyTime;
using namespace std;

TEST(myTime,timeFill)
{
	unsigned short hour = 0;
	unsigned short minute = 0;
	unsigned short seconds = 0;


	MyTime tick;
	tick.timeFill(&hour,&minute,&seconds);
	EXPECT_GE(hour,0) <<" value of hour is : "<< hour;
	EXPECT_LE(hour,23) <<" value of hour is : "<< hour;
	
	EXPECT_GE(minute,0) <<" value of minute is : "<< minute;
	EXPECT_LE(minute,59) <<" value of minute is : "<< minute;

	EXPECT_GE(seconds,0) <<" value of hour is : "<< seconds;
	EXPECT_LE(seconds,59) <<" value of secpmds is : "<< seconds;

	cout << "time is: " << hour <<":"<<minute<<":"<<seconds<<endl;

}

TEST(myTime,differenceInSeconds)
{
	unsigned short hourA = 0;
	unsigned short minuteA = 0;
	unsigned short secondA = 0;

	unsigned short hourB = 0;
	unsigned short minuteB = 0;
	unsigned short secondB = 0;

	long rValue = 0;

	MyTime tick;
	rValue = tick.differnceInSeconds(hourA,minuteA,secondA,hourB,minuteB,secondB);
	EXPECT_EQ(rValue,0) << "0 value not working";

	hourA = 23;
	minuteA = 59;
	secondA = 59;

	//extreme positive
	rValue = tick.differnceInSeconds(hourA,minuteA,secondA,hourB,minuteB,secondB);
	EXPECT_EQ(rValue,86399) << "extreme positive difference not working value:" << rValue;

	//extreme negative
	rValue = tick.differnceInSeconds(hourB,minuteB,secondB,hourA,minuteA,secondA);
	EXPECT_EQ(rValue,-86399) << "extreme negative difference not working value:" << rValue;

	//=======================================================================

	//just for peace of mind resetting values to 0
	hourA = 0;
	minuteA = 0;
	secondA = 0;

	//=======================================================================

	//minse hour value
	rValue = tick.differnceInSeconds(-1,minuteA,secondA,hourB,minuteB,secondB);
	EXPECT_EQ(rValue,-86401) << "extreme positive difference not working value:" << rValue;

	//minse hour value
	rValue = tick.differnceInSeconds(hourA,minuteA,secondA,-1,minuteB,secondB);
	EXPECT_EQ(rValue,-86401) << "extreme positive difference not working value:" << rValue;


	//hour value to large value
	rValue = tick.differnceInSeconds(24,minuteA,secondA,hourB,minuteB,secondB);
	EXPECT_EQ(rValue,-86401) << "extreme positive difference not working value:" << rValue;

	//hour value to large hour value
	rValue = tick.differnceInSeconds(hourA,minuteA,secondA,24,minuteB,secondB);
	EXPECT_EQ(rValue,-86401) << "extreme positive difference not working value:" << rValue;

	//======================================================================

	//minse hour value
	rValue = tick.differnceInSeconds(hourA,-1,secondA,hourB,minuteB,secondB);
	EXPECT_EQ(rValue,-86402) << "extreme positive difference not working value:" << rValue;

	//minse hour value
	rValue = tick.differnceInSeconds(hourA,minuteA,secondA,hourB,-1,secondB);
	EXPECT_EQ(rValue,-86402) << "extreme positive difference not working value:" << rValue;


	//hour value to large value
	rValue = tick.differnceInSeconds(hourA,61,secondA,hourB,minuteB,secondB);
	EXPECT_EQ(rValue,-86402) << "extreme positive difference not working value:" << rValue;

	//hour value to large hour value
	rValue = tick.differnceInSeconds(hourA,minuteA,secondA,hourB,61,secondB);
	EXPECT_EQ(rValue,-86402) << "extreme positive difference not working value:" << rValue;

	//======================================================
	
	//minse hour value
	rValue = tick.differnceInSeconds(hourA,minuteA,-1,hourB,minuteB,secondB);
	EXPECT_EQ(rValue,-86403) << "extreme positive difference not working value:" << rValue;

	//minse hour value
	rValue = tick.differnceInSeconds(hourA,minuteA,secondA,hourB,minuteB,-1);
	EXPECT_EQ(rValue,-86403) << "extreme positive difference not working value:" << rValue;


	//hour value to large value
	rValue = tick.differnceInSeconds(hourB,minuteA,61,hourB,minuteB,secondB);
	EXPECT_EQ(rValue,-86403) << "extreme positive difference not working value:" << rValue;

	//hour value to large hour value
	rValue = tick.differnceInSeconds(hourA,minuteA,secondA,hourB,minuteB,61);
	EXPECT_EQ(rValue,-86403) << "extreme positive difference not working value:" << rValue;

	//========================================================

}

TEST(myTime,differnceInMinutes)
{
	MyTime time;

	unsigned short hourA = 0;
	unsigned short minuteA = 0;
	unsigned short hourB = 0;
	unsigned short minuteB = 0;

	unsigned rValue = 0;
	
	//low
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,0) << "difference between minute ZERO not working. Value:"<<rValue;
	
	hourA = 23;
	minuteA = 59;

	//high
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,1439) << "difference between minute MAX not working. Value:"<<rValue;

	//================================================

	//hour test
	hourA = -1;
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,1441) << "difference between minute -1 not working. Value:"<<rValue;
	hourA = 24;
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,1441) << "difference between minute TO BIG not working. Value:"<<rValue;
	hourA = 0;

	hourB = -1;
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,1441) << "difference between minute -1 not working. Value:"<<rValue;
	hourB = 24;
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,1441) << "difference between minute TO BIG not working. Value:"<<rValue;
	hourB = 0;

	//===============================================

	//minute test
	minuteA = -1;
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,1442) << "difference between minute -1 not working. Value:"<<rValue;
	minuteA = 60;
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,1442) << "difference between minute TO BIG not working. Value:"<<rValue;
	minuteA = 0;

	minuteB = -1;
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,1442) << "difference between minute -1 not working. Value:"<<rValue;
	minuteB = 60;
	rValue = time.differnceInMinutes(hourA,minuteA,hourB,minuteB);
	EXPECT_EQ(rValue,1442) << "difference between minute TO BIG not working. Value:"<<rValue;
	minuteB = 0;
	
}

TEST(myTime,minusSeconds)
{
	unsigned short minute = 0;
	unsigned short seconds = 0;
	short minusValue = 0;

	unsigned short rValue = 0;

	MyTime myTime;

	rValue = myTime.minusTime(

}