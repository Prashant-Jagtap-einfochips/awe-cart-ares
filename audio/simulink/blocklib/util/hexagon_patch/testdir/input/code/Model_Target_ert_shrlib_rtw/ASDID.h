/*
 * File: ASDID.h
 *
 * Code generated for Simulink model 'Model_Target'.
 *
 * Model version                  : 1.12
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Dec 20 15:13:20 2023
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Qualcomm->Hexagon
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_ASDID_h_
#define RTW_HEADER_ASDID_h_
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
 *  _____  _               _                 _     _                   _
 * |  __ \(_)             | |       /\      | |   | |                 (_)
 * | |  | |_ _ __ ___  ___| |_     /  \   __| | __| |_ __ ___  ___ ___ _ _ __   __ _
 * | |  | | | '__/ _ \/ __| __|   / /\ \ / _` |/ _` | '__/ _ \/ __/ __| | '_ \ / _` |
 * | |__| | | | |  __/ (__| |_   / ____ \ (_| | (_| | | |  __/\__ \__ \ | | | | (_| |
 * |_____/|_|_|  \___|\___|\__| /_/    \_\__,_|\__,_|_|  \___||___/___/_|_| |_|\__, |
 *                                                                              __/ |
 *                                                                             |___/
 * ASDID Message ID High Level Parsing
 * =========================
 * The top 8 bits of RTC and TOP message IDs share a common format "ASDID"
 * The ASDID_... macros provide quick parsing and decoding of this format:
 *
 *   Shared ASDID Message ID Layout:
   ~~~{.c}
 *     1+3 bits+ 4 bits +                 +                                   +
 *    31|30 28 | 27  24 |        +        |        +        +        +        |
 *    +-+------------------------+--------+--------+--------+--------+--------+
 *    |R| core | group  |                                                     |
 *    +-+------+--------+-----------------------------------------------------+
 *
 *    R      : 1 bit value "reserved" (usage varies based on group)
 *
 *    core   : 3 bit value, ASDID_CORE
 *             [0-7] = Index of DSP core that handles this message
 *
 *    group  : 4 bit value, ASDID_GROUP
 *             [0]    = RTC message (Control messaging )
 *             [1-15] = TOP message (Room mode data for a feature)
 *
   ~~~
 * TOP (Target Overlay Parameter) Message IDs
 * ==========================================
 * For TOPs, the ASDID Group indicates which "room mode level" feature to address.
 * TOP message IDs add several more fields to the ASDID to describe whether
 * the ID represents a READ or WRITE operation, which bank of data will
 * be involved, and which offset within the bank to access.
 * Note that all TOP requests (both read and write) return a response.
 *
 *   TOP Message ID Layout: (see also https://goo.gl/Zksh2J)
   ~~~{.c}
 *     1+3 bits+ 4 bits +4 bits+        +              20 bits              +
 *    31|30  28| 27  24 | 23 20 |       |        |        |        |        |
 *    +-+------+--------+-------+-------+-----------------+-----------------+
 *    |R| core | group  | bank  |              offset                       |
 *    +-+------+--------+-------+-------------------------------------------+
 *
 *    R       : 1 bit value, see TOPID_IS_READ and TOPID_IS_WRITE
 *              [0] = Write access to Target Overlay Parameter
 *              [1] = Read access to Target Overlay Parameter
 *
 *    core    : 3 bit value, ASDID_CORE
 *              [0-7] = Index of DSP core that handles this message
 *
 *    group   : 4 bit value, ASDID_GROUP
 *              [1-14] = Overlay group to access
 *              Each group represents a feature such as Entertainment Audio,
 *              or EHC (Engine Harmonic Cancellation).
 *
 *    bank    : 4 bit value, TOPID_BANK
 *              [0-15] = Index of bank within the group to access
 *
 *    offset  : 20 bit value, TOPID_OFFSET
 *              [0-1048575] Starting byte offset within the bank to access
 *              (1 Megabyte)
   ~~~
 *
 * RTC (Run Time Control) Message IDs
 * ==========================================
 * RTC messages occupy ASDID Group "0". The RTC message ID includes additional
 * fields to indicate the desired handling priority of the message and of
 * course which specific RTC service is involved. In addition, RTC message IDs
 * provide several "info" bits to describe which payload types the specific
 * RTC supports. These "info" bits are ignored by the RTC handlers but included
 * only for the possible convenience of intermediate communications layers.
 * (e.g. check the into bits to determine whether a response is expected)
 *
 *   RTC Message ID Layout: (see also https://goo.gl/GzM67B)
   ~~~{.c}
 *     1+3 bits+ 4 bits + 4 bits + 4 bits +               16 bits             +
 *    31|30  28| 27  24 | 23 20  | 19  16 |        +        +        +        |
 *    +-+------------------------+--------+--------+--------+--------+--------+
 *    |R| core |  0000  |priority|tsp/info|              index                |
 *    +-+------+--------+-----------------------------------------------------+
 *
 *    R        : 1 bit value, reserved. Not used. "0" is a safe value
 *
 *    core     : 3 bit value, ASDID_CORE
 *               [0-7] = Index of DSP core that handles this message
 *
 *    group    : 4 bit value, ASDID_GROUP
 *               [0] = All RTC and TSP messages have a group of "0"
 *
 *    priority : 4 bit value, RTCID_PRIORITY
 *               [0-14] = High priority (i.e. low latency) message!
 *                        "0" is the highest priority and indicates that this
 *                        message is intended to be handled by the same thread
 *                        that processes the highest call-rate rate audio.
 *               [15] = Regular background priority handling. The message should
 *                        be handled by a background thread at a priority lower
 *                        than any audio sample processing.
 *
 *   tsp/info  : 4 bit further differentiation of the message
 *               see ASDID_IS_RTC, RTCID_SENDS_RSP, RTCID_HAS_NTF
 *               and ASDID_IS_TSP, RTCID_INFO
 *
 *   index     : 16 bit value RTCID_INDEX
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
 *
 *
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
#define TOPID_BANK(id)                 (((id) >> 20) & 0x7UL)    // bank index within group
#define TOPID_OFFSET(id)               (((id)) & 0xfffffUL)      // offset within bank
#define RTCID_PRIORITY(id)             (((id) >> 20) & 0xfUL)    // intended handler thread priority
#define RTCID_INFO(id)                 (((id) >> 16) & 0xfUL)    // TSP/Info bits for RTC ids
#define RTCID_HAS_NTF(id)              (RTCID_INFO(id) == 0x0UL) // true if service can return notifications
#define RTCID_SENDS_RSP(id)            (RTCID_INFO(id) <= 0x1UL) // true if service returns responses
#define RTCID_INDEX(id)                (((id) ) & 0xffffUL)      // RTC service index or TSP index
#define ASDID_CORE(id)                 (((id) >> 28) & 0x7UL)    // processor core
#define ASDID_GROUP(id)                (((id) >> 24) & 0xfUL)    // group, 0=RTC,[1-15]=TOP overlay group
#define ASDID_IS_RTC(id)               ((0x0UL == ASDID_GROUP(id)) && (RTCID_INFO(id) < 0x7UL))
#define ASDID_IS_TSP(id)               ((0x0UL == ASDID_GROUP(id)) && (RTCID_INFO(id) > 0x7UL))
#define ASDID_IS_TOP(id)               (0x0UL != ASDID_GROUP(id))
#define ASDID_IS_SCH(id)               ((0x0UL == ASDID_GROUP(id)) && (RTCID_INFO(id) == 0x7UL))

///@}

/// @ingroup ASDID
/// datatype of status codes in message responses and notifications
typedef int32_t ASDSTATUS;

/** @ref HandleAsdIdRequest may or may not return a response (as defined by
 * the service accessed). If a response *is* returned, it begins with the
 * following status structure
 * @ingroup ASDID
 **/
