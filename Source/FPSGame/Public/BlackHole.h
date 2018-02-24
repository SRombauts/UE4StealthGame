// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

class USphereComponent;


UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

protected:
	// The black hole black sphere
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	// Sphere Component that apply an attractive force in its range
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* AttractionComponent;

	// Sphere Component that destroy any overlapping actor
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* DestroyComponent;

	// The force of attraction to apply by unit of time
	UPROPERTY(EditAnywhere, Category = "Attraction")
	float AttractionForce = 1.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
