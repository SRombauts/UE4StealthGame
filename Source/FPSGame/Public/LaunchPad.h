// Copyright 2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

// A Launch Pad to launch the player in the air, as well as any other physic-enabled Actors
UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* PlaneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* OverlapComponent;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float LaunchPitchAngle = 45.f;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float LaunchVelocity = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* LaunchEffect;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* LaunchSound;


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
