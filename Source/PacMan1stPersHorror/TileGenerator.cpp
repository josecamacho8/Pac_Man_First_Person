// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGenerator.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h "
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATileGenerator::ATileGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set default values
	TileCountX = 28, TileCountY = 31;
	TotalTiles = TileCountX * TileCountY;
	TileWidth = TileHeight = 300;

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
	RootScene->SetupAttachment(RootComponent, FName(TEXT("Dummy Parent"), 0));


	// Call generator function
	GenerateTilesForLevel();
}

// Called when the game starts or when spawned
void ATileGenerator::BeginPlay()
{
	Super::BeginPlay();

	ThisWorld = GetWorld();
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
	int walls [] =
	{
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
		2, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 2, 2, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 2,
		2, 4, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 2, 2, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 4, 2,
		2, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 2, 2, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 2,
		2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
		2, 3, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 3, 2,
		2, 3, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 3, 2,
		2, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 2,
		2, 2, 2, 2, 2, 2, 3, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 3, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 3, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 3, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 3, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 3, 2, 2, 2, 2, 2, 2,

		2, 2, 2, 2, 2, 2, 3, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 3, 2, 2, 2, 2, 2, 2,

		2, 2, 2, 2, 2, 2, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 3, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 3, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 3, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 3, 2, 2, 2, 2, 2, 2,
		2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
		2, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 2,
		2, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 2,
		2, 4, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 4, 2,
		2, 2, 2, 3, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 3, 2, 2, 2,
		2, 2, 2, 3, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 3, 2, 2, 2,
		2, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 2,
		2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 2,
		2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 2,
		2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	};

	TileLevelLayout = TArray<int>(walls, TotalTiles);
	
	// Use 2D array to create tile system based off defined x, y, counts, and width heights
	for (int CreateTilesY = 0; CreateTilesY < TileCountY; CreateTilesY++)
	{
		TileYOffset = CreateTilesY * TileHeight;
		for (int CreateTilesX = 0; CreateTilesX < TileCountX; CreateTilesX++)
		{
			TileXOffset = CreateTilesX * TileWidth;
			CreateTile(TileXOffset, TileYOffset, TileZOffset, TilesCreated, walls[TilesCreated]);
			TilesCreated++;
		}
	}
}

void ATileGenerator::CreateTile(float RelativeX, float RelativeY, float RelativeZ, int TileN, int TileTypeInt)
{
	Tile* ATile = new Tile();
	auto Type = TileType(TileTypeInt);
	FVector	 TileLocation = FVector(RelativeX, RelativeY, RelativeZ);
	FVector PickupLocation = TileLocation;
	FRotator TileRotation = GetActorRotation();

	
	// Creating UStaticMeshComponent
	ATile->MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Tile"), TileN));
	ATile->Type = Type;

	if (Type == TileType::GlassWall)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_obj(TEXT("/Game/StarterContent/Architecture/Wall_300x300x300.Wall_300x300x300"));
		ATile->MeshComponent->SetStaticMesh(StaticMesh_obj.Object);
		ATile->MeshComponent->SetRelativeLocationAndRotation(TileLocation, TileRotation);
		ConstructorHelpers::FObjectFinder<UMaterial> Material_obj(TEXT("/Game/StarterContent/Materials/M_Glass.M_Glass"));
		ATile->MeshComponent->SetupAttachment(RootScene, FName(TEXT("Tile"), TileN));
		ATile->MeshComponent->SetStaticMesh(StaticMesh_obj.Object);
		ATile->MeshComponent->SetMaterial(0, Material_obj.Object);
	}
	else if (Type == TileType::ExternalWall)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_obj(TEXT("/Game/StarterContent/Architecture/Wall_300x300x300.Wall_300x300x300"));
		ATile->MeshComponent->SetStaticMesh(StaticMesh_obj.Object);
		ATile->MeshComponent->SetRelativeLocationAndRotation(TileLocation, TileRotation);
		ConstructorHelpers::FObjectFinder<UMaterial> Material_obj(TEXT("/Game/StarterContent/Materials/M_Tech_Panel.M_Tech_Panel"));
		ATile->MeshComponent->SetupAttachment(RootScene, FName(TEXT("Tile"), TileN));
		ATile->MeshComponent->SetStaticMesh(StaticMesh_obj.Object);
		ATile->MeshComponent->SetMaterial(0, Material_obj.Object);
	}
	else
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh_obj(TEXT("/Game/StarterContent/Architecture/Floor_300x300.Floor_300x300"));
		ATile->MeshComponent->SetStaticMesh(StaticMesh_obj.Object);
		ATile->MeshComponent->SetRelativeLocationAndRotation(TileLocation, TileRotation);
		ConstructorHelpers::FObjectFinder<UMaterial> Material_obj(TEXT("/Game/StarterContent/Materials/M_Tech_Panel.M_Tech_Panel"));
		ATile->MeshComponent->SetupAttachment(RootScene, FName(TEXT("Tile"), TileN));
		ATile->MeshComponent->SetStaticMesh(StaticMesh_obj.Object);
		ATile->MeshComponent->SetMaterial(0, Material_obj.Object);
	}

	// Adding to TArray
	TileArr.Add(ATile);
}

