; ModuleID = 'core.c'
source_filename = "core.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.g_xyz_grime_core_Int = type { i8*, {}*, {}* }

@.str = private unnamed_addr constant [13 x i8] c"%s is a test\00", align 1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @gc_literal_g_destruct_g_xyz_grime_core_Int(%struct.g_xyz_grime_core_Int* byval align 8) #0 {
  %2 = getelementptr inbounds %struct.g_xyz_grime_core_Int, %struct.g_xyz_grime_core_Int* %0, i32 0, i32 0
  %3 = load i8*, i8** %2, align 8
  call void @free(i8* %3) #3
  ret void
}

; Function Attrs: nounwind
declare void @free(i8*) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @gc_literal_g_cget_g_xyz_grime_core_Int(%struct.g_xyz_grime_core_Int* byval align 8) #0 {
  %2 = getelementptr inbounds %struct.g_xyz_grime_core_Int, %struct.g_xyz_grime_core_Int* %0, i32 0, i32 0
  %3 = load i8*, i8** %2, align 8
  %4 = bitcast i8* %3 to i32*
  %5 = getelementptr inbounds i32, i32* %4, i64 0
  %6 = load i32, i32* %5, align 4
  ret i32 %6
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @gc_literal_construct_g_xyz_grime_core_Int(%struct.g_xyz_grime_core_Int*, i32) #0 {
  %3 = alloca %struct.g_xyz_grime_core_Int*, align 8
  %4 = alloca i32, align 4
  store %struct.g_xyz_grime_core_Int* %0, %struct.g_xyz_grime_core_Int** %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = call noalias i8* @malloc(i64 4) #3
  %6 = load %struct.g_xyz_grime_core_Int*, %struct.g_xyz_grime_core_Int** %3, align 8
  %7 = getelementptr inbounds %struct.g_xyz_grime_core_Int, %struct.g_xyz_grime_core_Int* %6, i32 0, i32 0
  store i8* %5, i8** %7, align 8
  %8 = load i32, i32* %4, align 4
  %9 = load %struct.g_xyz_grime_core_Int*, %struct.g_xyz_grime_core_Int** %3, align 8
  %10 = getelementptr inbounds %struct.g_xyz_grime_core_Int, %struct.g_xyz_grime_core_Int* %9, i32 0, i32 0
  %11 = load i8*, i8** %10, align 8
  %12 = bitcast i8* %11 to i32*
  %13 = getelementptr inbounds i32, i32* %12, i64 0
  store i32 %8, i32* %13, align 4
  %14 = load %struct.g_xyz_grime_core_Int*, %struct.g_xyz_grime_core_Int** %3, align 8
  %15 = getelementptr inbounds %struct.g_xyz_grime_core_Int, %struct.g_xyz_grime_core_Int* %14, i32 0, i32 1
  %16 = bitcast {}** %15 to void (%struct.g_xyz_grime_core_Int*)**
  store void (%struct.g_xyz_grime_core_Int*)* @gc_literal_g_destruct_g_xyz_grime_core_Int, void (%struct.g_xyz_grime_core_Int*)** %16, align 8
  %17 = load %struct.g_xyz_grime_core_Int*, %struct.g_xyz_grime_core_Int** %3, align 8
  %18 = getelementptr inbounds %struct.g_xyz_grime_core_Int, %struct.g_xyz_grime_core_Int* %17, i32 0, i32 2
  %19 = bitcast {}** %18 to i32 (%struct.g_xyz_grime_core_Int*)**
  store i32 (%struct.g_xyz_grime_core_Int*)* @gc_literal_g_cget_g_xyz_grime_core_Int, i32 (%struct.g_xyz_grime_core_Int*)** %19, align 8
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @test(%struct.g_xyz_grime_core_Int* byval align 8) #0 {
  %2 = alloca i8, align 1
  store i8 0, i8* %2, align 1
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0), i8* %2)
  ret void
}

declare i32 @printf(i8*, ...) #2

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
