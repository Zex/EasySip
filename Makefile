# Makefile for EasySip

PROJECT		= EasySip
DESCRIPTION	= Easy-to-used SIP Stack
VERSION		= 0.0.1

INCLUDES	= include build
SRCS		= src
LIBS		=

SCM			= git

include makefiles/predef.mk
include makefiles/basic.mk

all: tests
#easysip

easysip: $(SHARED_OBJ)
#	@echo $(MAKEFILE_LIST)

$(SHARED_OBJ): $(SHARED_OBJ).$(VERSION)
	@echo
	@echo "===Building $@===>"
	$(RM) $(SHARED_OBJ)
	$(LN) $(shell basename $<) $@

$(SHARED_OBJ).$(VERSION) : $(OBJS)
	@echo
	@echo "===Building $@===>"
	$(CXX) $(CFLAGS) -shared $(LDFLAGS) $(INCLUDES:%=-I%) $(MACROS:%=-D%) $(LIBS:%=-l%) -o $@ $^

$(BUILD)/%.o : %.cpp
	@echo
	@echo "===Generating $@===>"
	$(MKDIR) $(dir $@)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCLUDES:%=-I%) $(MACROS:%=-D%) $(LIBS:%=-l%) -c -o $@ $<

tests: $(SHARED_OBJ)
	@echo
	@echo "===Generating $@===>"
	@make BUILD=../build -C test all

test/%: $(SHARED_OBJ)
	@echo
	@echo "===Generating $@===>"
	@make BUILD=../build -C test $(shell basename $@)

clean:
	@find $(BUILD) -type f -regex .*\.\[o\|d\|so] -delete
	@find $(BUILD) -type f -executable -delete

clean-all:
	$(RM) $(BUILD)

