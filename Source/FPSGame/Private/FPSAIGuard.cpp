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
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn)
	{
		ACharacter* Character = Cast<ACharacter>(SeenPawn);
		if (Character)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnPawnSeen: Character!");

			DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);
		}
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
