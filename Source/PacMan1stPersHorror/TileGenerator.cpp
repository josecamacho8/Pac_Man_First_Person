// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "TileGenerator.h"

// Sets default values
ATileGenerator::ATileGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setting defaults
	FVector Location = GetActorLocation();
	TileCoordinatesOriginX = Location.X;
	TileCoordinatesOriginY = Location.Y;
	TileCoordinatesOriginZ = Location.Z;
	TileCountX = 5;
	TileCountY = 6; 
	TileWidth = 10;
	TileHeight = 11;
	TotalTiles = TileCountX * TileCountY;

	// Initialize TileArr
	TileArr = TArray<ATile*>();
}

// Called when the game starts or when spawned
void ATileGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	// Call generator function
	GenerateTilesForLevel();
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileGenerator::GenerateTilesForLevel()
{
	UWorld* World;
	ATile* NewTile;
	FVector TileLocation = FVector();
	FRotator TileRotation = GetActorRotation(); 

	World = GetWorld();
	if (World != nullptr)
	{
		// Constant along z-axis
		TileLocation.Z = TileCoordinatesOriginZ;
		// Use 2D array to create tile system based off defined x, y, counts, and width heights
		for (int CreateTilesY = 0; CreateTilesY < TileCountY; CreateTilesY++)
		{
			TileLocation.Y = TileCoordinatesOriginY + (CreateTilesY * TileHeight);
			for (int CreateTilesX = 0; CreateTilesX < TileCountX; CreateTilesX++)
			{
				TileLocation.X = TileCoordinatesOriginX + (CreateTilesX * TileWidth);
				NewTile = World->SpawnActor<ATile>(TileLocation, TileRotation);
				NewTile->InitBounds(TileWidth, TileHeight);
				if (NewTile != nullptr) 
				{
					TileArr.Add(NewTile);
				}
				else
				{
					UE_LOG(LogTemp, Fatal, TEXT("%s: Tile at location (%u, %u, %u) not able to be spawned."), 
						__FUNCTION__, TileLocation.X, TileLocation.Y, TileLocation.Z);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("%s: World not found. Can't create tiles."), __FUNCTION__);
	}

}

int ATileGenerator::GetTileFromCoordinates(int CoordinateX, int CoordinateY)
{
	int		TileIndex;
	int		TileCoordinatesBoundX = TileCoordinatesOriginX + (TileCountX * TileWidth);
	int		TileCoordinatesBoundY = TileCoordinatesOriginY + (TileCountY * TileHeight);
	

	// Determine out of bounds
	if ((CoordinateX < TileCoordinatesOriginX) ||
		(CoordinateY < TileCoordinatesOriginY) ||
		(CoordinateX > TileCoordinatesBoundX) ||
		(CoordinateY > TileCoordinatesBoundY))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: TileOutOfBounds."), __FUNCTION__);
		return TotalTiles;
	}

	// Calculate index
	TileIndex = (CoordinateX / TileCoordinatesBoundX) +
		((CoordinateY / TileCoordinatesBoundY) * TileWidth);
	return TileIndex;

}
