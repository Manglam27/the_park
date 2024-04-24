// 


#include "Items/Master_Inventory.h"
#include "Items/Items_Uobject.h"

// Sets default values for this component's properties
UMaster_Inventory::UMaster_Inventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



void UMaster_Inventory::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// Called when the game starts
void UMaster_Inventory::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	/*
	Capacity = 50.f;

	for(auto& Items_test : DefaultItems)
	{
		AddItem(Items_test);
	}
	*/
}

/**
// Called every frame


/**
*		========================
*/
//=======================================================================

//
// void UMaster_Inventory::TryADDItems(UItems_Uobject* Item,bool &Added)
// {
// 	if(IsValid(Item))
// 	{
// 		for(auto& things:Items)
// 		{
// 			
// 		}
// 	}
// 	
// }
//
// void UMaster_Inventory::HasSpace(UItems_Uobject* Item, int32 TopLeftIndex)
// {
// 	
// }
//
// void UMaster_Inventory::macro_1(UItems_Uobject* Item, int32 TopLeftIndex)
// {
// 	// int x,y ;
// 	// x , y =IndexToTile(TopLeftIndex);
// 	//
// 	// FIntPoint a = Item->Dimension;
// 	//
// 	// for(int i = x ; i < (x+a.X) ; ++i)
// 	// {
// 	// 	for (int j = y; j<(y+a.Y);++j )
// 	// 	{
// 	// 		
// 	// 	}
// 	// }
// }
//
// int UMaster_Inventory::IndexToTile(int32 Index )
// {
// 	int x,y;
//
// 	x  = Index % Column;
// 	y = Index / Column;
// 	
// 	return x,y;
// }





