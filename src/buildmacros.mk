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
# buildmacros.mk                                                              #
#                                                                             #
# some useful helper rules/macros to be included by Makefiles                 #
###############################################################################

############################################################
# determine the subdirs within current working dir and     #
# save to var SUBDIRS                                      #
############################################################
DEFAULT_SUBDIRS	= $(shell find . -maxdepth 1 -mindepth 1 -type d)

############################################################
# rule to delegate target 'all' to all defined subdirs     #
#  SUBDIRS    = dirs to perform make 'all' within          #                            #
############################################################
all_subdirs:
	@for i in $(SUBDIRS) ; do \
		maketarget=all subdir=$$i $(MAKE) delegate_make ; \
	done

############################################################
# rule to delegate target 'clean' to all defined subdirs   #
#  SUBDIRS    = dirs to perform make 'clean' within        #                            #
############################################################
clean_subdirs:
	@for i in $(SUBDIRS) ; do \
		maketarget=clean subdir=$$i $(MAKE) delegate_make ; \
	done

############################################################
# rule to delegate a given target to a given subdir        #
#  subdir     = dir to change into before performing make  #
#  maketarget = target to make                             #
############################################################
delegate_make:
	@if [ ! -r "$(subdir)/Makefile" ] ; then \
		echo "!!! internal error: no Makefile in [$(subdir)] available" ; \
	else \
		echo "<<< make [$(maketarget)] in [$(subdir)]" ; \
		(cd $(subdir) && $(MAKE) $(ICM_MFLAGS) $(maketarget)) ; \
	fi ; \
	echo ""

## eof ##
