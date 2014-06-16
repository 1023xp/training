
a.out:     file format elf32-littlearm


Disassembly of section .init:

000082cc <_init>:
    82cc:	b508      	push	{r3, lr}
    82ce:	f000 f84f 	bl	8370 <call_gmon_start>
    82d2:	bf00      	nop
    82d4:	bd08      	pop	{r3, pc}

Disassembly of section .plt:

000082d8 <.plt>:
    82d8:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    82dc:	e59fe004 	ldr	lr, [pc, #4]	; 82e8 <_init+0x1c>
    82e0:	e08fe00e 	add	lr, pc, lr
    82e4:	e5bef008 	ldr	pc, [lr, #8]!
    82e8:	00008d18 	.word	0x00008d18
    82ec:	e28fc600 	add	ip, pc, #0
    82f0:	e28cca08 	add	ip, ip, #32768	; 0x8000
    82f4:	e5bcfd18 	ldr	pc, [ip, #3352]!	; 0xd18
    82f8:	4778      	bx	pc
    82fa:	46c0      	nop			; (mov r8, r8)
    82fc:	e28fc600 	add	ip, pc, #0
    8300:	e28cca08 	add	ip, ip, #32768	; 0x8000
    8304:	e5bcfd0c 	ldr	pc, [ip, #3340]!	; 0xd0c
    8308:	e28fc600 	add	ip, pc, #0
    830c:	e28cca08 	add	ip, ip, #32768	; 0x8000
    8310:	e5bcfd04 	ldr	pc, [ip, #3332]!	; 0xd04
    8314:	e28fc600 	add	ip, pc, #0
    8318:	e28cca08 	add	ip, ip, #32768	; 0x8000
    831c:	e5bcfcfc 	ldr	pc, [ip, #3324]!	; 0xcfc

Disassembly of section .text:

00008320 <main>:
    8320:	b508      	push	{r3, lr}
    8322:	2001      	movs	r0, #1
    8324:	2203      	movs	r2, #3
    8326:	2304      	movs	r3, #4
    8328:	f248 4130 	movw	r1, #33840	; 0x8430
    832c:	f2c0 0100 	movt	r1, #0
    8330:	f7ff efea 	blx	8308 <_init+0x3c>
    8334:	2000      	movs	r0, #0
    8336:	bd08      	pop	{r3, pc}

00008338 <_start>:
    8338:	f04f 0b00 	mov.w	fp, #0
    833c:	f04f 0e00 	mov.w	lr, #0
    8340:	f85d 1b04 	ldr.w	r1, [sp], #4
    8344:	466a      	mov	r2, sp
    8346:	f84d 2d04 	str.w	r2, [sp, #-4]!
    834a:	f84d 0d04 	str.w	r0, [sp, #-4]!
    834e:	f8df c014 	ldr.w	ip, [pc, #20]	; 8364 <_start+0x2c>
    8352:	f84d cd04 	str.w	ip, [sp, #-4]!
    8356:	4804      	ldr	r0, [pc, #16]	; (8368 <_start+0x30>)
    8358:	4b04      	ldr	r3, [pc, #16]	; (836c <_start+0x34>)
    835a:	f7ff efc8 	blx	82ec <_init+0x20>
    835e:	f7ff efda 	blx	8314 <_init+0x48>
    8362:	0000      	.short	0x0000
    8364:	00008421 	.word	0x00008421
    8368:	00008321 	.word	0x00008321
    836c:	000083dd 	.word	0x000083dd

00008370 <call_gmon_start>:
    8370:	4b03      	ldr	r3, [pc, #12]	; (8380 <call_gmon_start+0x10>)
    8372:	4a04      	ldr	r2, [pc, #16]	; (8384 <call_gmon_start+0x14>)
    8374:	447b      	add	r3, pc
    8376:	589b      	ldr	r3, [r3, r2]
    8378:	b10b      	cbz	r3, 837e <call_gmon_start+0xe>
    837a:	f7ff bfbd 	b.w	82f8 <_init+0x2c>
    837e:	4770      	bx	lr
    8380:	00008c88 	.word	0x00008c88
    8384:	0000001c 	.word	0x0000001c

00008388 <__do_global_dtors_aux>:
    8388:	4b02      	ldr	r3, [pc, #8]	; (8394 <__do_global_dtors_aux+0xc>)
    838a:	781a      	ldrb	r2, [r3, #0]
    838c:	b90a      	cbnz	r2, 8392 <__do_global_dtors_aux+0xa>
    838e:	2201      	movs	r2, #1
    8390:	701a      	strb	r2, [r3, #0]
    8392:	4770      	bx	lr
    8394:	00011028 	.word	0x00011028

00008398 <frame_dummy>:
    8398:	4803      	ldr	r0, [pc, #12]	; (83a8 <frame_dummy+0x10>)
    839a:	b508      	push	{r3, lr}
    839c:	6803      	ldr	r3, [r0, #0]
    839e:	b113      	cbz	r3, 83a6 <frame_dummy+0xe>
    83a0:	4b02      	ldr	r3, [pc, #8]	; (83ac <frame_dummy+0x14>)
    83a2:	b103      	cbz	r3, 83a6 <frame_dummy+0xe>
    83a4:	4798      	blx	r3
    83a6:	bd08      	pop	{r3, pc}
    83a8:	00010f14 	.word	0x00010f14
    83ac:	00000000 	.word	0x00000000

000083b0 <multi_add_r>:
    83b0:	6813      	ldr	r3, [r2, #0]
    83b2:	b410      	push	{r4}
    83b4:	680a      	ldr	r2, [r1, #0]
    83b6:	6804      	ldr	r4, [r0, #0]
    83b8:	18e4      	adds	r4, r4, r3
    83ba:	18d3      	adds	r3, r2, r3
    83bc:	6004      	str	r4, [r0, #0]
    83be:	600b      	str	r3, [r1, #0]
    83c0:	bc10      	pop	{r4}
    83c2:	4770      	bx	lr

000083c4 <multi_add_n>:
    83c4:	6803      	ldr	r3, [r0, #0]
    83c6:	b410      	push	{r4}
    83c8:	6814      	ldr	r4, [r2, #0]
    83ca:	191b      	adds	r3, r3, r4
    83cc:	6003      	str	r3, [r0, #0]
    83ce:	6808      	ldr	r0, [r1, #0]
    83d0:	6813      	ldr	r3, [r2, #0]
    83d2:	18c3      	adds	r3, r0, r3
    83d4:	600b      	str	r3, [r1, #0]
    83d6:	bc10      	pop	{r4}
    83d8:	4770      	bx	lr
    83da:	bf00      	nop

000083dc <__libc_csu_init>:
    83dc:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
    83e0:	4606      	mov	r6, r0
    83e2:	4d0d      	ldr	r5, [pc, #52]	; (8418 <__libc_csu_init+0x3c>)
    83e4:	460f      	mov	r7, r1
    83e6:	f8df 9034 	ldr.w	r9, [pc, #52]	; 841c <__libc_csu_init+0x40>
    83ea:	4690      	mov	r8, r2
    83ec:	447d      	add	r5, pc
    83ee:	f7ff ff6d 	bl	82cc <_init>
    83f2:	44f9      	add	r9, pc
    83f4:	ebc5 0909 	rsb	r9, r5, r9
    83f8:	ea5f 09a9 	movs.w	r9, r9, asr #2
    83fc:	d00a      	beq.n	8414 <__libc_csu_init+0x38>
    83fe:	3d04      	subs	r5, #4
    8400:	2400      	movs	r4, #0
    8402:	f855 3f04 	ldr.w	r3, [r5, #4]!
    8406:	4630      	mov	r0, r6
    8408:	4639      	mov	r1, r7
    840a:	4642      	mov	r2, r8
    840c:	3401      	adds	r4, #1
    840e:	4798      	blx	r3
    8410:	454c      	cmp	r4, r9
    8412:	d1f6      	bne.n	8402 <__libc_csu_init+0x26>
    8414:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
    8418:	00008b1c 	.word	0x00008b1c
    841c:	00008b1a 	.word	0x00008b1a

00008420 <__libc_csu_fini>:
    8420:	4770      	bx	lr
    8422:	bf00      	nop

Disassembly of section .fini:

00008424 <_fini>:
    8424:	b508      	push	{r3, lr}
    8426:	bf00      	nop
    8428:	bd08      	pop	{r3, pc}
