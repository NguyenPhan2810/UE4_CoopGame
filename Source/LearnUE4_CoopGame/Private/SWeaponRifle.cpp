// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponRifle.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>

void ASWeaponRifle::Fire()
{
	// Trace the world, from muzzle to cross hair location
	auto owner = GetOwner();
	if (owner)
	{
		FVector eyeLocation;
		FRotator eyeRotation;
		owner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

		auto cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;


		FVector traceDirection = cameraManager->GetCameraRotation().Vector();
		FVector traceBegin = eyeLocation;
		FVector traceEnd = traceBegin + traceDirection * 10000;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);
		queryParams.AddIgnoredActor(owner);
		queryParams.bTraceComplex = true;

		FHitResult hitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, traceBegin, traceEnd, ECC_Visibility, queryParams);

		if (bHit)
		{
			// Hit something
			auto hitActor = hitResult.GetActor();

			UGameplayStatics::ApplyPointDamage(hitActor, 20, traceDirection, hitResult,
				owner->GetInstigatorController(), this, DamageType);

			// Impact effect
			if (ImpactEffect)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hitResult.ImpactPoint, hitResult.ImpactNormal.Rotation());
		}

		// Muzzle effect
		if (MuzzleEffect)
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleFlashSocketName);

		// Smoke trail effect
		if (SmokeTrailEffect)
		{
			auto muzzleTrans = MeshComponent->GetSocketTransform(MuzzleFlashSocketName);
			auto smokeTrailComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeTrailEffect, muzzleTrans);
			if (smokeTrailComp)
			{
				smokeTrailComp->SetVectorParameter(SmokeTrailStartParamName, traceBegin);

				if (bHit)
					smokeTrailComp->SetVectorParameter(SmokeTrailEndParamName, hitResult.ImpactPoint);
				else
					smokeTrailComp->SetVectorParameter(SmokeTrailEndParamName, traceEnd);
			}
		}
	}
}