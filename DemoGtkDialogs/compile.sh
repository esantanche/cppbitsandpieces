#!/bin/bash

g++ main.cc examplewindow.cc $(pkg-config --cflags gtkmm-4.0) $(pkg-config --libs gtkmm-4.0) -o demo-dialogs
