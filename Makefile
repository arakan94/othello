# Makefile
# FIT VUT, ICP, hra2016
# Author: David Novák <xnovak1m>
# Date: 9. 5. 2016

# these variables are exported to Makefile in src/
export NAME=hra2016

# common modules
export OBJFILES=othelier.o field.o history.o ai_simple.o ai_easy.o command.o action.o game.o player.o

# modules used only by CLI
export CLIOBJ=$(NAME)-cli.o ui_cli.o

export CC=g++
export CFLAGS= -std=c++11 -pedantic -Wall -Wextra -g

export EXT=cc

all: build

build:
	$(MAKE) -C src  # recursive make for subdir src

run: all
	./$(NAME)-cli
	./$(NAME)

clean:
	rm -f src/*.o src/*~ src/dep.list src/moc_button.cpp src/moc_ui_gui.cpp src/Makefile.QT  $(NAME) $(NAME)-cli xnovak1m-xondra43.zip
	rm -rf doc/html

pack:
	zip xnovak1m-xondra43 src/*.$(EXT) src/*.h src/*.dox src/icp16.pro examples/* doc Makefile src/Makefile Doxyfile README.txt contributors.txt

doxygen:
	doxygen Doxyfile

metrics:
	find src/ -name '*.cc' | xargs wc -l
	find src/ -name '*.h' | xargs wc -l
