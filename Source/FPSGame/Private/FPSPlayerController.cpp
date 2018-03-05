// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSPlayerController.h"

#include "Kismet/GameplayStatics.h"

void AFPSPlayerController::OnMissionComplete(APawn* InstigatorPawn, bool bMissionSuccess)
{
	APawn* Pawn = GetPawn();
	if (Pawn)
	{
		Pawn->DisableInput(nullptr);
	}

	if (SpectatorViewpointClass)
	{
		// Get Spectator Viewpoint
		TArray<AActor*> SpectatorViewpoints;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatorViewpointClass, SpectatorViewpoints);
		if ((SpectatorViewpoints.Num() > 0) && SpectatorViewpoints[0])
		{
			AActor* SpectatorViewpoint = SpectatorViewpoints[0];

			// Move camera to Spectator Viewpoint
			SetViewTargetWithBlend(SpectatorViewpoint, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Spectator Viewpoint Class specified. Please update GameMode. Cannot change ViewTarget."));
	}

	// BP Implementation
	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}


