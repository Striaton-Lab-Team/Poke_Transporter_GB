	.syntax unified // one true syntax
	.arch armv4t   // all we need

	.section .iwram, "awx", %progbits
	.globl bps_patch
	.type bps_patch, %function
	.thumb
	// A compact UNCHECKED memory BPS patcher in ARMv4T Thumb assembly. This is designed to be as small as possible
	// without explicitly being inefficient. Aggressive outlining is used. 
	// Notes:
	//   - The patch is assumed to be well-formed (source_size and target_size are ignored!)
	//   - out must point to a buffer large enough to hold the entire patched file
	//   - No CRC validation is performed
	//   - Uses 32-bit offsets, which combined with how offsets are encoded, means that only files up
	//     up to 1 GiB are supported.
	// void bps_patch(
	//     const uint8_t *src,   // r0
	//     uint8_t *out,         // r1
	//     const uint8_t *patch, // r2
	//     size_t patch_size     // r3
	// )
       	.thumb_func
bps_patch:

	SRC	.req	r0		// Offset to output
	OUT	.req	r1		// Pointer to the output data
	PAT	.req	r2		// Pointer to the patch. We use direct indexing since patches are read forwards.
	TPTR	.req	r3		// Temp pointer for copy loops
	DATA	.req	r4		// Currently read varint, temp var
	ACTLEN	.req	r5		// Temp var
	TMP1	.req	r6		// Temp var
	TMP2	.req	r7		// Temp offset for copy loops, temp var

	hTGTOFF	.req	r12		// HREG: Pointer to the end of the patch (minus CRCs which are ignored)

	.set	sSRC_BASE, 0		// Stack: Offset to target
	.set	sOUT_BASE, 4			// Stack:  Pointer to the source data
	.set	sPATEND, 8
	subs	r3, r3, #12		// end = patch + patch_size - 12
	adds	r3, r3, PAT
	push	{r0, r1, r3, r4-r7, lr}		// Save registers
	mov	hTGTOFF, OUT
	adds	PAT, PAT, #4		// Skip header
	movs	ACTLEN, #3		// Set OUTOFF to 3 as a temp loop counter
.Lbps_patch.decode_loop:		// On Thumb, a loop is slightly smaller.
	bl	.Ldecode		// source_size, target_size, metadata_size
	subs	ACTLEN, ACTLEN, #1	// Loop counter, also sets OUTOFF to 0 at the end
	bne	.Lbps_patch.decode_loop
	adds	PAT, PAT, DATA		// skip metadata_size which is left in DATA after loop

