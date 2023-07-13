// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GunComponent.h"
#include "FirstPersonViewCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS()
class MAGICSAND_API AFirstPersonViewCharacter : public ACharacter
{
	GENERATED_BODY()


	UPROPERTY(Replicated, EditDefaultsOnly)
		UGunComponent* WeaponComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SwapAction;

	/** Ability inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AbilityActionOne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AbilityActionTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AbilityActionThree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* AbilityActionFour;

public:
	AFirstPersonViewCharacter();


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGunComponent> GunComponentBP;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UModifierBase> AbilityModifierOne;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UModifierBase> AbilityModifierTwo;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UModifierBase> AbilityModifierThree;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UModifierBase> AbilityModifierFour;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void LaunchFromPad(FVector Velocity);

	// Input functions
	UFUNCTION(BlueprintNativeEvent)
	void Move(const FInputActionValue& Value);
	virtual void Move_Implementation(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent)
	void Look(const FInputActionValue& Value);
	virtual void Look_Implementation(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent)
	void Dash(const FInputActionValue& Value);
	virtual void Dash_Implementation(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);

	void Reload(const FInputActionValue& Value);

	void SwapWeapon(const FInputActionValue& Value);

	void UseAbilityOne(const FInputActionValue& Value);

	void UseAbilityTwo(const FInputActionValue& Value);

	void UseAbilityThree(const FInputActionValue& Value);

	void UseAbilityFour(const FInputActionValue& Value);
};
