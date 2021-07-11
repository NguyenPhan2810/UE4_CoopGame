// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

UCLASS()
class LEARNUE4_COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	virtual void Fire();

protected:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components)
	class USkeletalMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<UDamageType> DamageType;


public:
	// Console variable for debugging
	// value = 0 means disable and > 0 means enable
	static int DebugWeaponDrawing;

public:	
};
