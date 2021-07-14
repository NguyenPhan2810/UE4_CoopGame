// Copyright Epic Games, Inc. All Rights Reserved.


#include "LearnUE4_CoopGameGameModeBase.h"

ALearnUE4_CoopGameGameModeBase::ALearnUE4_CoopGameGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerPawn"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
