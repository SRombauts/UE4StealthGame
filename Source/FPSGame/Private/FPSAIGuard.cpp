// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSAIGuard.h"

#include "Perception/PawnSensingComponent.h"

#include "Engine.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn)
	{
		ACharacter* Character = Cast<ACharacter>(SeenPawn);
		if (Character)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnPawnSeen: Character!");

			DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 48.0f, 12, FColor::Red, false, DistractionDuration);
		}
	}
}

void AFPSAIGuard::OnNoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Emerald, false, DistractionDuration);

	if (InstigatorPawn)
	{
		ACharacter* Character = Cast<ACharacter>(InstigatorPawn);
		if (Character)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnNoiseHeard: Character!");

			// Equivalent to Blueprint's FindLookAtRotation()
			const FVector Direction = Location - GetActorLocation();
			FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
			// But only Yaw rotation (around the Z axis)
			NewLookAt.Pitch = 0.0f;
			NewLookAt.Roll = 0.0f;
			SetActorRotation(NewLookAt);

			GetWorldTimerManager().ClearTimer(TimerHandler_ResetOrientation);
			GetWorldTimerManager().SetTimer(TimerHandler_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, DistractionDuration);
		}
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnNoiseHeard: no pawn!");
	}
}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
