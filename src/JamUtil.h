/// \file JamUtil.h
/// \author Paolo Mazzon
/// \brief A small collection of tools for quick game-dev with Vulkan2D
#pragma once
#include <stdbool.h>
#include <VK2D/VK2D.h>
#include "src/cute_sound.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************** Typedefs **********************/
typedef struct JUCharacter_t JUCharacter;
typedef struct JUFont_t* JUFont;
typedef struct JUSound_t* JUSound;
typedef struct JUSave_t* JUSave;
typedef struct JUBuffer_t* JUBuffer;
typedef struct JUSprite_t* JUSprite;
typedef struct JUPlayingSound_t JUPlayingSound;
typedef struct JURectangle_t JURectangle;
typedef struct JUPoint2D_t JUPoint2D;
typedef struct JUCircle_t JUCircle;
typedef struct JUData_t JUData;
typedef struct JUClock_t JUClock;

/********************** Enums **********************/

/// \brief Types of assets stored in the loader
typedef enum {
	JU_ASSET_TYPE_NONE = 0,
	JU_ASSET_TYPE_FONT = 1,
	JU_ASSET_TYPE_TEXTURE = 2,
	JU_ASSET_TYPE_SOUND = 3,
	JU_ASSET_TYPE_BUFFER = 4,
	JU_ASSET_TYPE_SPRITE = 5,
	JU_ASSET_TYPE_MAX = 6,
} JUAssetType;

/// \brief Types of data the save can handle
typedef enum {
	JU_DATA_TYPE_NONE = 0,
	JU_DATA_TYPE_FLOAT = 1,
	JU_DATA_TYPE_DOUBLE = 2,
	JU_DATA_TYPE_INT64 = 3,
	JU_DATA_TYPE_UINT64 = 4,
	JU_DATA_TYPE_STRING = 5,
	JU_DATA_TYPE_VOID = 6,
	JU_DATA_TYPE_MAX = 7,
} JUDataType;

/********************** Constants **********************/

///< Job channel for systems
extern const int JU_JOB_CHANNEL_SYSTEMS;

///< Job channel for component copy
extern const int JU_JOB_CHANNEL_COPY;

///< Value representing a disabled lock, user doesn't need this
extern const int32_t JU_DISABLED_LOCK;

/********************** Top-Level **********************/

/// \brief Initializes everything, make sure to call this before anything else
/// \param window Window that is used
/// \param jobChannels Number of channels for jobs (8 is a good number, 0 if you're not using jobs)
/// \param minimumThreads Minimum number of threads for job system, if 0 1 thread per cpu core - 1 will be made.
/// If there are more cores available than minimum threads specified a thread will be made per core. On certain
/// lower-end CPUs there may only be 2 cores which would make 1 job thread, but certain jobs may depend on other
/// jobs meaning multiple job threads are required.
/// \warning ECS requires jobs to be enabled and at least 2 channels, 0 and 1 are reserved for ECS
void juInit(SDL_Window* window, int jobChannels, int minimumThreads);

/// \brief Keeps various systems up to date, call every frame at the start before the SDL event loop
void juUpdate();

/// \brief Frees all resources, call at the end of the program
void juQuit();

/// \brief Returns the time in seconds that the last frame took
double juDelta();

/// \brief Returns the time in seconds since juInit was called
double juTime();

/********************** Clock **********************/

/// \brief Data needed to calculate timing things
struct JUClock_t {
	double totalTime;       ///< For calculating averages
	double totalIterations; ///< For calculating averages
	uint64_t lastTime;      ///< The last time the clock was called
};

/// \brief Sets up a clock (also calls `juClockStart`)
void juClockReset(JUClock* clock);

/// \brief Starts a clock timing period
void juClockStart(JUClock* clock);

/// \brief Gets the time in seconds since the last time the clock was started (does not restart the clock or add to the average)
double juClockTime(JUClock* clock);

/// \brief Gets the time in seconds since the last clock start, also restarts the clock and adds to the average
double juClockTick(JUClock* clock);

/// \brief Performs the same duties as `juClockTick` but also enforces a frame rate by waiting until that frame time is met
void juClockFramerate(JUClock* clock, double framerate);

/// \brief Gets the average clock time in seconds
double juClockGetAverage(JUClock* clock);

/********************** Font **********************/

