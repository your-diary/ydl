.RECIPEPREFIX +=
SHELL = /bin/bash
.DELETE_ON_ERROR :

override CXXFLAGS += -O3
prefix := /usr/local

binary_name := ydl
source_name := ydl.cpp

.PHONY: all install uninstall clean

all : $(binary_name)

$(binary_name) : $(source_name)
    g++ $(CXXFLAGS) -o $@ $<

install_list := $(binary_name) get_video_id easy_ydl

install : $(install_list)
    mkdir -p $(prefix)/bin
    cp $(install_list) $(prefix)/bin

uninstall :
    rm $(addprefix $(prefix)/bin/,$(install_list))

clean :
    rm $(binary_name)

