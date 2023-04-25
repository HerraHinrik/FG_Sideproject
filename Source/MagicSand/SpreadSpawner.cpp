// Fill out your copyright notice in the Description page of Project Settings.


#include "SpreadSpawner.h"


FVector USpreadSpawner::GetDirectionFromCone()
{
	// Spread in the YZ plane in a unit circle
	auto AngleTarget = FMath::RandPointInCircle(.5);

	// Use half angle to find distance to unit circle
	float Distance = .5 / FMath::Tan(HalfAngle);

	// Normalized direction gives us a unit vector pointing in within HalfAngle
	FVector Direction = FVector(Distance, AngleTarget.X, AngleTarget.Y).GetSafeNormal();

	return Direction;
}

TArray<AActor*> USpreadSpawner::SpawnProjectiles(FVector Location, FRotator Rotation)
{
	TArray<AActor*> Projectiles = TArray<AActor*>();
	for (int i = 0; i < BulletAmount; i++)
	{
		FVector RandDirection = GetDirectionFromCone();
		FVector TrueDirection = Rotation.RotateVector(RandDirection);
		AActor* Proj = SpawnSingleProjectile(Location, TrueDirection.ToOrientationRotator());
		Projectiles.Add(Proj);
	}
	
	return Projectiles;
}