// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchPad.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OverlapComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	OverlapComponent->SetBoxExtent(FVector(100.f));
	RootComponent = OverlapComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);

	PlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComp"));
	PlaneComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlaneComponent->SetupAttachment(RootComponent);
}

void ALaunchPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor)
	{
		const FVector LaunchPadForwardVector = GetActorForwardVector();
		const FVector LaunchVector = LaunchPadForwardVector.RotateAngleAxis(-LaunchPitchAngle, GetActorRightVector());
		const FVector LaunchVelocityVector = LaunchVector * LaunchVelocity;
		/* NOTE: equivalent to:
		FRotator LaunchPadRotator = GetActorRotation();
		LaunchPadRotator.Pitch += LaunchPitchAngle;
		const FVector LaunchVelocityVector = LaunchPadRotator.Vector() * LaunchVelocity;
		*/

		// Launch the Player Character
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character)
		{
			const bool bXYOverride = false;
			const bool bZOverride = false;
			Character->LaunchCharacter(LaunchVelocityVector, bXYOverride, bZOverride);
		}
		else
		{
			// Else, if its a cube, add the same impulse to it
			// NOTE: using the OnComponentBeginOverlap dynamic binding we would have directly received an "OtherComp" 
			UPrimitiveComponent* OtherComponent = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if (OtherComponent && OtherComponent->IsSimulatingPhysics())
			{
				const bool bVelChange = true;
				OtherComponent->AddImpulse(LaunchVelocityVector, NAME_None, bVelChange);
			}
		}

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchEffect, GetActorLocation());
		UGameplayStatics::PlaySound2D(this, LaunchSound);
	}
}
