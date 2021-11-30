#pragma once

/**
 * @addtogroup DiskSpaceManager
 * @{
 */

/// @brief      Initial size(in bytes) of newly created table file.
/// @details    It means 10MiB.
constexpr int INITIAL_TABLE_FILE_SIZE = 10 * 1024 * 1024;

/// @brief      Maximum number of table instances count.
constexpr int MAX_TABLE_INSTANCE = 32;

/// @brief      Initial number of page count in newly created table file.
/// @details    Its value is 2560.
constexpr int INITIAL_TABLE_CAPS = INITIAL_TABLE_FILE_SIZE / MAX_TABLE_INSTANCE;

/// @brief  Size of each page(in bytes).
constexpr int PAGE_SIZE = 4096;

/// @brief  Size of page header(in bytes).
constexpr int PAGE_HEADER_SIZE = 128;

/// @brief  Maximum number of page branches.
constexpr int MAX_PAGE_BRANCHES = 248;

/** @}*/

/**
 * @addtogroup BufferManager
 * @{
 */

/// @brief      Initial number of buffer pages when initializing db.
constexpr int DEFAULT_BUFFER_SIZE = 1024;

/** @}*/

/**
 * @addtogroup IndexManager
 * @{
 */

/// @brief      Redistribution threshold for split leaf nodes.
constexpr int REDISTRIBUTE_THRESHOLD = 2500;
/// @brief      Maximum size of the leaf node record size.
constexpr int MAX_VALUE_SIZE = 112;

/** @}*/