/// \brief Data as it relates to storing a bitmap character for VK2D
struct JUCharacter_t {
	float x;     ///< x position of this character in the bitmap
	float y;     ///< y position of this character in the bitmap
	float w;     ///< width of the character in the bitmap
	float h;     ///< height of the character in the bitmap
	float ykern; ///< Vertical displacement of the character
	bool drawn;  ///< For invisible characters that have width but need not be drawn (ie space)
};

/// \brief A bitmap font, essentially a sprite sheet and some characters
struct JUFont_t {
	uint32_t unicodeStart;   ///< Code point of the first character in the image (inclusive)
	uint32_t unicodeEnd;     ///< Code point of the last character in the image (exclusive)
	float newLineHeight;     ///< Height of a newline (calculated as the max character height)
	JUCharacter* characters; ///< Vector of characters
	VK2DTexture bitmap;      ///< Bitmap of the characters
	VK2DImage image;         ///< Bitmap image in case it was loaded from a jufnt
};

/// \brief Returns the size of a string
void juFontUTF8Size(JUFont font, float* w, float* h, float width, const char* fmt, ...);

/// \brief Returns the size of a string ignoring tokens
void juFontUTF8SizeExt(JUFont font, float* w, float* h, float width, const char* string);

/// \brief Loads a font from a .jufnt file (create them with the python script)
/// \return Returns a new font or NULL if it failed
JUFont juFontLoad(const char* filename);

/// \brief Loads a font from a
/// \param image Filename of the image to use for the font
/// \param unicodeStart First character in the image (inclusive)
/// \param unicodeEnd Last character in the range (exclusive)
/// \param w Width of each character in the image
/// \param h Height of each character in the image
/// \return Returns a new font or NULL if it failed
///
/// This can only load mono-spaced fonts and it expects the font to have at least
/// an amount of characters in the image equal to unicodeEnd - unicodeStart.
JUFont juFontLoadFromTexture(VK2DTexture texture, uint32_t unicodeStart, uint32_t unicodeEnd, float w, float h);

/// \brief Frees a font
void juFontFree(JUFont font);

/// \brief Draws a font to the screen (supports all printf % things)
///
/// Since this uses Vulkan2D to draw the current colour of the VK2D
/// renderer is used.
///
/// vsprintf is used internally, so any and all printf % operators work
/// in this. Newlines (\n) are also allowed.
void juFontDraw(JUFont font, float x, float y, const char* fmt, ...);

/// \brief Draws a font to the screen, wrapping every w pixels (supports all printf % things)
///
/// Since this uses Vulkan2D to draw the current colour of the VK2D
/// renderer is used.
///
/// vsprintf is used internally, so any and all printf % operators work
/// in this. Newlines (\n) are also allowed.
void juFontDrawWrapped(JUFont font, float x, float y, float w, const char* fmt, ...);

/// \brief Same as above but parses string tokens like [-15, 24]
void juFontDrawExt(JUFont font, float x, float y, const char* string);

/// \brief Same as above but parses string tokens like [-15, 24]
void juFontDrawWrappedExt(JUFont font, float x, float y, float w, const char* string);

/********************** Buffer **********************/

/// \brief Simple buffer to make loading binary easier
struct JUBuffer_t {
	void* data;    ///< Data stored in this buffer
	uint32_t size; ///< Size of the data stored in the buffer
};

/// \brief Loads a buffer from a file
JUBuffer juBufferLoad(const char* filename);

/// \brief Creates a buffer from given data, the data will be copied to the buffer
JUBuffer juBufferCreate(void* data, uint32_t size);

/// \brief Saves a buffer to a file
void juBufferSave(JUBuffer buffer, const char* filename);

/// \brief Frees a buffer from memory
void juBufferFree(JUBuffer buffer);

/// \brief Saves some data to a file without the need for a buffer
void juBufferSaveRaw(void* data, uint32_t size, const char* filename);

/********************** Audio **********************/

/// \brief A sound to be soundInfo
struct JUSound_t {
	cs_loaded_sound_t sound;
	cs_play_sound_def_t soundInfo;
};

/// \brief A currently playing sound
struct JUPlayingSound_t {
	cs_playing_sound_t* playingSound;
};

/// \brief Loads a sound from a file into memory - right now only WAV files are supported
JUSound juSoundLoad(const char* filename);

/// \brief Plays a sound
/// \return Returns a playing sound handle you can use to update/stop the sound, but it doesn't need
/// to be stored (it won't cause a memory leak)
JUPlayingSound juSoundPlay(JUSound sound, bool loop, float volumeLeft, float volumeRight);

/// \brief Change the properties of a currently playing sound
void juSoundUpdate(JUPlayingSound sound, bool loop, float volumeLeft, float volumeRight);

