
volatile:     file format elf32-littlearm


Disassembly of section .init:

000082c8 <_init>:
    82c8:	b508      	push	{r3, lr}
    82ca:	f000 f86f 	bl	83ac <call_gmon_start>
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
    8320:	b084      	sub	sp, #16
    8322:	2301      	movs	r3, #1
    8324:	9301      	str	r3, [sp, #4]
    8326:	2302      	movs	r3, #2
    8328:	9302      	str	r3, [sp, #8]
    832a:	2303      	movs	r3, #3
    832c:	9303      	str	r3, [sp, #12]
    832e:	9b01      	ldr	r3, [sp, #4]
    8330:	b18b      	cbz	r3, 8356 <main+0x36>
    8332:	9b02      	ldr	r3, [sp, #8]
    8334:	3301      	adds	r3, #1
    8336:	9302      	str	r3, [sp, #8]
    8338:	9b02      	ldr	r3, [sp, #8]
    833a:	3301      	adds	r3, #1
    833c:	9302      	str	r3, [sp, #8]
    833e:	9b02      	ldr	r3, [sp, #8]
    8340:	3301      	adds	r3, #1
    8342:	9302      	str	r3, [sp, #8]
    8344:	9b02      	ldr	r3, [sp, #8]
    8346:	3301      	adds	r3, #1
    8348:	9302      	str	r3, [sp, #8]
    834a:	9902      	ldr	r1, [sp, #8]
    834c:	4808      	ldr	r0, [pc, #32]	; (8370 <main+0x50>)
    834e:	9a03      	ldr	r2, [sp, #12]
    8350:	b004      	add	sp, #16
    8352:	f7ff bfc9 	b.w	82e8 <_init+0x20>
    8356:	9b03      	ldr	r3, [sp, #12]
    8358:	3b01      	subs	r3, #1
    835a:	9303      	str	r3, [sp, #12]
    835c:	9b03      	ldr	r3, [sp, #12]
    835e:	3b01      	subs	r3, #1
    8360:	9303      	str	r3, [sp, #12]
    8362:	9b03      	ldr	r3, [sp, #12]
    8364:	3b01      	subs	r3, #1
    8366:	9303      	str	r3, [sp, #12]
    8368:	9b03      	ldr	r3, [sp, #12]
    836a:	3b01      	subs	r3, #1
    836c:	9303      	str	r3, [sp, #12]
    836e:	e7ec      	b.n	834a <main+0x2a>
    8370:	00008440 	.word	0x00008440

00008374 <_start>:
    8374:	f04f 0b00 	mov.w	fp, #0
    8378:	f04f 0e00 	mov.w	lr, #0
    837c:	f85d 1b04 	ldr.w	r1, [sp], #4
    8380:	466a      	mov	r2, sp
    8382:	f84d 2d04 	str.w	r2, [sp, #-4]!
    8386:	f84d 0d04 	str.w	r0, [sp, #-4]!
    838a:	f8df c014 	ldr.w	ip, [pc, #20]	; 83a0 <_start+0x2c>
    838e:	f84d cd04 	str.w	ip, [sp, #-4]!
    8392:	4804      	ldr	r0, [pc, #16]	; (83a4 <_start+0x30>)
    8394:	4b04      	ldr	r3, [pc, #16]	; (83a8 <_start+0x34>)
    8396:	f7ff efb0 	blx	82f8 <_init+0x30>
    839a:	f7ff efbc 	blx	8314 <_init+0x4c>
    839e:	0000      	.short	0x0000
    83a0:	00008431 	.word	0x00008431
    83a4:	00008321 	.word	0x00008321
    83a8:	000083ed 	.word	0x000083ed

000083ac <call_gmon_start>:
    83ac:	4b03      	ldr	r3, [pc, #12]	; (83bc <call_gmon_start+0x10>)
    83ae:	4a04      	ldr	r2, [pc, #16]	; (83c0 <call_gmon_start+0x14>)
    83b0:	447b      	add	r3, pc
    83b2:	589b      	ldr	r3, [r3, r2]
    83b4:	b10b      	cbz	r3, 83ba <call_gmon_start+0xe>
    83b6:	f7ff bfa5 	b.w	8304 <_init+0x3c>
    83ba:	4770      	bx	lr
    83bc:	00008c4c 	.word	0x00008c4c
    83c0:	0000001c 	.word	0x0000001c

000083c4 <__do_global_dtors_aux>:
    83c4:	4b02      	ldr	r3, [pc, #8]	; (83d0 <__do_global_dtors_aux+0xc>)
    83c6:	781a      	ldrb	r2, [r3, #0]
    83c8:	b90a      	cbnz	r2, 83ce <__do_global_dtors_aux+0xa>
    83ca:	2201      	movs	r2, #1
    83cc:	701a      	strb	r2, [r3, #0]
    83ce:	4770      	bx	lr
    83d0:	00011028 	.word	0x00011028

000083d4 <frame_dummy>:
    83d4:	4803      	ldr	r0, [pc, #12]	; (83e4 <frame_dummy+0x10>)
    83d6:	b508      	push	{r3, lr}
    83d8:	6803      	ldr	r3, [r0, #0]
    83da:	b113      	cbz	r3, 83e2 <frame_dummy+0xe>
    83dc:	4b02      	ldr	r3, [pc, #8]	; (83e8 <frame_dummy+0x14>)
    83de:	b103      	cbz	r3, 83e2 <frame_dummy+0xe>
    83e0:	4798      	blx	r3
    83e2:	bd08      	pop	{r3, pc}
    83e4:	00010f14 	.word	0x00010f14
    83e8:	00000000 	.word	0x00000000

000083ec <__libc_csu_init>:
    83ec:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
    83f0:	4606      	mov	r6, r0
    83f2:	4d0d      	ldr	r5, [pc, #52]	; (8428 <__libc_csu_init+0x3c>)
    83f4:	460f      	mov	r7, r1
    83f6:	f8df 9034 	ldr.w	r9, [pc, #52]	; 842c <__libc_csu_init+0x40>
    83fa:	4690      	mov	r8, r2
    83fc:	447d      	add	r5, pc
    83fe:	f7ff ff63 	bl	82c8 <_init>
    8402:	44f9      	add	r9, pc
    8404:	ebc5 0909 	rsb	r9, r5, r9
    8408:	ea5f 09a9 	movs.w	r9, r9, asr #2
    840c:	d00a      	beq.n	8424 <__libc_csu_init+0x38>
    840e:	3d04      	subs	r5, #4
    8410:	2400      	movs	r4, #0
    8412:	f855 3f04 	ldr.w	r3, [r5, #4]!
    8416:	4630      	mov	r0, r6
    8418:	4639      	mov	r1, r7
    841a:	4642      	mov	r2, r8
    841c:	3401      	adds	r4, #1
    841e:	4798      	blx	r3
    8420:	454c      	cmp	r4, r9
    8422:	d1f6      	bne.n	8412 <__libc_csu_init+0x26>
    8424:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
    8428:	00008b0c 	.word	0x00008b0c
    842c:	00008b0a 	.word	0x00008b0a

00008430 <__libc_csu_fini>:
    8430:	4770      	bx	lr
    8432:	bf00      	nop

Disassembly of section .fini:

00008434 <_fini>:
    8434:	b508      	push	{r3, lr}
    8436:	bf00      	nop
    8438:	bd08      	pop	{r3, pc}
