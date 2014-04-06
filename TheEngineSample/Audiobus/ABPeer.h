//
//  ABPeer.h
//  Audiobus
//
//  Created by Michael Tyson on 10/12/2011.
//  Copyright (c) 2011-2014 Audiobus. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#import <UIKit/UIKit.h>
#import "ABCommon.h"

@class ABPort;

/*!
 * Audiobus peer
 *
 *  This class represents another Audiobus-compatible app, either running on the
 *  local device, or on another device accessed over the network.
 */
@interface ABPeer : NSObject
@property (nonatomic, strong, readonly) NSString *name;         //!< Peer name
@property (nonatomic, readonly) NSString *deviceName;           //!< Name of device peer is on
@property (nonatomic, strong, readonly) NSString *displayName;  //!< Peer display name
@property (nonatomic, readonly) BOOL present;                   //!< Whether the peer is currently present and accessible
@property (nonatomic, strong, readonly) NSURL *launchURL;       //!< The app's launch URL
@property (nonatomic, readonly) UIImage *icon;                  //!< App icon
@property (nonatomic, strong, readonly) NSArray *receiverPorts; //!< The receiver ports offered by the peer (array of @link ABPort @endlink).
@property (nonatomic, strong, readonly) NSArray *senderPorts;   //!< The sender ports offered by the peer (array of @link ABPort @endlink).
@property (nonatomic, strong, readonly) NSArray *filterPorts;   //!< The filter ports offered by the peer (array of @link ABPort @endlink).
@property (nonatomic, strong, readonly) NSArray *triggers;      //!< The available remote triggers
@end

#ifdef __cplusplus
}
#endif