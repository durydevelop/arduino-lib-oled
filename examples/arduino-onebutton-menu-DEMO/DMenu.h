/*
 *  Class:      DMenu
 *  Created on: 2018
 *  Author:     dury
 *  Version:    1.0
 */

#ifndef DMENU_H
#define DMENU_H

//#include "mbed.h"

#if defined(ARDUINO)
    #if ARDUINO >= 100
        #include "Arduino.h"
    #else
        #include "wiring.h"
        #include "WProgram.h"
    #endif
#else
	#include <cstring>
	#include <cstdlib>
	using namespace std;
#endif // defined

// DMenu defines
#define DMENU_DEFAULT_MAX_ITEM_TEXT_LEN 21
#define DMENU_MAX_ITEMS 255
#define DMENU_ACTION_UP     1
#define DMENU_ACTION_DOWN   2
#define DMENU_ACTION_BACK   3
#define DMENU_ACTION_SELECT 4

class DMenu {
	public:
		typedef void (*DMenuListener) (DMenu *MenuItem, uint8_t Action);
		DMenu(const char MenuItemName[], uint8_t MenuID=0, DMenu *Parent=NULL, DMenuListener CallbackFunc=NULL);
		~DMenu();
		void Init(const char MenuItemName[], uint8_t MenuID, DMenu *ParentItem, DMenuListener CallbackFunc);
		void SetName(const char ItemName[]);
		bool AddItem(const char Name[], uint8_t ItemID=0, bool SetCurrent=false);
		DMenu* Up(void);
		DMenu* Down(void);
		DMenu* Back(void);
		DMenu* Select(void);

		DMenu* GetItem(short int Index);
		DMenu* GetCurrItem(void);
		DMenu* GetParent(void);
        bool HasParent(void);
		uint8_t GetItemsCount(void);
		const char* GetName(void);
        bool NameEquals(const char MenuName[]);
		uint8_t GetID(void);
		short int GetCurrItemIndex(void);
		bool Loop;

	private:
		DMenuListener Callback;
		char *Name;
		DMenu *Parent;
		short int ItemIndex; //! Index used to handle the current selected Item
		uint8_t ItemsCount;
		uint8_t ID;
		uint8_t MaxItemTextLen;
		DMenu** Items;

};

#endif