int ATileGenerator::GetTileFromCoordinates(FVector Location)
{
	int	TileIndex;
	int X = Location.X, Y = Location.Y;

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
	TileIndex = X / (TileWidth);
	TileIndex += FMath::DivideAndRoundDown<int>(Y,  TileHeight) * TileCountX;
	return TileIndex;

}

FVector ATileGenerator::GetCoordinatesFromTile(int TileIndex)
{
	FVector Location;
	Location.Z = GetActorLocation().Z;

	// Determine out of bounds
	if ((TileIndex < 0) ||
		(TileIndex > TileArr.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: TileOutOfBounds."), __FUNCTION__);
		return FVector();
	}

	// Calculate index
	Location.X = ((TileIndex % TileCountX) * TileWidth) + (TileWidth / 2);
	Location.Y = ((TileIndex / TileCountX) * TileHeight) + (TileHeight / 2);
	return Location;

}

bool ATileGenerator::IsWall(Tile* ATile)
{
	if ((ATile->Type == TileType::GlassWall) || (ATile->Type == TileType::ExternalWall))
	{
		return true;
	}
	return false;
}

int ATileGenerator::GetClosestTileToTarget(FVector CurrentLocation, FVector TargetLocation, int PrevTile)
{
	// Variables for indices each cardinal direction from current tile.
	int UpTileIndex, DownTileIndex, LeftTileIndex, RightTileIndex;
	int ClosestTile;
	// Set shortest distance as large number so first distance calculated will always be smaller
	float ShortestDistance = 1000000.0f, Distance;
	int CurrentTile = GetTileFromCoordinates(CurrentLocation);


	// Assign indices based on tile position, if out of bounds set as -1
	UpTileIndex = CurrentTile + TileCountX < TotalTiles ? CurrentTile + TileCountX : -1;
	DownTileIndex = CurrentTile - TileCountX > 0 ? CurrentTile - TileCountX : -1;
	LeftTileIndex = (CurrentTile + 1) % TileCountX < TileCountX ? CurrentTile + 1 : -1;
	RightTileIndex = CurrentTile - 1 > 0 ? CurrentTile - 1 : -1;

	// Up is given highest priority
	// TODO: make custom distance from tiles instead of coordinates of tiles? Seems like same amnt of calc,
	// just 1 func call less.
	if (PrevTile != UpTileIndex && UpTileIndex != -1 && !IsWall(TileArr[UpTileIndex]))
	{
		ClosestTile = UpTileIndex;
		ShortestDistance = FVector::DistXY(TargetLocation, GetCoordinatesFromTile(UpTileIndex));
	}
	if (PrevTile != LeftTileIndex && LeftTileIndex != -1 && !IsWall(TileArr[LeftTileIndex]))
	{
		Distance = FVector::DistXY(TargetLocation, GetCoordinatesFromTile(LeftTileIndex));
		if (Distance < ShortestDistance)
		{
			ShortestDistance = Distance;
			ClosestTile = LeftTileIndex;
		}
	}
	if (PrevTile != DownTileIndex && DownTileIndex != -1 && !IsWall(TileArr[DownTileIndex]))
	{
		Distance = FVector::DistXY(TargetLocation, GetCoordinatesFromTile(DownTileIndex));
		if (Distance < ShortestDistance)
		{
			ShortestDistance = Distance;
			ClosestTile = DownTileIndex;
		}
	}
	if (PrevTile != RightTileIndex && RightTileIndex != -1 && !IsWall(TileArr[RightTileIndex]))
	{
		Distance = FVector::DistXY(TargetLocation, GetCoordinatesFromTile(RightTileIndex));
		if (Distance < ShortestDistance)
		{
			ShortestDistance = Distance;
			ClosestTile = RightTileIndex;
		}
	}

	return ClosestTile;
}

int ATileGenerator::GetOffsetTwoTilesFromPlayer(int PrevPlayerTile, int CurrentPlayerTile)
{
	int TargetTile;

	// Check if tiles are within bounds first
	if ((PrevPlayerTile < 0) ||
		(PrevPlayerTile > TileArr.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: PrevPlayerTile OutOfBounds."), __FUNCTION__);
		return -1;
	}

	// Check if tiles are within bounds first
	if ((CurrentPlayerTile < 0) ||
		(CurrentPlayerTile > TileArr.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: CurrentPlayerTile OutOfBounds."), __FUNCTION__);
		return -1;
	}

	// Up or same direction case
	if ((CurrentPlayerTile == PrevPlayerTile) ||
		(CurrentPlayerTile == PrevPlayerTile + TileCountX))
	{
		for (int i = 0; i < 2; i++)
		{
			// Tile above current player tile
			TargetTile = CurrentPlayerTile + TileCountX;
			
			// If next iteration is out of bounds
			if (TargetTile + TileCountX > TileArr.Num())
			{
				// Return the target tile above.
				return TargetTile;
			}
		}
		return TargetTile;
	}

	// Left direction case
	else if (CurrentPlayerTile == PrevPlayerTile + 1)
	{
		for (int i = 0; i < 2; i++)
		{
			// Tile to left of target
			TargetTile = CurrentPlayerTile + 1;

			// If next iteration is on left most column edge of tile system
			if ((TargetTile + 1) % TileCountX == (TileCountX - 1))
			{
				return TargetTile;
			}
		}
		return TargetTile;
	}

	// Down direction case
	else if (CurrentPlayerTile == PrevPlayerTile - TileCountX)
	{
		for (int i = 0; i < 2; i++)
		{
			// Tile above current player tile
			TargetTile = CurrentPlayerTile - TileCountX;

			// If next iteration is out of bounds
			if (TargetTile + TileCountX < 0)
			{
				// Return the target tile above.
				return TargetTile;
			}
		}
		return TargetTile;
	}

	// Right direction case
	else if (CurrentPlayerTile == PrevPlayerTile - 1)
	{
		for (int i = 0; i < 2; i++)
		{
			// Tile to left of target
			TargetTile = CurrentPlayerTile -1;

			// If next iteration is on left most column edge of tile system
			if ((TargetTile - 1) % TileCountX == 0)
			{
				return TargetTile;
			}
		}
	return TargetTile;
	}

	// No case, error
	UE_LOG(LogTemp, Warning, TEXT("%s: No direction found."), __FUNCTION__);
	return -1;
}

int ATileGenerator::GetPinkyChaseTile(int PrevPlayerTile, int CurrentPlayerTile)
{
	int TargetTile;

	// Check if tiles are within bounds first
	if ((PrevPlayerTile < 0) ||
		(PrevPlayerTile > TileArr.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: PrevPlayerTile OutOfBounds."), __FUNCTION__);
		return -1;
	}

	// Check if tiles are within bounds first
	if ((CurrentPlayerTile < 0) ||
		(CurrentPlayerTile > TileArr.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: CurrentPlayerTile OutOfBounds."), __FUNCTION__);
		return -1;
	}

	// Up or same direction case
	if ((CurrentPlayerTile == PrevPlayerTile) ||
		(CurrentPlayerTile == PrevPlayerTile + TileCountX))
	{
		for (int i = 0; i < 4; i++)
		{
			// Tile above current player tile
			TargetTile = CurrentPlayerTile + TileCountX;

			// If next iteration is out of bounds
			if (TargetTile + TileCountX > TileArr.Num())
			{
				// Return the target tile above.
				return TargetTile;
			}
		}
		return TargetTile;
	}

	// Left direction case
	else if (CurrentPlayerTile == PrevPlayerTile + 1)
	{
		for (int i = 0; i < 4; i++)
		{
			// Tile to left of target
			TargetTile = CurrentPlayerTile + 1;

			// If next iteration is on left most column edge of tile system
			if ((TargetTile + 1) % TileCountX == (TileCountX - 1))
			{
				return TargetTile;
			}
		}
		return TargetTile;
	}

	// Down direction case
	else if (CurrentPlayerTile == PrevPlayerTile - TileCountX)
	{
		for (int i = 0; i < 4; i++)
		{
			// Tile above current player tile
			TargetTile = CurrentPlayerTile - TileCountX;

			// If next iteration is out of bounds
			if (TargetTile - TileCountX < 0)
			{
				// Return the target tile above.
				return TargetTile;
			}
		}
		return TargetTile;
	}

	// Right direction case
	else if (CurrentPlayerTile == PrevPlayerTile - 1)
	{
		for (int i = 0; i < 4; i++)
		{
			// Tile to left of target
			TargetTile = CurrentPlayerTile - 1;

			// If next iteration is on left most column edge of tile system
			if ((TargetTile - 1) % TileCountX == 0)
			{
				return TargetTile;
			}
		}
		return TargetTile;
	}

	// No case, error
	UE_LOG(LogTemp, Warning, TEXT("%s: No direction found."), __FUNCTION__);
	return -1;
}

int ATileGenerator::GetInkyChaseTile(FVector BlinkyCurrentLocation, int PrevPlayerTile, int CurrentPlayerTile)
{
	FVector TargetLocation;
	int     XTile, YTile;
	int     PlayerOffsetTile = GetOffsetTwoTilesFromPlayer(PrevPlayerTile, CurrentPlayerTile);
	FVector PlayerOffsetCoordinates = GetCoordinatesFromTile(PlayerOffsetTile);


	// First calculate distance using vectors
	TargetLocation = PlayerOffsetCoordinates + (PlayerOffsetCoordinates - BlinkyCurrentLocation);
	
	// Then, increment from PlayerOffset to Tile to make sure not going out of bounds
	for (int x = PlayerOffsetCoordinates.X; x <= TargetLocation.X; x += (TileWidth / 2))
	{
		XTile = GetTileFromCoordinates(FVector(x, PlayerOffsetCoordinates.Y, 0));
		TargetLocation.X = x;
		if (((XTile - 1) % TileCountX == 0) || ((XTile + 1) % TileCountX == (TileCountX - 1)))
		{
			break;
		}
	}
	for (int y = PlayerOffsetCoordinates.Y; y <= TargetLocation.Y; y += (TileHeight / 2))
	{
		YTile = GetTileFromCoordinates(FVector(TargetLocation.X, y, 0));
		TargetLocation.Y = y;
		if ((YTile + TileCountX > TileArr.Num()) || (YTile - TileCountX < 0))
		{
			break;
		}
	}
	
	return GetTileFromCoordinates(TargetLocation);
}

bool ATileGenerator::IsClydeWithinEightTiles(FVector ClydeLocation, FVector PlayerLocation)
{
	float DistanceBetweenClydeAndPlayer = FVector::Dist2D(ClydeLocation, PlayerLocation);

	if (DistanceBetweenClydeAndPlayer < (8 * TileWidth))
	{
		return true;
	}

	return false;
}
