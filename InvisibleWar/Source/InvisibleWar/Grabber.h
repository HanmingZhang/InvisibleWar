// Copyright Hanming Zhang 2016

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVISIBLEWAR_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Setup (assumed) attached input component
	void SetupInputComponent();

	// Find attached physics handle component
	void FindPhysicsHandleComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

private:
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// It's Ray-cast and grab what's it reach
	void Grab();
	void Release();

	// Return the current end of reach line
	FVector GetLineTraceEnd();

	// Return the current start of reach line
	FVector GetLineTraceStart();

	// Return hit for first physics body in reach
	FHitResult GetFirstPhysicsBodyInReach();

};
