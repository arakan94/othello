Project:  Othello, ICP16 project (VUT FIT)
Authors:  David Novák, Roman Ondraszek
Version:  1.0
Date:     9. 5. 2016

Description
-----------
Project implements board game Othello (also known as Reversi) using C++11, OO design and several design patterns.

It allows to run several instances of application, save/load games, use undo/redo and play against AI player (two difficulties avalible). Goal was to separate game logic from UI using common interface. Documentation is generated directly from code using doxygen.

Source code can be found in src/ directory. Two save games from nearly finished game are located in examples/ directory (for testing purposes).

How to use
----------
Project requires g++ with c++11 support and GNU Make. For GUI, properly set QT Toolkit 5 or higher is required.

After compilation, two binaries are produced - hra2016 (GUI) and hra2016-cli (CLI).

make         - compiles project
make run     - compiles and runs both CLI and GUI versions of game
make clean   - deletes outcome of compilation (including binaries)
make pack    - packs everything needed for compilation into zip archive
make doxygen - generates documentation (output in doc/html)
make metrics - counts lines in .cc and .h files

Team
----
David Novák     - game logic, build system, managing git
Roman Ondraszek - CLI, GUI in QT Toolkit

Notes
-----
For some reason, loading a save from subdir in CLI (on merlin) doesn't work. GUI is fine. On my system, everything works and sadly, there is no time left to find the cause.
