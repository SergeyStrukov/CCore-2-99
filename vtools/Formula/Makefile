# Makefile
#----------------------------------------------------------------------------------------
#
#  Project: ContourLab 1.00
#
#  License: Boost Software License - Version 1.0 - August 17th, 2003
#
#            see http://www.boost.org/LICENSE_1_0.txt or the local copy
#
#  Copyright (c) 2017 Sergey Strukov. All rights reserved.
#
#----------------------------------------------------------------------------------------

CCORE_ROOT = ../..

LANG = $(HOME)/bin/CCore-CondLangLR1.exe

TEST = $(CCORE_ROOT)/tools/CondLangLR1/BFTest/BFTest.exe 

Formula.ddl: Formula.lang
	$(LANG) Formula.lang

test: Formula.ddl
	$(TEST) Formula.ddl

#----------------------------------------------------------------------------------------

include $(CCORE_ROOT)/Makefile.host

OBJ_PATH = obj

SRC_PATH_LIST = .

TARGET = ./gen.exe

include $(CCORE_ROOT)/Target/Makefile.app

gen: $(TARGET)

run: $(TARGET) Formula.ddl
	$(TARGET)



