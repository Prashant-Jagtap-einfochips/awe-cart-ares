%% TLC code generation file - hand written source
%% INCLUDED BY: FeaturePath_SFunc.tlc

#include <stdint.h>

// Code Generation Note:
// (The following code originates from "tlc_ASDID.h", pulled in via FeaturePath_SFunc.tlc)

#if defined(_WIN32) || defined(_WIN64)
// For Windows builds, turn off warning about zero length struct members ("uint8_t data[]")
#pragma warning(disable : 4200)
// For Windows builds, turn off warning about sscanf being unsafe. We use in translation engine
#pragma warning(disable : 4996)
#endif
        
/** @defgroup ASDID Message IDs
 *
 *  _____           _ _               _                 _     _                   _             
 * |_   _|         | (_)             | |       /\      | |   | |                 (_)            
 *   | |  _ __   __| |_ _ __ ___  ___| |_     /  \   __| | __| |_ __ ___  ___ ___ _ _ __   __ _ 
 *   | | | '_ \ / _` | | '__/ _ \/ __| __|   / /\ \ / _` |/ _` | '__/ _ \/ __/ __| | '_ \ / _` |
 *  _| |_| | | | (_| | | | |  __/ (__| |_   / ____ \ (_| | (_| | | |  __/\__ \__ \ | | | | (_| |
 * |_____|_| |_|\__,_|_|_|  \___|\___|\__| /_/    \_\__,_|\__,_|_|  \___||___/___/_|_| |_|\__, |
 *                                                                                         __/ |
 *                                                                                       |___/ 
 * ASDID Message ID High Level Parsing
 * ===================================
 * The message identifier / token used to communicate with RTC, TOP, and TSP
 * resources in the generated code all share the same 32-bit message ID format.
 * The top 12 bits of the shared message layout are shown below. TOP, RTC, and TSP
 * resources are collected into partitions which are members of an overlay group.
 * The message "type" indicates whether the ASDID is an RTC, TSP, or TOP
 *
 *   Shared ASDID Message ID Layout: 
 ~~~{.c}
 *    1+1+2 bits+ 4 bits   + 4 bits   +        |                                   +
 *   31 30 29 28| 27  24   | 23    20 |        |        |        |        |        |
 *   +-+-+------+----------+----------+--------+--------+--------+--------+--------+
 *   |R|P| type |partition#| group #  |                                            |
 *   +-+-+------+----------+-------------------------------------------------------+
 *  
 *    R      : 1 bit value used only for TOPs to indicate a READ operation
 *             see TOPID_IS_READ
 *
 *    P      : 1 bit value indicating high priority when "1"
 *             see RTCID_PRIORITY
 *
 *    type   : 2 bit value indicating the type of message
 *             see ASDID_TYPE
 *             %<::ASDID_TYPE_TOP> = Indicates the ID is for a TOP
 *             %<::ASDID_TYPE_RTC> = Indicates the ID is an RTC
 *             %<::ASDID_TYPE_TSP> = Indicates the ID is TSP related
 *             3 = Reserved (unused)
 *  
 *    partition# : 4 bit value, ASDID_PARTITION
 *             [0-15] = Atomic module number within the indicated group
 *
 *    group# : 4 bit value, ASDID_GROUP
 *             [0-15] = Indicates which feature (overlay group) of which
 *                      the partition is a member
 *  
 ~~~
 * TOP (Target Overlay Parameter) Message IDs
 * ==========================================
 * Use ASDID_IS_TOP(msgid) to recognize message ids which represent TOP access.
 * For TOPs, the ASDID Group indicates which "room mode level" feature to address.
 * TOP message IDs use the MSB "R" bit of the ASDID to describe whether
 * the ID represents a READ or WRITE operation.
 * Note that all TOP requests (both read and write) return a response.
 *
 *   TOP Message ID Layout: (see also http://bit.ly/2Gq4VGY)
 ~~~{.c}
 *    1+1+2 bits+ 4 bits   + 4 bits   + 4 bits +        16 bits                    +
 *   31 30 29 28| 27  24   |  23   20 |        +        |        |        |        |
 *   +-+-+------+----------+----------+--------+--------+--------+--------+--------+
 *   |R|P| type |partition#| group #  |  0000  |      parameter index              |
 *   +-+-+------+----------+-------------------------------------------------------+
 *   
 *    R       : 1 bit value, see TOPID_IS_READ and TOPID_IS_WRITE
 *              [0] = Write access to Target Overlay Parameter
 *              [1] = Read access to Target Overlay Parameter
 *
 *    P      : 1 bit value indicating high priority when "1".
 *             see RTCID_PRIORITY
 *             This bit is not intended for use with TOPs - but it could be...
 *
 *    type   : 2 bit value indicating the type of message, see ASDID_TYPE
 *             %<::ASDID_TYPE_TOP> is the value indicating a TOP token identifier
 *  
 *    partition# : 4 bit value, see ASDID_PARTITION
 *             [0-15] = Atomic module number within the indicated group
 *
 *    group# : 4 bit value, see ASDID_GROUP
 *             [0-15] = Indicates which feature (overlay group) of which
 *                      the partition is a member
 *   
 *    index  : 16 bit value, see TOPID_INDEX
 *              [0-65535] Index of the TOP within specified partition
 *
 ~~~
 *
 * RTC (Run Time Control) Message IDs
 * ==========================================
 * Use ASDID_IS_RTC(msgid) to recognize message ids which represent RTC messages.
 * The priority bit "P" is set high to indicate that an RTC message should be
 * handled by a high priority task. RTC message ids also define an additional
 * "INFO" field of 4 bits to announce whether the RTC service includes the use
 * of response and notification payloads. These "info" bits are ignored by the
 * RTC handlers and included in the ASDID only for the possible convenience of
 * intermediate communications layers that may wish to know the RTC protocol at
 * play for a particular RTC message id.
 * (e.g. one could check the into bits to determine whether a response is expected)
 *
 *   RTC Message ID Layout: (see also http://bit.ly/2PbwfLQ)
 ~~~{.c}
 *    1+1+2 bits+ 4 bits   + 4 bits   +  4 bits|         16 bits                   +
 *   31 30 29 28| 27  24   | 23    20 | 19  16 |        |        |        |        |
 *   +-+-+------+----------+----------+--------+--------+--------+--------+--------+
 *   |R|P| type |partition#| group #  |  info  |        message index              |
 *   +-+-+------+----------+-------------------------------------------------------+
 *
 *    R         : 1 bit value, completely igored by RTC handlers
 *
 *    P         : 1 bit value, see RTCID_PRIORITY
 *              [0] = Handle with normal priority lower than all audio processing tasks
 *              [1] = Handle with high priority up at audio processing task levels
 *
 *    type      : 2 bit value indicating the type of message, see ASDID_TYPE
 *              %<::ASDID_TYPE_RTC> is the value indicating an RTC message
 *
 *    partition : 4 bit value, see ASDID_PARTITION
 *               [0-15] = Indicates which atomic partition contains this service
 *
 *    group     : 4 bit value, see RTCID_GROUP
 *               [0-15] = Which highest level feature offers the service
 *
 *    tsp/info  : 4 bit further differentiation of the message, see RTCID_INFO
 *               see also RTCID_SENDS_RSP, RTCID_HAS_NTF
 *
 *    index     : 16 bit value, see RTCID_INDEX
 *               [0-65535] Index of RTC service involved
 ~~~
 *
 * TSP (Target State Probe) Message IDs: see ASDID_IS_TSP
 * ==========================================
 * Target State Probe messages coordinate the collection and retrieval of
 * state data parameters during operation. The support two methods of data
 * capture: triggered-capture and blind-read. The blind-read capture is
 * very much like and RTC or TOP in that it happens on a low-priority
 * thread which is easily interrupted by audio processing- thereby
 * gathering "corrupt" data from differing processing frames. In the TSP's
 * triggered-capture mode, the audio threads themselves perform the capture
 * so contiguous transaction-safe data capture is possible, but at the cost
 * of moderate MIPs (e.g. the data copy).
 *
 ~~~
 *
 * SCH (Static Command Handler) Message IDs: see ASDID_IS_SCH
 * ==========================================
 * Static Command Handler message tokens are valid regardless of Talaria's
 * addressing mode. Unlike the other types of tokens (TOP, RTC, TSP), the
 * SCH tokens are not categorized by Group# and do not appear or disappear
 * based on the content of the Simulink canvas. These select few commands
 * include version and compatibility checks and are handled very much like
 * RTC messages other than they are built-in and always present.
 * Also, even for indirect addressing mode, these tokens specify the core#
 *
 *     1+3 bits+ 4 bits + 4 bits + 4 bits +               16 bits             +
 *    31|30  28| 27  24 | 23 20  | 19  16 |        +        +        +        |
 *    +-+------------------------+--------+--------+--------+--------+--------+
 *    |R| core |  0000  |  1111  |  0111  |              index                |
 *    +-+------+--------+-----------------------------------------------------+
 *
 *    R is ignored
 *    core indicates target core # (0-7)
 *    index is the command index within the SCH command set
 ~~~
 */

 // ------------------------------------------------------------
 /// Message ID Parsing Macros
 /// ------------------------------------------------------------
 /// @ingroup ASDID
 /// @{

