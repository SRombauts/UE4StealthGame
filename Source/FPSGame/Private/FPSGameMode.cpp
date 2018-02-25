// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn * InstigatorPawn)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);

		// BP implementation
		OnMissionCompleted(InstigatorPawn);

		if (SpectatorViewpointClass)
		{
			// Get Spectator Viewpoint
			TArray<AActor*> SpectatorViewpoints;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatorViewpointClass, SpectatorViewpoints);
			if (SpectatorViewpoints.Num() > 0)
			{
				AActor* SpectatorViewpoint = SpectatorViewpoints[0];

				// Move camera to Spectator Viewpoint
				APlayerController* PlayerController = Cast<APlayerController>(InstigatorPawn->GetController());
				if (PlayerController && SpectatorViewpoint)
				{
					PlayerController->SetViewTargetWithBlend(SpectatorViewpoint, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Spectator Viewpoint Class specified. Please update GameMode. Cannot change ViewTarget."));
		}
	}
}
