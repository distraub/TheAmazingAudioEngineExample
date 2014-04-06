//
//  ABReceiverPort.h
//  Audiobus
//
//  Created by Michael Tyson on 03/03/2012.
//  Copyright (c) 2011-2014 Audiobus. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>
#import "ABCommon.h"
#import "ABPort.h"

/*!
 * Receiver port connections changed
 *
 *  Sent when the port's connections have changed, caused by connections
 *  or disconnections from within the Audiobus app.
 */
extern NSString * const ABReceiverPortConnectionsChangedNotification;
    
/*!
 * Port added notification
 *
 *  Sent when a new source is added to this port
 */
extern NSString * const ABReceiverPortPortAddedNotification;

/*!
 * Port removed notification
 *
 *  Sent when a source is removed from this port
 */
extern NSString * const ABReceiverPortPortRemovedNotification;

/*!
 * Port key for the userInfo dictionary of port added/remove notifications
 */
extern NSString * const ABReceiverPortPortKey;

@class ABReceiverPort;

/*!
 * Receiver port
 *
 *  This class is used to receive audio sent by other Audiobus-compatible apps. Create an instance using the
 *  @link ABAudiobusController::addReceiverPortNamed:title: addReceiverPortNamed:title: @endlink
 *  method of @link ABAudiobusController @endlink.
 *
 *  You can receive Audiobus audio in a number of ways.
 *
 *  The easiest is to use the @link ABAudiobusAudioUnitWrapper audio unit wrapper @endlink, which takes care
 *  of all receiving and/or sending for you, if your app is built upon Remote IO.
 *
 *  Alternatively, you can use this class directly in one of two ways.
 *
 *  If you provide an @link audioInputBlock audio input block @endlink to this class, then
 *  you will be notified whenever audio becomes available.  Then, you can call @link ABReceiverPortReceive @endlink in
 *  order to access the received audio.
 *
 *  Alternately, you can let this class perform buffering for you, and simply call @link ABReceiverPortReceive @endlink (and
 *  possibly @link ABReceiverPortPeek @endlink) when you need audio.
 *
 *  You can receive audio as separate streams (one per source) or as a single mixed audio stream (see @link receiveMixedAudio @endlink).
 *
 *  Both ABReceiverPortPeek and ABReceiverPortReceive are C functions designed for use within a realtime Core Audio context.
 *  That means they don't hold locks, don't allocate memory and don't call Objective-C methods. You should keep a local pointer
 *  to this class, to be passed as the first parameter to these functions.
 */
@interface ABReceiverPort : ABPort

/*!
 * Initialize
 *
 *  Initializes a new receiver port
 *
 * @param name Name of port, for internal use
 * @param title Title of port, show to the user
 */
- (id)initWithName:(NSString *)name title:(NSString*)title;

/*!
 * Receive audio
 *
 *  Use this C function to receive audio from Audiobus.
 *  It's suitable for use from within a realtime Core Audio context.
 *
 *  Please note that if you are receiving separate streams (@link receiveMixedAudio @endlink is NO), then this function will
 *  provide synchronized streams for each connected source port. The following procedures must be followed:
 *
 *  - All calls to ABReceiverPortReceive must be performed on the same thread.
 *  - You must call @link ABReceiverPortEndReceiveTimeInterval @endlink at the end of each time interval (such as for each render
 *    of your audio system, or each input notification), to tell Audiobus that you are finished with all audio for that interval.
 *    Audio for any sources that you did not receive audio for will be discarded.
 *
 * @param receiverPort         The receiver port.
 * @param sourcePortOrNil   If you are receiving separate streams (@link receiveMixedAudio @endlink is NO), this must be a valid source port - one of the ports from the
 *                          @link sources @endlink array. Otherwise, if you are receiving a mixed stream, pass nil.
 * @param bufferList        The audio buffer list to receive audio into, in the format specified by @link clientFormat @endlink. Must not be NULL.
 *                          If 'mData' pointers are NULL, then an internal buffer will be provided.
 * @param lengthInFrames    The number of frames requested. This method will never return less than the requested frames.
 * @param outTimestamp      On output, if not NULL, the timestamp of the returned audio.
 */
void ABReceiverPortReceive(ABReceiverPort *receiverPort, ABPort *sourcePortOrNil, AudioBufferList *bufferList, UInt32 lengthInFrames, AudioTimeStamp *outTimestamp);

/*!
 * When receiving separate streams, mark the end of the current time interval
 *
 *  When you are receiving separate streams (@link receiveMixedAudio @endlink is NO), and not using an 
 *  @link audioInputBlock @endlink then this function must be called at the end of each time interval to 
 *  signal to Audiobus that you have finished receiving the incoming audio for the given interval.
 *
 *  Note that there's no need to use this function if you are using an @link audioInputBlock @endlink.
 *
 * @param receiverPort         The receiver port.
 */
void ABReceiverPortEndReceiveTimeInterval(ABReceiverPort *receiverPort);

/*!
 * Audio Queue version of ABReceiverPortReceive
 *
 *  You can use this function to pull audio from Audiobus into an Audio Queue buffer. This may be used
 *  inside an AudioQueueInputCallback to replace the audio received from the microphone with audio
 *  from Audiobus, for instance.
 *
 *  See discussion for @link ABReceiverPortReceive @endlink.
 *
 * @param receiverPort         The receiver port.
 * @param sourcePortOrNil   If you are receiving separate streams (@link receiveMixedAudio @endlink is NO), this must be nil. Otherwise, pass the port to receive audio from.
 * @param bufferList        The buffer list to receive audio into, in the format specified by @link clientFormat @endlink. If NULL, then audio will simply be discarded.
 * @param lengthInFrames    The number of frames requested. This method will never return less than the requested frames.
 * @param outTimestamp      On output, if not NULL, the timestamp of the returned audio.
 */
