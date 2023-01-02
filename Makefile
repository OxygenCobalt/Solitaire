TARGET = FP
SRC_FILES = Card.cpp Column.cpp Deck.cpp Foundation.cpp History.cpp \
	main.cpp Selection.cpp Solitaire.cpp SolitaireInfo.cpp

CXX = g++
CFLAGS = -Wall -g -std=c++11

OBJECTS = $(SRC_FILES:.cpp=.o)

ifeq ($(OS),Windows_NT)
	TARGET := $(TARGET).exe
	DEL = del

	INC_PATH = Z:/CSCI200/include/
	LIB_PATH = Z:/CSCI200/lib/

	ARCH = 
else
	DEL = rm

	INC_PATH = /usr/local/include/
	LIB_PATH = /usr/local/lib/

	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CCFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		TARGET_MAKEFILE = Makefile.osx
		CCFLAGS += -D OSX
	endif

	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		ARCH = 
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		ARCH = 
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		ARCH = -arch x86_64
	endif
endif

LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(ARCH) -o $@ $^ -L$(LIB_PATH) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(ARCH) -o $@ -c $< -I$(INC_PATH)

clean:
	$(DEL) $(TARGET) $(OBJECTS)

# DEPENDENCIES
Card.o: Card.cpp Card.h CardRank.h CardSuit.h
Column.o: Column.cpp Column.h Card.h CardRank.h CardSuit.h Deck.h \
	ISelectable.h Selection.h IDroppable.h
Deck.o: Deck.cpp Deck.h Card.h CardRank.h CardSuit.h ISelectable.h \
 	Selection.h
Foundation.o: Foundation.cpp Foundation.h Card.h CardRank.h CardSuit.h \
 	IDroppable.h ISelectable.h Selection.h
History.o: History.cpp History.h SolitaireInfo.h Statistics.h
main.o: main.cpp History.h SolitaireInfo.h Statistics.h Selection.h \
 	Card.h CardRank.h CardSuit.h Solitaire.h Deck.h ISelectable.h Column.h \
 	IDroppable.h Foundation.h
Selection.o: Selection.cpp Selection.h Card.h CardRank.h CardSuit.h
Solitaire.o: Solitaire.cpp Solitaire.h Deck.h Card.h CardRank.h \
 	CardSuit.h ISelectable.h Selection.h Column.h IDroppable.h Foundation.h \
 	SolitaireInfo.h
SolitaireInfo.o: SolitaireInfo.cpp SolitaireInfo.h

.PHONY: all clean