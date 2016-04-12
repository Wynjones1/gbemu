cmake_minimum_required(VERSION 2.8)

include(ExternalProject)

ExternalProject_Add(WLA_DX
	GIT_REPOSITORY https://github.com/vhelin/wla-dx
)