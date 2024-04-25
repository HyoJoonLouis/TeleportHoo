// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeleportHooGameMode.h"
#include "TeleportHooCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATeleportHooGameMode::ATeleportHooGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
