# Makefile
#----------------------------------------------------------------------------------------
#
#  Project: Aspect 1.00
#
#  License: Boost Software License - Version 1.0 - August 17th, 2003 
#
#            see http://www.boost.org/LICENSE_1_0.txt or the local copy
#
#  Copyright (c) 2017 Sergey Strukov. All rights reserved.
#
#----------------------------------------------------------------------------------------

CCORE_ROOT = ../..

include $(CCORE_ROOT)/Makefile.host

OBJ_PATH = obj

SRC_PATH_LIST = .

TARGET = $(HOME)/bin/CCore-Aspect.exe

CCOPT_EXTRA = -I.

TARGET_DESKTOP = yes

include $(CCORE_ROOT)/Target/Makefile.app

run: $(TARGET)
	$(TARGET)

TypeSet = $(HOME)/bin/CCore-DDLTypeSet.exe

ddl:
	cd src && $(TypeSet) Aspect.ddl Aspect.TypeDef.gen.h Aspect.TypeSet.gen.h

