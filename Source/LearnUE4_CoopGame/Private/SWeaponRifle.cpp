// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponRifle.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include <DrawDebugHelpers.h>
#include <PhysicalMaterials/PhysicalMaterial.h>
#include "../LearnUE4_CoopGame.h"
#include <Net/UnrealNetwork.h>

ASWeaponRifle::ASWeaponRifle()
: Super()
{
	BaseDamage = 20;
	FireInterval = 1 / 10.0;
	bEnableAutomaticFire = true;
	HitScanTrace.replicationCounter = 0;
}

void ASWeaponRifle::Fire()
{
	Super::Fire();

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


		EPhysicalSurface surfaceType = SurfaceType_Default;
		FHitResult hitResult;
		FVector impactPoint = traceEnd;
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, traceBegin, traceEnd, COLLISION_WEAPON, queryParams);

		if (bHit)
		{
			impactPoint = hitResult.ImpactPoint;
			// Hit something
			auto hitActor = hitResult.GetActor();

			// Query surface type
			surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitResult.PhysMaterial.Get());

			// Increase base damage on surface vulnerable
			float actualDamage = BaseDamage;
			if (surfaceType == SURFACE_FLESHVULNERABLE)
				actualDamage *= 100;

			// Apply damage
			UGameplayStatics::ApplyPointDamage(hitActor, actualDamage, traceDirection, hitResult,
				owner->GetInstigatorController(), this, DamageType);			
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

		// Fire effect
		PlayFireEffect(impactPoint);
		// Impact effect
		PlayImpactEffect(surfaceType, impactPoint);

		// Update to all clients
		if (GetLocalRole() == ROLE_Authority)
		{
			HitScanTrace.TraceEnd = impactPoint;
			HitScanTrace.SurfaceType = surfaceType;
			HitScanTrace.replicationCounter++; // Force update even if the other values do not change
		}
	}
}

void ASWeaponRifle::OnRep_HitScanTrace()
{
	PlayFireEffect(HitScanTrace.TraceEnd);
	PlayImpactEffect(HitScanTrace.SurfaceType, HitScanTrace.TraceEnd);
}

	
void ASWeaponRifle::PlayFireEffect(FVector traceEnd)
{
	// Sound
	if (FireSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());

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

void ASWeaponRifle::PlayImpactEffect(EPhysicalSurface surfaceType, FVector traceEnd)
{
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
	{
		auto shotDirection = traceEnd - MeshComponent->GetSocketLocation(MuzzleFlashSocketName);
		shotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectedImpactEffect, traceEnd, shotDirection.Rotation());
	}
}

void ASWeaponRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeaponRifle, HitScanTrace, COND_SkipOwner);
}