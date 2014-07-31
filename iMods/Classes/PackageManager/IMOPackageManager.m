//
//  IMOPackageManager.m
//  iMods
//
//  Created by Ryan Feng on 7/30/14.
//  Copyright (c) 2014 Ryan Feng. All rights reserved.
//

#import "IMOPackageManager.h"

@implementation IMOPackageManager

+ (IMOPackageManager*) sharedPackageManager {
    static IMOPackageManager* sharedIMOPackageManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken,^{
        sharedIMOPackageManager = [[IMOPackageManager alloc] init];
    });
    return sharedIMOPackageManager;
}

- (id) init{
    if(self == [super init]){
        // initialize properties
    }
    return self;
}
@end
