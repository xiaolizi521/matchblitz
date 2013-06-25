#include "MainScene.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace CocosDenshion;

/**
 * Constructor
 */
CCScene* MatchThree::scene()
{
    //srand(time(0));
    CCScene *scene = CCScene::create();
    MatchThree *layer = MatchThree::create();
    // layer->retain();
    CCAssert(layer, "No layer created");
    scene->addChild(layer);                     // Adding layer to the main scene
    return scene;
}

/**
 * Initialize
 */
bool MatchThree::init()
{
    // initialize parent
    if ( !CCLayerColor::initWithColor(ccc4(140,216,250,255)) ) //sky blue
    {
        return false;
    }

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    //CCSize size2 = CCDirector::sharedDirector()->getWinSizeInPixels();
    
    // Adding background sprite
    CCSprite *bg = CCSprite::create(bg_filename.c_str());
    bg->setPosition(ccp(size.width/2, size.height/2));
    bg->setScaleX(size.width/bg->getContentSize().width);
    bg->setScaleY(size.height/bg->getContentSize().height);
    this->addChild(bg, 0);

    // Creating the Box (n x n) grid
    _box = Box::create();
    if (!_box) {
        return false;
    }
    _box->retain();
    _box->layer = this;
    _box->initWithSize(CCSizeMake(kBoxWidth,kBoxHeight), 6);    // Initialize the grid with tiles
        _box->lock = true;
    //this->setColor(ccc3(200,200,255));
    this->setTouchEnabled(true);
    
    return true;
}

/**
 * When all screen transitions are done, 
 * this function is called which in turn
 * calls Box::check to verify the current 
 * status of the grid/tiles. 
 */
void MatchThree::onEnterTransitionDidFinish ()
{
    _box->check();
}

/**
 * Function to handle touch events
 */
void MatchThree::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    if (_box->lock) {
       return;
    }
    
    CCTouch* touch = (CCTouch *) touches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    this->_debugChangeToHStriped = false;
    this->_debugChangeToVStriped = false;
    this->_debugCycleColor = false;
    
    int x = (location.x -kStartX) / kTileSize;
    int y = (location.y -kStartY) / kTileSize;
    
    if (location.x - kStartX < 0){
        this->_debugChangeToHStriped = true;
    } else if (location.y - kStartY < 0) {
         this->_debugChangeToVStriped = true;
    } else if (x>=kBoxWidth) {
        this->_debugCycleColor = true;
    }
    
    if (x < 0 || y < 0 || x >= kBoxWidth || y >= kBoxHeight) {
        return;
    }
    // Getting the tile at location where touch was made
    Tile2 *tile = _box->objectAtX(x, y);
    _selectedTile = tile;    
}

/**
 * Function to handle touch events
 */
void MatchThree::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    if (_box->lock || !_selectedTile) {
        return;
    }
    
    CCTouch* touch = (CCTouch *) touches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    int x = (location.x -kStartX) / kTileSize;
    int y = (location.y -kStartY) / kTileSize;
    if (x < 0 || y < 0 || x >= kBoxWidth || y >= kBoxHeight) {
        return;
    }
    
    if (_selectedTile && _selectedTile->x == x && _selectedTile->y == y) {
        return;
    }
    
    // Getting the tile at location where touch was made
    Tile2 *tile = _box->objectAtX(x, y);
    
    if (_selectedTile->nearTile(tile)) {
        // If its the second touch, do the swap!
        _box->lock = true;
        this->changeWithTileA(_selectedTile, tile, callfuncND_selector(MatchThree::check));
        _selectedTile = NULL;
    } 
}

void MatchThree::ccTouchesEnded(CCSet* touches, CCEvent* event){
    // Debug mode:: set the tile to striped;
    if(this->_debugChangeToHStriped || this->_debugChangeToVStriped || this->_debugCycleColor) {
        CCTouch* touch = (CCTouch *) touches->anyObject();
        CCPoint location = touch->getLocationInView();
        location = CCDirector::sharedDirector()->convertToGL(location);
        
        int x = (location.x -kStartX) / kTileSize;
        int y = (location.y -kStartY) / kTileSize;
        if (x < 0 || y < 0 || x >= kBoxWidth || y >= kBoxHeight) {
            return;
        }
        Tile2 *tile = _box->objectAtX(x, y);
        if (this->_debugChangeToHStriped || this->_debugChangeToVStriped) {
            tile->type =  (this->_debugChangeToHStriped)?StripedHorizontal:StripedVertical;
        }
        if (this->_debugCycleColor) {
            // cycle through the color
            int value = tile->value -1;
            value = (value +1) % kKindCount;
            tile->value = value + 1;
        }
        tile->sprite->setVisible(false);
        tile->sprite = Tile2::getBalloonSprite(tile->value, tile->type);
        tile->sprite->setPosition(tile->pixPosition());
        _box->layer->addChild(tile->sprite);
    }
    _selectedTile = NULL;
}


/**
 * Change tile A with tile B
 */
void MatchThree::changeWithTileA(Tile2 * a, Tile2 * b, SEL_CallFuncND sel)
{
    CCFiniteTimeAction *actionA = CCSequence::create(
                            CCMoveTo::create(kMoveTileTime, b->pixPosition()),
                            CCCallFuncND::create(this, sel, a),
                            NULL);
    CCFiniteTimeAction *actionB = CCSequence::create(
                            CCMoveTo::create(kMoveTileTime, a->pixPosition()),
                            CCCallFuncND::create(this, sel, b),
                            NULL);
                                                         
    a->sprite->runAction(actionA);
    b->sprite->runAction(actionB);
    a->trade(b);
}
                                            
void MatchThree::backCheck(CCNode * sender, Tile2 * data)
{
    if(NULL == _firstOne){
        _firstOne = data;
        return;
    }
    _firstOne = NULL;
    _box->lock = false;
}

void MatchThree::check(CCNode * sender, Tile2 * data)
{
    if(NULL == _firstOne){
        _firstOne = data;
        return;
    }
    bool result = _box->check();
    if (result)
    {
        _box->lock = false;
    }
    else
    {
        this->changeWithTileA(data, _firstOne, callfuncND_selector(MatchThree::backCheck));
        this->runAction(CCSequence::create(
                            CCDelayTime::create(kMoveTileTime + 0.03f),
                            CCCallFuncN::create(_box, callfuncN_selector(Box::unlock)), NULL));
    }
    
    _firstOne = NULL;
}


void MatchThree::afterTurn(CCSprite * node)
{
   if (_selectedTile && node == _selectedTile->sprite) {
        CCSprite *sprite = (CCSprite *)node;
        CCFiniteTimeAction *someAction = CCSequence::create(
                            CCScaleBy::create(kMoveTileTime, 0.5f),
                            CCScaleBy::create(kMoveTileTime, 2.0f),
                            CCCallFuncN::create(this, callfuncN_selector(MatchThree::afterTurn)), NULL);
        
        sprite->runAction(someAction);
    }
}


void MatchThree::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
