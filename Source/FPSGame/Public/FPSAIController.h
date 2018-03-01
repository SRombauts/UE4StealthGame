// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called on completing current movement request */
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	void GoToNextPoint();

	TArray<AActor*> TargetPoints;
	int32 TargetPointsIndex = -1;
};
