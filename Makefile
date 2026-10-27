########################################################
# Makefile for CSCI 435 
# Author: Dr. Zoppetti
########################################################

########################################################
# Variable definitions
########################################################
CXX := g++

# C++ compiler flags
CXXFLAGS := -Werror -Wall -std=c++26 -Isrc

# Linker. For C++ should be $(CXX).
LINK := $(CXX)

# Executable name. 
EXEC := CMinus


SRC_DIR := src

SRCS := $(shell find $(SRC_DIR) -name '*.cc')

OBJS := $(SRCS:.cc=.o)


#############################################################
# Rules
#   Rules have the form
#   target : prerequisites
#   	  recipe
#############################################################


$(EXEC) : $(OBJS)
	$(LINK) $^ -o $@

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cc : %.l
	flex --nounput -o $@ $<

#############################################################

.PHONY : clean
clean :
	$(RM) $(EXEC) a.out core
	$(RM) *.o *.d *~

#############################################################
