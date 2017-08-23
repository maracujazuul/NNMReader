/*
 * NOTE: this source was taken from an nvidia demo
 *
 * i haven't enought time to write all component
 * of this little framework
 */
#ifndef TIMER_H
#define TIMER_H

#include "timer.h"
#include <windows.h>

class Timer
{
	public:
				Timer			(void);
		double	begin			(void);
		double	end				(void);
		void	clearTotalTime	(void);
		double	getTotalTime	(void) const;

		double	tick(void);
		double	time(void);

	protected:
		double	_callTime;
		double	_frequency;
		double	_lastTime;
		double	_totalTime;

		double	convert(const LARGE_INTEGER& value);
};

#endif

