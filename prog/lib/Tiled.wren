// Tiled.wren
// Author: Paolo Mazzon
// Internal Tiled JSON parser

// For internal use
foreign class TiledMap {
    static NO_MORE_LAYERS { 0 }
    static TILE_LAYER { 1 }
    static OBJECT_LAYER { 2 }

    // Opens a new tiled map from a .tmj
    construct open(filename) {}

    // Returns how many tiles wide this map is
    foreign width

    // How many tiles tall the map is
    foreign height

    // Cell width of each tile
    foreign cell_width

    // Cell height of each tile
    foreign cell_height

    // Moves the layer pointer up, returning what kind of layer this is
    foreign next_layer()

    // Returns a map containing the details of every object in the layer
    foreign get_objects()

    // Returns a list of every tile on the layer
    foreign get_tiles()

    // Returns a list of maps where each map is {"gid": <Num>, "filename": <String>}
    foreign get_tilesets()
}