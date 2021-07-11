// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponGrenadeLauncher.h"
#include "SGrenade.h"
#include <Kismet/GameplayStatics.h>

ASWeaponGrenadeLauncher::ASWeaponGrenadeLauncher()
: Super()
{
	FireInterval = 1;
	bEnableAutomaticFire = true;
}

void ASWeaponGrenadeLauncher::Fire()
{
	Super::Fire();
	if (MuzzleEffect)
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleFlashSocketName);

	// Trace the world, from muzzle to cross hair location
	if (GrenadeBP)
	{
		auto cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

		auto spawnRotation = cameraManager->GetCameraRotation();

		auto spawnLocation = MeshComponent->GetSocketLocation(MuzzleFlashSocketName);

		FTransform spawnTransform;
		spawnTransform.SetLocation(spawnLocation);
		spawnTransform.SetRotation(spawnRotation.Quaternion());
		auto theGrenade = GetWorld()->SpawnActor<ASGrenade>(GrenadeBP, spawnTransform);
		theGrenade->SetOwner(this);
	}
}
