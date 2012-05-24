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
# componenttargets.mk                                                         #
#                                                                             #
# make targets to be included by the components                               #
###############################################################################

COMPONENT_INCLUDES=$(shell find $(PROJECT_ROOT)/src/app -maxdepth 1 -mindepth 1 -type d | awk '{printf "-I%s ", $$1 }')

build_objs: $(OBJS)

clean_objs:
	rm -f $(OBJS)

$$BINDIR/%.o: %.cpp
	@echo '>>>> building file: $<'
	@if [ -n "$(BINDIR)" ] ; then \
		mkdir -p $(BINDIR) ; \
	fi
	@if [ -n "$(ICMBUILD)" ] ; then \
		$(CC) $(CFLAGS) $(INCLUDES) $(COMPONENT_INCLUDES) -c -o $@ $< ; \
		echo '>>>> finished building: $<' ; \
	else \
		echo '!!!!! environment is missing (please issue build from project root)' ; \
	fi
	@echo

.PHONY = clean_objs

## eof ##
