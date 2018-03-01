// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAIController.generated.h"

enum class EGuardState : uint8;

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	void OnStateChanged(EGuardState NewState);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called on completing current movement request */
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	void SelectTargetPoint();
	void MoveToTargetPoint();

	TArray<AActor*> TargetPoints;
	int32 TargetPointsIndex = 0;
};
