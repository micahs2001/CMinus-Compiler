########################################################
# Makefile for CSCI 435 
# Author: Micah Schafer, Dr. Gary M. Zoppetti
########################################################

########################################################
# Variable definitions
########################################################
CXX := g++-16

# C++ compiler flags
CXXFLAGS := -Werror -Wall -std=c++26 -Isrc

# Linker. For C++ should be $(CXX).
LINK := $(CXX)

# Executable name. 
EXEC := CMinus


SRC_DIR := src

SRCS := $(shell find $(SRC_DIR) -name '*.cc')

BUILD_DIR := build

OBJS := $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SRCS))


#############################################################
# Rules
#   Rules have the form
#   target : prerequisites
#   	  recipe
#############################################################


$(EXEC) : $(OBJS)
	$(LINK) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cc : %.l
	flex --nounput -o $@ $<

#############################################################

.PHONY : clean
clean :
	$(RM) $(EXEC) a.out core
	$(RM) -r $(BUILD_DIR)

#############################################################
