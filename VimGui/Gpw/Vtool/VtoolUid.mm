/**********************************
 ***   Vtool.mm   ***
 **********************************/
#include	<string.h>

#include	"Uigp/igp.h"
#include	"Ulog/Log.h"
#include	"Utime/GpTime.h"

#import "OpenUDID.h"


#include	"Vtool.h"


void CVtool::clearSessionID(){
    if (sessionID){
        [sessionID release];
        sessionID = nil;
    }
}

const char* CVtool::GetSessionID( char *session )
{
    if (sessionID){
        strcpy( session, sessionID.UTF8String );
        return( session );
 //       return sessionID.UTF8String;
    }
    
    NSString *generatedID;
    
    NSDate *date = [NSDate date];
	
	NSInteger secondsFromGTM = 0;
	//NSComparisonResult order = [[UIDevice currentDevice].systemVersion compare: @"4.1" options: NSNumericSearch];
	//if (order == NSOrderedDescending){			// < 4.1
	if ([[UIDevice currentDevice].systemVersion floatValue] < 4.1){								// < 4.1
        secondsFromGTM = [[NSTimeZone localTimeZone] secondsFromGMTForDate:date];
        date = [date dateByAddingTimeInterval:-secondsFromGTM];				// absolute time
	}
	
	NSString *millis = [NSString stringWithFormat:@"%0.0f", [date timeIntervalSince1970] * 1000];
	NSArray *dateSplitted = [[NSArray alloc] initWithArray:[[date description] componentsSeparatedByString:@" "]];
	NSArray *split = [[NSArray alloc] initWithArray:[((NSString*)[dateSplitted objectAtIndex:0]) componentsSeparatedByString:@"-"]];
	NSString *year = [[split objectAtIndex:0] substringFromIndex:2];
	NSString *month = [split objectAtIndex:1];
	NSString *day = [split objectAtIndex:2];
	[split release];
	
	split = [[NSArray alloc] initWithArray:[((NSString*)[dateSplitted objectAtIndex:1]) componentsSeparatedByString:@":"]];
	NSString *hour = [split objectAtIndex:0];
	NSString *minutes = [split objectAtIndex:1];
	NSString *seconds = [split objectAtIndex:2];
	NSString *millisecs = [NSString stringWithFormat:@"%c%c", [millis characterAtIndex:([millis length]-2)], [millis characterAtIndex:([millis length]-1)]];
	[split release];
	
	generatedID = [NSString stringWithFormat:@"%@%@%@%@%@%@%@", day, month, year, hour, minutes, seconds, millisecs];
    //generatedID = millis;
    NSString *temp = [NSString stringWithFormat:@"%@_%@", [OpenUDID value], generatedID];
    sessionID = [[temp stringByAddingPercentEscapesUsingEncoding:NSASCIIStringEncoding] retain];
    
    strcpy( session, sessionID.UTF8String );
    return( session );
    
   // return generatedID.UTF8String;
}

