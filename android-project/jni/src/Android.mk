LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include ../../../src
LOCAL_CFLAGS += --std=c11 -DNDEBUG -O3

SRC_DIR := ../../../
# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	../../../src/rom_image.c ../../../src/memory.c \
	../../../src/opcodes.c ../../../src/ppm.c \
	../../../src/main.c ../../../src/boot_image.c \
	../../../src/arg_defs.c ../../../src/audio.c \
	../../../src/display.c ../../../src/cpu.c \
	../../../src/instructions/or.c ../../../src/instructions/nop.c \
	../../../src/instructions/halt.c ../../../src/instructions/rl.c \
	../../../src/instructions/pop.c ../../../src/instructions/dec.c \
	../../../src/instructions/daa.c ../../../src/instructions/sbc.c \
	../../../src/instructions/call.c ../../../src/instructions/res.c \
	../../../src/instructions/stop.c ../../../src/instructions/srl.c \
	../../../src/instructions/rrc.c ../../../src/instructions/swap.c \
	../../../src/instructions/ccf.c ../../../src/instructions/push.c \
	../../../src/instructions/di.c ../../../src/instructions/rrca.c \
	../../../src/instructions/ret.c ../../../src/instructions/jr.c \
	../../../src/instructions/rr.c ../../../src/instructions/cpl.c \
	../../../src/instructions/invalid.c ../../../src/instructions/rlc.c \
	../../../src/instructions/cp.c ../../../src/instructions/rlca.c \
	../../../src/instructions/prefix.c ../../../src/instructions/ld.c \
	../../../src/instructions/reti.c ../../../src/instructions/bit.c \
	../../../src/instructions/sra.c ../../../src/instructions/sla.c \
	../../../src/instructions/and.c ../../../src/instructions/sub.c \
	../../../src/instructions/jp.c ../../../src/instructions/rra.c \
	../../../src/instructions/set.c ../../../src/instructions/ldh.c \
	../../../src/instructions/rst.c ../../../src/instructions/rla.c \
	../../../src/instructions/inc.c ../../../src/instructions/adc.c \
	../../../src/instructions/add.c ../../../src/instructions/xor.c \
	../../../src/instructions/scf.c ../../../src/instructions/ei.c \
	../../../src/debug.c ../../../src/events.c \
	../../../src/common.c ../../../src/prefix_cb_opcodes.c \
	../../../src/cmdline.c

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
