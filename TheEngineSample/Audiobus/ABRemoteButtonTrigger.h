//
//  ABRemoteButtonTrigger.h
//  Audiobus
//
//  Created by Michael Tyson on 30/08/2013.
//  Copyright (c) 2011-2014 Audiobus. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#import "ABRemoteTrigger.h"

/*!
 * Remote button trigger
 *
 *  This class provides a remote representation for button triggers that are published
 *  by other peers. Button triggers can be activated, altering the state of the app at the other end.
 */
@interface ABRemoteButtonTrigger : ABRemoteTrigger

/*!
 * Activate the trigger
 */
- (void)activate;

@property (nonatomic, readonly) UIImage *icon;          //!< The icon to be displayed when the trigger is in the normal state
@property (nonatomic, readonly) UIImage *selectedIcon;  //!< The icon to be displayed when the trigger is in the selected state
@property (nonatomic, readonly) UIImage *alternateIcon; //!< The icon to be displayed when the trigger is in the alternate state
@property (nonatomic, strong, readonly) UIColor *color;           //!< The color in which the normal icon should be displayed
@property (nonatomic, strong, readonly) UIColor *selectedColor;   //!< The color in which the selected icon should be displayed
@property (nonatomic, strong, readonly) UIColor *alternateColor;  //!< The color in which the alternate icon should be displayed
@property (nonatomic, strong, readonly) NSString *title;          //!< The title of the trigger in the normal state
@property (nonatomic, strong, readonly) NSString *selectedTitle;  //!< The title of the trigger in the selected state
@property (nonatomic, strong, readonly) NSString *alternateTitle; //!< The title of the trigger in the alernate state
@end
    
#ifdef __cplusplus
}
#endif