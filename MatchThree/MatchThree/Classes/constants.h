//
//  constants.h
//  MatchThree
//
//  Created by kpuspesh on 6/2/13.
//
//

#ifndef MatchThree_constants_h
#define MatchThree_constants_h

#define kTileSize 78.0f
#define kPieceHeight 68.0f
#define kPieceWidth 68.0f
#define kMoveTileTime 0.3f
#define kBoxWidth 9
#define kBoxHeight 9
#define kStartX 280
#define kStartY 25
#define kMaxLevelNo 10
#define kMaxRecordCount 5
#define kKindCount 6
#define kTileFadeInTime 0.3f
#define kTileBGOpacity 180 // between 0 and 255
#define kRepairDelayTime 0.3f

enum Orientation{
	OrientationHori,
	OrientationVert,
};
typedef enum Orientation Orientation;


#endif
