// Fill out your copyright notice in the Description page of Project Settings.


#include "SpreadSpawner.h"
#include "Kismet/KismetMathLibrary.h"


FVector USpreadSpawner::GetDirectionFromCone()
{
	// Spread in the YZ plane in a unit circle
	auto AngleTarget = FMath::RandPointInCircle(1);

	
	// Use half angle to find distance to unit circle
	float Distance = FMath::Tan(HalfAngle);

	// Normalized direction gives us a unit vector pointing in within HalfAngle
	FVector Direction = FVector(Distance, AngleTarget.X, AngleTarget.Y );
	Direction.Normalize();
	UE_LOG(LogTemp, Warning, TEXT("Direction is: %s"), *Direction.ToCompactString())

	return Direction;
}

TArray<AActor*> USpreadSpawner::SpawnProjectiles_Implementation(FVector Location, FRotator Rotation)
{
	TArray<AActor*> Projectiles = TArray<AActor*>();
	for (int i = 0; i < BulletAmount; i++)
	{
		FVector RandDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Rotation.Vector(), HalfAngle);
		FQuat RotatedDirection = RandDirection.ToOrientationQuat() * Rotation.Quaternion();

		FVector AdjustedLoaction = Location + Rotation.RotateVector(MuzzleOffset);
		AActor* Proj = SpawnSingleProjectile(AdjustedLoaction, RandDirection.ToOrientationRotator());
		Projectiles.Add(Proj);
	}
	
	return Projectiles;
}