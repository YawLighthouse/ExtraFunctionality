#include "ExtraDataTypes.h"

FExtraSplineConstructionInfo::FExtraSplineConstructionInfo()
{
	SplineMesh = nullptr;

	SplineTileLength = 200.0f;
	StartScale = FVector2D(1.0f, 1.0f);
	EndScale = FVector2D(1.0f, 1.0f);
	ForwardAxis = ESplineMeshAxis::X;

	bAffectNavigation = false;
	bGenerateOverlapEvents = false;
	CollisionEnabled = ECollisionEnabled::NoCollision;
	ObjectType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);	
	Mobility = EComponentMobility::Movable;

	bDebugMode = false;
	ArrowLength = 100.0f;
	ArrowSize = 50.0f;
	ArrowThickness = 2.5f;
	DebugTime = 5.0f;
}
