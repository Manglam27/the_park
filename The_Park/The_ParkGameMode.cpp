// Copyright Epic Games, Inc. All Rights Reserved.

#include "The_ParkGameMode.h"
#include "The_ParkCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThe_ParkGameMode::AThe_ParkGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
