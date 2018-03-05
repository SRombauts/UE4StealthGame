// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSAIGuard.h"

#include "FPSGameMode.h"
#include "FPSAIController.h"

#include "Perception/PawnSensingComponent.h"
#include "Components/WidgetComponent.h"

//#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
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
		//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnPawnSeen: Character!");

			DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 48.0f, 12, FColor::Red, false, DistractionDuration);

			AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
			if (GameMode)
			{
				SetGuardState(EGuardState::Alerted);
				GameMode->CompleteMission(Character, false); // false: Mission Failed
			}
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
		//	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnNoiseHeard: Character!");

			// Equivalent to Blueprint's FindLookAtRotation()
			const FVector Direction = Location - GetActorLocation();
			FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
			// But only Yaw rotation (around the Z axis)
			NewLookAt.Pitch = 0.0f;
			NewLookAt.Roll = 0.0f;
			SetActorRotation(NewLookAt);

			GetWorldTimerManager().ClearTimer(TimerHandler_ResetOrientation);
			GetWorldTimerManager().SetTimer(TimerHandler_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, DistractionDuration);

			SetGuardState(EGuardState::Sucpicious);
		}
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnNoiseHeard: no pawn!");
	}
}

// Client only replication callback
void AFPSAIGuard::OnRep_GuardState()
{
	// BP Implementation
	OnStateChanged(GuardState);
}

void AFPSAIGuard::SetGuardState(EGuardState NewState)
{
	// Do not change state when mission already lost or completed
	if (NewState != GuardState && GuardState != EGuardState::Alerted && GuardState != EGuardState::MissionComplete)
	{
		GuardState = NewState; // This is done on the Server, and will replicate to the client

		// BP Implementation
		OnStateChanged(NewState);

		// Notify AI Controller
		AFPSAIController* FPSAIController = Cast<AFPSAIController>(GetController());
		if (FPSAIController)
		{
			FPSAIController->OnStateChanged(NewState);
		}
	}
}

void AFPSAIGuard::ResetOrientation()
{
	// Do not reset orientation when mission already lost or completed
	if (GuardState != EGuardState::Alerted && GuardState != EGuardState::MissionComplete)
	{
		SetActorRotation(OriginalRotation);
		SetGuardState(EGuardState::Patrol);
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}

