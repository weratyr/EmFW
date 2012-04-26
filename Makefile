############################################################################
#  openICM
############################################################################
#  Copyright 2010 Joachim Wietzke and Manh Tien Tran
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
# makefile to setup global build environment for supported platforms          #
# and link the binaries builded within sub directories                        #
###############################################################################

# Makefile global settings: 
CPUS			= $(shell cat /proc/cpuinfo  | grep MHz | wc -l)
PARALLELPROC		= $(shell echo ${CPUS}*2 | bc)
PROJECT_ROOT  		= $(shell pwd)
TARGET			= unknown
PROG          		= Framework
DEBUGFLAGS    		= -g #-DAUTO_TERMINATE
COMMON_INCLUDES		= -I$(PROJECT_ROOT)/src/base -I$(PROJECT_ROOT)/src/app
CFLAGS 			= -Wall -DDEBUG_MODE
#MUDFLAP		= 1

# ----- TARGET SPECIFIG SETTINGS (compile and link) -----
# Targets are: 
# 1. Linux x86
# 2. QNX (v. 641) x86
# 3. Linux (v. 632) sh4

TARGETS = LINUX_X86 QNX_X86
BINDIR	= $(PROJECT_ROOT)/bin/$(TARGET)

ICM_ENV=/opt/icm_env_00/host

# check if the environment for QNX is available
#  if not - only build linux by default
have_icm_env := $(wildcard $(ICM_ENV))
ifeq ($(strip $(have_icm_env)),)
TARGETS = LINUX_X86
endif

# vars for packing a archive
TIMESTAMP	= $(shell date "+%Y%m%d_%H%M%S")
VERSION		= openICM_$(TIMESTAMP)
SHIPPINGDIR	= $(VERSION)
ARCHIVE		= $(PROJECT_ROOT)/$(VERSION).tar.gz

ifeq ($(TARGET), LINUX_X86) 
	CC 		= g++
	LD 		= g++
	LIBS		= -lrt -lpthread -lGL -lglut -lfreetype 
	CFLAGS		+= -DLINUX  $(DEBUGFLAGS)
ifeq ($(MUDFLAP), 1)
	LIBS		+= -lmudflap
	CFLAGS		+= -fmudflap
endif
	LDFLAGS		= $(DEBUGFLAGS)
	INCLUDES	= $(COMMON_INCLUDES) -I/usr/include/freetype2
	QNX_TARGET	=
	QNX_HOST	=
endif # LINUX_X86

ifeq ($(TARGET), QNX_X86)
	QNX_VERSION	=641
	QNX_ROOT=/opt/qnx$(QNX_VERSION)
	QNXBIN=$(QNXROOT)/host/linux/x86/usr/bin
	QNX_TARGET=$(QNX_ROOT)/target/qnx6
	QNX_HOST=$(QNX_ROOT)/host/linux/x86

	CC 		= $(QNX_HOST)/usr/bin/QCC
	LD 		= $(QNX_HOST)/usr/bin/QCC
	LIBS		= -lgf -lGLES_CM $(ICM_ENV)/qnx$(QNX_VERSION)/lib/libfreetype.a -lz 
	CFLAGS		+= -Vgcc_ntox86 -Y_ecpp-ne -DQNX -DEGL $(DEBUGFLAGS)
	LDFLAGS		= $(DEBUGFLAGS) $(BUILDTYPE)
	INCLUDES	= $(COMMON_INCLUDES) -I$(ICM_ENV)/qnx$(QNX_VERSION)/include
endif # QNX_X86

## setup make flags ##
ICM_MFLAGS += ICMBUILD=1
ICM_MFLAGS += MFLAGS="$(MFLAGS)"
ICM_MFLAGS += CC="$(CC)"
ICM_MFLAGS += LD="$(LD)"
ICM_MFLAGS += CFLAGS="$(CFLAGS)"
ICM_MFLAGS += LDFLAGS="$(LDFLAGS)"
ICM_MFLAGS += BINDIR="$(BINDIR)"
ICM_MFLAGS += INCLUDES="$(INCLUDES)"
ICM_MFLAGS += PROJECT_ROOT="$(PROJECT_ROOT)"

## main targets ##

## 
# All Targets
all: 
	@echo 
	@echo "Now perform build for all platforms ..."
	@echo 
	@for i in $(TARGETS); do \
	echo "   ... build target $$i"; \
	($(MAKE) $$i > /dev/null); done
	@echo
	@echo "Finished build for all platforms"
	
LINUX_%:
	$(MAKE) $(PROG) TARGET=$@

QNX_%:
	$(MAKE) $(PROG) TARGET=$@

# Linking Objects and Libs
$(PROG): check recursive_all
	@echo 'Invoking linker for [$@]'
	@cd $(BINDIR);  $(LD) $(LDFLAGS) $(LIBDIR) -o $(BINDIR)/$(PROG) $(BINDIR)/*.o $(LIBS)
	@echo 'Finished linking target [$@]'
	@echo ' '

# Other Targets
clean: recursive_clean
	rm -rf $(PROJECT_ROOT)/bin/* openICM_* doc/html
	@echo

# build
recursive_all:
	@(cd src && $(MAKE) -j$(PARALLELPROC) $(ICM_MFLAGS) all)
	
# clean
recursive_clean:
	@(cd src && $(MAKE) $(ICM_MFLAGS) clean)

# Printing the environment settings
check: 
	@echo 
	@echo " ---- Environment Settings ------ "
	@echo "      Target             [$(TARGET)]"
	@echo "      Compiler           [$(CC)]"
	@echo "      Linker             [$(LD)]"
	@echo "      Includes           [$(INCLUDES)]"
	@echo "      Cores Available    [$(CPUS)]"
	@echo "      Parallel Tasks     [$(PARALLELPROC)]"
	@echo "      Binaries           [$(BINDIR)]"	
	@echo "      QNX TARGET         [$(QNX_TARGET)]"
	@echo "      QNX HOST           [$(QNX_HOST)]"
	@echo "      PATH               [$(PATH)]"
	@echo " -------------------------------- " 
	@echo 

	
# bundle a distribution package	
dist: clean
	@echo "Preparing distribution"
	@echo "Clean up target directory $(SHIPPINGDIR) ..."
	rm -rf $(SHIPPINGDIR)
	mkdir -p $(SHIPPINGDIR)
	cp -pr ./src Makefile README LICENSE $(SHIPPINGDIR)/
	tar --no-same-owner -czf $(ARCHIVE) $(SHIPPINGDIR)
	@rm -rf $(SHIPPINGDIR)

# documentation
docu: misc/Doxyfile
	@doxygen misc/Doxyfile
	@echo
	@echo "check [$(PROJECT_ROOT)/doc/html/index.html] for documentation"
	@echo

.PHONY: clean check dist docu
## eof ##
