/*
 * Flash.h
 *
 *  Created on: 11 juin 2011
 *      Author: !!M
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <windows.h>
#include <gl/gl.h>

class Flash
{
	public:
					Flash		(unsigned char aStep = 255);
		virtual		~Flash		();
		void		spawn		();
		void		render		();
		bool		isAlive		();
		bool		getState	();

	protected:
		const	float	step;
				float	opacity;
				bool	flashState;
				bool	alive;
};

#endif /* FLASH_H_ */