// Provide endian swapper that works even with immediate (constant) values
#define ENDIAN_SWAP32(id)              (((id)&0xFF000000UL)>>24) | (((id)&0x00FF0000UL)>>8) | (((id)&0x0000FF00UL)<<8) | (((id)&0x000000FFUL)<<24)
#define FAIL_BUILD_IF_FALSE(condition) ((void)sizeof(char[1 - 2*!(condition)]))

#define TOPID_IS_READ(id)              ((((id) >> 31) & 0x1UL) != 0) // read access of TOP value
#define TOPID_IS_WRITE(id)             ((((id) >> 31) & 0x1UL) == 0) // write access of TOP value
#define TOPID_INDEX(id)                (((id) ) & 0xffffUL)          // index of top within partition

#define RTCID_PRIORITY(id)             (((id) >> 30) & 0x1UL)      // intended handler thread priority
#define RTCID_INFO(id)                 (((id) >> 16) & 0xfUL)      // TSP/Info bits for RTC ids
#define RTCID_HAS_NTF(id)              (RTCID_INFO(id) == 0x0UL)   // true if service can return notifications
#define RTCID_SENDS_RSP(id)            (RTCID_INFO(id) <= 0x1UL)   // true if service returns responses
#define RTCID_INDEX(id)                (((id) ) & 0xffff)          // RTC service index or TSP index

