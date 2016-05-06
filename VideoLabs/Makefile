# Makefile
#----------------------------------------------------------------------------------------
#
#  Project: VideoLabs
#
#  License: Boost Software License - Version 1.0 - August 17th, 2003 
#
#            see http://www.boost.org/LICENSE_1_0.txt or the local copy
#
#  Copyright (c) 2016 Sergey Strukov. All rights reserved.
#
#----------------------------------------------------------------------------------------

OPT = -j 8

all:
	make $(OPT) -C DrawLab
	make $(OPT) -C FontLab
	make $(OPT) -C Life
	make $(OPT) -C ShapeLab
	make $(OPT) -C Test
	make $(OPT) -C DrawTest

clean:
	make -C DrawLab       clean
	make -C FontLab       clean
	make -C Life          clean
	make -C ShapeLab      clean
	make -C Test          clean
	make -C DrawTest      clean

list:
	make -C DrawLab       list
	make -C FontLab       list
	make -C Life          list
	make -C ShapeLab      list
	make -C Test          list
	make -C DrawTest      list
