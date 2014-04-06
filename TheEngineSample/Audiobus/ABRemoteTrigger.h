//
//  ABRemoteTrigger.h
//  Audiobus
//
//  Created by Michael Tyson on 16/05/2012.
//  Copyright (c) 2011-2014 Audiobus. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#import <Foundation/Foundation.h>
#import "ABTrigger.h"

@class ABPeer;

/*!
 * Remote trigger
 *
 *  This base class represents the common features of triggers that are published
 *  by other peers. These triggers can be manipulated to change the state of the remote peer.
 */
@interface ABRemoteTrigger : NSObject

/*!
 * Host peer
 *
 *  This is the peer that published the trigger
 */
@property (nonatomic, strong, readonly) ABPeer *peer;

/*!
 * Trigger state
 */
@property (nonatomic, readonly) ABTriggerState state;

/*!
 * A numeric identifier for this trigger
 */
@property (nonatomic, readonly) uint32_t numericIdentifier;

@end

#ifdef __cplusplus
}
#endif