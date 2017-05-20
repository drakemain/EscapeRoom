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

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerLocation;
		FRotator PlayerRotation;
		FVector GrabberReach;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
		GrabberReach = PlayerRotation.Vector() * this->ReachDistance + PlayerLocation;
		PhysicsHandle->SetTargetLocation(GrabberReach);
	}
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
	FVector PlayerLocation;
	FRotator PlayerRotation;
	FVector GrabberReach;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	GrabberReach = PlayerRotation.Vector() * this->ReachDistance + PlayerLocation;

	DrawDebugLine(GetWorld(), PlayerLocation, GrabberReach, FColor(255, 0, 0), false, 0.f, 0, 1.f);

	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(Hit, PlayerLocation, GrabberReach, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	if (Hit.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("GRABBER HIT: %s"), *Hit.GetActor()->GetName())
	}

	return Hit;
}