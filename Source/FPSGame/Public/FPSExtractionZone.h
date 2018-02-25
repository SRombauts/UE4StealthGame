// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExtractionZone.generated.h"

// The Extraction Zone where to carry the Objective
UCLASS()
class FPSGAME_API AFPSExtractionZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSExtractionZone();

protected:
	// Extraction zone overlap component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* OverlapComponent;

	// Decal for the extraction zone
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UDecalComponent* DecalComponent;

	// Sound to play when entering the extraction zone without the objective
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* MissingObjectiveSound;

	// Sound to play on mission complete
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* MissionCompleteSound;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
