// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "APlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PHYSICSLUNARLANDER_API AAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAPlayerController();

protected:
	// Setup input bindings
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	// Thrust movement functions
	void ThrustLeft(float AxisValue);
	void ThrustRight(float AxisValue);
	void ThrustUpward(float AxisValue);

	// Grapple activation
	//void ActivateGrapple(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// Reference to the PlayerLander pawn
	class APlayerLander* PlayerLanderPawn;

	
};
