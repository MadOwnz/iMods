//
//  IMONetworkManager.m
//  iMods
//
//  Created by Ryan Feng on 7/30/14.
//  Copyright (c) 2014 Ryan Feng. All rights reserved.
//

#import <AFNetworking/AFURLRequestSerialization.h>
#import <AFNetworking/AFURLResponseSerialization.h>

#import "IMONetworkManager.h"
#import "IMONetworkErrorResponse.h"

#import "IMOUser.h"
#import "IMOBillingInfo.h"
#import "IMOCategory.h"
#import "IMODevice.h"
#import "IMOItem.h"
#import "IMOOrder.h"
#import "IMOResponse.h"
#import "IMOReview.h"

@implementation IMONetworkManager

#pragma mark -
#pragma mark Static objects

static IMONetworkManager * _sharedNetworkManager =  nil;
static dispatch_once_t _onceToken = 0;
static AFJSONRequestSerializer* _jsonRequestSerializer = nil;
static AFJSONResponseSerializer* _jsonResponseSerializer = nil;

#pragma mark -
#pragma mark Initialization

+ (IMONetworkManager*) sharedNetworkManager:(NSURL*)baseAPIEndpoint {
    if (_sharedNetworkManager != nil) {
        return _sharedNetworkManager;
    }
    dispatch_once(&_onceToken, ^{
        _sharedNetworkManager = [[IMONetworkManager alloc] initWithBaseURL:baseAPIEndpoint];
        _jsonRequestSerializer = [[AFJSONRequestSerializer alloc] init];
        _jsonResponseSerializer = [[AFJSONResponseSerializer alloc] init];
        _sharedNetworkManager.requestSerializer = _jsonRequestSerializer;
    });
    
    return _sharedNetworkManager;
}

+ (IMONetworkManager*) sharedNetworkManager{
    if(_sharedNetworkManager == nil){
        NSLog(@"Shared network manager is not initialized corrected, call sharedNetworkManager:baseUrl to initialize.");
        return nil;
    }
    return _sharedNetworkManager;
}

- (id) init {
    self = [super init];
    return self;
}

#pragma mark -
#pragma mark Overloaded methods

+ (NSDictionary *)modelClassesByResourcePath {
    return @{
             @"user/register": [IMOUser class],
             @"user/profile": [IMOUser class],
             @"user/login": [IMOResponse class],
             @"user/logout": [IMOResponse class],
             @"user/reset_password/*": [IMOResponse class],
             @"user/update": [IMOResponse class],
             
             @"billing/list": [IMOBillingInfo class],
             @"billing/id/*": [IMOBillingInfo class],
             @"billing/add": [IMOBillingInfo class],
             @"billing/update/*": [IMOResponse class],
             @"billing/delete/*": [IMOResponse class],
             
             @"category/featured": [IMOCategory class],
             @"category/id/*": [IMOCategory class],
             @"category/name/*": [IMOCategory class],
             @"category/update/*": [IMOResponse class],
             @"category/delete/*": [IMOResponse class],
             
             @"device/add": [IMODevice class],
             @"device/list": [IMOResponse class],
             
             @"item/list": [IMOItem class],
             @"item/id/*": [IMOItem class],
             @"item/pkg/*": [IMOItem class],
             @"item/add": [IMOResponse class],
             @"item/update/*": [IMOResponse class],
             @"item/delete/*": [IMOResponse class],
             @"item/featured": [IMOItem class],
             
             @"order/list": [IMOOrder class],
             @"order/id/*": [IMOOrder class],
             @"order/add": [IMOOrder class],
             @"order/update/*": [IMOOrder class],
             @"order/cancel/*": [IMOOrder class],
             
             @"wishlist": [IMOItem class],
             @"wishlist/add": [IMOResponse class],
             @"wishlist/delete/*": [IMOResponse class],
             @"wishlist/clear": [IMOResponse class],
             
             @"review/add": [IMOReview class],
             @"review/list": [IMOReview class],
             @"review/user/*": [IMOReview class],
             @"review/item/*": [IMOReview class],
             @"review/delete/*": [IMOResponse class],
             };
}

@end
