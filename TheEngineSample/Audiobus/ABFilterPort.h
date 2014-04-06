//
//  ABFilterPort.h
//  Audiobus
//
//  Created by Michael Tyson on 04/05/2012.
//  Copyright (c) 2011-2014 Audiobus. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>
#import "ABPort.h"
#import "ABCommon.h"

/*!
 * Filter port connections changed
 *
 *  Sent when the port's connections have changed, caused by connections
 *  or disconnections from within the Audiobus app.
 */
extern NSString * const ABFilterPortConnectionsChangedNotification;

/*!
 * Audio processing block
 *
 *  This block is called when there is audio to be processed.
 *  Your app should modify the audio in the `audio' parameter.
 *
 * @param audio Audio to be filtered, in the client format you specified
 * @param frames Number of frames of audio
 * @param timestamp The timestamp of the audio
 */
typedef void (^ABAudioFilterBlock)(AudioBufferList *audio, UInt32 frames, AudioTimeStamp *timestamp);
    
/*!
 * Filter port
 *
 *  This class is used to filter audio.  Create an instance using the
 *  @link ABAudiobusController::addFilterPortNamed:title:processBlock: addFilterPortNamed:title:processBlock: @endlink
 *  method of @link ABAudiobusController @endlink, passing in a filter implementation via the processBlock
 *  parameter.
 *
 *  When your filter port is connected as part of an Audiobus connection, the block will be
 *  called as audio enters the system, to process the audio.
 */
@interface ABFilterPort : ABPort

/*!
 * Initialize
 *
 *  Initializes a new filter port, with a block to use for filtering
 *
 * @param name The name of the filter port, for internal use
 * @param title Title of port, show to the user
 * @param processBlock A block to use to process audio as it arrives at the filter port
 * @param processBlockSize Specify the number of frames you want to process each time
 *          the filter block is called. Audiobus will automatically queue up frames until this
 *          number is reached, then call the audio block for this number of frames. Set to
 *          0 if this value is unimportant, and Audiobus will use whatever number of frames results
 *          in the least latency.
 */
- (id)initWithName:(NSString *)name title:(NSString*)title processBlock:(ABAudioFilterBlock)processBlock processBlockSize:(UInt32)processBlockSize;

/*!
 * Initialize
 *
 *  Initializes a new filter port, with an audio unit for filtering
 *
 * @param name The name of the filter port, for internal use
 * @param title Title of port, show to the user
 * @param audioUnit The output audio unit to use for processing. The audio unit's input will be replaced with the audio to process.
 */
- (id)initWithName:(NSString *)name title:(NSString*)title audioUnit:(AudioUnit)audioUnit;

/*!
 * Determine if the filter port is currently connected
 *
 *  This function is suitable for use from within a realtime Core Audio context.
 *
 * @param filterPort        The filter port.
 * @return YES the filter port is currently connected; NO otherwise.
 */
BOOL ABFilterPortIsConnected(ABFilterPort *filterPort);

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
 * @param filterPort        The filter port.
 * @return Whether the output should be muted
 */
BOOL ABFilterPortGetIsMuted(ABFilterPort *filterPort);

/*!
 * Audio unit
 *
 *  The audio unit to use for processing. If you uninitialize the audio unit passed
 *  to this class's initializer, be sure to set this property to NULL immediately beforehand.
 */
@property (nonatomic, assign) AudioUnit audioUnit;

/*!
 * Client format
 *
 *  If you're using a process block for processing, use this to specify what audio 
 *  format to use. Audio will be automatically converted to and from the Audiobus line format.
 *
 *  The default value is @link ABAudioStreamBasicDescription @endlink.
 */
@property (nonatomic, assign) AudioStreamBasicDescription clientFormat;

/*!
 * Currently-connected sources
 *
 *  This is an array of @link ABPort ABPorts @endlink.
 */
@property (nonatomic, strong, readonly) NSArray *sources;

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
 * The attributes of all downstream ports connected to this port
 *
 *  Currently unused.
 */
@property (nonatomic, readonly) ABReceiverPortAttributes connectedDownstreamPortAttributes;

/*!
 * The attributes of all upstream ports connected to this port
 *
 *  Currently unused.
 */
@property (nonatomic, readonly) ABSenderPortAttributes connectedUpstreamPortAttributes;

/*!
 * The title of the port, for display to the user
 */
@property (nonatomic, strong, readwrite) NSString *title;

/*!
 * The port icon (a 32x32 image)
 *
 *  This is optional if your app only has one filter port, but if your app
 *  defines multiple filter ports, it is highly recommended that you provide icons
 *  for each, for easy identification by the user.
 */
@property (nonatomic, strong, readwrite) UIImage *icon;

/*!
 * The constant latency of this filter, in frames
 *
 *  If your filter code adds a constant amount of latency to the audio stream
 *  (such as an FFT or lookahead operation), you should specify that here in order
 *  to have Audiobus automatically account for it.
 *
 *  This is important when users have the same input signal going through different
 *  paths, so that Audiobus can synchronize these properly at the output. If you don't
 *  specify the correct latency, the user will hear phasing due to incorrectly aligned
 *  signals at the output.
 *
 *  Default: 0
 *
 */
@property (nonatomic, assign) UInt32 latency;

/*!
 * Whether the port is bypassed
 *
 *  This property value is automatically managed by an in-built trigger that appears
 *  to users from within the Connection Panel. If your filter app also provides its own
 *  bypass controls, you can use this property to keep the state of the built-in bypass
 *  feature and your app's own bypass feature synchronized.
 */
@property (nonatomic, assign) BOOL bypassed;

@end

#ifdef __cplusplus
}
#endif
