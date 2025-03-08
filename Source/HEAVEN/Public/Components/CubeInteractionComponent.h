#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CubeInteractionComponent.generated.h"

// 立方体交互组件（支持蓝图生成和编辑）
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEAVEN_API UCubeInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// 外立方体尺寸（单位：厘米，定义交互区域范围）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dimensions")
	FVector OuterCubeSize = FVector(1000, 1000, 1000);

	// 内立方体尺寸（单位：厘米，用于元素生成基准）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dimensions")
	FVector InnerCubeSize = FVector(100, 100, 100);

	// 在立方体表面生成点击交互元素（SurfaceNormal：表面法线方向）
	UFUNCTION(BlueprintCallable, Category="Element Generation")
	void SpawnSurfaceTapElement(FVector SurfaceNormal);

	// 在立方体边缘生成滑动交互元素（StartEdge/EndEdge：边缘起点和终点）
	UFUNCTION(BlueprintCallable, Category="Element Generation")
	void SpawnEdgeSlideElement(FVector StartEdge, FVector EndEdge);

	// 应用立方体空间变换（Rotation：四元数旋转，ScaleFactor：缩放系数）
	UFUNCTION(BlueprintCallable, Category="Spatial Manipulation")
	void ApplyCubeTransformation(FQuat Rotation, float ScaleFactor);

	// 变换插值曲线（控制动画速度曲线）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spatial Manipulation")
	UCurveFloat* TransformCurve;

	// 插值持续时间（单位：秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spatial Manipulation")
	float InterpolationDuration = 1.0f;

private:
	// 变换插值初始状态
	FTransform StartTransform;
	// 变换插值目标状态
	FTransform TargetTransform;
	// 当前插值进度时间
	float CurrentInterpolationTime;
	// 插值更新计时器句柄
	FTimerHandle InterpolationTimer;

	// 插值变换更新实现（每帧调用）
	void TickTransformInterpolation();
};