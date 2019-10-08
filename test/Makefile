#
# Makefile
#

ROOT=.
PROJECTROOT=..

BINDIR=$(ROOT)/bin
SRCDIR=$(PROJECTROOT)/src
INCDIR=$(PROJECTROOT)/include
TESTDIR=$(ROOT)/src
OUTBIN:=$(BINDIR)/main.exe

WARNFLAGS+= -Wall -Wextra -Wshadow -Wno-psabi -Wno-unused-function -Wnull-dereference
EXTRA_CXXFLAGS= -std=gnu++17 -g -pthread -O0 -fprofile-arcs -ftest-coverage -D THREADS_STD 
EXTRA_CXXFLAGS+= -fsanitize=undefined -fsanitize=vptr -fsanitize=integer-divide-by-zero -fsanitize=null
# EXTRA_CXXFLAGS+= -fsanitize=address 

CXXFLAGS= $(WARNFLAGS) $(EXTRA_CXXFLAGS) -I googletest/googletest/include -I ./include
LDFLAGS= $(WARNFLAGS) $(EXTRA_CXXFLAGS) -lm -lgcc -lstdc++ -lsupc++ -lgcov -lgtest_main -lgtest

CXX:=g++
LD:=g++

rwildcard=$(foreach d,$(filter-out $3,$(wildcard $1*)),$(call rwildcard,$d/,$2,$3)$(filter $(subst *,%,$2),$d))
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
R = @echo
D = @
VV = @

INCLUDE=-iquote$(INCDIR)

CXXSRC=$(call rwildcard, $(SRCDIR),*.cpp, $1)
CXXOBJ=$(addprefix $(BINDIR)/,$(patsubst $(SRCDIR)/%,%.o,$(call CXXSRC,$1)))

CXXSRCTEST=$(call rwildcard, $(TESTDIR),*.cpp, $1)
CXXOBJTEST=$(addprefix $(BINDIR)/,$(patsubst $(TESTDIR)/%,%.o,$(call CXXSRCTEST,$1)))

GETALLOBJ=$(sort $(call CXXOBJ,$1) $(call CXXOBJTEST,$1))

.DEFAULT_GOAL=quick

.PHONY: all clean quick

quick: $(OUTBIN)

all: clean $(OUTBIN)

clean:
	@echo Cleaning project
	-$Drm -rf $(BINDIR)

$(OUTBIN): $(GETALLOBJ) $(BINDIR)
	@echo -n "Creating $@ "
	$(call test_output,$D$(LD) -o $(OUTBIN) $(GETALLOBJ) $(LDFLAGS),$(DONE_STRING))

define cxx_rule
$(BINDIR)/%.$1.o: $(SRCDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$$(call test_output,$D$(CXX) -c $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(eval $(call cxx_rule,cpp))

define cxx_rule_test
$(BINDIR)/%.$1.o: $(TESTDIR)/%.$1
	$(VV)mkdir -p $$(dir $$@)
	@echo -n "Compiling $$< "
	$$(call test_output,$D$(CXX) -c $(INCLUDE) -iquote$(INCDIR)/$$(dir $$*) $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $$@ $$<,$(OK_STRING))
endef
$(eval $(call cxx_rule_test,cpp))

