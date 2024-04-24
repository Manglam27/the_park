// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Master_Inventory.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class THE_PARK_API UMaster_Inventory : public UActorComponent 
{
	GENERATED_BODY()

public:
	UMaster_Inventory();

	/*/=================================================
	
	bool AddItem(class UItems_Uobject* Item);
	bool RemoveItems(class UItems_Uobject* Item);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Inventory")
	TArray<class UItems_Uobject*> DefaultItems;

	UPROPERTY(EditDefaultsOnly,Category="Inventory")
	int32 Capacity;

	UPROPERTY(BlueprintAssignable,Category="Inventory")
	FOnInventoryUpdate OnInventoryUpdate;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Inventory")
	TArray<class UItems_Uobject*> AllItemses; 

*/
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

	
//============================================================================================
public:


	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="New_Main")
	// int Column;
	//
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="New_Main")
		// int Rows;

	// UFUNCTION(BlueprintCallable)
	// void TryADDItems(class UItems_Uobject* Item,bool &added);

	// UPROPERTY(VisibleAnywhere,Category="New_Main")
	// TArray<class UItems_Uobject*> Items;

	// UFUNCTION(BlueprintCallable)
	// void HasSpace(class UItems_Uobject* Item,int32 TopLeftIndex);
	//
	// void macro_1(class UItems_Uobject* Item,int32 TopLeftIndex);
	//
	// int IndexToTile(int32 Index );
	//
	// UFUNCTION(BlueprintImplementableEvent)
	// void NewFunction_0();
};
