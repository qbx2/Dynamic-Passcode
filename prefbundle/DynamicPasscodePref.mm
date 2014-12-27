#import <Preferences/Preferences.h>
#import "../DynamicPasscode.h"

@interface DynamicPasscodePrefListController: PSListController {
}
@end

@implementation DynamicPasscodePrefListController
- (id)specifiers {
    if(_specifiers == nil) {
        _specifiers = [[self loadSpecifiersFromPlistName:@"DynamicPasscodePref" target:self] retain];

        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Test title2" message:@"test message" delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        [alert show];
        [alert release];

    }

    return _specifiers;
}

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex {
    if(buttonIndex == 0) {
        PSSpecifier *SecureFormat = [self specifierForID:@"Format"];

        PSSpecifier *specifier = [PSSpecifier preferenceSpecifierNamed:@"Format" target:self set:@selector(saveFormatValue:specifier:) get:@selector(readPreferenceValue:) detail:Nil cell:PSEditTextCell edit:Nil];
        [specifier setProperty:@"Format" forKey:@"label"];
        [specifier setProperty:@"format" forKey:@"key"];
        [specifier setProperty:@"me.funso.dynamicpasscode" forKey:@"defaults"];
        [specifier setProperty:@YES forKey:@"noAutoCorrect"];

        [self replaceContiguousSpecifiers:[NSArray arrayWithObjects:SecureFormat, nil] withSpecifiers:[NSArray arrayWithObjects:specifier, nil] animated:false];
    }
}

- (void)saveFormatValue:(id)value specifier:(PSSpecifier *)specifier {
    NSString *title = @"Error";
    NSString *message;
    NSString *out;
    
    switch(generatePasscodeWithFormat(value, &out)) {
        case -2: //format error, couldn't get battery information, so don't use b1,b2,b3
            message = @"Your format could not be saved because an error has occured while obtaining battery information. So please avoid using b1, b2 and b3.";
            break;
        case -1: //format error, invalid format character
            message = [NSString stringWithFormat:@"Your format could not be saved because an error has occured while processing your format. Invalid expression: %@", out];
            break;
        case 0: //format error, the format must have four pairs of round brackets like this : (1)(2)(3)(4)
            message = @"Your format could not be saved because an error has occured while processing your format. The format must have four pairs of round brackets which have an expression represents each digit.";
            break;
        default:
            title = @"Hint";
            message = [NSString stringWithFormat:@"Your format has been saved. And the passcode will be <%@> at this moment.", out];
            [self performSelector:@selector(setPreferenceValue:specifier:) withObject:value withObject:specifier];
            break;
    }

    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    [alert show];
    [alert release];
}

- (void)respring {
    system("killall -9 SpringBoard");
}

- (void)viewsourcecode {
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://github.com/qbx2/Dynamic-Passcode"]];
}

- (void)viewemailaddress {
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"E-Mail Address" message:@"admin@funso.me" delegate:nil cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
    [alert show];
    [alert release];
}
@end

// vim:ft=objc
