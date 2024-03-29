// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

/**
 * The Game Mode is a Server-only instance that is called either:
 * - by the Extraction Zone to trigger mission completion when the objective has been carried successfully,
 * - by the AI Guard to trigger mission failure when the player has been spotted.
 *
 *  It notifies this Mission Completion Event to the Game State via Multicast replication.
*/
UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AFPSGameMode();

	void CompleteMission(APawn* InstigatorPawn, bool bSuccess);
};



