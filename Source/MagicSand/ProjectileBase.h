// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "OnHitComponentBase.h"
#include "ProjectileBase.generated.h"

UCLASS()
class MAGICSAND_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	/** Mesh component */
	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* Mesh;

	/** Sphere collision component */
	UPROPERTY(EditAnywhere, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite)
	int32 TeamNumber;

	UPROPERTY(EditDefaultsOnly)
	float RawDamage;

	UPROPERTY(EditDefaultsOnly)
	float DamageMultiplier;

	//Temporary solution
	const float DamageMultiplierCap = 2.5;

	UPROPERTY(BlueprintReadOnly)
	float Lifetime;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 3.0;

	TArray<UOnHitComponentBase*> OnHitComponents;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool DestroyProjectileOnHit;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float CalculateDamage();

	UFUNCTION(BlueprintCallable)
	void SetRawDamage(float Amount);

	UFUNCTION(BlueprintCallable)
	void SetTeamNumber(int32 Team);

	UFUNCTION(BlueprintCallable)
	USphereComponent* GetCollisionComponent();


	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetMovementComponent();

	UFUNCTION(BlueprintCallable)
	void RegisterOnHitComponent(UOnHitComponentBase* OnHitComponent);

	UFUNCTION(BlueprintCallable)
	void PropagateHitToComponents(AActor* InActor, FVector HitLocation, FVector ImpactNormal);

	UFUNCTION(BlueprintCallable)
	void AddToRawDamage(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void AddToDamageMultiplier(float Amount);
};
