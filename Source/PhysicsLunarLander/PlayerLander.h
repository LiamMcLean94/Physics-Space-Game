// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerLander.generated.h"

// Forward declarations
class UStaticMeshComponent;
class USphereComponent;
class USplineComponent;

UCLASS()
class PHYSICSLUNARLANDER_API APlayerLander : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerLander();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ThrustLeft(float AxisValue);
	void ThrustRight(float AxisValue);
	void ThrustUpward(float AxisValue);
	void ActivateGrapple(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DeactivateGrapple();
	void HandleActiveGrappleInput();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LanderBody;

	UPROPERTY(EditAnywhere)
	USphereComponent* GrappleCollider;

	UPROPERTY(EditAnywhere)
	USplineComponent* GrappleSpline;

	UPROPERTY(EditAnywhere)
	float ThrustPower;
	UPROPERTY(EditAnywhere)
	float GravityScale;

	bool bGrappleActive;
	AActor* AttachedObject;
	void AdjustPhysicsWithObject(AActor* Object);
};