#define TSPID_IS_READ(id)               ((((id) >> 31) & 0x1UL) == 0)   // read access of TSP value
#define TSPID_IS_WRITE(id)              ((((id) >> 31) & 0x1UL) != 0)   // write access of TSP value

#define ASDID_GROUP(id)                (((id) >> 20) & 0xfUL)      // [0-15] = overlay group
#define ASDID_PARTITION(id)            (((id) >> 24) & 0xfUL)      // [0-15] = partition
#define ASDID_TYPE(id)                 (((id) >> 28) & 0x3UL)      // type of message (RTC,TSP,TOP)
#define ASDID_IS_TOP(id)               (!ASDID_IS_SCH(id) && (%<::ASDID_TYPE_TOP>UL == ASDID_TYPE(id)))
#define ASDID_IS_RTC(id)               (!ASDID_IS_SCH(id) && (%<::ASDID_TYPE_RTC>UL == ASDID_TYPE(id)))
#define ASDID_IS_TSP(id)               (!ASDID_IS_SCH(id) && (%<::ASDID_TYPE_TSP>UL == ASDID_TYPE(id)))
#define ASDID_IS_SCH(id)               ((0x0fff000UL & (id)) == 0x00f70000UL)

#define SCHID_CORE_NUM(id) (((id) >> 28) & 0x7UL) // only valid for SCH-type tokens

