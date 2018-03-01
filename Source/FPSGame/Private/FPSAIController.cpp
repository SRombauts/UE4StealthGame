// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSAIController.h"

#include "FPSAIGuard.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"

// Called when the game starts or when spawned
void AFPSAIController::BeginPlay()
{
	Super::BeginPlay();

	// Get TargetPoint once at start-time
	UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPoints);

	MoveToTargetPoint();
}

void AFPSAIController::OnStateChanged(EGuardState NewState)
{
	// stop/resume moving
	if (NewState != EGuardState::Patrol)
	{
		StopMovement();
	}
	else
	{
		MoveToTargetPoint();
	}
}

void AFPSAIController::SelectTargetPoint()
{
	if (TargetPoints.Num() > 0)
	{
		if (++TargetPointsIndex == TargetPoints.Num())
		{
			TargetPointsIndex = 0;
		}
	}
}

void AFPSAIController::MoveToTargetPoint()
{
	if (TargetPoints.Num() > 0)
	{
		AActor* TargetPoint = TargetPoints[TargetPointsIndex];
		if (TargetPoint)
		{
			MoveToActor(TargetPoint);
		}
	}
}

/** Called on completing current movement request */
void AFPSAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (!Result.IsInterrupted())
	{
		SelectTargetPoint();
		MoveToTargetPoint();
	}
}
