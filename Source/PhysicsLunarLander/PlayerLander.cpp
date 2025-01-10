#include "PlayerLander.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
APlayerLander::APlayerLander()
{
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize LanderBody as the root component
	LanderBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanderBody"));
	RootComponent = LanderBody;
	LanderBody->SetSimulatePhysics(true);

	// Initialize Grapple components
	GrappleCollider = CreateDefaultSubobject<USphereComponent>(TEXT("GrappleCollider"));
	GrappleCollider->SetupAttachment(LanderBody);
	GrappleCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerLander::ActivateGrapple);

	GrappleSpline = CreateDefaultSubobject<USplineComponent>(TEXT("GrappleSpline"));
	GrappleSpline->SetupAttachment(LanderBody);

	// Set initial thrust power and gravity scale
	ThrustPower = 500.0f;
	GravityScale = 1.0f;

	bGrappleActive = false;
	AttachedObject = nullptr;
}

// Called when the game starts or when spawned
void APlayerLander::BeginPlay()
{
	Super::BeginPlay();
	LanderBody->SetSimulatePhysics(true);  // Ensure physics is simulated
	LanderBody->SetEnableGravity(true);    // Gravity is enabled
	LanderBody->SetLinearDamping(0.1f);
	LanderBody->SetAngularDamping(0.1f);

	APlayerController* LocalPlayerController = Cast<APlayerController>(GetController());
	if (LocalPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is assigned to the pawn"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No controller assigned"));
	}

	// Ensure the camera is following the player
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetViewTarget(this); // Set the view to the player pawn
	}
}

// Called every frame
void APlayerLander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerLander::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//PlayerInputComponent->BindAction("ActivateGrapple", IE_Pressed, this, &APlayerLander::HandleActiveGrappleInput);
}

void APlayerLander::ThrustLeft(float AxisValue)
{
	if (LanderBody && AxisValue != 0)
	{
		// Thrust force direction is left on the X-axis
		FVector ThrustDirection = FVector(-1, 0, 0);  // Left on X
		FVector ThrustForce = ThrustDirection * AxisValue * ThrustPower;

		// Check if ThrustPower is large enough
		if (ThrustForce.Size() > 0.1f) // Ensure some force is applied
		{
			LanderBody->AddForce(ThrustForce);
			UE_LOG(LogTemp, Warning, TEXT("ThrustLeft Axis Value: %f, Thrust Force: %s"), AxisValue, *ThrustForce.ToString());
		}
	}
}

void APlayerLander::ThrustRight(float AxisValue)
{
	if (LanderBody)
	{
		LanderBody->AddForce(FVector(ThrustPower * AxisValue, 0.0f, 0.0f));
	}
}

void APlayerLander::ThrustUpward(float AxisValue)
{
	if (LanderBody)
	{
		LanderBody->AddForce(FVector(0.0f, 0.0f, ThrustPower * AxisValue));
	}
}

void APlayerLander::ActivateGrapple(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if we collided with a valid object
	if (OtherActor && OtherActor->ActorHasTag("PickUp"))
	{
		AttachedObject = OtherActor;
		FVector StartLocation = GrappleSpline->GetComponentLocation();
		FVector EndLocation = OtherActor->GetActorLocation();

		// Update the spline points
		GrappleSpline->ClearSplinePoints();
		GrappleSpline->AddSplinePoint(StartLocation, ESplineCoordinateSpace::World);
		GrappleSpline->AddSplinePoint(EndLocation, ESplineCoordinateSpace::World);
		GrappleSpline->UpdateSpline();
	}
}

void APlayerLander::DeactivateGrapple()
{
	AttachedObject = nullptr;
	GrappleSpline->ClearSplinePoints();
}

void APlayerLander::AdjustPhysicsWithObject(AActor* Object)
{
	if (!Object) return;

	FVector ObjectMassCenter = Object->GetActorLocation();
	FVector LanderMassCenter = LanderBody->GetComponentLocation();
	FVector ForceDirection = (ObjectMassCenter - LanderMassCenter).GetSafeNormal();

	LanderBody->AddForce(ForceDirection * 100.0f); // Adjust force based on object's mass
}

void APlayerLander::HandleActiveGrappleInput()
{
	// Toggle grapple activation when the key is pressed
	if (!bGrappleActive)
	{
		ActivateGrapple(nullptr, nullptr, nullptr, 0, false, FHitResult());
	}
	else
	{
		DeactivateGrapple();
	}
}