/// \brief Stops a sound if its currently playing
void juSoundStop(JUPlayingSound sound);

/// \brief Frees a sound from memory
void juSoundFree(JUSound sound);

/// \brief Stops all currently playing sounds
void juSoundStopAll();

/********************** File I/O **********************/

/// \brief A piece of data stored in a save
struct JUData_t {
	JUDataType type; ///< Type of this data
	const char* key; ///< Key of this data

	union {
		int64_t i64;        ///< 64 bit signed int
		uint64_t u64;       ///< 64 bit unsigned int
		float f32;          ///< 32 bit float
		double f64;         ///< 64 bit float
		const char* string; ///< String
		struct {
			void* data;    ///< Raw data
			uint32_t size; ///< Size in bytes of the data
		} data;
	} Data;
};

/// \brief Save data for easily saving and loading many different types of data
struct JUSave_t {
	uint32_t size; ///< Number of "datas" stored in this save
	JUData* data;  ///< Vector of data
};

/// \brief Loads a save from a save file or returns an empty save if the file wasn't found
///
/// These are basically just tables of data, you use a key to set some data then use a key
/// to later find that data again. The real functionality comes in the form of saving and
/// loading from files with it, since you can save all your data in a human-readable form
/// and load it right into your game easily.
///
/// These aren't particularly fast, and they are not meant to be used every frame non-stop,
/// especially in larger games.
JUSave juSaveLoad(const char* filename);

/// \brief Saves a save to a file
void juSaveStore(JUSave save, const char* filename);

/// \brief Frees a save from memory
void juSaveFree(JUSave save);

/// \brief Returns true if the key exists in the save file
bool juSaveKeyExists(JUSave save, const char* key);

/// \brief Sets some data in a save
void juSaveSetInt64(JUSave save, const char* key, int64_t data);

/// \brief Gets some data from a save
int64_t juSaveGetInt64(JUSave save, const char* key);

/// \brief Sets some data in a save
void juSaveSetUInt64(JUSave save, const char* key, uint64_t data);

/// \brief Gets some data from a save
uint64_t juSaveGetUInt64(JUSave save, const char* key);

/// \brief Sets some data in a save
void juSaveSetFloat(JUSave save, const char* key, float data);

/// \brief Gets some data from a save
float juSaveGetFloat(JUSave save, const char* key);

/// \brief Sets some data in a save
void juSaveSetDouble(JUSave save, const char* key, double data);

/// \brief Gets some data from a save
double juSaveGetDouble(JUSave save, const char* key);

/// \brief Sets some data in a save
void juSaveSetString(JUSave save, const char* key, const char* data);

/// \brief Gets some data from a save
/// \warning The pointer/data belongs to the save itself and will be freed with it - copy it if you need it longer
const char* juSaveGetString(JUSave save, const char* key);

/// \brief Sets some data in a save
///
/// The save will make a local copy of the data
void juSaveSetData(JUSave save, const char* key, void* data, uint32_t size);

/// \brief Gets some data from a save
/// \warning The pointer/data belongs to the save itself and will be freed with it - copy it if you need it longer
void* juSaveGetData(JUSave save, const char* key, uint32_t* size);


/********************** Collisions/Math **********************/

/// \brief A simple rectangle
struct JURectangle_t {
	double x; ///< x position of the top left of the rectangle
	double y; ///< y position of the top left of the rectangle
	double w; ///< Width of the rectangle
	double h; ///< Height of the rectangle
};

/// \brief A simple circle
struct JUCircle_t {
	double x; ///< x position of the center of the circle
	double y; ///< y position of the center of the circle
	double r; ///< Radius in pixels
};

/// \brief A 2D coordinate
struct JUPoint2D_t {
	double x; ///< x position in 2D space
	double y; ///< y position in 2D space
};

/// \brief Gets the angle between two points
double juPointAngle(double x1, double y1, double x2, double y2);

/// \brief Gets the distance between two points
double juPointDistance(double x1, double y1, double x2, double y2);

/// \brief Rotates a point in 2D space about an (absolute) origin
JUPoint2D juRotatePoint(double x, double y, double originX, double originY, double rotation);

/// \brief Checks for a collision between two rectangles
bool juRectangleCollision(JURectangle* r1, JURectangle* r2);

/// \brief Checks for a collision between two rotated rectangles
bool juRotatedRectangleCollision(JURectangle* r1, double rot1, double originX1, double originY1, JURectangle* r2, double rot2, double originX2, double originY2);

