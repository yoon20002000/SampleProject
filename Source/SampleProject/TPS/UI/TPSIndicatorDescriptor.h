// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSIndicatorDescriptor.generated.h"
class UTPSIndicatorDescriptor;
class UTPSIndicatorManagerComponent;
class STPSActorCanvas;

struct FIndicatorProjection
{
	bool Project(const UTPSIndicatorDescriptor& IndicatorDescriptor, const FSceneViewProjectionData& InProjectionData, const FVector2f& ScreenSize, FVector& OutScreenPositionWithDepth);
};

UENUM(BlueprintType)
enum class EActorCanvasProjectionMode : uint8
{
	ComponentPoint,
	ComponentBoundingBox,
	ComponentScreenBoundingBox,
	ActorBoundingBox,
	ActorScreenBoundingBox
};
/**
 * 
 */
UCLASS(BlueprintType)
class SAMPLEPROJECT_API UTPSIndicatorDescriptor : public UObject
{
	GENERATED_BODY()
public:
	UTPSIndicatorDescriptor();
	
	UFUNCTION(BlueprintCallable)
	UObject* GetDataObject() const;
	UFUNCTION(BlueprintCallable)
	void SetDataObject(UObject* InDataObject);

	UFUNCTION(BlueprintCallable)
	USceneComponent* GetTargetSceneComponent()const;
	UFUNCTION(BlueprintCallable)
	void SetTargetSceneComponent(USceneComponent* InSceneComponent);

	UFUNCTION(BlueprintCallable)
	FName GetComponentSocketName() const;
	UFUNCTION(BlueprintCallable)
	void SetComponentSocketName(FName InSocketName);

	UFUNCTION(BlueprintCallable)
	TSoftClassPtr<UUserWidget> GetIndicatorWidgetClass() const;
	UFUNCTION(BlueprintCallable)
	void SetIndicatorClass(TSoftClassPtr<UUserWidget> InIndicatorClass);

	UTPSIndicatorManagerComponent* GetIndicatorManagerComponent() const;
	void SetIndicatorManagerComponent(UTPSIndicatorManagerComponent* InManager);

	UFUNCTION(BlueprintCallable)
	void UnregisterIndicator();
	
	// Layout
	UFUNCTION(BlueprintCallable)
	bool GetAutoRemoveWhenIndicatorComponentIsNull() const;
	UFUNCTION(BlueprintCallable)
	void SetAutoRemoveWhenIndicatorComponentIsNull(bool CanAutomaticallyRemove);
	/// Target에 해당하는 Actor의 Component가 valid 하고 자동 초기화 옵션이 켜져있을 경우 return true
	UFUNCTION(BlueprintCallable)
	bool CanAutomaticallyRemove() const;

	
	UFUNCTION(BlueprintCallable)
	bool GetIsVisible()const ;
	UFUNCTION(BlueprintCallable)
	void SetDesiredVisibility(bool InVisible);

	UFUNCTION(BlueprintCallable)
	EActorCanvasProjectionMode GetProjectionMode()const;
	UFUNCTION(BlueprintCallable)
	void SetProjectionMode(EActorCanvasProjectionMode InProjectionMode);

	UFUNCTION(BlueprintCallable)
	EHorizontalAlignment GetHorizontalAlign()const;
	UFUNCTION(BlueprintCallable)
	void SetHorizontalAlign(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable)
	EVerticalAlignment GetVerticalAlign()const;
	UFUNCTION(BlueprintCallable)
	void SetVerticalAlign(EVerticalAlignment InVerticalAlignment);

	UFUNCTION(BlueprintCallable)
	bool GetClampToScreen() const;
	UFUNCTION(BlueprintCallable)
	void SetClampToScreen(bool InClampToScreen);

	UFUNCTION(BlueprintCallable)
	bool GetShowClampToScreenArrow() const;
	UFUNCTION(BlueprintCallable)
	void SetShowClampToScreenArrow(bool InShowClampToScreenArrow);

	UFUNCTION(BlueprintCallable)
	FVector GetWorldPositionOffset() const;
	UFUNCTION(BlueprintCallable)
	void SetWorldPositionOffset(FVector WorldPosOffset);

	UFUNCTION(BlueprintCallable)
	FVector2D GetScreenSpaceOffset() const;
	UFUNCTION(BlueprintCallable)
	void SetScreenSpaceOffset(FVector2D InScreenSpaceOffset);

	UFUNCTION(BlueprintCallable)
	FVector GetBoundingBoxAnchor() const;
	UFUNCTION(BlueprintCallable)
	void SetBoundingBoxAnchor(FVector InBoundingBoxAnchor);

	// Sorting
	UFUNCTION(BlueprintCallable)
	int32 GetPriority();
	UFUNCTION(BlueprintCallable)
	void SetPriority(int32 InPriority);
private:
	friend STPSActorCanvas;
	
	UPROPERTY()
	TObjectPtr<UObject> DataObject;
	UPROPERTY()
	TObjectPtr<USceneComponent> TargetSceneComponent;
	UPROPERTY()
	FName ComponentSocketName = NAME_None;
	UPROPERTY()
	TSoftClassPtr<UUserWidget> IndicatorWidgetClass;
	UPROPERTY()
	TWeakObjectPtr<UTPSIndicatorManagerComponent> IndicatorManagerComp;
	UPROPERTY()
	TWeakObjectPtr<UUserWidget> IndicatorWidget;

	TWeakPtr<SWidget> Content;
	TWeakPtr<SWidget> CanvasHost;


	UPROPERTY()
	bool bVisible = true;
	UPROPERTY()
	bool bClampToScreen = false;
	UPROPERTY()
	bool bShowClampToScreenArrow = false;
	UPROPERTY()
	bool bAutoRemoveWhenIndicatorComponentIsNull;

	
	UPROPERTY()
	EActorCanvasProjectionMode ProjectionMode = EActorCanvasProjectionMode::ComponentPoint;
	UPROPERTY()
	TEnumAsByte<EHorizontalAlignment> HAlignment = HAlign_Center;
	UPROPERTY()
	TEnumAsByte<EVerticalAlignment> VAlignment = VAlign_Center;

	UPROPERTY()
	int32 Priority;
	
	UPROPERTY()
	FVector BoundingBoxAnchor = FVector::ZeroVector;
	UPROPERTY()
	FVector2D ScreenSpaceOffset = FVector2D::ZeroVector;
	UPROPERTY()
	FVector WorldPositionOffset = FVector(0,0,0);
};
