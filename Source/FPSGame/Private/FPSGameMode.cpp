// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSGameMode.h"

#include "FPSCharacter.h"
#include "FPSGameState.h"
#include "FPSHUD.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

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
	if (InstigatorPawn)
	{
		// Changes the Camera ViewPoint of all Player Controllers on either Event
		// TODO: this whole thing should go directly into our custom PlayerController
		if (SpectatorViewpointClass)
		{
			// Get Spectator Viewpoint
			TArray<AActor*> SpectatorViewpoints;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatorViewpointClass, SpectatorViewpoints);
			if ((SpectatorViewpoints.Num() > 0) && SpectatorViewpoints[0])
			{
				AActor* SpectatorViewpoint = SpectatorViewpoints[0];

				// Move camera to Spectator Viewpoint
				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{
					APlayerController* PlayerController = Cast<APlayerController>(It->Get());
					if (PlayerController)
					{
						PlayerController->SetViewTargetWithBlend(SpectatorViewpoint, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Spectator Viewpoint Class specified. Please update GameMode. Cannot change ViewTarget."));
		}
	}

	// Notifies the Game State via Multicast replication
	AFPSGameState* GameState = GetGameState<AFPSGameState>();
	if (GameState)
	{
		GameState->MulticastOnMissionComplete(InstigatorPawn, bSuccess);
	}
}
