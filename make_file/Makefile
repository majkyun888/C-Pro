# Copyright (c) 2004-2005 Northwestern University.
#                         NuCAD Group.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# NuCAD Makefile
#
# Authors: Hai Zhou, haizhou@ece.northwestern.edu
#          Jia Wang, jwa112@ece.northwestern.edu
#
# Avoid modifying this file. Please modify the SOURCES file instead.
#

# Updated by Jia Wang Sep. 2016

# Override $(srcdir) if the source tree is in the other directory.
srcdir=.

# Override $(SOURCES) if you maintain multiple target in one directory.
SOURCES=SOURCES

# Clear default extension list and build a new one.
.SUFFIXES:
.SUFFIXES:.o .c .C .cc .cxx .cpp .d

# Always build the following targets.
.PHONY: all clean cleanall depends show

# Tools.
DEPEND=gcc -MM -std=c++11
CC=gcc -Wall 
CXX=g++ -Wall -std=c++11 
LD=g++
AR=ar rcs

# all should be the first.
all:

# Source definition.
include $(SOURCES)

# Make sure Library_Path and Program_Path are correct.
ifeq "$(Library_Path)" ""
Library_Path:=./
endif
ifeq "$(Program_Path)" ""
Program_Path:=./
endif

# Additional names.
Target_LIB=$(Library_Path)/lib$(Target_Name)$(Target_Suffix).a
Target_Test=$(Program_Path)/$(Target_Name)_test$(Target_Suffix)
Target_Program=$(Program_Path)/$(Target_Name)$(Target_Suffix)
Target_Shared=$(Program_Path)/lib$(Target_Name)$(Target_Suffix).so

# Full pathname.
Sources:=$(abspath $(addprefix $(srcdir)/,$(Sources)))
Includes:=$(addprefix -I,$(Includes))
Test_Sources:=$(abspath $(addprefix $(srcdir)/,$(Test_Sources)))
Test_Includes:=$(addprefix -I,$(Test_Includes))

# Temp dir for building.
Target_TMP=$(Target_Name)$(Target_Suffix).build

# Dependency files.
Depends=$(addprefix $(Target_TMP),$(addsuffix .d,$(Sources)))
Test_Depends=$(addprefix $(Target_TMP),$(addsuffix .d,$(Test_Sources)))

# Object files.
fn_Source_to_Obj=$(patsubst $(1),%.o,$(filter $(1),$(2)))

Objects_C=$(addprefix $(Target_TMP),$(foreach ext,%.c,$(call fn_Source_to_Obj,$(ext),$(Sources))))
Test_Objects_C=$(addprefix $(Target_TMP),$(foreach ext,%.c,$(call fn_Source_to_Obj,$(ext),$(Test_Sources))))

Objects_CXX=$(addprefix $(Target_TMP),$(foreach ext,%.C %.cc %.cxx %.cpp,$(call fn_Source_to_Obj,$(ext),$(Sources))))
Test_Objects_CXX=$(addprefix $(Target_TMP),$(foreach ext,%.C %.cc %.cxx %.cpp,$(call fn_Source_to_Obj,$(ext),$(Test_Sources))))

Objects=$(Objects_C) $(Objects_CXX)
Test_Objects=$(Test_Objects_C) $(Test_Objects_CXX)

# What kind of target?
ifeq "$(Target_Type)" "LIBNOTEST"
  all: $(Library_Path) $(Target_LIB);
  $(Target_LIB): $(Objects)
	@echo -e '$(CLR_TTL)building library $@$(CLR_TRM)'
	@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
	@echo -e '$(CLR_CMD)$(AR) $@ $(Objects)$(CLR_TRM)'
	@$(AR) $@ $(Objects)
