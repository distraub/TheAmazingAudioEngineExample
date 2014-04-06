//
//  ABCommon.h
//  Audiobus
//
//  Created by Michael Tyson on 27/01/2012.
//  Copyright (c) 2011-2014 Audiobus. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>

/*!
 * Receiver port attributes
 */
enum {
    ABReceiverPortAttributeNone            = 0x0,//!< No attributes
    ABReceiverPortAttributePlaysLiveAudio  = 0x1  //!< The receiver will play the received audio out loud, live.
                                               //!< Connected senders should mute their output.
};
typedef uint8_t ABReceiverPortAttributes;

/*!
 * Sender port attributes (currently unused)
 */
enum {
    ABSenderPortAttributeNone            = 0x0,
};
typedef uint8_t ABSenderPortAttributes;

/*!
 * App capabilities
 */
typedef enum {
    ABAppCapabilitySend = 1,
    ABAppCapabilityReceive = 2,
    ABAppCapabilityFilter = 4
} ABAppCapability;

/*!
 * Connection panel position
 *
 *  Defines the positioning of the connection panel in your app, when it is visible.
 */
typedef enum {
    ABConnectionPanelPositionRight,
    ABConnectionPanelPositionLeft,
    ABConnectionPanelPositionBottom
} ABConnectionPanelPosition;

/*!
 * Peer resource identifier
 */
typedef uint32_t ABPeerResourceID;

/*!
 * Audiobus line format's sample rate
 */
#define ABAudioFormatSampleRate             44100.0

/*!
 * Audiobus line format
 */
extern const AudioStreamBasicDescription ABAudioStreamBasicDescription;

/*!
 * Determine if ASBD is compatible with the Audiobus line format without automatic conversion
 */
bool ABASBDIsCompatible(AudioStreamBasicDescription asbd);

int _ABAssert(int condition, char* msg, char* file, int line);

#define ABAssert(condition,msg) (_ABAssert((condition),(msg),strrchr(__FILE__, '/')+1,__LINE__))

    
#ifdef __cplusplus
}
#endif