.Lbps_patch.loop:			// while (pat < patend)
	ldr	TMP1, [sp, #sPATEND]
	cmp	PAT, TMP1		// Check if at the end
	bhs	.Lcleanup
	bl	.Ldecode		// Read action
	lsrs	ACTLEN, DATA, #2	// length = (data >> 2) + 1, the +1 is handled by returning from copy on carry.
	lsls	DATA, DATA, #31		// Test DATA & 0x3 using lsls shenanigans. C = bit 1, Z = not bit 0
	bhi	.Lbps_patch.target_copy	//  C == 1  &  Z == 0  -> 0x3 -> target_copy
	bcs	.Lbps_patch.source_copy	//  C == 1  & (Z == 1) -> 0x2 -> source_copy
	bne	.Lbps_patch.target_read	// (C == 0) &  Z == 0  -> 0x1 -> target_read
					// (C == 0) & (Z == 1) -> 0x0 -> source_read (fallthrough)

	// The main structure is to set up temp registers and call either copy
	// or copy_with_offset, then if necessary, write back the temp offset to the
	// offset local variable.

	// void sourceRead() {
	//   while(length--) {
	//     target[outputOffset] = source[outputOffset];
	//     outputOffset++;
	//   }
	// }
.Lbps_patch.source_read: // case 0
	ldr	TMP2, [sp, #sSRC_BASE]	// sourceRead is very rarely used. 
	ldr	TMP1, [sp, #sOUT_BASE]
	subs	TMP1, OUT, TMP1
	adds	TPTR, TMP1, TMP2
	bl	.Lcopy			// copy(SRC, OUTOFF)
					// Don't write back to OUTOFF, the loop does that automatically.
	b	.Lbps_patch.loop

	// void targetRead() {
	//   while(length--) {
	//     target[outputOffset++] = read();
	//   }
	// }
.Lbps_patch.target_read: // case 1
	movs	TPTR, PAT		// add to the pointer.
	bl	.Lcopy			// copy(NULL, PAT)
	movs	PAT, TPTR		// Update PAT pointer
	b	.Lbps_patch.loop

	// void sourceCopy() {
	//   uint32 data = decode();
	//   sourceRelativeOffset += (data & 1 ? -1 : +1) * (data >> 1);
	//   while(length--) {
	//     target[outputOffset++] = source[sourceRelativeOffset++];
	//   }
	// }
.Lbps_patch.source_copy: // case 2
	bl	.Ldecode		// Decode length (avoids push lr)
	movs	TPTR, SRC		// Set the pointers to &SRC[SRCOFF]
	bl	.Lcopy_with_offset
	movs	SRC, TPTR
	b	.Lbps_patch.loop

	// void targetCopy() {
	//   uint32 data = decode();
	//   targetRelativeOffset += (data & 1 ? -1 : +1) * (data >> 1);
	//   while(length--) {
	//     target[outputOffset++] = target[targetRelativeOffset++];
	//   }
	// }
.Lbps_patch.target_copy:
	bl	.Ldecode
	mov	TPTR, hTGTOFF
	bl	.Lcopy_with_offset
	mov	hTGTOFF, TPTR
	b	.Lbps_patch.loop

.Lcleanup:
.ifdef NO_INTERWORK
	pop	{r0, r1, r3, r4-r7, pc}
.else
	pop	{r0, r1, r3, r4-r7}
	pop	{r3}
	bx	r3
.endif
	// SUBROUTINE
	//    Reads a signed varint offset, adds to TOFF, then copies memory
	//    DATA: Directly from decode() (prevents pushing lr)
	//    TOFF: Offset for indexing
	//    TPTR: Pointer for indexing
	//    ACTLEN: Length of data to copy - 1
	//    Modifies TOFF, OUTOFF, TMP, DATA, ACTLEN
.Lcopy_with_offset:
					// offset += (data & 1 ? -1 : +1) * (data >> 1)
					// equivalent:
	lsrs	DATA, DATA, #1		//   data >>= 1; CF = data & 1;
	bcc	.Lcopy_with_offset.pos	//   if (CF)
	rsbs	DATA, DATA, #0		//       data = -data;
.Lcopy_with_offset.pos:
	adds	TPTR, TPTR, DATA	//   offset += data;
	// fallthrough

	// SUBROUTINE
	//   Copies memory forwards like the loops in the reference code. Modifies the offset.
	//   TOFF: Offset for indexing
	//   TPTR: Pointer for indexing
	//   ACTLEN: Length of data to copy - 1
	//   Modifies TOFF, OUTOFF, TMP, ACTLEN (=-1)
.Lcopy:
.Lcopy.loop:				// do {
	ldrb	TMP1, [TPTR]		//    tmp1 = *tmpptr;
	adds	TPTR, TPTR, #1		//    ++tmpptr;
	strb	TMP1, [OUT]		//    *outptr = tmp1;
	adds	OUT, OUT, #1		//    ++outptr;
	subs	ACTLEN, ACTLEN, #1	//    actlen--;
	bcs	.Lcopy.loop		// } while (actlen != -1);
	bx	lr			// We use -1 because all offsets are +1. Note that carry is inverted for subs.

	// SUBROUTINE
	//   Reads a varint from PAT, advances pointer
	//   Returns in DATA
	//   Modifies DATA, TOFF, TPTR, TMP (=0), PAT
	// Compared to the ARM version, this one needs an extra register and
	// far more instructions.
.Ldecode:				// Slightly shorter translation of decode().
	SHIFT	.req TPTR

	movs	DATA, #0		// uint32 data = 0;
	movs	SHIFT, #0		// uint32 shift = 0;
.Ldecode.loop:				// do {
	ldrb	TMP1, [PAT]		//     uint8 x = read();
	adds	PAT, PAT, #1
	movs	TMP2, #0x80		//     uint8 tmp = x ^ 0x80
	eors	TMP2, TMP2, TMP1
	lsls	TMP2, TMP2, SHIFT	//     tmp <<= shift;
	adds	DATA, DATA, TMP2	//     data += tmp;
	adds	SHIFT, SHIFT, #7	//     shift += 7;
	lsrs	TMP1, TMP1, #8		//     bool flag = x & 0x80; x = 0;
	bcc	.Ldecode.loop		// } while (!flag);
	bx	lr
	.size bps_patch, . - bps_patch

	.section ".note.GNU-stack",""	// Disable executable stack
