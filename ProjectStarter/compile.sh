#!/bin/bash

# This bash script runs the compiler to compile the app.

g++ main.cc mainwindow.cc configurationparser.cc $(pkg-config --cflags gtkmm-4.0) $(pkg-config --libs gtkmm-4.0) -o project-starter
