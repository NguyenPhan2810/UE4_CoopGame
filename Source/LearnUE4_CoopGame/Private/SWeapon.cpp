// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include <DrawDebugHelpers.h>

int ASWeapon::DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	L"COOP.DebugWeapons", 
	ASWeapon::DebugWeaponDrawing, 
	L"Draw debug for weapons",
	ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);

	FireInterval = 1;
	bEnableAutomaticFire = true;
	lastFireTime = 0;

	bReplicates = true;
}

void ASWeapon::Fire()
{
	lastFireTime = GetWorld()->TimeSeconds;

	BlueprintFireEvent();
}

void ASWeapon::BeginFire()
{
	if (bEnableAutomaticFire)
	{
		float firstDelay = FMath::Max(lastFireTime + FireInterval - GetWorld()->TimeSeconds, 0.0f);
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ASWeapon::Fire, FireInterval, true, firstDelay);
	}
}

void ASWeapon::EndFire()
{
	if (bEnableAutomaticFire)
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
}
