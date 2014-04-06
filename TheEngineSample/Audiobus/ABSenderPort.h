//
//  ABSenderPort.h
//  Audiobus
//
//  Created by Michael Tyson on 25/11/2011.
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
 * Sender port connections changed
 *
 *  Sent when the port's connections have changed, caused by connections
 *  or disconnections from within the Audiobus app.
 */
extern NSString * const ABSenderPortConnectionsChangedNotification;

/*!
 * Sender port
 *
 *  This class is used to transmit audio. Create an instance using the
 *  @link ABAudiobusController::addSenderPortNamed:title: addSenderPortNamed:title: @endlink
 *  method of @link ABAudiobusController @endlink.
 *
 *  There are two ways to send audio with Audiobus.
 *
 *  The easiest is to use the @link ABAudiobusAudioUnitWrapper audio unit wrapper @endlink, which takes care
 *  of all sending and/or receiving for you, if your app is built upon Remote IO.
 *
 *  Alternatively, you can use this class directly, after specifying your @link clientFormat audio format @endlink.
 *
 *  Use this class by keeping a pointer to it from within your audio generation context, then send the audio using the
 *  @link ABSenderPortSendAudio ABSenderPortSendAudio @endlink C function.
 *
 *  This class has been designed to be used from within a realtime Core Audio thread. The ABSenderPortSendAudio function
 *  does not hold locks, allocate memory, or call Objective-C methods.  You should keep a local pointer
 *  to this class, to be passed as the first parameter to these functions.
 */
@interface ABSenderPort : ABPort

/*!
 * Initialize dynamic sender port
 *
 *  Initializes a new sender port. Use @link ABSenderPortSendAudio @endlink to send audio.
 *
 * @param name Name of port, for internal use
 * @param title Title of port, show to the user
 */
- (id)initWithName:(NSString *)name title:(NSString*)title;

/*!
 * Initialize dynamic sender port, with an audio unit
 *
 *  Initializes a new sender port, with an audio unit to be used for generating audio.
 *
 *
 * @param name Name of port, for internal use
 * @param title Title of port, show to the user
 * @param audioUnit The output audio unit to use for sending audio. The audio unit's output will be transmitted.
 */
- (id)initWithName:(NSString *)name title:(NSString*)title audioUnit:(AudioUnit)audioUnit;

/*!
 * Send audio
 *
 *  This C function is used to send audio. It's suitable for use within a realtime thread, as it does not hold locks,
 *  allocate memory or call Objective-C methods.  You should keep a local pointer to the ABSenderPort instance, to be
 *  passed as the first parameter.
 *
 *  Note: If you provided an audio unit when you initialized this class, you cannot use this function.
 *
 * @param senderPort        Sender port.
 * @param audio             Audio buffer list to send, in the @link clientFormat client format @endlink.
 * @param lengthInFrames    Length of the audio, in frames.
 * @param timestamp         The timestamp of the audio.
 * @return Success/failure. In general, this return value can be safely ignored.
 */
BOOL ABSenderPortSendAudio(ABSenderPort* senderPort, const AudioBufferList *audio, UInt32 lengthInFrames, const AudioTimeStamp *timestamp);

/*!
 * Determine if the sender port is currently connected to any destinations
 *
 *  This function is suitable for use from within a realtime Core Audio context.
 *
 * @param senderPort        Sender port.
 * @return YES if there are currently destinations connected; NO otherwise.
 */
BOOL ABSenderPortIsConnected(ABSenderPort* senderPort);

/*!
 * Get connected port attributes
 *
 *  Deprecated in version 2.0
 *
 * @param senderPort        Sender port.
 * @return Connected port attributes.
 */
ABReceiverPortAttributes ABSenderPortGetConnectedPortAttributes(ABSenderPort *senderPort) __deprecated_msg("Use ABSenderPortGetMuted instead");

/*!
 * Determine whether output should be muted
 *
 *  This C function allows you to determine whether your output should be muted. This can be because the
 *  receiving end is handling the audio output for you, or because your app's recently been disconnected
 *  from Audiobus.
 *
 *  Note that this muting is handled for you automatically if you are using an audio unit with the port,
 *  via @link initWithName:title:audioUnit: @endlink.
 *
 *  If you are *not* using an audio unit with the port, then you are required to check this function and
 *  mute your audio output if the return value is YES. You can do this by zeroing your buffers using
 *  memset, and setting the `kAudioUnitRenderAction_OutputIsSilence` flag on the ioActionFlags variable.
 *
 * @param senderPort        Sender port.
 * @return Whether the output should be muted
 */
BOOL ABSenderPortGetIsMuted(ABSenderPort *senderPort);

/*!
 * Get average latency
 *
 *  This C function returns the average transmission latency across all connected remote ports. Where appropriate, use
 *  it to offset generated audio to compensate.
 *
 * @param senderPort        Sender port.
 * @return Latency, in seconds.
 */
NSTimeInterval ABSenderPortGetAverageLatency(ABSenderPort *senderPort);

/*!
 * Currently-connected destinations
 *
 *  This is an array of @link ABPort ABPorts @endlink.
 */
@property (nonatomic, strong, readonly) NSArray *destinations;

/*!
 * Whether the port is connected
 */
@property (nonatomic, readonly) BOOL connected;

/*!
 * Client format
 *
 *  Use this to specify what audio format your app uses. Audio will be automatically
 *  converted to the Audiobus line format.
 *
 *  Note: If you provided an audio unit when you initialized this class, you cannot use this property.
 *
 *  The default value is @link ABAudioStreamBasicDescription @endlink.
 */
@property (nonatomic, assign) AudioStreamBasicDescription clientFormat;

/*!
 * Audio unit
 *
 *  The output audio unit to use for sending audio. The audio unit's output will be transmitted.
 *  If you uninitialize the audio unit passed to this class's initializer, be sure to set this
 *  property to NULL immediately beforehand.
 */
@property (nonatomic, assign) AudioUnit audioUnit;

/*!
 * The title of the port, for display to the user.
 */
@property (nonatomic, strong, readwrite) NSString *title;

/*!
 * The port icon (a 32x32 image)
 *
 *  This is optional if your app only has one sender port, but if your app
 *  defines multiple sender ports, it is highly recommended that you provide icons
 *  for each, for easy identification by the user.
 */
@property (nonatomic, strong, readwrite) UIImage *icon;

/*!
 * Port attributes
 *
 *  Currently unused.
 */
@property (nonatomic, assign) ABSenderPortAttributes attributes;

/*!
 * The attributes of all downstream ports connected to this port
 *
 *  Currently unused in version 2.0.
 *
 *  Note that output muting now taken care of for you if you use an audio unit
 *  with this port.
 */
@property (nonatomic, readonly) ABReceiverPortAttributes connectedPortAttributes;

@end

#ifdef __cplusplus
}
#endif