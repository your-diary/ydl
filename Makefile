.RECIPEPREFIX := $(.RECIPEPREFIX) 
SHELL = /bin/bash
.DELETE_ON_ERROR :

override CXXFLAGS += -O3
prefix := /usr/local

binary_list := ydl easy_ydl
source_list := ydl.cpp easy_ydl.cpp

.PHONY: all install uninstall clean

all : $(binary_list)

$(word 1,$(binary_list)) : $(word 1,$(source_list))
    g++ $(CXXFLAGS) -o $@ $<

$(word 2,$(binary_list)) : $(word 2,$(source_list))
    g++ $(CXXFLAGS) -o $@ $<

install_list := $(binary_list) get_video_id

install : $(install_list)
    mkdir -p $(prefix)/bin
    cp $(install_list) $(prefix)/bin

uninstall :
    rm $(addprefix $(prefix)/bin/,$(install_list))

clean :
    rm $(binary_list)

