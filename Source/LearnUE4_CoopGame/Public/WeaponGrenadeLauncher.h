// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "WeaponGrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class LEARNUE4_COOPGAME_API AWeaponGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleFlashSocketName;
};
