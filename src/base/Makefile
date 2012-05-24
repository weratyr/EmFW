############################################################################
#  openICM
############################################################################
#  Copyright 2011 Joachim Wietzke
#
#  Contact and informations:
#  University of Applied Sciences Darmstadt, Germany
#  	http://www.fbi.h-da.de/~j.wietzke
#  or
#  	http://www.fbi.h-da.de/~openicm
#
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  	http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
############################################################################
###############################################################################
# makefile to build the framework base classes                                #
###############################################################################

OBJS = $$BINDIR/AComponentBase.o \
$$BINDIR/CBinarySemaphore.o \
$$BINDIR/CCommQueue.o \
$$BINDIR/CComponentContext.o \
$$BINDIR/CDispatcher.o \
$$BINDIR/CMutex.o \
$$BINDIR/CThread.o \

include $(PROJECT_ROOT)/src/componenttargets.mk

all: build_objs

clean: clean_objs

## eof ##
