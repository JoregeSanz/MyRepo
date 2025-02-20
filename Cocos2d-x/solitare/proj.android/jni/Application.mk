APP_STL := c++_static
#NDK_TOOLCHAIN_VERSION=4.9

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic
APP_CFLAGS += -Wno-error=format-security
NDK_DEBUG:=1
#APP_ABI:= armeabi armeabi-v7a
#APP_ABI := armeabi x86
APP_PLATFORM = android-19

APP_DEBUG := $(strip $(NDK_DEBUG))
ifeq ($(APP_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
