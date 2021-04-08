#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef id<NSObject> OCResultValueSupertype;
typedef OCResultValueSupertype OCResultNewValueType;

typedef NS_ENUM(NSUInteger, OCResultKind) {
    /*! The result represents a success and holds a value. */
    OCResultSuccess,
    /*! The result represents a failure and holds an error. */
    OCResultFailure,
};

/*!
 @abstract OCResult holds either a value or an error.
 @discussion A result object is a union of an operation result or an error.
 One and only one thing is present at a time.
 It helps to avoid proliferation of "if not nil" checks in your code,
 because the result always is guaranteed to contain something.
 It also helps to make sure that every error is either handled
 or passed along and not lost.
 */
@interface OCResult<ValueType: OCResultValueSupertype> : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/*! Constructs a result that holds a value. */
+ (OCResult<ValueType> *)success:(ValueType)value;
/*! Constructs a result that holds an error. */
+ (OCResult<ValueType> *)failure:(NSError *)error;

/*!
 Either OCResultSuccess or OCResultFailure.
 It is recommended to use a switch statement on the kind before querying the value/error.
 */
@property (readonly) OCResultKind kind;
/*! A value contained in a success result. Asserts if it is a failure result. */
@property (readonly) ValueType value;
/*! An error contained in a failure result. Asserts if it is a success result. */
@property (readonly) NSError *error;

/*!
 Replaces a success result value and wraps it inside a new success result object.
 A failure result is returned unchanged.
 @param transform A value mapping block that takes an existing result value and returns a new result value.
 */
- (OCResult<OCResultNewValueType> *)map:(OCResultNewValueType(^ NS_NOESCAPE)(ValueType value))transform;
/*!
 Replaces a failure result error and wraps it inside a new failure result object.
 A success result is returned unchanged.
 @param transform An error mapping block that takes an existing result error and returns a new result error.
 */
- (OCResult<ValueType> *)mapError:(NSError * (^ NS_NOESCAPE)(NSError * error))transform;

/*!
 Provides a new result based on the original success result value.
 A failure result is returned unchanged.
 @param transform A constructor block that takes an existing result value and produces a new result object of any kind.
 @return An OCResult containing a value of any new type. Must match the type in the transform block return type.
 */
- (OCResult<OCResultNewValueType> *)flatMap:(OCResult<OCResultNewValueType> * (^ NS_NOESCAPE)(ValueType value))transform;
/*!
 Provides a new result based on the original failure result error.
 A success result is returned unchanged.
 @param transform A constructor block that takes an existing result error and produces a new result object of any kind.
 */
- (OCResult<ValueType> *)flatMapError:(OCResult<ValueType> * (^ NS_NOESCAPE)(NSError * error))transform;

@end

NS_ASSUME_NONNULL_END
