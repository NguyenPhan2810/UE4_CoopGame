// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponGrenadeLauncher.h"
#include <Kismet/GameplayStatics.h>

void ASWeaponGrenadeLauncher::Fire()
{
	if (MuzzleEffect)
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleFlashSocketName);

	//if (GrenadeBP)
		//GetWorld()->SpawnActor<AGrenade>(GrenadeBP, GetTransform());
}
