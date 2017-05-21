// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetPositions();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Open();
	void Close();

		
private:
	float GetTriggerWeight();

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	UPROPERTY(EditAnywhere)
	float CloseDelay = 0.f;
	float LastDoorOpenTime = 0.f;
	FRotator ClosedPosition;
	FRotator OpenPosition;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* Trigger;
	UPROPERTY(EditAnywhere)
	float TriggerWeightMinimum = 0.f;
};
