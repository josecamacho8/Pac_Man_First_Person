// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGenerator.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h "
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

// Sets default values
ATileGenerator::ATileGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set default values
	TileCountX = TileCountY = 5;
	TileWidth = TileHeight = 400;

	// Set x, y upper bounds
	FVector	TileLocation = GetActorLocation();
	TileXLowerBound = TileLocation.X;
	TileXUpperBound = TileLocation.X + (TileCountX * TileWidth);
	TileYLowerBound = TileLocation.Y;
	TileYUpperBound = TileLocation.Y + (TileCountY * TileHeight);
	
	UE_LOG(LogTemp, Warning, TEXT("Creating TileGenerator"));

	// Used as root for all 
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("DummyRootScene"));
	RootScene->SetRelativeLocationAndRotation(TileLocation, GetActorRotation());
	RootScene->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName(TEXT("Dummy Parent"), 0));

	// Call generator function
	GenerateTilesForLevel();


}

// Called when the game starts or when spawned
void ATileGenerator::BeginPlay()
{
	Super::BeginPlay();


}


// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileGenerator::GenerateTilesForLevel()
{
	float TileXOffset, TileYOffset, TileZOffset = 0;
	int TilesCreated = 0;
	

	UE_LOG(LogTemp, Warning, TEXT("%s: Entered."), __FUNCTION__);


	
	// Use 2D array to create tile system based off defined x, y, counts, and width heights
	for (int CreateTilesY = 0; CreateTilesY < TileCountY; CreateTilesY++)
	{
		TileYOffset = CreateTilesY * TileHeight;
		for (int CreateTilesX = 0; CreateTilesX < TileCountX; CreateTilesX++)
		{
			TileXOffset = CreateTilesX * TileWidth;
			TilesCreated++;
			CreateTileMesh(TileXOffset, TileYOffset, TileZOffset, TilesCreated);
		}
	}
}

void ATileGenerator::CreateTileMesh(float RelativeX, float RelativeY, float RelativeZ, int TileN)
{
	UStaticMeshComponent* SuperMesh;
	FVector	 TileLocation = FVector(RelativeX, RelativeY, RelativeZ);
	FRotator TileRotation = GetActorRotation();

	UE_LOG(LogTemp, Warning, TEXT("%s: Creating Tile%u: x-%u, y-%u, z-%u."),
		   __FUNCTION__, TileN, RelativeX, RelativeY, RelativeZ);

	// Creating UStaticMeshComponent
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Tile"), TileN));
	if (SuperMesh == nullptr)
	{
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_obj(TEXT("/Game/StarterContent/Architecture/Floor_400x400.Floor_400x400"));
	SuperMesh->SetStaticMesh(StaticMesh_obj.Object);
	SuperMesh->SetRelativeLocationAndRotation(TileLocation, TileRotation);
	
	//Attaching material
	ConstructorHelpers::FObjectFinder<UMaterial> Material_obj(TEXT("/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut"));
	SuperMesh->SetMaterial(0, Material_obj.Object);
	SuperMesh->AttachToComponent(RootScene, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
								 FName(TEXT("Tile"), TileN));

	// Adding to TArray
	TileArr.Add(SuperMesh);
}

int ATileGenerator::GetTileFromCoordinates(int X, int Y)
{
	int	TileIndex;

	// Determine out of bounds
	if ((X < TileXLowerBound) ||
		(Y < TileYLowerBound) ||
		(X > TileXUpperBound) ||
		(Y > TileYUpperBound))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: TileOutOfBounds."), __FUNCTION__);
		return -1;
	}

	// Calculate index
	TileIndex = (X / TileXUpperBound) + ((Y / TileYUpperBound) * TileWidth);
	return TileIndex;

}
