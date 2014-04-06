//
//  AppDelegate.m
//  Audio Controller Test Suite
//
//  Created by Michael Tyson on 13/02/2012.
//  Copyright (c) 2012 A Tasty Pixel. All rights reserved.
//

#import "AppDelegate.h"
#import "ViewController.h"
#import "TheAmazingAudioEngine.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;
@synthesize audioController = _audioController;

- (void)dealloc
{
    [_window release];
    [_viewController release];
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    
    // Create an instance of the audio controller, set it up and start it running
    self.audioController = [[[AEAudioController alloc] initWithAudioDescription:[AEAudioController nonInterleaved16BitStereoAudioDescription] inputEnabled:YES] autorelease];
    _audioController.preferredBufferDuration = 0.005;
    [_audioController start:NULL];
    
    // Create and display view controller
    self.viewController = [[[ViewController alloc] initWithAudioController:_audioController] autorelease];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
    [self initAudioBus];
    
    return YES;
}

-(void)initAudioBus{
    
    self.audiobusController = [[ABAudiobusController alloc] initWithApiKey:@"MTM5Nzk2MjY5NSoqKlRoZUVuZ2luZVNhbXBsZSoqKlRoZUVuZ2luZVNhbXBsZS5hdWRpb2J1czovLw==:qcxBGEpMHIUT5joltcJVQj6j6Lmk2zixsefDUoYHjTqhepZ6Jqkyry4geGh8rdZJICjSa98OdIu35xP+uoOawhxMfJKz6Hc8jkFP/Im7Se+qNHHtA9fMocHfkb6Ktzzn"];
    
    self.ABFilter = [[ABFilterPort alloc] initWithName:@"Example" title:@"ExampleFilter" audioUnit:self.audioController.audioUnit];
    
    [_audiobusController addFilterPort:_ABFilter];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(connectionsChanged:)
                                                 name:ABConnectionsChangedNotification
                                               object:nil];
    
    
    
}

- (void)connectionsChanged:(NSNotification*)notification {
    if(ABFilterPortIsConnected(self.ABFilter)){
        self.myReceiver = [[AEPlaythroughChannel alloc]initWithAudioController:self.audioController];
    
        
    
        NSArray *ABInputChannels = @[self.myReceiver];
    
        [self.audioController addChannels:ABInputChannels];
        [_audioController addInputReceiver:self.myReceiver];
    }
    
}

@end
