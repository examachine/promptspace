#
# makefile for g++
#
# by exa
#

SHELL = /bin/sh

.SUFFIXES: .c .cc .cxx .o .flex .flex.c .bison .bison.c .tex .dvi

PROJECT := ann
VERSION := 0.0.3
BUILDARCH = i686
# valid build configs : dbg, opt, optdbg
BUILD := dbg

# Directories

srcdir = ./src
builddir := $(shell pwd)
parentdir := $(shell cd ..; pwd)
dirname := $(subst $(parentdir)/,,$(builddir))
# CODE: Allow different configs to co-exist
workingdir = ./$(BUILD)
VPATH = $(srcdir) $(workingdir)
#vpath %.cc $(srcdir)

# Project files

EXEC = nn	# Executable

# Object Files

OBJS = Application.o Log.o Test.o

# Source Files

CXXFILES = Application.cxx Log.cxx Test.cxx
CCFILES =
CFILES =
AFILES =
INCFILES = General.hxx ANSILibrary.hxx Log.hxx \
	   Neuron.hxx Layer.hxx
BUILDFILES = Makefile
GRAMMARFILES =
LEXFILES =

# Documentation

TEXFILES =
DVIFILES = $(addsuffix .dvi, $(basename $(TEXFILES)))
DOCFILES = $(TEXFILES)

# Top level classification of source files

IMPLEMENTATION = $(CXXFILES) $(CCFILES) $(CFILES) $(AFILES) $(GRAMMARFILES)
INTERFACE = $(INCFILES)
SRC = $(IMPLEMENTATION) $(INTERFACE) $(BUILDFILES)
VERSUFFIX:=,v
#VER = $(addsuffix $(VERSUFFIX), $(SRC)) # BUG: gives rise to wrong dep.

DOC = $(DOCFILES)

AUX = TODO Design.txt
ARCHIVE = $(PROJECT)-$(VERSION).tar.gz
ARCPATH = $(HOME)/arc
OBJFILES := $(addprefix $(workingdir)/,$(OBJS))

# System Resources

LIBS = -lnana $(shell gtkmm-config --libs)
INCLUDEDIR =
LIBDIR =

# Options

GENERAL_DEBUG = -g -DDEBUG -DLOG# -pg -a
GENERAL_OPTIMISE = -O3 -DWITHOUT_NANA #-DNDEBUG
GENERAL_WARN = -Wcomment -Wimplicit -Wmain -Wswitch -Wunknown-pragmas
GENERAL_OTHER = -march=$(BUILDARCH) #$(shell gtkmm-config --cflags)
CPPFLAGS =
CXXOPTIMISE =
CXXDEBUG =
CXXWARN = -ftemplate-depth-64 #-Wtemplate-debugging -Weffc++
CXXOTHER = -foperator-names
CXXFLAGS = $(GENERAL_WARN) $(GENERAL_OTHER) $(CXXWARN) $(CXXOTHER)
COPTIMISE =
CDEBUG =
CWARN =
COTHER = 
CFLAGS = $(GENERAL_WARN) $(GENERAL_OTHER) $(CWARN) $(COTHER)
ASFLAGS =
LDFLAGS =

ifeq ($(BUILD),dbg)
CFLAGS += $(GENERAL_DEBUG) $(CDEBUG)
CXXFLAGS += $(GENERAL_DEBUG) $(CXXDEBUG)
endif
ifeq ($(BUILD),opt)
CFLAGS += $(GENERAL_OPTIMISE) $(COPTIMISE)
CXXFLAGS += $(GENERAL_OPTIMISE) $(CXXOPTIMISE)
endif
ifeq ($(BUILD),optdbg)
CFLAGS += $(GENERAL_OPTIMISE) $(COPTIMISE) $(GENERAL_DEBUG) $(CDEBUG)
CXXFLAGS += $(GENERAL_OPTIMISE) $(CXXOPTIMISE) $(GENERAL_DEBUG) $(CXXDEBUG)
endif

# Programs used

#MAKE =	make		# For subsequent make's
CC =	gcc		# C compiler
CXX =	g++		# C++ compiler
ASM =	as		# Assembler (self arch)
LD = 	g++		# Linker
BISON =	bison --defines	# Syntax Analyzer
FLEX =	flex		# Lexical Analyzer
LS =	ls -l
CP =    cp --backup=numbered --preserve --recursive
ED =	xemacs
LATEX = latex
BIBTEX = bibtex
PAGER = most
INSTALL = install

define check-dir
@if [ ! -d $(workingdir) ]; then\
  echo Creating directory $(workingdir);\
  mkdir $(workingdir);\
 fi
endef

# Tool Options

OUTPUTFLAG = -o 	# Flag for output

# Rules

# Phony Targets

.PHONY: all compileinfo checkdir clean realclean list listsrc \
	arc listarc extractarc rmarc backup restore edit

# Pattern Rules

# Automatic dependency rules

