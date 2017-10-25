// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Throw.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVISIBLEWAR_API UThrow : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UThrow();

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
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
		
	float throwScale;
	float throwScaleAddSpeed;
	float throwScaleMax;
	float throwScaleInitial;
	bool isThrowButtonPressed;

	void Throw();
	void AddThrowScale();
};
