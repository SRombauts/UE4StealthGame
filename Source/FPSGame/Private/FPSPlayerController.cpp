// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSPlayerController.h"


void AFPSPlayerController::OnMissionComplete(APawn* InstigatorPawn, bool bMissionSuccess)
{
	APawn* Pawn = GetPawn();
	if (Pawn)
	{
		Pawn->DisableInput(nullptr);
	}

	// BP Implementation
	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}


