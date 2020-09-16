#######################################
# Template-Makefile for SVN-structure #

# To Modify:
#
# PROGNAMES: should contain the (base-) name(s) of the file containing the "main"-function(s)
# HEADERSSHORT: should contain the (base-) name(s) of the included c-Files 
# Eventually some more mkdirs repsctve rms are needed in subdirectories...
###
# Output:
#
# Objects: in $(OBJPATH)
# Libraries: in $(LIBPATH)
# Binaries: in $(BINPATH)

SHELL = /bin/sh
# Optimize-Levels for Compiler and Linker:
CFLAGS = -Wall -O3 -g -pedantic -fomit-frame-pointer -Wno-long-long
LFLAGS = -lntl -lgmp -lm
CXX = g++
PRAEPROCCMD =

#Paths
# SOURCEPATH should match "./[dir]" or ".", dir mustn't be empty, NO "/" at the end
SOURCEPATH = ./src
SOURCEBODY = $(notdir $(SOURCEPATH))
SEDSOURCEBODY = $(SOURCEBODY)
OBJSOURCEPATH = $(SOURCEPATH)
ifeq ($(strip $(SEDSOURCEBODY)),.)
SEDSOURCEBODY = "\."
OBJSOURCEPATH = "./"
endif
# OBJPATH with same rules as SOURCEPATH
OBJPATH = ./obj
OBJBODY = $(notdir $(OBJPATH))
SEDOBJBODY = $(OBJBODY)
OBJOBJPATH = $(OBJPATH)
ifeq ($(strip $(SEDOBJBODY)),.)
SEDOBJBODY = "\.\/"
endif
ifeq ($(SEDSOURCEBODY),"\.")
OBJOBJPATH = $(OBJPATH)/
ifeq ($OBJBODY,.)
SEDOBJBODY = "\.\/"
else
SEDOBJBODY = "\.\/"$(OBJBODY)
endif
endif
# LIBPATH with same rules as SOURCEPATH
LIBPATH  = ./lib
LIBBODY  = $(notdir $(LIBPATH))
BINPATH  = ./bin
INCPATHS = -I$(SOURCEPATH)

####### Files
DEPENDFILE   = $(OBJPATH)/.depend
LIBNAME      = $(LIBPATH)/commonlib.a
PROGNAMES    = tlpairing
PROGS        = $(addprefix $(BINPATH)/,$(PROGNAMES))
PROGOBJECTS  = $(addsuffix .o,$(addprefix $(OBJPATH)/,$(PROGNAMES)))
PROGSOURCES  = $(addsuffix .c,$(addprefix $(SOURCEPATH)/,$(PROGNAMES)))
HEADERSSHORT = algorithms curve curvepoint_fp curvepoint_fpk fp fpk init points utils weil_tate
HEADERS      = $(addprefix $(SOURCEPATH)/, $(addsuffix .h, $(HEADERSSHORT)))
SOURCES      = $(HEADERS:.h=.c)
OBJECTS      = $(addprefix $(OBJPATH)/, $(addsuffix .o, $(HEADERSSHORT)))

####### Implicit rules

$(OBJPATH)/%.o: $(SOURCEPATH)/%.c $(OBJPATH)/cpucycles.o $(SOURCEPATH)/cpucycles.h
	$(CXX) $(CFLAGS) -c $(PRAEPROCCMD) $(INCPATHS) $< -o $(OBJPATH)/$(notdir $@) 
$(BINPATH)/% : $(LIBNAME) $(OBJPATH)/$(notdir %).o
	$(CXX) $(OBJPATH)/$(notdir $@).o $(LIBNAME) -o $@ $(LFLAGS) $(PRAEPROCCMD)
#	strip $@

####### Build rules
all: $(DEPEND) $(OBJECTS) $(PROGS)

$(DEPENDFILE): 
	@-mkdir $(BINPATH) 2>/dev/null
	@-mkdir $(LIBPATH) 2>/dev/null
	@-mkdir $(OBJPATH) 2>/dev/null
	touch $(DEPENDFILE)
	makedepend -f$(DEPENDFILE) $(INCPATHS) $(SOURCES) $(PROGSOURCES) 2> /dev/null
	sed s/$(SEDSOURCEBODY)/$(SEDOBJBODY)/ $(DEPENDFILE) > $(DEPENDFILE).new
	rm $(DEPENDFILE)
	mv $(DEPENDFILE).new $(DEPENDFILE)

$(LIBNAME): $(OBJECTS) $(OBJPATH)/cpucycles.o
	ar cr $(LIBNAME) $(OBJPATH)/*.o

$(OBJPATH)/cpucycles.o: ./benchmarking/* 
	cd ./benchmarking/ && sh do; cd ..

$(SOURCEPATH)/cpucycles.h: ./benchmarking/*
	cd ./benchmarking/ && sh do; cd ..

.PHONY: clean cleanall depend full showfiles

clean:
	-rm -f $(OBJECTS)
	-rm -f $(LIBPATH)/*.a
#	-rm -f $(DEPENDFILE)
	-rm -f $(PROGS)
	-rm -f $(OBJPATH)/$(PROGNAMES).o
	-rm -f $(OBJPATH)/$(BATNAME).o

cleanall: clean
	-rm -f $(SOURCEPATH)/cpucycles.h
	-rm -f $(OBJPATH)/*.o


depend: $(DEPENDFILE)

full: $(PROGS)

showfiles:
	@echo $(HEADERS) 
	@echo $(SOURCES)
	@echo $(PROGSOURCES)

include $(DEPENDFILE)
