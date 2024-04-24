// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items_Uobject.generated.h"

/**
 * 
 */
UCLASS(Abstract,EditInlineNew, Blueprintable, BlueprintType, DefaultToInstanced)
class THE_PARK_API UItems_Uobject : public UObject
{
	GENERATED_BODY()
public:
	UItems_Uobject();
};
