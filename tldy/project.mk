# #############################################################################
# Project Customization
# #############################################################################

PROJECT = tldy

UCSRC = $(wildcard ./lib/*.c)

UCXXSRC = ./delay.cpp

# NOTE: Relative path from `Makefile` that refer this file.
UINCDIR = ./lib

UDEFS =

ULIB = 

ULIBDIR =
