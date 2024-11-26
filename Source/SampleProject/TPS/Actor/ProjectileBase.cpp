// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ProjectileBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

#if WITH_EDITOR
void AProjectileBase::MoveDataToSparseClassDataStruct() const
{
	if (UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
		BPClass == nullptr || BPClass->bIsSparseClassDataSerializable == true)
	{
		return;
	}
	
	Super::MoveDataToSparseClassDataStruct();
	
#if WITH_EDITORONLY_DATA
	FProjectileSparseData* SparseData = GetProjectileSparseData();
	SparseData->ImpactShakeInnerRadius = ImpactShakeInnerRadius_DEPRECATED;
	SparseData->ImpactShakeOuterRadius = ImpactShakeOuterRadius_DEPRECATED;
#endif
}
#endif