/// \brief Checks for a collision between two circles
bool juCircleCollision(JUCircle* c1, JUCircle* c2);

/// \brief Checks if a point exists within a given rectangle
bool juPointInRectangle(JURectangle* rect, double x, double y);

/// \brief Checks if a point exists within a given rotated rectangle
bool juPointInRotatedRectangle(JURectangle* rect, double rot, double originX, double originY, double x, double y);

/// \brief Checks if a point exists within a given circle
bool juPointInCircle(JUCircle* circle, double x, double y);

/// \brief Linear interpolation (given a start, stop, and percent it returns the point x% along that distance)
double juLerp(double percent, double start, double stop);

/// \brief Same as lerp but on a sin graph instead of a linear graph (for smooth transitions)
double juSerp(double percent, double start, double stop);

/// \brief Casts a ray out at a given angle and returns the x component
double juCastX(double length, double angle);

/// \brief Casts a ray out at a given angle and returns the y component
double juCastY(double length, double angle);

/// \brief Returns the sign of a number (1 for positive, -1 for negative, 0 for 0)
double juSign(double x);

/// \brief Subtracts y from x towards 0 and returns it, so if x is negative y will be added to it, if it would flip signs 0 is returned (y should be positive)
double juSubToZero(double x, double y);

/// \brief If x is between min and max, x is returned, if x is above max, max is returned and vice versa
double juClamp(double x, double min, double max);

/********************** Keyboard **********************/

/// \brief Checks if a key is currently pressed
bool juKeyboardGetKey(SDL_Scancode key);

/// \brief Checks if a key was just pressed
bool juKeyboardGetKeyPressed(SDL_Scancode key);

/// \brief Checks if a key is currently pressed
bool juKeyboardGetKeyReleased(SDL_Scancode key);

/********************** Animations **********************/

/// \brief Information for sprites
///
/// No "cells" are stored because the image coordinates that need to be
/// drawn are calculated on the fly.
struct JUSprite_t {
	struct {
		uint64_t lastTime; ///< Last time the animation was updated
		uint32_t frames;   ///< Number of frames in the animation
		uint32_t frame;    ///< Current frame in the animation
		float w;           ///< Width of each cell
		float h;           ///< Height of each cell
		VK2DTexture tex;   ///< Sprite sheet
		bool copy;         ///< If this is a copy of a sprite or not (for the purposes of only freeing the texture once)
	} Internal;     ///< Data for the sprite to keep track of itself
	double delay;   ///< Time in seconds a single frame lasts
	float x;        ///< X position in the texture where the sprite sheet starts
	float y;        ///< Y position in the texture where the sprite sheet starts
	float originX;  ///< X origin of the sprite (used for drawing position and rotation)
	float originY;  ///< Y origin of the sprite (used for drawing position and rotation)
	float scaleX;   ///< X scale of the sprite
	float scaleY;   ///< Y scale of the sprite
	float rotation; ///< Rotation of the sprite
};

/// \brief Loads an animation from a sprite sheet file
/// \param filename Image file of the spritesheet
/// \param x X in the sprite sheet where the cells begin
/// \param y Y in the sprite sheet where the cells begin
/// \param w Width of each cell
/// \param h Height of each cell
/// \param delay Delay in seconds between animation frames
/// \param frames Animation frame count (a value of zero will be interpreted as 1)
/// \return Returns a new sprite or NULL if it failed
JUSprite juSpriteCreate(const char* filename, float x, float y, float w, float h, float delay, int frames);

/// \brief The same as `juSpriteCreate` except it creates a sprite from an already existing texture (which is not freed with the sprite)
/// \return Returns a new sprite that uses `tex` but does not technically own `tex`
JUSprite juSpriteFrom(VK2DTexture tex, float x, float y, float w, float h, float delay, int frames);

/// \brief Makes a copy of a sprite (usually only use this with a loader)
/// \param original Sprite to copy from
/// \warning The sprite copy does not own the texture pointer and if the original is freed the copy can still
/// be safely freed but it can no longer be used.
JUSprite juSpriteCopy(JUSprite original);

/// \brief Draws an animation, advancing the current frame if enough time has passed
void juSpriteDraw(JUSprite spr, float x, float y);

/// \brief Draws a specific frame of a frame, not doing any sprite updating or anything
void juSpriteDrawFrame(JUSprite spr, uint32_t index, float x, float y);

/// \brief Frees an animation from memory
void juSpriteFree(JUSprite spr);

#ifdef __cplusplus
}
#endif