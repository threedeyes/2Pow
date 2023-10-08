NAME = 2Pow
TYPE = APP
APP_MIME_SIG = application/x-vnd.twopow-game
SRCS = Application.cpp MainWindow.cpp GameManager.cpp BoardView.cpp Tile.cpp
RDEFS = Resources.rdef
LIBS = be localestub
LOCALES = en ru
OPTIMIZE := FULL
SYMBOLS := FALSE
DEBUGGER := FALSE
DEVEL_DIRECTORY := \
	$(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine
