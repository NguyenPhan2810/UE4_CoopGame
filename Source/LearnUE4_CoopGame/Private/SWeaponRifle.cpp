// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponRifle.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include <DrawDebugHelpers.h>
#include <PhysicalMaterials/PhysicalMaterial.h>
#include "../LearnUE4_CoopGame.h"

ASWeaponRifle::ASWeaponRifle()
: Super()
{
	BaseDamage = 20;
	FireInterval = 1 / 10.0;
	bEnableAutomaticFire = true;
}

void ASWeaponRifle::Fire()
{
	Super::Fire();

	if (!bAllowedToFire)
		return;

	// Trace the world, from muzzle to cross hair location
	auto owner = GetOwner();
	if (owner)
	{
		FVector eyeLocation;
		FRotator eyeRotation;
		owner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

		FVector traceDirection = eyeRotation.Vector();
		FVector traceBegin = eyeLocation;
		FVector traceEnd = traceBegin + traceDirection * 10000;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);
		queryParams.AddIgnoredActor(owner);
		queryParams.bTraceComplex = true;
		queryParams.bReturnPhysicalMaterial = true;

		FHitResult hitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, traceBegin, traceEnd, COLLISION_WEAPON, queryParams);

		if (bHit)
		{
			// Hit something
			auto hitActor = hitResult.GetActor();

			// Query surface type
			auto surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitResult.PhysMaterial.Get());

			// Increase base damage on surface vulnerable
			float actualDamage = BaseDamage;
			if (surfaceType == SURFACE_FLESHVULNERABLE)
				actualDamage *= 100;

			// Apply damage
			UGameplayStatics::ApplyPointDamage(hitActor, actualDamage, traceDirection, hitResult,
				owner->GetInstigatorController(), this, DamageType);


			// Impact effect
			UParticleSystem* selectedImpactEffect = nullptr;
			switch (surfaceType)
			{
			case SURFACE_FLESHDEFAULT:
			case SURFACE_FLESHVULNERABLE:
				selectedImpactEffect = ImpactEffectVulnerable; 
				break;
			default: 
				selectedImpactEffect = ImpactEffectDefault; 
				break;
			}

			if (selectedImpactEffect)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectedImpactEffect, hitResult.ImpactPoint, hitResult.ImpactNormal.Rotation());
		}

		// Draw debug
		if (DebugWeaponDrawing > 0)
		{
			if (bHit)
			{
				DrawDebugLine(GetWorld(), hitResult.TraceStart, hitResult.ImpactPoint, FColor::Green, false, 1.5, 0, 1);
				DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 8, 8, FColor::Yellow, false, 1.5, 0, 1);
			}
			else
				DrawDebugLine(GetWorld(), hitResult.TraceStart, hitResult.TraceEnd, FColor::Red, false, 1.5, 0, 1);
		}

		PlayFireEffect(hitResult.TraceEnd);
	}
}
	
void ASWeaponRifle::PlayFireEffect(FVector traceEnd)
{

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
			smokeTrailComp->SetVectorParameter(SmokeTrailStartParamName, muzzleTrans.GetLocation());
			smokeTrailComp->SetVectorParameter(SmokeTrailEndParamName, traceEnd);
		}
	}

	// Camera shake
	auto player = Cast<APawn>(GetOwner());
	if (player)
	{
		auto controller = Cast<APlayerController>(player->GetController());
		if (controller)
		{
			controller->ClientPlayCameraShake(FireCameraShake);
		}
	}
}