else
ifeq "$(Target_Type)" "LIBRARY"
  all: $(Library_Path) $(Program_Path) $(Target_LIB) $(Target_Test);
  $(Target_LIB): $(Objects)
	@echo -e '$(CLR_TTL)building library $@$(CLR_TRM)'
	@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
	@echo -e '$(CLR_CMD)$(AR) $@ $(Objects)$(CLR_TRM)'
	@$(AR) $@ $(Objects)
  $(Target_Test): $(Target_LIB) $(Test_Objects)
	@echo -e '$(CLR_TTL)building testing $@$(CLR_TRM)'
	@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
	@echo -e '$(CLR_CMD)$(LD) $(Test_Objects) -L$(Library_Path) -l$(Target_Name)$(Target_Suffix) $(Opt_LD) $(LDFLAGS) -o $@$(CLR_TRM)'
	@$(LD) $(Test_Objects) -L$(Library_Path) -l$(Target_Name)$(Target_Suffix) $(Opt_LD) $(LDFLAGS) -o $@
else
ifeq "$(Target_Type)" "PROGRAM"
  all: $(Program_Path) $(Target_Program);
  $(Target_Program): $(Objects)
	@echo -e '$(CLR_TTL)building program $@$(CLR_TRM)'
	@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
	@echo -e '$(CLR_CMD)$(LD) $(Objects) $(Opt_LD) $(LDFLAGS) -o $@$(CLR_TRM)'
	@$(LD) $(Objects) $(Opt_LD) $(LDFLAGS) -o $@
else
ifeq "$(Target_Type)" "SHARED"
  all: $(Program_Path) $(Target_Shared);
  $(Target_Shared): $(Objects)
	@echo -e '$(CLR_TTL)building shared $@$(CLR_TRM)'
	@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
	@echo -e '$(CLR_CMD)$(LD) $(Objects) $(Opt_LD) $(LDFLAGS) -shared -o $@$(CLR_TRM)'
	@$(LD) $(Objects) $(Opt_LD) $(LDFLAGS) -shared -o $@
else
  $(error Unknown Target_Type=$(Target_Type))
endif
endif
endif
endif

# Rules.

# Rules for directories.
ifeq "$(Program_Path)" "$(Library_Path)"
$(Program_Path):
	mkdir $@
else
$(Program_Path) $(Library_Path):
	mkdir $@
endif

# Rules for C/CXX.
$(Objects_C):
	@echo -e '$(CLR_TTL)compiling $<$(CLR_TRM)'
	@echo -e '$(CLR_TTL)       -> $@$(CLR_TRM)'
	@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
	@echo -e '$(CLR_CMD)$(CC) $(CFLAGS) $(Opt_CC) $(Includes) -c $< -o $@$(CLR_TRM)'
	@$(CC) $(CFLAGS) $(Opt_CC) $(Includes) -c $< -o $@

$(Test_Objects_C):
	@echo -e '$(CLR_TTL)compiling $<$(CLR_TRM)'
	@echo -e '$(CLR_TTL)       -> $@$(CLR_TRM)'
	@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
	@echo -e '$(CLR_CMD)$(CC) $(CFLAGS) $(Opt_CC) $(Test_Includes) -c $< -o $@$(CLR_TRM)'
	@$(CC) $(CFLAGS) $(Opt_CC) $(Test_Includes) -c $< -o $@

$(Objects_CXX):
	@echo -e '$(CLR_TTL)compiling $<$(CLR_TRM)'
	@echo -e '$(CLR_TTL)       -> $@$(CLR_TRM)'
	@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
	@echo -e '$(CLR_CMD)$(CXX) $(CXXFLAGS) $(Opt_CXX) $(Includes) -c $< -o $@$(CLR_TRM)'
	@$(CXX) $(CXXFLAGS) $(Opt_CXX) $(Includes) -c $< -o $@

$(Test_Objects_CXX):
	@echo -e '$(CLR_TTL)compiling $<$(CLR_TRM)'
	@echo -e '$(CLR_TTL)       -> $@$(CLR_TRM)'
	@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
	@echo -e '$(CLR_CMD)$(CXX) $(CXXFLAGS) $(Opt_CXX) $(Test_Includes) -c $< -o $@$(CLR_TRM)'
	@$(CXX) $(CXXFLAGS) $(Opt_CXX) $(Test_Includes) -c $< -o $@

