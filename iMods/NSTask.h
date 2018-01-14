/* Generated by RuntimeBrowser
 Image: /System/Library/Frameworks/Foundation.framework/Foundation
 */

@interface NSTask : NSObject

@property (copy) NSArray *arguments;
@property (copy) NSString *currentDirectoryPath;
@property (copy) NSDictionary *environment;
@property (copy) NSString *launchPath;
@property (readonly) int processIdentifier;
@property int qualityOfService;
@property (getter=isRunning, readonly) BOOL running;
@property (retain) id standardError;
@property (retain) id standardInput;
@property (retain) id standardOutput;
@property (copy) id /* block */ terminationHandler;
@property (readonly) int terminationReason;
@property (readonly) int terminationStatus;

+ (id)currentTaskDictionary;
+ (id)launchedTaskWithDictionary:(id)arg1;
+ (id)launchedTaskWithLaunchPath:(id)arg1 arguments:(id)arg2;

- (id)init;
- (void)interrupt;
- (BOOL)isRunning;
- (void)launch;
- (int)processIdentifier;
- (int)qualityOfService;
- (BOOL)resume;
- (void)setQualityOfService:(int)arg1;
- (void)setStandardError:(id)arg1;
- (void)setStandardInput:(id)arg1;
- (void)setStandardOutput:(id)arg1;
- (void)setTerminationHandler:(id /* block */)arg1;
- (id)standardError;
- (id)standardInput;
- (id)standardOutput;
- (BOOL)suspend;
- (int)suspendCount;
- (void)terminate;
- (id /* block */)terminationHandler;
- (int)terminationReason;
- (int)terminationStatus;

@end