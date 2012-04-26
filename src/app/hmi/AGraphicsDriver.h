/***************************************************************************
 *  openICM-application
 ***************************************************************************
 *  Copyright 2010 Joachim Wietzke and Manh Tien Tran
 *
 *  Contact and informations:
 *  University of Applied Sciences Darmstadt, Germany
 *  	http://www.fbi.h-da.de/~j.wietzke
 *  or
 *  	http://www.fbi.h-da.de/~openicm
 *
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  	http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
*****************************************************************************/

#ifndef AGRAPHICSDRIVER_H_
#define AGRAPHICSDRIVER_H_

#ifndef _GLOBAL_H
#include "Global.h"
#endif
#ifndef _CFTRENDER_H_
#include "CFTRender.h"
#endif

typedef void(*PTtoDrawFunction)(void);


class AGraphicsDriver
{
public:
	AGraphicsDriver(Int32 windowWidth, Int32 windowHeight) {
		this->mWindowWidth = windowWidth;
		this->mWindowHeight = windowHeight;
	}

	virtual void init() = 0;
	virtual void run()  = 0;

	virtual void renderText(float pos_x, float pos_y, const char* text, Int32 fontSize = (Int32)CFTRender::FONT_SIZE_DEFAULT);
	virtual void setDrawRoutine(PTtoDrawFunction);
	virtual PTtoDrawFunction getDrawRoutine() {
		return mDrawFunction;
	}

	virtual void finishDraw() = 0;

	Int32 getWindowWidth() const
    {
        return mWindowWidth;
    }

    void setWindowWidth(Int32 windowWidth)
    {
        this->mWindowWidth = windowWidth;
    }

    Int32 getWindowHeight() const
    {
        return mWindowHeight;
    }

    void setWindowHeight(Int32 windowHeight)
    {
        this->mWindowHeight = windowHeight;
    }



private:
    void (*mDrawFunction)(void);
    Int32 mWindowWidth;
    Int32 mWindowHeight;

	static CFTRender mRender;
};

#endif /* AGRAPHICSDRIVER_H_ */
