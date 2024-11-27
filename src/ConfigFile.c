/// \file ConfigFile.c
/// \author Paolo Mazzon
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "src/ConfigFile.h"
#include "src/Validation.h"

// I don't feel like writing another one lmfao
bool _vk2dFileExists(const char *filename);

// Returns the desired key, creating it if it doesn't
static _VKSK_ConfigKey *_vksk_ConfigGetKey(VKSK_Config config, const char *header, const char *key) {
	_VKSK_ConfigHeader *foundHeader = NULL;
	_VKSK_ConfigKey *foundKey = NULL;

	// Find header
	for (int i = 0; i < config->size && foundHeader == NULL; i++)
		if (strcmp(header, config->headers[i].header) == 0)
			foundHeader = &config->headers[i];

	// It doesn't exist, make it
	if (foundHeader == NULL) {
		config->headers = realloc(config->headers, sizeof(struct _VKSK_ConfigHeader) * (config->size + 1));
		foundHeader = &config->headers[config->size];
		foundHeader->size = 0;
		foundHeader->keys = NULL;
		foundHeader->header = malloc(strlen(header) + 1);
		strcpy((void*)foundHeader->header, header);
		config->size += 1;
	}

	// Find the key if it exists
	for (int i = 0; i < foundHeader->size && foundKey == NULL; i++)
		if (strcmp(key, foundHeader->keys[i].key) == 0)
			foundKey = &foundHeader->keys[i];

	// Didn't find it, make a new one
	if (foundKey == NULL) {
		foundHeader->keys = realloc(foundHeader->keys, sizeof(struct _VKSK_ConfigKey) * (foundHeader->size + 1));
		foundKey = &foundHeader->keys[foundHeader->size];
		foundHeader->size += 1;
		foundKey->key = malloc(strlen(key) + 1);
		strcpy((void*)foundKey->key, key);
		foundKey->val = NULL;
	}

	return foundKey;
}

static const char *cleanINIString(const char *ini) {
	char *new = malloc(strlen(ini) + 1);
	int newP = 0;

	// Loop through and remove garbage characters
	for (int i = 0; i < strlen(ini); i++) {
		if (ini[i] != '\r') {
			new[newP] = ini[i];
			newP++;
		}
	}

	new[newP] = 0;
	return new;
}

VKSK_Config vksk_ConfigLoadFromString(const char *string) {
	VKSK_Config conf = malloc(sizeof(struct VKSK_Config_t));
	conf->size = 0;
	conf->headers = NULL;

	if (string != NULL) {
		const char *file = cleanINIString(string);
		char header[100] = "none";
		char key[100] = "";
		const char *line = file;
		bool first = true;

		// Loop through each line of the file
		while (line != NULL) {
			if (first)
				first = false;
			else
				line += 1;
			const char *nextLine = strchr(line, '\n');
			int len;
			if (nextLine == NULL)
				len = strlen(line);
			else
				len = nextLine - line;

			// Determine if its a key, header, empty line, or comment
			if (len > 2 || line[0] != ';') {
				const char *equal = memchr(line, '=', len);
				if (line[0] == '[' && line[len - 1] == ']' && len < 102) {
					strncpy(header, line + 1, len - 2);
					header[len - 2] = 0;
				} else if (equal != NULL && equal - line < 100) {
					strncpy(key, line, equal - line);
					key[equal - line] = 0;
					_VKSK_ConfigKey *newkey = _vksk_ConfigGetKey(conf, header, key);
					newkey->val = malloc((line + len) - equal);
					strncpy((void *) newkey->val, equal + 1, (line + len) - equal - 1);
					((char *) newkey->val)[(line + len) - equal - 1] = 0;
				}
			}

			// Find the next line
			line = nextLine;
		}


		free((void *) file);
	}

	return conf;
}

const char* loadFile(const char *filename); // from VMConfig.c

VKSK_Config vksk_ConfigLoad(const char *filename) {
	VKSK_Config conf = NULL;
	if (_vk2dFileExists(filename)) {
		char *file = (void*)loadFile(filename);
		conf = vksk_ConfigLoadFromString(file);
		free(file);
	} else {
		conf = malloc(sizeof(struct VKSK_Config_t));
		conf->size = 0;
		conf->headers = NULL;
	}

	return conf;
}

