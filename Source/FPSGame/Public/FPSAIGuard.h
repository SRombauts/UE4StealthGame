// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

UENUM(BlueprintType)
enum class EGuardState : uint8
{
	Patrol,
	Sucpicious,
	Alerted,
 	MissionComplete // Either Success or Failure
};

// The AI Guard opponent that tracks the player
UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

	void SetGuardState(EGuardState NewState);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* WidgetComponent;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume);

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float DistractionDuration = 4.0f;

	UPROPERTY(ReplicatedUsing = OnRep_GuardState, BlueprintReadOnly, Category = "AI")
	EGuardState GuardState = EGuardState::Patrol;

	UFUNCTION()
	void OnRep_GuardState();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EGuardState NewState);

	UPROPERTY(EditAnywhere, Category = "AI")
	bool bPatrol = false;

public:
	bool DoPatrol() const { return bPatrol; }

private:
	FRotator OriginalRotation;
	FTimerHandle TimerHandler_ResetOrientation;

	void ResetOrientation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
