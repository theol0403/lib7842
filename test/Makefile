#
# Makefile
#
CEXTS:=c
CXXEXTS:=cpp c++ cc

# probably shouldn't modify these, but you may need them below
ROOT=.
PROJECTROOT=..
BINDIR=$(ROOT)/bin
SRCDIR=$(PROJECTROOT)/src
INCDIR=$(PROJECTROOT)/include

WARNFLAGS+= -Wall -Wextra -Wshadow -Wnull-dereference -Wno-psabi -Wno-unused-function
EXTRA_CFLAGS=
EXTRA_CXXFLAGS= -std=gnu++17 -pthread -g -O0 -fprofile-arcs -ftest-coverage -D THREADS_STD

.DEFAULT_GOAL=quick

# common.mk
ARCHTUPLE=

WARNFLAGS+=

CFLAGS= $(WARNFLAGS) --std=gnu11
CXXFLAGS= $(WARNFLAGS) $(EXTRA_CXXFLAGS) 
LDFLAGS= $(WARNFLAGS) -lm -lgcc -lstdc++ -lsupc++ -lgcov

AR:=$(ARCHTUPLE)ar
CC:=$(ARCHTUPLE)gcc
CXX:=$(ARCHTUPLE)g++
LD:=$(ARCHTUPLE)g++

rwildcard=$(foreach d,$(filter-out $3,$(wildcard $1*)),$(call rwildcard,$d/,$2,$3)$(filter $(subst *,%,$2),$d))

# Colors
NO_COLOR=\x1b[0m
OK_COLOR=\x1b[32;01m
ERROR_COLOR=\x1b[31;01m
WARN_COLOR=\x1b[33;01m
STEP_COLOR=\x1b[37;01m
OK_STRING=$(OK_COLOR)[OK]$(NO_COLOR)
DONE_STRING=$(OK_COLOR)[DONE]$(NO_COLOR)
ERROR_STRING=$(ERROR_COLOR)[ERRORS]$(NO_COLOR)
WARN_STRING=$(WARN_COLOR)[WARNINGS]$(NO_COLOR)
ECHO=/bin/echo -e
echo=@$(ECHO) "$2$1$(NO_COLOR)"
echon=@$(ECHO) -n "$2$1$(NO_COLOR)"

define test_output
@rm -f temp.log temp.errors
$1 2> temp.log || touch temp.errors
@if test -e temp.errors; then $(ECHO) "$(ERROR_STRING)" && cat temp.log; elif test -s temp.log; then $(ECHO) "$(WARN_STRING)" && cat temp.log; else $(ECHO) "$2"; fi;
@if test -e temp.errors; then rm -f temp.log temp.errors && false; fi;
@rm -f temp.log temp.errors
endef

# Makefile Verbosity
ifeq ("$(origin VERBOSE)", "command line")
BUILD_VERBOSE = $(VERBOSE)
endif
ifeq ("$(origin V)", "command line")
BUILD_VERBOSE = $(V)
endif

ifndef BUILD_VERBOSE
BUILD_VERBOSE = 0
endif

# R is reduced (default messages) - build verbose = 0
# V is verbose messages - verbosity = 1
# VV is super verbose - verbosity = 2
ifeq ($(BUILD_VERBOSE), 0)
R = @echo
D = @
VV = @
endif
ifeq ($(BUILD_VERBOSE), 1)
R = @echo
D =
VV = @
endif
ifeq ($(BUILD_VERBOSE), 2)
R =
D =
VV =
endif
# common.mk

INCLUDE=-iquote$(INCDIR)

CSRC=$(foreach cext,$(CEXTS),$(call rwildcard, $(SRCDIR),*.$(cext), $1))
COBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call CSRC, $1)))
CXXSRC=$(foreach cxxext,$(CXXEXTS),$(call rwildcard, $(SRCDIR),*.$(cxxext), $1))
CXXOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call CXXSRC,$1)))

GETALLOBJ=$(sort $(call ASMOBJ,$1) $(call COBJ,$1) $(call CXXOBJ,$1))

OUTBIN:=$(BINDIR)/main.exe

.PHONY: all clean quick

quick: $(OUTBIN)

all: clean $(OUTBIN)

clean:
	@echo Cleaning project
	-$Drm -rf $(BINDIR)

$(OUTBIN): $(GETALLOBJ) $(BINDIR)
	@echo -n "Creating $@ "
	$(call test_output,$(LD) -o $(OUTBIN) $(CXXOBJ) $(LDFLAGS),$(DONE_STRING))


define c_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$$(call test_output,$D$(CC) -c $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CFLAGS) $(EXTRA_CFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cext,$(CEXTS),$(eval $(call c_rule,$(cext))))

define cxx_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$$(call test_output,$D$(CXX) -c $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(foreach cxxext,$(CXXEXTS),$(eval $(call cxx_rule,$(cxxext))))

