#include "DMenu.h"

/**
 * @brief Construct a new DMenu::DMenu object
 * 
 * @param MenuItemName  ->	name of new menu item.
 * @param MenuID        ->  ID of this menu. You can use it to handle it by id instead of by name. Can be a value from 0 to 254.
 * @param ParentItem    ->	pointer to the parent item of the new one. If parameter is NULL the menu item is the root one and Back() does't fire the Callback.
 * @param CallbackFunc  ->	pointer to the DMenuListener function that will be called on Select() or Back() call.
  */
DMenu::DMenu(const char MenuItemName[], uint8_t MenuID, DMenu *ParentItem, DMenuListener CallbackFunc)
{
    Init(MenuItemName,MenuID,ParentItem,CallbackFunc);
}

/**
 * @brief 
 * 
 * @param MenuItemName  ->	name of new menu item.
 * @param MenuID        ->  ID of this menu. You can use it to handle it by id instead of by name. Can be a value from 0 to 254.
 * @param ParentItem    ->	pointer to the parent item of the new one. If parameter is NULL the menu item is the root one and Back() does't fire the Callback.
 * @param CallbackFunc  ->	pointer to the DMenuListener function that will be called on Select() or Back() call.
 */
void DMenu::Init(const char MenuItemName[], uint8_t MenuID, DMenu *ParentItem, DMenuListener CallbackFunc)
{
	Callback=CallbackFunc;

	if (ParentItem == NULL) {
        Parent=this;
	}
	else {
        Parent=ParentItem;
	}
    
	//ID=rand() % DMENU_MAX_ITEMS + 1;
    MenuID=ID;
	ItemIndex=-1;
	ItemsCount=0;
	Loop=false;
	MaxItemTextLen=DMENU_DEFAULT_MAX_ITEM_TEXT_LEN;
	Name=NULL;
	SetName(MenuItemName);
}

//! Destructor
DMenu::~DMenu()
{
	// free all created Items
	if (ItemsCount > 0) {
		for (short int ixI=0;ixI<ItemsCount;ixI++) {
			delete Items[ixI];
		}
		free(Items);
	}
}

/**
 * @brief Set the name of Item.
 * 
 * @param new name of this Item.
 *
 * N.B. The Name will be truncated to MaxItenTextLen value
 */
void DMenu::SetName(const char ItemName[])
{
    size_t len=strlen(ItemName);
	if (len > MaxItemTextLen) {
        len=MaxItemTextLen;
	}
	if (Name == NULL) {
        Name=new char[len+1];
    }
	strncpy(Name,ItemName,len);
	Name[len]='\0';
}

/**
 * @brief Add new menu item.
 * 
 * @param ItemName	    ->	Name of Item.
 * @param MenuID        ->  ID of new sub menu. You can use it to handle it by id instead of by name. Can be a value from 0 to 254.
 * @param SetCurrent    ->	if true set new Item as current one.
 *
 * @return true of the created item or false if any error occours.
 */
bool DMenu::AddItem(const char ItemName[], uint8_t ItemID, bool SetCurrent)
{
		if (ItemsCount == 0) {
			// No Items yet
			// Allocate first DMenu pointer
			Items=(DMenu **) malloc(sizeof(DMenu*));
			if (Items == NULL) {
				return 0;
			}
		}
		else {
			// Items alreay present
			// try to allocate an other DMenu pointer
			DMenu **tmp=(DMenu **) realloc(Items,sizeof(DMenu*)*(ItemsCount+1));
			if (tmp == NULL) {
				return 0;
			}
			Items=tmp;
		}

		// Create new DMenu and assign it to last array pointer
		DMenu *NewItem=new DMenu(ItemName,ItemID,this,Callback);
		if (NewItem == NULL) {
            return 0;
		}

		Items[ItemsCount]=NewItem;
		// Inc items counter
		ItemsCount++;

		if (SetCurrent || ItemsCount == 1) {
            // Set as current item (also if it is the first one)
            ItemIndex=ItemsCount-1;
		}

		return(NewItem->GetID());
}

//! @return Item at Index position
DMenu* DMenu::GetItem(short int Index)
{
    if (ItemsCount == 0) {
        return(this);
    }

    return(Items[Index]);
}

//! @return current selected Item inside this Menu
DMenu* DMenu::GetCurrItem(void)
{
	if (ItemIndex == -1) {
        return(this);
	}
	return(Items[ItemIndex]);
}

//!  @return parent Item
/**
* N.B. If this menu has no Parent the function return this
**/
DMenu *DMenu::GetParent(void)
{
    return(Parent);
}

bool DMenu::HasParent(void)
{
    return (Parent != NULL);
}

//! @return numbers of DMenu Items in this Menu
uint8_t DMenu::GetItemsCount(void)
{
    return(ItemsCount);
}

//! Change current selected Item to previous one
/**
 * @return	->	a pointer to new current selected Item
 */
DMenu* DMenu::Up(void)
{
	if (ItemsCount == 0) {
        return(this);
	}

	if (ItemIndex == -1) {
        ItemIndex=0;
	}
	else if (ItemIndex == 0) {
		if (Loop) {
			ItemIndex=ItemsCount-1;
		}
	}
	else if (ItemIndex > 0) {
            ItemIndex--;
	}

	if (Callback != NULL) {
        Callback(Items[ItemIndex],DMENU_ACTION_UP);
    }

	return(Items[ItemIndex]);
}

//! Change current selected Item to next one
/**
 * @return	->	a pointer to new current selected Item
 */
DMenu* DMenu::Down(void)
{
	if (ItemsCount == 0) {
        return(this);
	}

	if (ItemIndex == -1) {
        ItemIndex=ItemsCount-1;
	}
	else if (ItemIndex == ItemsCount-1) {
		if (Loop) {
			ItemIndex=0;
		}
	}
	else if (ItemIndex < ItemsCount-1) {
        ItemIndex++;
	}

	if (Callback != NULL) {
        Callback(Items[ItemIndex],DMENU_ACTION_DOWN);
    }

	return(Items[ItemIndex]);
}

//! @return the Parent Item of this
/**
 * If this have a Parent Item, Calling Back() will also fire a Callback event passing the Parent Item as argument, otherwise return NULL.
 */
DMenu* DMenu::Back(void)
{
	if (Callback != NULL) {
        Callback(Parent,DMENU_ACTION_BACK);
    }
	return(Parent);
}

//! Fire a Callback event sending the current selected Item as argument
/**
 * N.B. Callback will be called only if there is a selected Item.
 **/
DMenu* DMenu::Select(void)
{
	if (ItemIndex == -1) {
		return(this);
	}

    if (Callback != NULL) {
    	Callback(Items[ItemIndex],DMENU_ACTION_SELECT);
    }

	return (Items[ItemIndex]);
}

//! @return name string of this Item
const char* DMenu::GetName(void)
{
	return(Name);
}

bool DMenu::NameEquals(const char MenuName[])
{
    return (strcmp(MenuName,Name) == 0);
}

//! @return ID of this Item
uint8_t DMenu::GetID(void)
{
	return(ID);
}

//! @return zero based index of current selected item
/**
* if -1 is returned, no item is selected
**/

short int DMenu::GetCurrItemIndex(void)
{
	return(ItemIndex);
}
