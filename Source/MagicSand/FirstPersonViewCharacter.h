// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "HandComponent.h"
#include "DeckComponentBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GunComponent.h"
#include "PlayerModifierComponent.h"
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


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		UGunComponent* WeaponComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
		UPlayerModifierComponent* PlayerModifierComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Cards, meta = (AllowPrivateAccess = "true"))
		UHandComponent* HandComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Cards , meta = (AllowPrivateAccess = "true"))
		UDeckComponentBase* DeckComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY()
	float InitialSpeed;

public:
	AFirstPersonViewCharacter();


protected:
	virtual void BeginPlay() override;

	UFUNCTION(Client, reliable)
	void LocalClientSetUp();

	UFUNCTION()
	void UpdateMovement(UPlayerModifier* Modifier);

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGunComponent> GunComponentBP;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UHandComponent> HandComponentBP;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UDeckComponentBase> DeckComponentBP;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UModifierBase> AbilityModifierOne;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UModifierBase> AbilityModifierTwo;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UModifierBase> AbilityModifierThree;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UModifierBase> AbilityModifierFour;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetUpHUD();

	UFUNCTION(BlueprintCallable)
	void LaunchFromPad(FVector Velocity);

	// Input functions
	UFUNCTION(BlueprintNativeEvent)
	void JumpEvent(const FInputActionValue& Value);
	virtual void JumpEvent_Implementation(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent)
	void Move(const FInputActionValue& Value);
	virtual void Move_Implementation(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent)
	void Look(const FInputActionValue& Value);
	virtual void Look_Implementation(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent)
	void Dash(const FInputActionValue& Value);
	virtual void Dash_Implementation(const FInputActionValue& Value);

	UFUNCTION()
	void Fire(const FInputActionValue& Value);

	UFUNCTION()
	void Reload(const FInputActionValue& Value);

	UFUNCTION()
	void SwapWeapon(const FInputActionValue& Value);

	UFUNCTION()
	void UseAbilityOne(const FInputActionValue& Value);

	UFUNCTION()
	void UseAbilityTwo(const FInputActionValue& Value);

	UFUNCTION()
	void UseAbilityThree(const FInputActionValue& Value);

	UFUNCTION()
	void UseAbilityFour(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	UGunComponent* GetWeaponComponenet();

	UFUNCTION(BlueprintCallable)
	UPlayerModifierComponent* GetPlayerModifierComponent();

	UFUNCTION(BlueprintCallable)
	UHandComponent* GetHandComponenet();

	UFUNCTION(BlueprintCallable)
	UDeckComponentBase* GetDeckComponenet();
};
