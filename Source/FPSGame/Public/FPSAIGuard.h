// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

// The AI Guard opponent that tracks the player
UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume);

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float DistractionDuration = 3.0f;

private:
	FRotator OriginalRotation;
	FTimerHandle TimerHandler_ResetOrientation;

	void ResetOrientation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
