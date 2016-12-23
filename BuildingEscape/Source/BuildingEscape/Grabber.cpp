// POULIN Lancelot Copyright 2016

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}


// Look for attached physics handle
void  UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) // physics handle is not found
	{
		UE_LOG(LogTemp, Error, TEXT("%s : No physics handle found"), *GetOwner()->GetName());
	}
}

// Look for attached input component and setup it
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) // input component is found
	{
		// Bind input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s : No input component found"), *GetOwner()->GetName());
	}
}


// Ray-cast and grab what's in reach
void UGrabber::Grab()
{
	// If we hit something with line trace then attach a physics handle
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	if (HitResult.GetActor())
	{
		// attach physics handle (grab)
		PhysicsHandle->GrabComponent(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.GetComponent()->GetOwner()->GetActorLocation(),
			true
		);
	}
}

// Release the thing who's grabbed (the physics handle)
void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}


// Return the first physics body who is reach by the line tracer
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Draw a red trace in the world to visualise the line tracer  ######### voir vecteur pdf #########
	//DrawDebugLine(GetWorld(), GetReachLineStart(), GetReachLineEnd(), FColor(255, 0, 0), false, 0.0f, 0, 10.0f);

	// Setup query parameters and Ray-cast out to reach distance and see what we hit
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return Hit;
}


// Return the reach of the player viewpoint location vector (line tracer start)
FVector UGrabber::GetReachLineStart()
{
	// Get the player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

// Return the reach of the hand vector (for line tracer & set physics handle position after grab)
FVector UGrabber::GetReachLineEnd()
{
	// Get the player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	// addition vectoriel du V location joueur a son V rotation
	// PlayerViewPointRotation.Vector() est normalisé (rotation --> vecteur) * 100(norme ou longueur) pour avoir une taille
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 100;
}
