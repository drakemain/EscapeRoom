// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoom.h"
#include "Grabber.h"


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
	this->Init();
}

void UGrabber::Init()
{
	this->GetPhysicsHandle();

	this->GetInputComponent();
}

void UGrabber::GetInputComponent()
{
	this->Input = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!this->Input)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find Input for %s"), *GetOwner()->GetName())
	}
	else {
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::GetPhysicsHandle()
{
	this->PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!this->PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find Physics Handle for %s"), *GetOwner()->GetName())
	}
}




// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveGrabbedObject();
}

void UGrabber::MoveGrabbedObject()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(this->GetReachVector());
	}
}

void UGrabber::UpdateReach()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(this->ViewPortLocation, this->ViewPortRotation);
}

FVector UGrabber::GetReachVector()
{
	this->UpdateReach();
	return this->ViewPortRotation.Vector() * this->ReachDistance + this->ViewPortLocation;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber grab!"))
	const FHitResult Hit = this->GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();

	if (Hit.GetActor())
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
	
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber release!"))

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(this->ViewPortLocation, this->ViewPortRotation);

	DrawDebugLine(GetWorld(), this->ViewPortLocation, this->GetReachVector(), FColor(255, 0, 0), true, 3.f, 0, 1.f);

	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(Hit, this->ViewPortLocation, this->GetReachVector(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	if (Hit.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("GRABBER HIT: %s"), *Hit.GetActor()->GetName())
	}

	return Hit;
}