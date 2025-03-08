#include "CubeInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// ... 文件头部包含 ...

// 在立方体表面生成点击交互元素
void UCubeInteractionComponent::SpawnSurfaceTapElement(FVector SurfaceNormal)
{
    // 设置生成参数（强制生成忽略碰撞）
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    // 基于立方体尺寸和表面法线计算生成位置和旋转
    FVector SpawnLocation = InnerCubeSize * 0.5f * SurfaceNormal; // 沿法线方向偏移半个立方体尺寸
    FRotator SpawnRotation = SurfaceNormal.Rotation(); // 使元素朝向法线方向
    
    // 生成新的交互元素
    AActor* NewElement = GetWorld()->SpawnActor<AActor>(ElementClass, SpawnLocation, SpawnRotation, SpawnParams);
    
    if(NewElement)
    {
        // 设置生成元素的初始缩放
        UStaticMeshComponent* MeshComp = NewElement->FindComponentByClass<UStaticMeshComponent>();
        if(MeshComp)
        {
            MeshComp->SetWorldScale3D(FVector(0.1f)); // 初始缩放为10%
        }
    }
}

// 应用立方体变换（旋转 + 缩放）
void UCubeInteractionComponent::ApplyCubeTransformation(FQuat Rotation, float ScaleFactor)
{
    // 记录初始变换并计算目标变换
    StartTransform = GetOwner()->GetActorTransform();
    TargetTransform = StartTransform;
    TargetTransform.SetRotation(Rotation * StartTransform.GetRotation()); // 组合旋转
    TargetTransform.SetScale3D(FVector(ScaleFactor)); // 应用缩放因子

    // 启动插值计时器（每帧执行）
    CurrentInterpolationTime = 0.0f;
    GetWorld()->GetTimerManager().SetTimer(InterpolationTimerHandle, this, &UCubeInteractionComponent::TickTransformInterpolation, 0.016f, true);
}

// 每帧更新变换插值
void UCubeInteractionComponent::TickTransformInterpolation()
{
    // 计算插值进度（0-1）
    CurrentInterpolationTime += 0.016f; // 约60FPS时间步长
    float Alpha = FMath::Clamp(CurrentInterpolationTime / InterpolationDuration, 0.0f, 1.0f);
    
    // 应用曲线调整插值速度
    if(TransformCurve)
    {
        Alpha = TransformCurve->GetFloatValue(Alpha); // 使用曲线数据重映射插值进度
    }

    // 应用线性插值变换
    FTransform NewTransform = FTransform::Lerp(StartTransform, TargetTransform, Alpha);
    GetOwner()->SetActorTransform(NewTransform);

    // 完成时清除计时器
    if(Alpha >= 1.0f)
    {
        GetWorld()->GetTimerManager().ClearTimer(InterpolationTimerHandle);
    }
}