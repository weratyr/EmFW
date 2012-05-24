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
# makefile for all icm components                                             #
#                                                                             #
# for each subdirectory within the current directory it is assumed            #
# it contains software compontents implementing parts of the application      #
# logic                                                                       #
###############################################################################

OBJS    = $$BINDIR/CContext.o
SUBDIRS = navi main_dispatcher gps admin hmi cd input tuner

include $(PROJECT_ROOT)/src/buildmacros.mk 
include $(PROJECT_ROOT)/src/componenttargets.mk

### main targets ###

all: build_objs all_subdirs

clean: clean_objs clean_subdirs

## eof ##
