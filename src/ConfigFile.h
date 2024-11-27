/// \file ConfigFile.h
/// \author Paolo Mazzon
/// \brief Saves and loads .ini files
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _VKSK_ConfigKey {
	const char *key;
	const char *val;
} _VKSK_ConfigKey;

typedef struct _VKSK_ConfigHeader {
	const char *header;
	_VKSK_ConfigKey *keys;
	int size;
} _VKSK_ConfigHeader;

typedef struct VKSK_Config_t {
	_VKSK_ConfigHeader *headers; // List of headers
	int size; // How many headers in memory
} *VKSK_Config;

// Loads an ini file or returns an empty config if it doesn't exist
// The parser is not fancy
//  + if a line starts with ';' its a comment and ignored
//  + if a line starts with '[' and ends with ']' everything between is the header
//  + if its not a header and there is a '=', everything before it is the key and everything after is the value
VKSK_Config vksk_ConfigLoad(const char *filename);

// Loads an ini file or returns an empty config if it doesn't exist
// The parser is not fancy
//  + if a line starts with ';' its a comment and ignored
//  + if a line starts with '[' and ends with ']' everything between is the header
//  + if its not a header and there is a '=', everything before it is the key and everything after is the value
VKSK_Config vksk_ConfigLoadFromString(const char *string);

// Saves a config to a file
void vksk_ConfigFlush(VKSK_Config config, const char *filename);

// Gets a string from the config or returns def if the key/header doesn't exist
const char *vksk_ConfigGetString(VKSK_Config config, const char *header, const char *key, const char *def);

// Gets a double from the config or returns def if the key/header doesn't exist
double vksk_ConfigGetDouble(VKSK_Config config, const char *header, const char *key, double def);

// Gets a bool from the config file or returns def if the key/header doesn't exist
bool vksk_ConfigGetBool(VKSK_Config config, const char *header, const char *key, bool def);

// Sets a string in the config file
void vksk_ConfigSetString(VKSK_Config config, const char *header, const char *key, const char *string);

// Sets a double in the config file
void vksk_ConfigSetDouble(VKSK_Config config, const char *header, const char *key, double real);

// Sets a bool in the config file
void vksk_ConfigSetBool(VKSK_Config config, const char *header, const char *key, bool boolean);

// Returns true if a header exists, false otherwise
bool vksk_ConfigHeaderExists(VKSK_Config config, const char *header);

// Returns true if a header/key value exists, false otherwise
bool vksk_ConfigKeyExists(VKSK_Config config, const char *header, const char *key);

// Frees a config from memory - does not save
void vksk_ConfigFree(VKSK_Config config);

#ifdef __cplusplus
}
#endif