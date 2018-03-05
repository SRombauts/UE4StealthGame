// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSGameMode.h"

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
	// Notifies the Game State via Multicast replication
	AFPSGameState* GameState = GetGameState<AFPSGameState>();
	if (GameState)
	{
		GameState->MulticastOnMissionComplete(InstigatorPawn, bSuccess);
	}

	// TODO: notify All AI Controllers
}