#define ASDID_ATOMICID(asdId)          ((ASDID_GROUP(asdId) * 16UL) + ASDID_PARTITION(asdId))
///@}

/// @ingroup ASDID
/// datatype of status codes in message responses and notifications
typedef int32_t ASDSTATUS;

/// @ingroup ASDID
/// datatype of the tokens themselves
typedef uint32_t TalariaToken_t;

/** @ref HandleAsdIdRequest may or may not return a response (as defined by
 * the service accessed). If a response *is* returned, it begins with the
 * following status structure
 * @ingroup ASDID
 **/
typedef struct AsdIdStatus_tag
{
    uint32_t            msgId;          // See ASDID_... macros to parse this
    uint32_t            callerHandle;   // Echo of request's callerHandle
    ASDSTATUS           statusCode;     // See ASDSTAT_CODE_IS_ERROR(code)
    uint8_t             data[];         // Rest of payload (if any) is here
} AsdIdStatus_type;

/// @ingroup ASDID
/// Macro to check AsdIdStatus_type statusCode values to see if they are errors
#define ASDSTAT_CODE_IS_ERROR(code) ((ASDSTATUS)(code) < 0)

// ------------------------------------------------------------
// ERROR STATUS CODES RETURNED BY HandleAsdIdRequest ACCESS ROUTINES
// ------------------------------------------------------------
/// @ingroup ASDID
/// @{
#define ASDSTATUS_OK                   (0)           /// The operation was successful (no error)
#define ASDSTATUS_RSP_BUFFER_SMALL     (-1)          /// The supplied response buffer is too small
#define ASDSTATUS_REQ_BUFFER_SMALL     (-2)          /// The supplied request buffer is too small
%% INDIRECT-ADDRESSING ONLY:
#define ASDSTATUS_TOP_BAD_PARTITION_ID (-1000)       /// The group & partition id was not recognized
#define ASDSTATUS_TOP_BAD_INDEX        (-1001)       /// The TOP index was beyond what the partition contains
%%
#define ASDSTATUS_TOP_OUT_OF_RANGE     (-1002)       /// The specified offset and byte count extends outside the bank
#define ASDSTATUS_RTC_UNKNOWN_SERVICE  (-2000)       /// The RTC msgid was not recognized
#define ASDSTATUS_TSP_BAD_STATE_ID     (-3000)       /// The TSP state value ID was not recognized (i.e. bad name crc value)
#define ASDSTATUS_TSP_CAPTURE_OVERFLOW (-3001)       /// The TSP capture buffer is not big enough to capture everything (try clearing the capture list)
#define ASDSTATUS_TSP_CAPTURE_ALL_OFF  (-3002)       /// The TSP advanced trigger capture service is not available (not built into the code)
#define ASDSTATUS_TSP_CAPTURE_THIS_OFF (-3003)       /// The TSP advanced trigger capture service is disabled for the specific state requested
#define ASDSTATUS_TSP_WRITE_IMPOSSIBLE (-3004)       /// The specific target state requested is constant and cannot be changed
#define ASDSTATUS_VALUE_NOT_AVAILABLE  (-4000)       /// The requested value was not found, lookup failed
#define ASDSTATUS_NO_COMPAT_CODES      (-4001)       /// The built binary does not include compatibility must built separately after Talaria codegen
#define ASDSTATUS_RTC_NOTIFICATION     (0x00000001)  /// Status code of all RTC notification payloads
/// @}
