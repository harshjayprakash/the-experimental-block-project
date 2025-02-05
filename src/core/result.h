/**
 * \file result.h
 * \date 13-08-2024
 * \brief Function definitions, enumerations and structures for the result module.
 *
 * This file contains an enumeration of the result codes, a result structure that can be
 * used as return value and logging. This also include the function definition of creating
 * a result.
 */

#ifndef BLOK_RESULT_H
#define BLOK_RESULT_H

#include <wchar.h>

/**
 * \brief Result codes to dictate specific successes and failures.
 */
typedef enum __BlokResultCode
{
    BlokSuccess = 0x000000,      ///< Operation was successful
    BlokNone = 0x000001,         ///< Operation does not require a result
    BlokFail = 0x000001,         ///< Operation failed
    BlokCannotReInit = 0x000010, ///< Cannot Re-Initialise Error
    BlokNotInit = 0x000011,      ///< Not Initialised Error
    BlokNullPtr = 0x000012,      ///< Null Pointer Error
    BlokNotImpl = 0x000100,      ///< Not Implementated Error
    BlokOutOfRange = 0x000400,   ///< Out of Range Error
} BlokResultCode;

/**
 * \brief Represents a result or exception with a message.
 */
typedef struct __BlokResult
{
    BlokResultCode code;  ///< Result Code
    wchar_t message[260]; ///< Additional Result Message
} BlokResult;

/**
 * \brief Creates a result
 *
 * \param code The result code.
 * \param pMessage The additional message.
 * \return A new stack allocated result.
 */
BlokResult BlokCreateResult(BlokResultCode code, wchar_t *pMessage);

#endif