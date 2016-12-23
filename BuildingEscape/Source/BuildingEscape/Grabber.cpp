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

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting"));

	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Get the player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	/*UE_LOG(LogTemp, Warning, TEXT("Grabber -> Location: %s | Rotation: %s"),
		*PlayerViewPointLocation.ToString(), 
		*PlayerViewPointRotation.ToString()
	);*/

	// addition vectoriel
	// PlayerViewPointRotation.Vector() est normalisé (rotation --> vecteur) * 100(norme) pour avoir une taille
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 100;
	UE_LOG(LogTemp, Warning, TEXT("Grabber -> Location: %s | Rotation: %s | %s"),
		*PlayerViewPointLocation.ToString(),
		*PlayerViewPointRotation.Vector().ToString(),
		*((PlayerViewPointRotation.Vector() * 100) + PlayerViewPointLocation).ToString()
	);
	// Draw a red trace in the world to visualise
	// ######### voir vecteur pdf #########
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0,
		10.0f
	);


	// Ray-cast out to reach distance

	// See what we hit
}

