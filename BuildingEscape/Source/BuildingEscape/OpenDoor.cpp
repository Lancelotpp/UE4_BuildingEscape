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
	if (!PressurePlate) // pressure plate is not found
	{
		UE_LOG(LogTemp, Error, TEXT("%s : Missing pressure plate"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll Trigger Volume until mass on volume is > than ...
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast(); // Change actor rotation : open door by BP class
	}
	else
	{
		OnClose.Broadcast(); // Change actor rotation : close door by BP class
	}
}

// Return the mass of actors that's on the plate
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	if (!PressurePlate) { return TotalMass; }
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