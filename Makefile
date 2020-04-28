# Makefile
SMSDK = ../sourcemod
#
# navfile_blocker.ext.so
#
INCLUDE = -I. -I$(SMSDK)/public -I$(SMSDK)/public/amtl -I$(SMSDK)/public/amtl/amtl -I$(SMSDK)/sourcepawn/include
CFLAGS  = -DPOSIX -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp \
	-D_snprintf=snprintf -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp -DCOMPILER_GCC \
	-Wno-switch -Wall -Werror -mfpmath=sse -msse -DSOURCEMOD_BUILD -DHAVE_STDINT_H -m32

CPP_FLAGS = -Wno-non-virtual-dtor -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -std=c++11

OBJECTS = Release/smsdk_ext.o Release/extension.o Release/detours.o
#
all:
	mkdir -p Release
#
	gcc $(INCLUDE) $(CFLAGS) $(CPP_FLAGS) -o Release/smsdk_ext.o -c $(SMSDK)/public/smsdk_ext.cpp
	gcc $(INCLUDE) $(CFLAGS) $(CPP_FLAGS) -o Release/detours.o -c CDetour/detours.cpp
	gcc $(INCLUDE) $(CFLAGS) $(CPP_FLAGS) -o Release/extension.o -c extension.cpp
#
	gcc $(OBJECTS) asm/asm.c -static-libgcc -shared -m32 -lm -ldl -o Release/navfile_blocker.ext.so
#
	rm -rf Release/*.o
