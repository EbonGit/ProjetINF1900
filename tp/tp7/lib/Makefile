# Specify extensions of files to delete when cleaning
CLEANEXTS   = o a d
CC = avr-gcc
# Specify the target file and the install directory
OUTPUTFILE  = librairie.a
INSTALLDIR  = ../binaries

MCU=atmega324pa
AVRDUDE_PROGRAMMERID=usbasp
OPTLEVEL=s

# Flags pour le compilateur en C
CFLAGS=-I. -I/usr/include/simavr  -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-std=c++14 -fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall                                        

# Flags pour le compilateur en C++
CXXFLAGS=-fno-exceptions   

# Default target
.PHONY: all
all: $(OUTPUTFILE)

%.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

# Build libjohnpaul.a from john.o, paul.o, and johnpaul.o
$(OUTPUTFILE): librairie.o memoire_24.o
	avr-ar rcs $@ $^

# No rule to build john.o, paul.o, and johnpaul.o from .cpp 
# files is required; this is handled by make's database of
# implicit rules

.PHONY: install
install: $(OUTPUTFILE)
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)
	for file in $(CLEANEXTS); do rm -f *.$$file; done

.PHONY: clean 
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done

# Indicate dependencies of .ccp files on .hpp files
librairie.o: librairie.h
memoire_24.o: memoire_24.h
