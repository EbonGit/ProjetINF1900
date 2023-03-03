CLEANEXTS   = o a d
CC = avr-gcc

OUTPUTFILE  = liblibrairie.a
INSTALLDIR  = ../

MCU=atmega324pa
AVRDUDE_PROGRAMMERID=usbasp
OPTLEVEL=s

CFLAGS=-I. -I/usr/include/simavr  -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-std=c++14 -fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall                                        

CXXFLAGS=-fno-exceptions   

.PHONY: all
all: $(OUTPUTFILE)

%.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

$(OUTPUTFILE): librairie.o memoire_24.o
	avr-ar rcs $@ $^



.PHONY: install
install: $(OUTPUTFILE)
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)
	for file in $(CLEANEXTS); do rm -f *.$$file; done

.PHONY: clean 
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done

librairie.o: librairie.h
memoire_24.o: memoire_24.h
