//
//  IMOTabBarController.h
//  iMods
//
//  Created by Brendon Roberto on 9/30/14.
//  Copyright (c) 2014 Ryan Feng. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "IMOLoginViewController.h"


@interface IMOTabBarController : UITabBarController <IMOLoginDelegate, UITabBarDelegate>
- (IBAction)unwindToTabBarController:(UIStoryboardSegue *)sender;
- (CGPoint)getSelectedIndicatorCenterPoint;
- (void)appendSelector;
@property NSString *pass;
@property (strong, nonatomic) UIView *selectedItemIndicatorView;

@end
