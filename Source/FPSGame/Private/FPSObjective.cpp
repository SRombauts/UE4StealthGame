// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSObjective.h"
#include "FPSCharacter.h"
#include "Components/DrawSphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSObjective::AFPSObjective()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComponent->SetupAttachment(MeshComponent);

	// Replicate for Network Multi-player
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSObjective::BeginPlay()
{
	Super::BeginPlay();

	PlayEffects();
}

void AFPSObjective::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

void AFPSObjective::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	if (Role == ENetRole::ROLE_Authority)
	{
		AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
		if (Character)
		{
			Character->bIsCarryingObjective = true;

			Destroy();
		}
	}
}
