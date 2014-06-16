
no-volatile:     file format elf32-littlearm


Disassembly of section .init:

000082c8 <_init>:
    82c8:	b508      	push	{r3, lr}
    82ca:	f000 f84d 	bl	8368 <call_gmon_start>
    82ce:	bf00      	nop
    82d0:	bd08      	pop	{r3, pc}

Disassembly of section .plt:

000082d4 <.plt>:
    82d4:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    82d8:	e59fe004 	ldr	lr, [pc, #4]	; 82e4 <_init+0x1c>
    82dc:	e08fe00e 	add	lr, pc, lr
    82e0:	e5bef008 	ldr	pc, [lr, #8]!
    82e4:	00008d1c 	.word	0x00008d1c
    82e8:	4778      	bx	pc
    82ea:	46c0      	nop			; (mov r8, r8)
    82ec:	e28fc600 	add	ip, pc, #0
    82f0:	e28cca08 	add	ip, ip, #32768	; 0x8000
    82f4:	e5bcfd18 	ldr	pc, [ip, #3352]!	; 0xd18
    82f8:	e28fc600 	add	ip, pc, #0
    82fc:	e28cca08 	add	ip, ip, #32768	; 0x8000
    8300:	e5bcfd10 	ldr	pc, [ip, #3344]!	; 0xd10
    8304:	4778      	bx	pc
    8306:	46c0      	nop			; (mov r8, r8)
    8308:	e28fc600 	add	ip, pc, #0
    830c:	e28cca08 	add	ip, ip, #32768	; 0x8000
    8310:	e5bcfd04 	ldr	pc, [ip, #3332]!	; 0xd04
    8314:	e28fc600 	add	ip, pc, #0
    8318:	e28cca08 	add	ip, ip, #32768	; 0x8000
    831c:	e5bcfcfc 	ldr	pc, [ip, #3324]!	; 0xcfc

Disassembly of section .text:

00008320 <main>:
    8320:	2106      	movs	r1, #6
    8322:	f248 30fc 	movw	r0, #33788	; 0x83fc
    8326:	2203      	movs	r2, #3
    8328:	f2c0 0000 	movt	r0, #0
    832c:	f7ff bfdc 	b.w	82e8 <_init+0x20>

00008330 <_start>:
    8330:	f04f 0b00 	mov.w	fp, #0
    8334:	f04f 0e00 	mov.w	lr, #0
    8338:	f85d 1b04 	ldr.w	r1, [sp], #4
    833c:	466a      	mov	r2, sp
    833e:	f84d 2d04 	str.w	r2, [sp, #-4]!
    8342:	f84d 0d04 	str.w	r0, [sp, #-4]!
    8346:	f8df c014 	ldr.w	ip, [pc, #20]	; 835c <_start+0x2c>
    834a:	f84d cd04 	str.w	ip, [sp, #-4]!
    834e:	4804      	ldr	r0, [pc, #16]	; (8360 <_start+0x30>)
    8350:	4b04      	ldr	r3, [pc, #16]	; (8364 <_start+0x34>)
    8352:	f7ff efd2 	blx	82f8 <_init+0x30>
    8356:	f7ff efde 	blx	8314 <_init+0x4c>
    835a:	0000      	.short	0x0000
    835c:	000083ed 	.word	0x000083ed
    8360:	00008321 	.word	0x00008321
    8364:	000083a9 	.word	0x000083a9

00008368 <call_gmon_start>:
    8368:	4b03      	ldr	r3, [pc, #12]	; (8378 <call_gmon_start+0x10>)
    836a:	4a04      	ldr	r2, [pc, #16]	; (837c <call_gmon_start+0x14>)
    836c:	447b      	add	r3, pc
    836e:	589b      	ldr	r3, [r3, r2]
    8370:	b10b      	cbz	r3, 8376 <call_gmon_start+0xe>
    8372:	f7ff bfc7 	b.w	8304 <_init+0x3c>
    8376:	4770      	bx	lr
    8378:	00008c90 	.word	0x00008c90
    837c:	0000001c 	.word	0x0000001c

00008380 <__do_global_dtors_aux>:
    8380:	4b02      	ldr	r3, [pc, #8]	; (838c <__do_global_dtors_aux+0xc>)
    8382:	781a      	ldrb	r2, [r3, #0]
    8384:	b90a      	cbnz	r2, 838a <__do_global_dtors_aux+0xa>
    8386:	2201      	movs	r2, #1
    8388:	701a      	strb	r2, [r3, #0]
    838a:	4770      	bx	lr
    838c:	00011028 	.word	0x00011028

00008390 <frame_dummy>:
    8390:	4803      	ldr	r0, [pc, #12]	; (83a0 <frame_dummy+0x10>)
    8392:	b508      	push	{r3, lr}
    8394:	6803      	ldr	r3, [r0, #0]
    8396:	b113      	cbz	r3, 839e <frame_dummy+0xe>
    8398:	4b02      	ldr	r3, [pc, #8]	; (83a4 <frame_dummy+0x14>)
    839a:	b103      	cbz	r3, 839e <frame_dummy+0xe>
    839c:	4798      	blx	r3
    839e:	bd08      	pop	{r3, pc}
    83a0:	00010f14 	.word	0x00010f14
    83a4:	00000000 	.word	0x00000000

000083a8 <__libc_csu_init>:
    83a8:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
    83ac:	4606      	mov	r6, r0
    83ae:	4d0d      	ldr	r5, [pc, #52]	; (83e4 <__libc_csu_init+0x3c>)
    83b0:	460f      	mov	r7, r1
    83b2:	f8df 9034 	ldr.w	r9, [pc, #52]	; 83e8 <__libc_csu_init+0x40>
    83b6:	4690      	mov	r8, r2
    83b8:	447d      	add	r5, pc
    83ba:	f7ff ff85 	bl	82c8 <_init>
    83be:	44f9      	add	r9, pc
    83c0:	ebc5 0909 	rsb	r9, r5, r9
    83c4:	ea5f 09a9 	movs.w	r9, r9, asr #2
    83c8:	d00a      	beq.n	83e0 <__libc_csu_init+0x38>
    83ca:	3d04      	subs	r5, #4
    83cc:	2400      	movs	r4, #0
    83ce:	f855 3f04 	ldr.w	r3, [r5, #4]!
    83d2:	4630      	mov	r0, r6
    83d4:	4639      	mov	r1, r7
    83d6:	4642      	mov	r2, r8
    83d8:	3401      	adds	r4, #1
    83da:	4798      	blx	r3
    83dc:	454c      	cmp	r4, r9
    83de:	d1f6      	bne.n	83ce <__libc_csu_init+0x26>
    83e0:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
    83e4:	00008b50 	.word	0x00008b50
    83e8:	00008b4e 	.word	0x00008b4e

000083ec <__libc_csu_fini>:
    83ec:	4770      	bx	lr
    83ee:	bf00      	nop

Disassembly of section .fini:

000083f0 <_fini>:
    83f0:	b508      	push	{r3, lr}
    83f2:	bf00      	nop
    83f4:	bd08      	pop	{r3, pc}
