// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ASWeapon::Fire()
{
	// Trace the world, from muzzle to cross hair location
	auto owner = GetOwner();
	if (owner)
	{
		FVector eyeLocation;
		FRotator eyeRotation;
		owner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

		FVector traceDirection = eyeRotation.Vector();
		FVector traceEnd = eyeLocation + traceDirection * 10000;

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);
		queryParams.AddIgnoredActor(owner);
		//queryParams.bTraceComplex = true;

		FHitResult hitResult;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, eyeLocation, traceEnd, ECC_Visibility, queryParams))
		{
			// Hit something
			DrawDebugLine(GetWorld(), hitResult.TraceStart, hitResult.TraceEnd, FColor::Green, false, 1, 0, 1);
		
			auto hitActor = hitResult.GetActor();

			UGameplayStatics::ApplyPointDamage(hitActor, 20, traceDirection, hitResult,
				owner->GetInstigatorController(), this, DamageType);
		}
		else

			DrawDebugLine(GetWorld(), eyeLocation, traceEnd, FColor::Blue, false, 1, 0, 1);
	}
}