typedef struct AsdIdStatus_tag
{
    uint32_t msgId;                    // See ASDID_... macros to parse this
    uint32_t callerHandle;             // Echo of request's callerHandle
    ASDSTATUS statusCode;              // See ASDSTAT_CODE_IS_ERROR(code)
    uint8_t data[];                    // Rest of payload (if any) is here
}
AsdIdStatus_type;

/// @ingroup ASDID
/// Macro to check AsdIdStatus_type statusCode values to see if they are errors
#define ASDSTAT_CODE_IS_ERROR(code)    ((ASDSTATUS)(code) < 0)

// ------------------------------------------------------------
// ERROR STATUS CODES RETURNED BY HandleAsdIdRequest ACCESS ROUTINES
// ------------------------------------------------------------
/// @ingroup ASDID
/// @{
#define ASDSTATUS_OK                   (0)                       /// The operation was successful (no error)
#define ASDSTATUS_RSP_BUFFER_SMALL     (-1)                      /// The supplied response buffer is too small
#define ASDSTATUS_REQ_BUFFER_SMALL     (-2)                      /// The supplied request buffer is too small
#define ASDSTATUS_TOP_BAD_GROUP_ID     (-1000)                   /// The group id was not recognized
#define ASDSTATUS_TOP_BAD_BANK_ID      (-1001)                   /// The bank id was not recognized within the given group
#define ASDSTATUS_TOP_OUT_OF_RANGE     (-1002)                   /// The specified offset and byte count extends outside the bank
#define ASDSTATUS_RTC_UNKNOWN_SERVICE  (-2000)                   /// The RTC msgid was not recognized
#define ASDSTATUS_TSP_BAD_STATE_ID     (-3000)                   /// The TSP state value ID was not recognized (i.e. bad name crc value)
#define ASDSTATUS_TSP_CAPTURE_OVERFLOW (-3001)                   /// The TSP capture buffer is not big enough to capture everything (try clearing the capture list)
#define ASDSTATUS_TSP_CAPTURE_ALL_OFF  (-3002)                   /// The TSP advanced trigger capture service is not available (not built into the code)
#define ASDSTATUS_TSP_CAPTURE_THIS_OFF (-3003)                   /// The TSP advanced trigger capture service is disabled for the specific state requested
#define ASDSTATUS_VALUE_NOT_AVAILABLE  (-4000)                   /// The requested value was not found, lookup failed
#define ASDSTATUS_NO_COMPAT_CODES      (-4001)                   /// The built binary does not include compatibility must built separately after Talaria codegen
#define ASDSTATUS_RTC_NOTIFICATION     (0x00000001)              /// Status code of all RTC notification payloads

/// @}

// ------------------------------------------------------------
// DATATYPE CODES
// ------------------------------------------------------------
typedef enum
{
    TSS_DOUBLE = 0,
    TSS_SINGLE = 1,
    TSS_INT8 = 2,
    TSS_UINT8 = 3,
    TSS_INT16 = 4,
    TSS_UINT16 = 5,
    TSS_INT32 = 6,
    TSS_UINT32 = 7,
    TSS_BOOLEAN = 8
}
SimulinkDatatypeId;

#endif                                 /* RTW_HEADER_ASDID_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
