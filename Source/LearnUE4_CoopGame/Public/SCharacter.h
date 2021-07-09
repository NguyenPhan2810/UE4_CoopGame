// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class LEARNUE4_COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Directional movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Crouch
	void BeginCrouch();
	void EndCrouch();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class USpringArmComponent* SpringArmComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
