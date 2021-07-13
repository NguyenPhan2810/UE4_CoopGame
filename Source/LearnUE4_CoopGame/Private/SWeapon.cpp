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
	bAllowedToFire = false;
	
	SetReplicates(true);
}

void ASWeapon::Fire()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		bAllowedToFire = true;

		lastFireTime = GetWorld()->TimeSeconds;

		BlueprintFireEvent();
	}
	else // Client
	{
		bAllowedToFire = false;
		ServerFire();
	}
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

// If there's anything wrong with the code that sent to the server
// the client who sent that code will be disconnected
bool ASWeapon::ServerFire_Validate()
{
	return true;
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
