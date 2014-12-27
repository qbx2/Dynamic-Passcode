include theos/makefiles/common.mk

BUNDLE_NAME = DynamicPasscodePref
DynamicPasscodePref_FILES = DynamicPasscodePref.mm
DynamicPasscodePref_INSTALL_PATH = /Library/PreferenceBundles
DynamicPasscodePref_FRAMEWORKS = UIKit
DynamicPasscodePref_PRIVATE_FRAMEWORKS = Preferences

include $(THEOS_MAKE_PATH)/bundle.mk

internal-stage::
	$(ECHO_NOTHING)mkdir -p $(THEOS_STAGING_DIR)/Library/PreferenceLoader/Preferences$(ECHO_END)
	$(ECHO_NOTHING)cp entry.plist $(THEOS_STAGING_DIR)/Library/PreferenceLoader/Preferences/DynamicPasscodePref.plist$(ECHO_END)
