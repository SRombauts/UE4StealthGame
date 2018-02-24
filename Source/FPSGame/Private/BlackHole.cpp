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
	AttractionComponent->SetSphereRadius(3000);
	AttractionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttractionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttractionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap); // collision filters : only IsSimulatingPhysics() components
	AttractionComponent->SetupAttachment(MeshComponent);

	DestroyComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyComp"));
	AttractionComponent->SetSphereRadius(100);
	DestroyComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroyComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DestroyComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	DestroyComponent->SetupAttachment(MeshComponent);
	DestroyComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OnBeginOverlapDestroyComponent);
}

void ABlackHole::OnBeginOverlapDestroyComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
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
		if (ComponentToAttract && ComponentToAttract->IsSimulatingPhysics()) // NOTE: this is redundant with the collision filters we have applied
		{
			const FVector ComponentLocation = ComponentToAttract->GetComponentLocation();
			const FVector VectorToBlackHole = BlackHoleLocation - ComponentLocation;
			const float Size = VectorToBlackHole.Size();
			FVector Direction = VectorToBlackHole;
			Direction.Normalize();
			// NOTE: Force should be inverse to the distance (using SizeSquared() is more "realist" and optimized but to harsh)
			const float Coefficient = 1 / Size;
			const FVector ForceTowardBlackHole = Direction * Coefficient * AttractionForce; // NOTE: no more DeltaTime since AddForce already take timestep into account
			const bool bAccelChange = true; // If true, Force is taken as a change in acceleration instead of a physical force (i.e. mass will have no affect).
			ComponentToAttract->AddForce(ForceTowardBlackHole, NAME_None, bAccelChange);
		}
	}
}

