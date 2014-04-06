//
//  ABVariableTrigger.h
//  Audiobus
//
//  Created by Michael Tyson on 30/08/2013.
//  Copyright (c) 2011-2014 Audiobus. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#import <Foundation/Foundation.h>
#import "ABTrigger.h"

typedef enum {
    ABVariableTriggerDisplayTypeSlider,
    ABVariableTriggerDisplayTypeDial
} ABVariableTriggerDisplayType;

/*!
 *  Variable trigger
 *
 *  This class implements a [trigger](@ref ABTrigger) that takes a variable
 *  floating-point value, and can be represented as a slider or a dial.
 */
@interface ABVariableTrigger : ABTrigger

/*!
 * Create a variable trigger
 *
 * @param title A user-readable title (used for accessibility)
 * @param block Block to be called when trigger value is changed
 */
+ (ABVariableTrigger*)variableTriggerWithTitle:(NSString*)title block:(ABTriggerPerformBlock)block;

/*!
 * The current value
 */
@property (nonatomic, assign) float value;

/*!
 * The maximum possible value
 */
@property (nonatomic, assign) float maximumValue;

/*!
 * The minimum possible value
 */
@property (nonatomic, assign) float minimumValue;

/*!
 * The trigger title
 */
@property (nonatomic, strong) NSString *title;

/*!
 * How the trigger should be displayed
 *
 *  Default value is ABVariableTriggerDisplayTypeSlider
 */
@property (nonatomic, assign) ABVariableTriggerDisplayType displayType;

/*!
 * An icon to be displayed at the maximum end of the slider (does not apply to dial display mode)
 */
@property (nonatomic, strong) UIImage *maximumImage;

/*!
 * An icon to be displayed at the maximum end of the slider (does not apply to dial display mode)
 */
@property (nonatomic, strong) UIImage *minimumImage;

@end


#ifdef __cplusplus
}
#endif