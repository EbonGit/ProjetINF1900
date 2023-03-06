# extension qui seront clean
CLEANEXTS   = o a d
# compilateur
CC = avr-gcc

# nom et emplacement du fichier .a
OUTPUTFILE  = librairie.a
INSTALLDIR  = ../build

MCU=atmega324pa
AVRDUDE_PROGRAMMERID=usbasp
OPTLEVEL=s

# flags
CFLAGS=-I. -I/usr/include/simavr  -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-std=c++14 -fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall                                        

CXXFLAGS=-fno-exceptions   

.PHONY: all
all: $(OUTPUTFILE)

%.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

# utilisation de avr-ar pour compiler une librairie
$(OUTPUTFILE): librairie.o memoire_24.o debug.o
	avr-ar rcs $@ $^


# la règle install compile la librairie puis crée le dossier build et copie .a à l'intérieur, puis clean
.PHONY: install
install: $(OUTPUTFILE)
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)
	for file in $(CLEANEXTS); do rm -f *.$$file; done

# clean les fichiers avec extensions CLEANEXTS
.PHONY: clean 
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done


librairie.o: librairie.h
debug.o: debug.h
memoire_24.o: memoire_24.h
