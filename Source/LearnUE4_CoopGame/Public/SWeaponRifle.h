// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SWeaponRifle.generated.h"

// Contains information of a single hitscan weapon linetrace
USTRUCT()
struct FHitScanTrace 
{
	GENERATED_BODY()
public:
	UPROPERTY()
    TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceEnd;

	UPROPERTY()
	unsigned int replicationCounter;
};

/**
 * 
 */
UCLASS()
class LEARNUE4_COOPGAME_API ASWeaponRifle : public ASWeapon
{
	GENERATED_BODY()

	
public:
	ASWeaponRifle();


	virtual void Fire() override;

	virtual void PlayFireEffect(FVector traceEnd);
	virtual void PlayImpactEffect(EPhysicalSurface surfaceType, FVector traceEnd);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName MuzzleFlashSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float BaseDamage;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* ImpactEffectDefault;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* ImpactEffectVulnerable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* SmokeTrailEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName SmokeTrailStartParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName SmokeTrailEndParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<UCameraShakeBase> FireCameraShake;
};