# C/C++

$(workingdir)/%.d: %.cxx $(checkdir)
	$(check-dir)
	@echo Generating dependencies for $*.cxx
	@echo -n "$(workingdir)/" > $@
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< >> $@'
#	$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< \
#                 | sed '\''s/$*\\.o[ :]*/& "$@"/g'\'' >> $@'
$(workingdir)/%.d: %.cc
	@echo Generating dependencies for $*.cc
	@echo -n "$(workingdir)/" > $@
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                 | sed '\''s/$*\\.o[ :]*/& $@/g'\'' >> $@'
$(workingdir)/%.d: %.c
	@echo Generating dependencies for $*.c
	@$(SHELL) -ec '$(CC) -MM $(CPPFLAGS) $< \
                 | sed '\''s/$*\\.o[ :]*/& $@/g'\'' > $@'

# Compilation rules

# C/C++

$(workingdir)/%.o : %.cxx
	@echo Compiling $<
	@$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@
$(workingdir)/%.o : %.cc
	@echo Compiling $<
	@$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@
$(workingdir)/%.o : %.c
	@echo Compiling $<
	@$(CC) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

# Flex - Bison rules

%.tab.c %.tab.h : %.grammar %.hxx
	$(BISON) $< --output-file=$*.tab.c

%.flex.c : %.flex %.tab.h %.hxx
	$(FLEX) -o$@ $<

# Other compilers

%.dvi : %.tex
	@echo Compiling latex to dvi $<
	@$(LATEX) $<

# Default rule

all: defaultmesg TAGS compileinfo checkdir $(workingdir)/$(EXEC) $(DOC)

defaultmesg:
	@echo Default rule invoked, building everything.

compileinfo:
	@echo Project name: $(PROJECT) 
	@echo C preprocessor flags: $(CPPFLAGS)
	@echo C flags: $(CFLAGS)
	@echo C++ flags: $(CXXFLAGS)
	@echo Build architecture: $(BUILDARCH)
	@echo Build configuration: $(BUILD)

checkdir:
	$(check-dir)

$(workingdir)/$(EXEC): $(OBJFILES)
	@echo Building executable: $(EXEC)
	@echo LD flags: $(LDFLAGS)
	@echo Libraries used: $(LIBS)
	@$(LD) $(LDFLAGS) $(CXXFLAGS) $(LIBDIR) $(INCLUDEDIR) \
	       $^ $(LIBS) $(OUTPUTFLAG) $@
	@echo Installing executable in the current directory
	@$(INSTALL) $@ $(EXEC) 

TAGS:	$(SRC)
	@echo Building Emacs tags
	@etags $^

# Include automatic dependencies
ifneq ($(CXXFILES),)
include $(addprefix $(workingdir)/,$(CXXFILES:.cxx=.d))
endif

ifneq ($(CCFILES),)
include $(addprefix $(workingdir)/,$(CXXFILES:.cc=.d))
endif

ifneq ($(CFILES),)
include $(addprefix $(workingdir)/,$(CXXFILES:.c=.d))
endif

# Clean commands

clean:
	@echo Deleting object modules and executables
	@-rm -rf $(OBJFILES) $(EXEC)
	@-rmdir $(workingdir)

distclean: clean
	@echo Deleting archive
	@-rm -rf $(ARCHIVE)

maintainer-clean: distclean
	@echo Deleting miscallenous files
	@-rm -rf $(srcdir)/*.bak $(srcdir)/*~

realclean: maintainer-clean
	@-rm -rf $(workingdir)/*.d

# List commands

list: $(SRC) $(AUX)
	@$(LS) $^

listsrc: $(SRC)
	@$(LS) $^

# Archive

arc:	$(ARCHIVE)

listarc:
	@echo Listing content of tar archive; \
	tar tzvf $(ARCHIVE) | $(PAGER)

$(ARCHIVE): $(SRC) $(VER) $(DOC) $(AUX)
	@cd .. ;\
	echo Creating tar archive $(ARCHIVE);\
	tar czvf $(dirname)/$(ARCHIVE) $(addprefix $(dirname)/,$?)

extractarc:
	@cd .. ;\
	echo Extracting from tar archive $(ARCHIVE);\
	tar xzvf $(dirname)/$(ARCHIVE)

updatearc:
	@cd .. ;\
	echo Updating tar archive $(ARCHIVE);\
	tar uzvf $(dirname)/$(ARCHIVE) $(addprefix $(dirname)/,$?)

rmarc:
	@Removing tar archive
	@-rm -rf $(ARCHIVE)

backup: $(ARCHIVE)
	@echo Backing up archive $(ARCHIVE)
	@$(CP) $(ARCHIVE) $(ARCPATH)

restore:
	@echo Restoring archive
	@$(CP) $(ARCPATH)/$(ARCHIVE) .

edit:	$(SRC)
	@echo Invoking editor for all source files
	@$(ED) $^&

