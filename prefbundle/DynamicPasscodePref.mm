#import <Preferences/Preferences.h>

@interface DynamicPasscodePrefListController: PSListController {
}
@end

@implementation DynamicPasscodePrefListController
- (id)specifiers {
	if(_specifiers == nil) {
		_specifiers = [[self loadSpecifiersFromPlistName:@"DynamicPasscodePref" target:self] retain];
	}
	return _specifiers;
}
@end

// vim:ft=objc
