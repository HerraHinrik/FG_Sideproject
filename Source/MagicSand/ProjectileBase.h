// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectileBase.generated.h"

UCLASS()
class MAGICSAND_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	/** Sphere collision component */
	UPROPERTY(EditAnywhere, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
	float RawDamage;

	UPROPERTY()
	float DamageMultiplier;


	const float DamageMultiplierCap = 2.5;


	UPROPERTY()
	float Lifetime;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 3.0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float CalculateDamage();

	UFUNCTION(BlueprintCallable)
	void SetRawDamage(float Amount);

	UFUNCTION(BlueprintCallable)
	void ApplyDamageModifier(float Amount);


	UFUNCTION(BlueprintCallable)
	USphereComponent* GetCollisionComponent();


	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetMovementComponent();

};
