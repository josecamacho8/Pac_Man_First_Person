// Fill out your copyright notice in the Description page of Project Settings.

#include "ScorePickup.h"
#include "PlayerCharacter.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h "
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AScorePickup::AScorePickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	/*
	// Setting up mesh for Blueprint editing. Set as RootComponent.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("PointPickupMesh");
	ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshScore_obj(TEXT("/Game/StarterContent/Props/MaterialSphere.MaterialSphere"));
	MeshComponent->SetStaticMesh(StaticMeshScore_obj.Object);
	ConstructorHelpers::FObjectFinder<UMaterial> MaterialScore_obj(TEXT("/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor"));
	MeshComponent->SetMaterial(0, MaterialScore_obj.Object);
	RootComponent = MeshComponent;

	// Setting default collision preset
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	*/
	// Binding overlap function
	OnActorBeginOverlap.AddDynamic(this, &AScorePickup::OnOverlap);

}

// Called when the game starts or when spawned
void AScorePickup::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AScorePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScorePickup::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<APlayerCharacter>(OtherActor) != nullptr)
	{
		Destroy();
	}
}