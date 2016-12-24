// POULIN Lancelot Copyright 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	// get the owner (which door)
	Owner = GetOwner();
}

// Change actor rotation : open door
void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

// Change actor rotation : close door
void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll Trigger Volume until mass on volume is > than ...
	if (GetTotalMassOfActorsOnPlate() > 50.0f) // TODO make the mass nb into parms
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	// check if it's time to close door
	if (LastDoorOpenTime + DoorCloseDelay < GetWorld()->GetTimeSeconds())
	{
		CloseDoor();
	}
}

// Return the mass of actors that's on the plate
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	// Find all overlapping actor on plate
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// Iterate through them adding their masses
	for (auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->GetRootPrimitiveComponent()->GetMass(); // Mass is associated to the primitive component of actor
	}
	return TotalMass;
}