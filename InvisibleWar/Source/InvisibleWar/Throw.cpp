// Fill out your copyright notice in the Description page of Project Settings.

#include "Throw.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UThrow::UThrow()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UThrow::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();

	throwScaleInitial = 20000.0f;
	throwScaleAddSpeed = 100.0f;
	throwScaleMax = 200000.f;
	throwScale = throwScaleInitial;
	isThrowButtonPressed = false;
}

// Setup (assumed) attached input component
void UThrow::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		// Input component is found
		UE_LOG(LogTemp, Warning, TEXT("Input component found!"));
		// Bind  inpuit
		InputComponent->BindAction("Throw", IE_Pressed, this, &UThrow::AddThrowScale);
		InputComponent->BindAction("Throw", IE_Released, this, &UThrow::Throw);

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}

// Find attached physics handle component
void UThrow::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UThrow::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (isThrowButtonPressed && throwScale < throwScaleMax) {
		throwScale += throwScaleAddSpeed;
	}
}

void UThrow::Throw() {
	

	if (!PhysicsHandle) { return; }

	float scale = throwScale;

	UE_LOG(LogTemp, Warning, TEXT("Throw scale is %f!"), scale);

	// reset throw scale stuff
	throwScale = throwScaleInitial;
	isThrowButtonPressed = false;

	// if there is something handles
	if (PhysicsHandle->GrabbedComponent) {
		auto GrabbedComponent = PhysicsHandle->GetGrabbedComponent();

		PhysicsHandle->ReleaseComponent();


		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotation
		);

		FVector impluseDirVec = PlayerViewPointRotation.Vector();


		FVector throwImpulse = scale * impluseDirVec;

		GrabbedComponent->AddImpulse(throwImpulse,
									 NAME_None, // no bones 
									 false);
	
	}
}

void UThrow::AddThrowScale() {

	isThrowButtonPressed = true;

}