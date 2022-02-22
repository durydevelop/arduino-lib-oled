#include "Arduino.h"
#include "DMenu.h"
#include "DDigitalButton.h"

#include <oled.h>       // **** OLED ****
//#include <U8x8lib.h>  // **** U8g2 ****

// For compatibility with U8g2 font
#define COLS 16
#define ROWS 8

DMenu* CreateMainMenu(void);
DMenu* CreateSubMenu1(void);
DMenu* CreateSubMenu2(void);
void PrintMenu(DMenu *Menu, uint8_t X = 0, uint8_t Y = 0);
void Debug(const String& msg);

OLED display(18,19,NO_RESET_PIN,OLED::W_128,OLED::H_64);            // **** OLED ****
//U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE,19,18);   // **** U8g2 ****

// Button
DDigitalButton *Button=NULL;
// Menu
DMenu *CurrMenu=NULL;

void setup()
{
	// Begin display
	display.begin();
	//display.setFont(u8x8_font_chroma48medium8_r); // **** only for U8g2 ****

	// Begin serial debug
	Serial.begin(9600);
	Debug("Start");

	// Create Button
	Button=new DDigitalButton(6);
	Debug("Button on input pin 6");

	// Create MainMenu
	CurrMenu=CreateMainMenu();
	PrintMenu(CurrMenu);
}

// Poll Button and handle read status
void loop()
{
	DMenu *SelItem;

	switch (Button->Read()) {
		case DDigitalButton::PRESSED:
			Debug("<PRESSED>");
			CurrMenu->Down();
			PrintMenu(CurrMenu);
			break;
		case DDigitalButton::LONG_PRESSED: {
			Debug("<LONG_PRESSED>");
			SelItem=CurrMenu->Select();
			String ItemName=SelItem->GetName();
			Debug("Item "+ItemName);
			if (ItemName == "SubMenu 1") {
				CurrMenu=CreateSubMenu1();
				PrintMenu(CurrMenu);
			}
			else if (ItemName == "SubMenu 2") {
				CurrMenu=CreateSubMenu2();
				PrintMenu(CurrMenu);
			}
			else if (ItemName == "Back") {
				if (String(CurrMenu->GetName()) == "SubMenu 1" || String(CurrMenu->GetName()) == "SubMenu 2") {
					CurrMenu=CreateMainMenu();
				}
				PrintMenu(CurrMenu);
			}
			break;
		}
		default:
			break;
	}
}

DMenu* CreateMainMenu(void)
{
	if (CurrMenu != NULL) {
		// Only one menu must be in memory
		delete CurrMenu;
	}

	CurrMenu=new DMenu("Main Menu");
	CurrMenu->Loop=true;

	CurrMenu->AddItem("SubMenu 1",true);
	CurrMenu->AddItem("SubMenu 2");

	return(CurrMenu);
}

DMenu* CreateSubMenu1(void)
{
	if (CurrMenu != NULL) {
		// Only one menu must be in memory
		delete CurrMenu;
	}

	CurrMenu=new DMenu("SubMenu 1");
	CurrMenu->Loop=true;

	CurrMenu->AddItem("Execute",true);
	CurrMenu->AddItem("Back");

	return(CurrMenu);
}

DMenu* CreateSubMenu2(void)
{
	if (CurrMenu != NULL) {
		// Only one menu must be in memory
		delete CurrMenu;
	}

	CurrMenu=new DMenu("SubMenu 2");
	CurrMenu->Loop=true;

	CurrMenu->AddItem("Execute",true);
	CurrMenu->AddItem("Back");

	return(CurrMenu);
}

void PrintMenu(DMenu* Menu, uint8_t X, uint8_t Y)
{
	display.clear();
	// Line buffer
	char Line[COLS+1];
	memset(Line,' ',COLS+1);

	// Create centered menu title line
	uint8_t nameLen=strlen(Menu->GetName());
	uint8_t startCol=(COLS-nameLen)/2;
	memcpy(&Line[startCol],Menu->GetName(),nameLen);
	Line[COLS]='\0';

	// Print menu title
	display.drawString(X,Y,Line);

	// Starts from second line
	for (uint8_t iY=Y; iY<Menu->GetItemsCount();iY++) {
		DMenu* Item=CurrMenu->GetItem(iY);
		// Starts with item name 
		uint8_t itemLen=strlen(Item->GetName());
		memcpy(Line,Item->GetName(),itemLen);
		// Fill with spaces
		memset(&Line[itemLen],' ',COLS-itemLen);
		// Add NULL
		Line[COLS]='\0';
		if (iY == CurrMenu->GetCurrItemIndex()) {
			// Draw selected
			display.inverse();
			//display.drawString(X,iY+2,">");
			display.drawString(X,iY+2,Line);
			display.noInverse();
		}
		else {
			// Draw not selected
			//display.drawString(X,iY+2," ");
			display.drawString(X,iY+2,Line);
		}
		//display.drawString(X+2,iY+2,Line);
	}
	display.display();  // **** only for OLED ****
}

void Debug(const String& msg)
{
	Serial.println(msg);
}
