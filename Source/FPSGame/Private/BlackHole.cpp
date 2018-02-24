// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	AttractionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("AttractionComp"));
	AttractionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttractionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttractionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	AttractionComponent->SetupAttachment(MeshComponent);

	DestroyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyComp"));
	DestroyComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroyComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DestroyComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	DestroyComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector BlackHoleLocation = GetActorLocation();

	TArray<UPrimitiveComponent*> ComponentsToAttract;
	AttractionComponent->GetOverlappingComponents(ComponentsToAttract);
	for (auto& ComponentToAttract : ComponentsToAttract)
	{
		const FVector ComponentLocation = ComponentToAttract->GetComponentLocation();
		const FVector VectorToBlackHole = BlackHoleLocation - ComponentLocation;
		const float Size = VectorToBlackHole.Size();
		FVector Direction = VectorToBlackHole;
		Direction.Normalize();
		// Force should be inverse to the distance (SizeSquared() is more optimized but to harsh)
		const float Coefficient = 1000 / Size;
		const FVector ForceTowardBlackHole = Direction * DeltaTime * Coefficient * AttractionForce;
		const bool bAccelChange = false; // If true, Force is taken as a change in acceleration instead of a physical force (i.e. mass will have no affect).
		ComponentToAttract->AddForce(ForceTowardBlackHole, NAME_None, bAccelChange);
	}

	TArray<AActor*> ActorsToDestroy;
	DestroyComponent->GetOverlappingActors(ActorsToDestroy);
	for (auto& ActorToDestroy : ActorsToDestroy)
	{
		ActorToDestroy->Destroy();
	}
}

