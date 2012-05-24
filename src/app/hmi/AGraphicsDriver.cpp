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

#include "AGraphicsDriver.h"

CFTRender AGraphicsDriver::mRender(10000);

void AGraphicsDriver::renderText(float pos_x, float pos_y, const char* text, Int32 fontSize)
{
	DEBUG_PRINT("entered for [%s]",text)

	mRender.setFont(CFTRender::NORMAL_NPROP);
	mRender.setFontSize(fontSize);
	mRender.setForegroundColor(1, 1, 1, 1); //white with alpha //TODO prio 5 :: set color depending on current gl color
	mRender.drawString(text, pos_x, pos_y);
}

void AGraphicsDriver::setDrawRoutine(void(*pt2drawFunction)())
{
	mDrawFunction = pt2drawFunction;
}

