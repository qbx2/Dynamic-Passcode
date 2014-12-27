#include "CoreFoundation/CoreFoundation.h"
#include "IOKit/IOPowerSources.h"
#include "IOKit/IOPSKeys.h"

int getBatteryPercent(int *piBatteryPercent)
{
    CFTypeRef blob = IOPSCopyPowerSourcesInfo();
    CFArrayRef sources = IOPSCopyPowerSourcesList(blob);
    
    CFDictionaryRef pSource = NULL;
    const void *psValue;
    
    if(CFArrayGetCount(sources) == 0) {
        return 0; // Could not retrieve battery information.  System may not have a battery.
    }

    pSource = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(sources, 0));
    psValue = (CFStringRef)CFDictionaryGetValue(pSource, CFSTR(kIOPSNameKey));
    
    int curCapacity = 0;
    int maxCapacity = 0;
    int percent;
    
    psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSCurrentCapacityKey));
    CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &curCapacity);
    
    psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSMaxCapacityKey));
    CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &maxCapacity);
    
    *piBatteryPercent = percent = (int)((double)curCapacity/(double)maxCapacity * 100);
    
    if(percent < 0 || percent > 100) {
        return -1; // Could not retrieve the battery information
    } else {
    	return 1; // Success
    }
}

int generatePasscodeWithFormat(NSString *format, NSString **out) {
    NSError *error = NULL;
        
    NSArray *matches = [[NSRegularExpression regularExpressionWithPattern:@"\\((.*?)\\)" options:0 error:&error] matchesInString:format options:0 range:NSMakeRange(0, [format length])];
    NSUInteger matchCount = [matches count];
        
    NSString *result = [[[NSString alloc] init] autorelease];
        
    if(matchCount == 4) {
        //b1 b2 b3 (battery)
        int batteryPercent = -1;
       
        getBatteryPercent(&batteryPercent);

        NSString *b1 = [NSString stringWithFormat:@"%d", batteryPercent / 100 % 10];
        NSString *b2 = [NSString stringWithFormat:@"%d", batteryPercent / 10 % 10];
        NSString *b3 = [NSString stringWithFormat:@"%d", batteryPercent / 1 % 10];
       
        NSLog(@"%@ %@ %@", b1, b2, b3);
       
        NSDate *now = [NSDate date];
        NSCalendar *calendar = [NSCalendar currentCalendar];
        NSDateComponents *components = [calendar components:NSCalendarUnitYear|NSCalendarUnitMonth|NSCalendarUnitDay|NSCalendarUnitWeekday|NSCalendarUnitHour|NSCalendarUnitMinute fromDate:now];
       
        //H1 H2 h1 h2 m1 m2 (time)
        NSString *H1 = [NSString stringWithFormat:@"%d", (int)[components hour] / 10 % 10];
        NSString *H2 = [NSString stringWithFormat:@"%d", (int)[components hour] / 1 % 10];
       
        NSString *h1 = [NSString stringWithFormat:@"%d", ((int)[components hour] % 12) / 10 % 10];
        NSString *h2 = [NSString stringWithFormat:@"%d", ((int)[components hour] % 12) / 1 % 10];
       
        NSString *m1 = [NSString stringWithFormat:@"%d", (int)([components minute] / 10 % 10)];
        NSString *m2 = [NSString stringWithFormat:@"%d", (int)([components minute] / 1% 10)];

        NSLog(@"%@ %@ / %@ %@ / %@ %@", H1, H2, h1, h2, m1, m2);
       
        //y1 y2 y3 y4 M1 M2 d1 d2 w
        NSString *y1 = [NSString stringWithFormat:@"%d", (int)([components year]) / 1000 % 10];
        NSString *y2 = [NSString stringWithFormat:@"%d", (int)([components year]) / 100 % 10];
        NSString *y3 = [NSString stringWithFormat:@"%d", (int)([components year]) / 10 % 10];
        NSString *y4 = [NSString stringWithFormat:@"%d", (int)([components year]) / 1 % 10];
       
        NSString *M1 = [NSString stringWithFormat:@"%d", (int)([components month]) / 10 % 10];
        NSString *M2 = [NSString stringWithFormat:@"%d", (int)([components month]) / 1 % 10];
       
        NSString *d1 = [NSString stringWithFormat:@"%d", (int)([components day]) / 10 % 10];
        NSString *d2 = [NSString stringWithFormat:@"%d", (int)([components day]) / 1 % 10];
       
        NSString *w = [NSString stringWithFormat:@"%d", (int)[components weekday]];
       
        NSLog(@"%@ %@ %@ %@ / %@ %@ / %@ %@ / %@", y1, y2, y3, y4, M1, M2, d1, d2, w);
       
        for(NSTextCheckingResult *match in matches) {
            NSString *string = [format substringWithRange:[match rangeAtIndex:1]];
            string = [string stringByReplacingOccurrencesOfString:@" " withString:@""];
       
            if(batteryPercent != -1) {
                //b1 b2 b3
                string = [string stringByReplacingOccurrencesOfString:@"b1" withString:b1];
                string = [string stringByReplacingOccurrencesOfString:@"b2" withString:b2];
                string = [string stringByReplacingOccurrencesOfString:@"b3" withString:b3];
            }

            //H1 H2 h1 h2 m1 m2
            string = [string stringByReplacingOccurrencesOfString:@"H1" withString:H1];
            string = [string stringByReplacingOccurrencesOfString:@"H2" withString:H2];
           
            string = [string stringByReplacingOccurrencesOfString:@"h1" withString:h1];
            string = [string stringByReplacingOccurrencesOfString:@"h2" withString:h2];
           
            string = [string stringByReplacingOccurrencesOfString:@"m1" withString:m1];
            string = [string stringByReplacingOccurrencesOfString:@"m2" withString:m2];

            //y1 y2 y3 y4 m1 m2 d1 d2 w
            string = [string stringByReplacingOccurrencesOfString:@"y1" withString:y1];
            string = [string stringByReplacingOccurrencesOfString:@"y2" withString:y2];
            string = [string stringByReplacingOccurrencesOfString:@"y3" withString:y3];
            string = [string stringByReplacingOccurrencesOfString:@"y4" withString:y4];
           
            string = [string stringByReplacingOccurrencesOfString:@"M1" withString:M1];
            string = [string stringByReplacingOccurrencesOfString:@"M2" withString:M2];
            
            string = [string stringByReplacingOccurrencesOfString:@"d1" withString:d1];
            string = [string stringByReplacingOccurrencesOfString:@"d2" withString:d2];
           
            string = [string stringByReplacingOccurrencesOfString:@"w" withString:w];
           
            NSTextCheckingResult *check = [[NSRegularExpression regularExpressionWithPattern:@"^[0-9\\(\\)\\+\\*\\/\\-]*$" options:0 error:&error] firstMatchInString:string options:0 range:NSMakeRange(0, [string length])];
           
            if(check) {
                NSInteger digit = [[[NSExpression expressionWithFormat:string] expressionValueWithObject:nil context:nil] intValue] % 10; // use only last digit.
                NSLog(@"digit: %ld", (long)digit);
                result = [result stringByAppendingFormat:@"%ld", (long)digit];
            } else {
                NSLog(@"Invalid Format: %@", string);
                *out = string;
                return -1 + (batteryPercent == -1 ? -1 : 0); //battery error -> -2
            }
        }
    } else {
        NSLog(@"Invalid Format");
        return 0;
    }
        
    NSLog(@"Passcode: %@", result);
    *out = result;
    return 1;
}
