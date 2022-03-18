#!/bin/bash

g++ main.cc mainwindow.cc $(pkg-config --cflags gtkmm-4.0) $(pkg-config --libs gtkmm-4.0) -o project-starter