# Rules for maintaining dependency files.

# Call me with source file name and includes.
define fn_Create_Dependency
@echo -e '$(CLR_TTL)building dependency for $(1)$(CLR_TRM)'
@echo -e '$(CLR_CHG)changed:  $?$(CLR_TRM)'
@mkdir -p $(Target_TMP)$(dir $(1))
@echo $(Target_TMP)$(1).d $(Target_TMP)$(dir $(1))$(shell $(DEPEND) $(2) $(1)) > $(Target_TMP)$(1).d

endef

ifeq "$(MAKECMDGOALS)" "depends"
  $(Target_TMP):
	mkdir -p $(Target_TMP)
  depends:$(Target_TMP)
	rm -f $(Depends) $(Test_Depends)
	@echo $(Sources) >$(Target_TMP)/sources.list
	@echo $(Test_Sources) >$(Target_TMP)/test_sources.list
	$(foreach src,$(Sources),$(call fn_Create_Dependency,$(src),$(Includes)))
	$(foreach src,$(Test_Sources),$(call fn_Create_Dependency,$(src),$(Test_Includes)))
else
ifeq "$(MAKECMDGOALS)" "clean"
  clean:
	-rm -f $(Objects) $(Test_Objects)
else
ifeq "$(MAKECMDGOALS)" "cleanall"
  cleanall:
	-rm -rf $(Target_TMP)
    ifeq "$(Target_Type)" "LIBNOTEST"
	-rm -f $(Target_LIB)
    else
    ifeq "$(Target_Type)" "LIBRARY"
	-rm -f $(Target_LIB) $(Target_Test) $(Target_Test).exe
    else
    ifeq "$(Target_Type)" "PROGRAM"
	-rm -f $(Target_Program) $(Target_Program).exe
    else
    ifeq "$(Target_Type)" "SHARED"
	-rm -f $(Target_Shared)
    else
      $(error Unknown Target_Type=$(Target_Type))
    endif
    endif
    endif
    endif
else
ifeq "$(MAKECMDGOALS)" "show"
  show:
	@echo Project Information
	@echo Target_Name=$(Target_Name)
	@echo Target_Type=$(Target_Type)
	@echo Library_Path=$(Library_Path)
	@echo Program_Path=$(Program_Path)
	@echo srcdir=$(srcdir)
	@echo Sources=$(Sources)
	@echo Includes=$(Includes)
	@echo Test_Sources=$(Test_Sources)
	@echo Test_Includes=$(Test_Includes)
	@echo CFLAGS=$(CFLAGS)
	@echo CXXFLAGS=$(CXXFLAGS)
	@echo LDFLAGS=$(LDFLAGS)
	@echo
	@echo Configuration
	@echo Config=$(Config)
	@echo Target_Suffix=$(Target_Suffix)
	@echo Opt_CC=$(Opt_CC)
	@echo Opt_CXX=$(Opt_CXX)
	@echo Opt_LD=$(Opt_LD)
	@echo
	@echo Automatic Generated Information
	@echo Target_LIB=$(Target_LIB)
	@echo Target_Test=$(Target_Test)
	@echo Target_Program=$(Target_Program)
	@echo Target_Shared=$(Target_Shared)
	@echo Target_TMP=$(Target_TMP)
	@echo Depends=$(Depends)
	@echo Test_Depends=$(Test_Depends)
	@echo Objects=$(Objects)
	@echo Test_Objects=$(Test_Objects)
else
  ifneq "$(shell cat $(Target_TMP)/sources.list)" "$(Sources)"
    $(error source files changes, make depends first)
  endif
  ifneq "$(shell cat $(Target_TMP)/test_sources.list)" "$(Test_Sources)"
    $(error source files changes, make depends first)
  endif
  include $(Depends) $(Test_Depends)
  $(Depends):
	$(call fn_Create_Dependency,$<,$(Includes))
  $(Test_Depends):
	$(call fn_Create_Dependency,$<,$(Test_Includes))
endif
endif
endif
endif
