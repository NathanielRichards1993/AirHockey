#ifndef MY_TIME_H
#define MY_TIME_H

namespace clockFunction
{
	class _declspec(dllexport) MyTime
	{
		public:
			 void timeFill(unsigned short* hour, unsigned short* minute, unsigned short* seconds);

			 unsigned short plusTimeMinutes
				(unsigned short plusValue,unsigned short* fHour, unsigned short* fMinute);

			 unsigned short plusTimeSeconds(
				unsigned short plusValue,unsigned short* min, unsigned short* sec);


			 unsigned short minusTime(
				unsigned short minusValue,unsigned short* minute, unsigned short* seconds);


			 long differnceInSeconds(unsigned short hourA,unsigned short minuteA, unsigned short secondsA,
										unsigned short hourB,unsigned short minuteB,unsigned short secondsB);


			 short differnceInMinutes(unsigned short hourA, unsigned short minuteA,
										unsigned short hourB,unsigned short minuteB);
		private:
	};
}
#endif