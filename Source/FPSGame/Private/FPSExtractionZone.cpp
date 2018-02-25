// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#include "FPSExtractionZone.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

#include "Engine.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OverlapComponent->SetBoxExtent(FVector(200.f));
	OverlapComponent->SetHiddenInGame(false);
	RootComponent = OverlapComponent;

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::OnBeginOverlap);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->DecalSize = FVector(200.f);
}

void AFPSExtractionZone::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	if (Character)
	{
		if (Character->bIsCarryingObjective)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Extraction Zone reached with the objective.");

			UGameplayStatics::PlaySound2D(this, MissionCompleteSound);

			AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
			if (GameMode)
			{
				GameMode->CompleteMission(Character);
			}
		}
		else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Extraction Zone reached without the objective!");

			UGameplayStatics::PlaySound2D(this, MissingObjectiveSound);
		}
	}
}
