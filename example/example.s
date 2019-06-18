	.text
	.file	"example.ll"
	.globl	main__Int_ARR__String   # -- Begin function main__Int_ARR__String
	.p2align	4, 0x90
	.type	main__Int_ARR__String,@function
main__Int_ARR__String:                  # @main__Int_ARR__String
	.cfi_startproc
# %bb.0:                                # %entry
	xorl	%eax, %eax
	retq
.Lfunc_end0:
	.size	main__Int_ARR__String, .Lfunc_end0-main__Int_ARR__String
	.cfi_endproc
                                        # -- End function
	.globl	src.main                # -- Begin function src.main
	.p2align	4, 0x90
	.type	src.main,@function
src.main:                               # @src.main
	.cfi_startproc
# %bb.0:                                # %entry
	xorl	%eax, %eax
	retq
.Lfunc_end1:
	.size	src.main, .Lfunc_end1-src.main
	.cfi_endproc
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	xorl	%eax, %eax
	retq
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
	.cfi_endproc
                                        # -- End function

	.section	".note.GNU-stack","",@progbits
