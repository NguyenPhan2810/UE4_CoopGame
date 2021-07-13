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
	FVector_NetQuantize TraceBegin;

	UPROPERTY()
	FVector_NetQuantize TraceEnd;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	FName MuzzleFlashSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<UCameraShakeBase> FireCameraShake;
};
