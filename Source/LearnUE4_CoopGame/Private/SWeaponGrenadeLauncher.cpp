// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponGrenadeLauncher.h"
#include "SGrenade.h"
#include <Kismet/GameplayStatics.h>

void ASWeaponGrenadeLauncher::Fire()
{
	if (MuzzleEffect)
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleFlashSocketName);

	if (GrenadeBP)
	{
		auto muzzleTransform = MeshComponent->GetSocketTransform(MuzzleFlashSocketName);
		muzzleTransform.AddToTranslation(FVector(10, 0, 0));
		auto theGrenade = GetWorld()->SpawnActor<ASGrenade>(GrenadeBP, muzzleTransform);
		theGrenade->SetOwner(this);
	}
}
