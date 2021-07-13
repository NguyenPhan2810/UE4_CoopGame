// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponGrenadeLauncher.h"
#include "SGrenade.h"
#include <Kismet/GameplayStatics.h>

ASWeaponGrenadeLauncher::ASWeaponGrenadeLauncher()
: Super()
{
	FireInterval = 1;
	bEnableAutomaticFire = true;
	LaunchStrength = 1000;
}

void ASWeaponGrenadeLauncher::Fire()
{
	Super::Fire();

	if (!bAllowedToFire)
		return;

	if (MuzzleEffect)
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleFlashSocketName);

	// Trace the world, from muzzle to cross hair location
	auto owner = GetOwner();
	if (GrenadeBP && owner)
	{
		FVector eyeLocation;
		FRotator eyeRotation;
		owner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

		auto spawnRotation = eyeRotation;
		auto spawnLocation = MeshComponent->GetSocketLocation(MuzzleFlashSocketName);

		FTransform spawnTransform;
		spawnTransform.SetLocation(spawnLocation);
		spawnTransform.SetRotation(spawnRotation.Quaternion());
		ASGrenade* theGrenade = GetWorld()->SpawnActor<ASGrenade>(GrenadeBP, spawnTransform);
		theGrenade->SetOwner(this);
		auto meshComponentGrenade = theGrenade->FindComponentByClass<UMeshComponent>();
		if (meshComponentGrenade)
		{
			auto launchDirection = spawnRotation.Vector();
			meshComponentGrenade->AddImpulse(launchDirection * LaunchStrength, NAME_None, false);
		}
	}
}