void vksk_ConfigFlush(VKSK_Config config, const char *filename) {
	FILE *file = fopen(filename, "w");

	for (int i = 0; i < config->size; i++) {
		fprintf(file, "[%s]\n", config->headers[i].header);
		for (int j = 0; j < config->headers[i].size; j++) {
			fprintf(file, "%s=%s\n", config->headers[i].keys[j].key, config->headers[i].keys[j].val);
		}
		fprintf(file, "\n");
	}

	fclose(file);
}

const char *vksk_ConfigGetString(VKSK_Config config, const char *header, const char *key, const char *def) {
	const char *string = def;
	bool out = false;
	for (int i = 0; i < config->size && !out; i++) {
		if (strcmp(config->headers[i].header, header) == 0) {
			for (int j = 0; j < config->headers[i].size && !out; j++)
				if (strcmp(config->headers[i].keys[j].key, key) == 0) {
					string = config->headers[i].keys[j].val;
					out = true;
				}
		}
	}
	return string;
}

double vksk_ConfigGetDouble(VKSK_Config config, const char *header, const char *key, double def) {
	double real = def;
	bool out = false;
	for (int i = 0; i < config->size && !out; i++) {
		if (strcmp(config->headers[i].header, header) == 0) {
			for (int j = 0; j < config->headers[i].size && !out; j++)
				if (strcmp(config->headers[i].keys[j].key, key) == 0) {
					real = strtod(config->headers[i].keys[j].val, NULL);
					out = true;
				}
		}
	}
	return real;
}

bool strToBool(const char *string) {
	if (strcmp(string, "yes") == 0 || strcmp(string, "true") == 0 || strcmp(string, "1") == 0 ||
			strcmp(string, "Yes") == 0 || strcmp(string, "True") == 0 || strcmp(string, "YES") == 0
																		 || strcmp(string, "TRUE") == 0)
		return true;
	return false;
}

bool vksk_ConfigGetBool(VKSK_Config config, const char *header, const char *key, bool def) {
	bool boolean = def;
	bool out = false;
	for (int i = 0; i < config->size && !out; i++) {
		if (strcmp(config->headers[i].header, header) == 0) {
			for (int j = 0; j < config->headers[i].size && !out; j++)
				if (strcmp(config->headers[i].keys[j].key, key) == 0) {
					boolean = strToBool(config->headers[i].keys[j].val);
					out = true;
				}
		}
	}
	return boolean;
}

void vksk_ConfigSetString(VKSK_Config config, const char *header, const char *key, const char *string) {
	_VKSK_ConfigKey *internalKey = _vksk_ConfigGetKey(config, header, key);
	free((void*)internalKey->val);
	internalKey->val = malloc(strlen(string) + 1);
	strcpy((void*)internalKey->val, string);
}

void vksk_ConfigSetDouble(VKSK_Config config, const char *header, const char *key, double real) {
	char string[100];
	snprintf(string, 99, "%f", real);
	_VKSK_ConfigKey *internalKey = _vksk_ConfigGetKey(config, header, key);
	free((void*)internalKey->val);
	internalKey->val = malloc(strlen(string) + 1);
	strcpy((void*)internalKey->val, string);
}

void vksk_ConfigSetBool(VKSK_Config config, const char *header, const char *key, bool boolean) {
	const char *string;
	if (boolean)
		string = "Yes";
	else
		string = "No";
	_VKSK_ConfigKey *internalKey = _vksk_ConfigGetKey(config, header, key);
	free((void*)internalKey->val);
	internalKey->val = malloc(strlen(string) + 1);
	strcpy((void*)internalKey->val, string);
}

bool vksk_ConfigHeaderExists(VKSK_Config config, const char *header) {
	for (int i = 0; i < config->size; i++) {
		if (strcmp(config->headers[i].header, header) == 0)
			return true;
	}
	return false;
}

bool vksk_ConfigKeyExists(VKSK_Config config, const char *header, const char *key) {
	for (int i = 0; i < config->size; i++) {
		if (strcmp(config->headers[i].header, header) == 0) {
			for (int j = 0; j < config->headers[i].size; j++)
				if (strcmp(config->headers[i].keys[j].key, key) == 0)
					return true;
		}
	}
	return false;
}

void vksk_ConfigFree(VKSK_Config config) {
	if (config != NULL) {
		for (int i = 0; i < config->size; i++) {
			for (int j = 0; j < config->headers[i].size; j++) {
				free((void*)config->headers[i].keys[j].key);
				free((void*)config->headers[i].keys[j].val);
			}
			free((void*)config->headers[i].header);
			free(config->headers[i].keys);
		}
		free(config->headers);
		free(config);
	}
}