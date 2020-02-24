//
//  AppDelegate.m
//  SnyMediaServer.app
//
//  Created by developer on 9/25/18.
//

#import "AppDelegate.h"
#import <IOKit/pwr_mgt/IOPMLib.h>
#include "SNYMediaServer.h"
@interface AppDelegate ()
@property (retain, strong) NSStatusItem *item;
@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate
static SNYMediaServer* snyMediaServer = nullptr;
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    [self addIcon];
    [self addItem];
    [self preventSystemSleep];
  NSBundle *main = [NSBundle mainBundle];
  NSString *adminWebroot = [main pathForResource:@"web" ofType:@""];
  std::string webroot = std::string([adminWebroot UTF8String]);
    signal(SIGPIPE, SIG_IGN);
    if (snyMediaServer == nullptr) {
        snyMediaServer = new SNYMediaServer();
      snyMediaServer->setAdminWebDirectory(webroot);
        snyMediaServer->start();
    }
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (void)addIcon {
    self.item = [[NSStatusBar systemStatusBar] statusItemWithLength:30];
    NSImage *image = [NSImage imageNamed:@"statusbar"];
    [image setTemplate:YES];
    self.item.image = image;
    self.item.highlightMode = YES;
}

- (void)addItem {
    NSMenu *menu = [[NSMenu alloc] initWithTitle:@"cloudscreen"];
    [menu setMinimumWidth:200];
    self.item.menu = menu;
    
    //NSMenuItem *startSnyMediaServer = [[NSMenuItem alloc]initWithTitle:@"start" action:@selector(start) keyEquivalent:@""];
    //[startSnyMediaServer setTarget:self];
    //[menu addItem:startSnyMediaServer];
    
    //NSMenuItem *stopSnyMediaServer = [[NSMenuItem alloc]initWithTitle:@"stop" action:@selector(stop) keyEquivalent:@""];
    //[stopSnyMediaServer setTarget:self];
    //[menu addItem:stopSnyMediaServer];
    
    NSMenuItem *systemsleep = [[NSMenuItem alloc]initWithTitle:@"stop preventing systemsleep" action:@selector(SystemSleep) keyEquivalent:@""];
    [systemsleep setTarget:self];
    [menu addItem:systemsleep];
    
    NSMenuItem *aboutSny = [[NSMenuItem alloc]initWithTitle:@"update" action:@selector(about) keyEquivalent:@""];
    [aboutSny setTarget:self];
    [menu addItem:aboutSny];
    
    [menu addItem:[NSMenuItem separatorItem]];
    
    NSMenuItem *exitItem = [[NSMenuItem allocWithZone:[NSMenu menuZone]]initWithTitle:@"exit" action:@selector(exit) keyEquivalent:@""];
    [exitItem setTarget:self];
    [menu addItem:exitItem];
}

- (void)start {
    NSLog(@"Sny Start");
}

- (void) stop {
    NSLog(@"Sny Stop");
}

- (void) about {
    NSLog(@"Sny about");
    NSURL * url = [NSURL URLWithString:@"https://www.streamingnology.com"];
    [[NSWorkspace sharedWorkspace] openURL: url];
}

- (void)exit {
    NSLog(@"Sny Exit");
    [[NSApplication sharedApplication] terminate:nil];
}

IOPMAssertionID assertionID = 0;
- (void)preventSystemSleep {
    CFStringRef reasonForActivity= CFSTR("SnyMediaServer prevent system sleep");
    IOReturn success = IOPMAssertionCreateWithName(kIOPMAssertionTypeNoDisplaySleep,
                                                   kIOPMAssertionLevelOn, reasonForActivity, &assertionID);
    if (success == kIOReturnSuccess) {
        //success = IOPMAssertionRelease(assertionID);
        //  The system will be able to sleep again.
    }
}

- (void)preventSystemSleep1 {
    CFStringRef reasonForActivity= CFSTR("SnyMediaServer prevent system sleep");
    IOReturn success = IOPMAssertionCreateWithName(kIOPMAssertionTypeNoDisplaySleep,
                                                   kIOPMAssertionLevelOn, reasonForActivity, &assertionID);
    if (success == kIOReturnSuccess) {
        //success = IOPMAssertionRelease(assertionID);
        //  The system will be able to sleep again.
        NSMenuItem *item = [self.item.menu  itemWithTitle:(@"start preventing systemsleep")];
        [item setTitle:(@"stop preventing systemsleep")];
    }
}

- (void)resetSystemSleep {
    if (assertionID != 0) {
        IOReturn success = IOPMAssertionRelease(assertionID);
        if (success == kIOReturnSuccess) {
            assertionID = 0;
            NSMenuItem *item = [self.item.menu  itemWithTitle:(@"stop preventing systemsleep")];
            [item setTitle:(@"start preventing systemsleep")];
        }
    }
}

- (void)SystemSleep {
    if (assertionID != 0) {
        [self resetSystemSleep];
    } else {
        [self preventSystemSleep1];
    }
}
@end
