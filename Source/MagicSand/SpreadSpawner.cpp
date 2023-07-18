// By Amos Johan Persson


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

TArray<AProjectileBase*> USpreadSpawner::SpawnProjectiles_Implementation(FVector Location, FRotator Rotation)
{
	TArray<AProjectileBase*> Projectiles = TArray<AProjectileBase*>();
	for (uint32 i = 0; i < BulletAmount; i++)
	{
		FVector RandDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Rotation.Vector(), HalfAngle);
		FQuat RotatedDirection = RandDirection.ToOrientationQuat() * Rotation.Quaternion();

		FVector AdjustedLoaction = Location + Rotation.RotateVector(MuzzleOffset);
		AProjectileBase* Proj = SpawnSingleProjectile(AdjustedLoaction, RandDirection.ToOrientationRotator());
		Projectiles.Add(Proj);
	}
	
	return Projectiles;
}