void ABReceiverPortReceiveAQ(ABReceiverPort *receiverPort, ABPort *sourcePortOrNil, AudioQueueBufferRef bufferList, UInt32 lengthInFrames, AudioTimeStamp *outTimestamp);

/*!
 * Determine if the receiver port is currently connected to any sources
 *
 *  This function is suitable for use from within a realtime Core Audio context.
 *
 * @param receiverPort The receiver port.
 * @return YES if there are currently sources connected; NO otherwise.
 */
BOOL ABReceiverPortIsConnected(ABReceiverPort *receiverPort);

/*
 * Whether the port is connected to self
 *
 *  C function suitable for use within Core Audio context, to access the 
 *  @link connectedToSelf @endlink property.
 *
 * @param receiverPort The receiver port.
 * @return YES if one of this port's sources belongs to this app
 */
BOOL ABReceiverPortIsConnectedToSelf(ABReceiverPort *receiverPort);

/*!
 * Set the volume level for a particular source
 *
 *  Note that this applies to the mixed stream as accessed via
 *  ABReceiverPortReceive when the receiveMixedAudio property is YES,
 *  as well as the live mixed stream as accessed via ABReceiverPortReceiveLive.
 *
 *  It does not affect separate streams accessed via ABReceiverPortReceive
 *  when receiveMixedAudio is NO.
 *
 * @param volume            Volume level (0 - 1); default 1
 * @param port              Source port
 */
- (void)setVolume:(float)volume forSourcePort:(ABPort*)port;

/*!
 * Get the volume level for a source
 *
 * @param port              Source port
 * @return Volume for the given port (0 - 1)
 */
- (float)volumeForSourcePort:(ABPort*)port;

/*!
 * Set the pan for a particular source
 *
 *  Note that this applies to the mixed stream as accessed via
 *  ABReceiverPortReceive when the receiveMixedAudio property is YES,
 *  as well as the live mixed stream as accessed via ABReceiverPortReceiveLive.
 *
 *  It does not affect separate streams accessed via ABReceiverPortReceive
 *  when receiveMixedAudio is NO.
 *
 * @param pan               Pan (-1.0 - 1.0); default 0.0
 * @param port              Source port
 */
- (void)setPan:(float)pan forSourcePort:(ABPort*)port;

/*!
 * Get the pan level for a source
 *
 * @param port              Source port
 * @return Pan for the given port (-1.0 - 1.0)
 */
- (float)panForSourcePort:(ABPort*)port;

/*!
 * Currently-connected sources
 *
 *  This is an array of @link ABPort ABPorts @endlink.
 */
@property (nonatomic, strong, readonly) NSArray *sources;

/*!
 * Whether the port is connected
 */
@property (nonatomic, readonly) BOOL connected;

/*!
 * Whether to receive audio as a mixed stream
 *
 *  This setting applies to the lossless receive mode only, not the live, low-latency mode (ABReceiverPortReceiveLive).
 *
 *  If YES (default), then all incoming audio across all sources will be mixed to a single audio stream.
 *  Otherwise, you will receive separate audio streams for each connected port.
 */
@property (nonatomic, assign) BOOL receiveMixedAudio;

/*!
 * Client format
 *
 *  Use this to specify what audio format your app uses. Audio will be automatically
 *  converted from the Audiobus line format.
 *
 *  The default value is @link ABAudioStreamBasicDescription @endlink.
 */
@property (nonatomic, assign) AudioStreamBasicDescription clientFormat;

/*!
 * The title of the port, for display to the user
 */
@property (nonatomic, strong, readwrite) NSString *title;

/*!
 * The port icon (a 32x32 image)
 *
 *  This is optional if your app only has one receiver port, but if your app
 *  defines multiple receiver ports, it is highly recommended that you provide icons
 *  for each, for easy identification by the user.
 */
@property (nonatomic, strong, readwrite) UIImage *icon;

/*!
 * Port attributes
 *
 *  This is a combination of zero or more attributes that are sent to peers to inform them how your app behaves with this port.
 *  For example, if your app is a receiver and plays the received audio out the speaker, then you must indicate
 *  this with the flag ABReceiverPortAttributePlaysLiveAudio, so the sender knows to mute its output.
 *
 *  You may change these flags at any time, and other peers will be informed.
 */
@property (nonatomic, assign) ABReceiverPortAttributes attributes;

/*!
 * The attributes of all upstream ports connected to this port
 *
 *  Currently unused
 */
@property (nonatomic, readonly) ABSenderPortAttributes connectedPortAttributes;

/*!
 * Whether the port is connected to self
 *
 *  This returns YES when the receiver port is connected to a sender or filter port also belonging
 *  to your app.
 *
 *  If your app supports being in multiple positions (input, effect, output) at the same time (ABAudiobusController's
 *  @link ABAudiobusController::allowsMultipleInstancesInConnectionGraph allowsMultipleInstancesInConnectionGraph @endlink
 *  is set to YES), then you should take care to avoid feedback issues when the app's input is being fed from
 *  its own output.
 *
 *  Primarily, this means not sending live output derived from the input through the sender port.
 *
 *  You can use @link ABReceiverPortIsConnectedToSelf @endlink to determine this state from the Core Audio realtime
 *  thread, and perform muting/etc as appropriate.
 *
 *  This property is key-value observable.
 */
@property (nonatomic, readonly) BOOL connectedToSelf;

@end

#ifdef __cplusplus
}
#endif