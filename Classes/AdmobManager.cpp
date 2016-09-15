#include "AdmobManager.h"
#include "cocos2d.h"
#include "platform\android\jni\JniHelper.h"
#define METHOD_NAME "showToast"
#define PARAM_CODE "(Ljava/lang/String;)V"
bool AdmobManager::isAdShowing = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
//#include <android/log.h>
const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";
void AdmobManager::hideAd()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "hideAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = false;
	}
}

void AdmobManager::showAd()
{

	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = true;
	}

}
void AdmobManager::showStartappAd()
{
	cocos2d::JniMethodInfo method;
	if (cocos2d::JniHelper::getStaticMethodInfo(method, AppActivityClassName, "showStartappAd", "()V"))
	{
		method.env->CallStaticVoidMethod(method.classID, method.methodID);
		method.env->DeleteLocalRef(method.classID);
		isAdShowing = true;
	}
}
void AdmobManager::openURL(const char* url)
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "openURL", PARAM_CODE))
	{
		jstring stringArg = t.env->NewStringUTF(url);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg);
		t.env->DeleteLocalRef(stringArg);
	}
}
void AdmobManager::showToast(const char* message)
{
	cocos2d::JniMethodInfo method;
	cocos2d::JniHelper::getStaticMethodInfo(method, AppActivityClassName, "showToast", PARAM_CODE);
	jstring stringMessage = method.env->NewStringUTF(message);
	method.env->CallStaticVoidMethod(method.classID, method.methodID,
		stringMessage);
	method.env->DeleteLocalRef(stringMessage);
}

#else


void AdmobManager::hideAd()
{
	isAdShowing = false;
	return; //nothing
}


void AdmobManager::showAd()
{
	isAdShowing = true;
	return; //nothing;
}

#endif
