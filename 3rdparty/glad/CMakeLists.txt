cmake_minimum_required(VERSION 3.23)

add_library(glad STATIC
	include/glad/glad.h
	src/glad.c
)
