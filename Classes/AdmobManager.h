#ifndef  __ADMOB_MANAGER_H_
#define  __ADMOB_MANAGER_H_

class AdmobManager
{
public:
	static void hideAd();
	static void showAd();
	static bool isAdShowing;
	static void openURL(const char* url);
	static void showToast(const char* message);
	static void showStartappAd();
};


#endif // __ADMOB_MANAGER_H_
