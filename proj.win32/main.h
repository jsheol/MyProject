#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

class HelloWorld : public cocos2d::Layer
{
public:
    //Http Response Callback
    void onHttpRequestCompleted(
		cocos2d::network::HttpClient *sender, 
		cocos2d::network::HttpResponse *response);

	~HelloWorld();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

private:
    cocos2d::Label*         _label;
};

#endif // __HELLOWORLD_SCENE_H__
