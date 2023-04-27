#pragma once

KLIBAPI
/**
 * @brief Convert a Intermediate Language file into a Binary file.
 *
 * @param filename A file with Intermediate Language code.
 * @param output Path of the file to write the output.
 * @return 0 on failure, 1 on success.
 */
kbyte klCompileIntermediateFile(const char* filename, const char* output);