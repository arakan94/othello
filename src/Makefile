# Makefile for subdir src
# FIT VUT, ICP, hra2016
# Author: David Novák <xnovak1m>
# Date: 9. 5. 2016

all: dep.list $(NAME)-cli $(NAME)

dep: dep.list

dep.list:
	$(CC) -MM *.$(EXT) >dep.list

-include dep.list

%.o : %.$(EXT)
	$(CC) $(CFLAGS) -c $<

$(NAME)-cli: $(OBJFILES) $(CLIOBJ)
	$(CC) $(CFLAGS) $(OBJFILES) $(CLIOBJ) -o ../$@

$(NAME): icp16.pro
	qmake -o Makefile.QT
	make -f Makefile.QT

