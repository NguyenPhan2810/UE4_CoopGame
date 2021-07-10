// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponGrenadeLauncher.h"
#include <Kismet/GameplayStatics.h>

void AWeaponGrenadeLauncher::Fire()
{
	if (MuzzleEffect)
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleFlashSocketName);

}
