ARCHS = armv7 armv7s arm64
include theos/makefiles/common.mk

TWEAK_NAME = DynamicPasscode
DynamicPasscode_FILES = Tweak.xm

LDFLAGS += -lIOKit -L$(THEOS_PROJECT_DIR)/IOKit/
export LDFLAGS

include $(THEOS_MAKE_PATH)/tweak.mk

SUBPROJECTS += prefbundle
include $(THEOS_MAKE_PATH)/aggregate.mk

after-install::
	install.exec "killall -9 SpringBoard"
