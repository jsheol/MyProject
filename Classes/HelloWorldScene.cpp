#include "HelloWorldScene.h"
#include "stdio.h"
#include "stdlib.h"
#include "curl/curl.h"
#include "VisibleRect.h"

USING_NS_CC;

std::vector<char>   _responseData;

// Callback function used by libcurl for collect response data
static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld()
{
	_label->release();
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Curl Test", "fonts/arial.ttf", 28);
    addChild(label, 0);
    label->setPosition( Vec2(VisibleRect::center().x, VisibleRect::top().y-50) );

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // create a label to display the tip string
    _label = Label::createWithTTF("Touch the screen to connect", "fonts/arial.ttf", 22);
    _label->setPosition(VisibleRect::center());
    addChild(_label, 0);
    
    _label->retain();
    
    return true;
}

// the test code is
// http://curl.haxx.se/mail/lib-2009-12/0071.html
void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
    CURL *curl;
    CURLcode res;
    char buffer[10];

    curl = curl_easy_init();
    if (curl) 
    {
//        curl_easy_setopt(curl, CURLOPT_URL, "http://webtest.cocos2d-x.org/curltest");
//        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.127.90/test.php");
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:1337");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_responseData);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

		res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        if (res == 0)
        {
//            _label->setString("Connect successfully!");
			_label->setString(std::string(_responseData.begin(), _responseData.end()));
        }
        else
        {
            sprintf(buffer,"code: %i",res);
            _label->setString(buffer);
        }
    } 
    else 
    {
        _label->setString("no curl");
    }



}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
