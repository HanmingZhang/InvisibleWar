// Copyright Hanming Zhang 2016
#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

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
	//SetupInputComponent();
}

// Look for attached Input Component
//void UGrabber::SetupInputComponent()
//{
//	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
//	if (InputComponent) {
//		// Input component is found
//		UE_LOG(LogTemp, Warning, TEXT("Input component found!"));
//		/// Bind the inpuit axis
//		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
//		//InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
//		InputComponent->BindAction("Drop", IE_Pressed, this, &UGrabber::Release);
//
//	}
//	else {
//		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
//	}
//}

// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetHoldPos());
	}
}

void UGrabber::Grab()
{
	// UE_LOG(LogTemp, Warning, TEXT("Grab() is called"));
	
	// if there is already something grabbed
	if (PhysicsHandle->GrabbedComponent) {
		return;
	}

	// line trace and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); //gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	// If we hit something that attaches a physics handle
	if (ActorHit) {
		if (!PhysicsHandle) { return; }

		// attach physics handle
		//PhysicsHandle->GrabComponent(
		//	ComponentToGrab, // 
		//	NAME_None, // no bones needed
		//	ComponentToGrab->GetOwner()->GetActorLocation(),
		//	true // allow rotation
		//);

		//UE_LOG(LogTemp, Warning, TEXT("%s is hit."), ActorHit->GetName());

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab, // 
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);

	}
	return;
}

void UGrabber::Release()
{
	// UE_LOG(LogTemp, Warning, TEXT("Release() is called"));
	// release physics handle
	if (!PhysicsHandle) { return; }
	
	// If there is something grabbed in hand
	// release
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->ReleaseComponent();
	}
	return;
}

FVector UGrabber::GetLineTraceEnd()
{	
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + Reach * PlayerViewPointRotation.Vector();
}

FVector UGrabber::GetHoldPos()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + Reach_Hold * PlayerViewPointRotation.Vector();
}

FVector UGrabber::GetLineTraceStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{ 
	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Ray-cast(line-trace) out to reach certain distance
	FHitResult LineTraceHit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		GetLineTraceStart(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		//FCollisionObjectQueryParams(ECollisionChannel::),
		TraceParameters
	);

	return LineTraceHit;
}
