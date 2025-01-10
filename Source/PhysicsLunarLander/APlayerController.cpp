// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayerController.h"
#include "PlayerLander.h"
#include "GameFramework/Pawn.h"

AAPlayerController::AAPlayerController()
{
	// Constructor
	PlayerLanderPawn = nullptr;
}

void AAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("PlayerController BeginPlay called"));

	// Get the controlled pawn (which should be the PlayerLander)
	PlayerLanderPawn = Cast<APlayerLander>(GetPawn());
	if (!PlayerLanderPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerLanderPawn not found!"));
	}
}

void AAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent is set up"));
	}

	// Bind axes for thrust movement
	InputComponent->BindAxis("LeftThrust", this, &AAPlayerController::ThrustLeft);
	InputComponent->BindAxis("RightThrust", this, &AAPlayerController::ThrustRight);
	InputComponent->BindAxis("UpwardThrust", this, &AAPlayerController::ThrustUpward);

	// Bind action for grapple
	//InputComponent->BindAction("ActivateGrapple", IE_Pressed, this, &AAPlayerController::ActivateGrapple);
}

void AAPlayerController::ThrustLeft(float AxisValue)
{

	UE_LOG(LogTemp, Warning, TEXT("ThrustLeft Axis Value: %f"), AxisValue);
	if (PlayerLanderPawn)
	{
		PlayerLanderPawn->ThrustLeft(AxisValue);
	}
}

void AAPlayerController::ThrustRight(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ThrustRight Axis Value: %f"), AxisValue);
	if (PlayerLanderPawn)
	{
		PlayerLanderPawn->ThrustRight(AxisValue);
	}
}

void AAPlayerController::ThrustUpward(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("ThrustUp Axis Value: %f"), AxisValue);
	if (PlayerLanderPawn)
	{
		PlayerLanderPawn->ThrustUpward(AxisValue);
	}
}



