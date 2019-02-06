// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * The Player Controller is used to display Mission Complete Failure/Success text and change the Camera ViewPoint
 * on Event triggered by the Game Mode (server side) through the Game State (replicated).
 *
 * In networked games, PlayerControllers exist on the server for every player-controlled pawn,
 * and also on the controlling client's machine.
 */
UCLASS()
class FPSGAME_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void OnMissionComplete(APawn* InstigatorPawn, bool bMissionSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerController")
	void OnMissionCompleted(APawn* InstigatorPawn, bool bMissionSuccess);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerController")
	TSubclassOf<AActor>	SpectatorViewpointClass;
};
