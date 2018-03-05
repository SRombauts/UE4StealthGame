// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSGameMode.h"

#include "FPSAIController.h"
#include "FPSAIGuard.h"
#include "FPSCharacter.h"
#include "FPSGameState.h"
#include "FPSHUD.h"

#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn * InstigatorPawn, bool bSuccess)
{
	// Notify the Game State via Multicast replication
	AFPSGameState* GameState = GetGameState<AFPSGameState>();
	if (GameState)
	{
		GameState->MulticastOnMissionComplete(InstigatorPawn, bSuccess);
	}

	// Notify all AI Guards (through their Controller) that the Game as stopped
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; It++)
	{
		AFPSAIController* AIController = Cast<AFPSAIController>(It->Get());
		if (AIController)
		{
			AFPSAIGuard* AIGuard = Cast<AFPSAIGuard>(AIController->GetPawn());
			if (AIGuard)
			{
				AIGuard->SetGuardState(EGuardState::MissionComplete);
			}
		}
	}
}
