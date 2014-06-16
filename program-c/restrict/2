
a.out:     file format elf32-littlearm


Disassembly of section .init:

0000829c <_init>:
    829c:	b508      	push	{r3, lr}
    829e:	f000 f83f 	bl	8320 <call_gmon_start>
    82a2:	bf00      	nop
    82a4:	bd08      	pop	{r3, pc}

Disassembly of section .plt:

000082a8 <.plt>:
    82a8:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    82ac:	e59fe004 	ldr	lr, [pc, #4]	; 82b8 <_init+0x1c>
    82b0:	e08fe00e 	add	lr, pc, lr
    82b4:	e5bef008 	ldr	pc, [lr, #8]!
    82b8:	00008d48 	.word	0x00008d48
    82bc:	e28fc600 	add	ip, pc, #0
    82c0:	e28cca08 	add	ip, ip, #32768	; 0x8000
    82c4:	e5bcfd48 	ldr	pc, [ip, #3400]!	; 0xd48
    82c8:	4778      	bx	pc
    82ca:	46c0      	nop			; (mov r8, r8)
    82cc:	e28fc600 	add	ip, pc, #0
    82d0:	e28cca08 	add	ip, ip, #32768	; 0x8000
    82d4:	e5bcfd3c 	ldr	pc, [ip, #3388]!	; 0xd3c
    82d8:	e28fc600 	add	ip, pc, #0
    82dc:	e28cca08 	add	ip, ip, #32768	; 0x8000
    82e0:	e5bcfd34 	ldr	pc, [ip, #3380]!	; 0xd34

Disassembly of section .text:

000082e4 <main>:
    82e4:	2000      	movs	r0, #0
    82e6:	4770      	bx	lr

000082e8 <_start>:
    82e8:	f04f 0b00 	mov.w	fp, #0
    82ec:	f04f 0e00 	mov.w	lr, #0
    82f0:	f85d 1b04 	ldr.w	r1, [sp], #4
    82f4:	466a      	mov	r2, sp
    82f6:	f84d 2d04 	str.w	r2, [sp, #-4]!
    82fa:	f84d 0d04 	str.w	r0, [sp, #-4]!
    82fe:	f8df c014 	ldr.w	ip, [pc, #20]	; 8314 <_start+0x2c>
    8302:	f84d cd04 	str.w	ip, [sp, #-4]!
    8306:	4804      	ldr	r0, [pc, #16]	; (8318 <_start+0x30>)
    8308:	4b04      	ldr	r3, [pc, #16]	; (831c <_start+0x34>)
    830a:	f7ff efd8 	blx	82bc <_init+0x20>
    830e:	f7ff efe4 	blx	82d8 <_init+0x3c>
    8312:	0000      	.short	0x0000
    8314:	000083bd 	.word	0x000083bd
    8318:	000082e5 	.word	0x000082e5
    831c:	00008379 	.word	0x00008379

00008320 <call_gmon_start>:
    8320:	4b03      	ldr	r3, [pc, #12]	; (8330 <call_gmon_start+0x10>)
    8322:	4a04      	ldr	r2, [pc, #16]	; (8334 <call_gmon_start+0x14>)
    8324:	447b      	add	r3, pc
    8326:	589b      	ldr	r3, [r3, r2]
    8328:	b10b      	cbz	r3, 832e <call_gmon_start+0xe>
    832a:	f7ff bfcd 	b.w	82c8 <_init+0x2c>
    832e:	4770      	bx	lr
    8330:	00008cd8 	.word	0x00008cd8
    8334:	00000018 	.word	0x00000018

00008338 <__do_global_dtors_aux>:
    8338:	4b02      	ldr	r3, [pc, #8]	; (8344 <__do_global_dtors_aux+0xc>)
    833a:	781a      	ldrb	r2, [r3, #0]
    833c:	b90a      	cbnz	r2, 8342 <__do_global_dtors_aux+0xa>
    833e:	2201      	movs	r2, #1
    8340:	701a      	strb	r2, [r3, #0]
    8342:	4770      	bx	lr
    8344:	00011024 	.word	0x00011024

00008348 <frame_dummy>:
    8348:	4803      	ldr	r0, [pc, #12]	; (8358 <frame_dummy+0x10>)
    834a:	b508      	push	{r3, lr}
    834c:	6803      	ldr	r3, [r0, #0]
    834e:	b113      	cbz	r3, 8356 <frame_dummy+0xe>
    8350:	4b02      	ldr	r3, [pc, #8]	; (835c <frame_dummy+0x14>)
    8352:	b103      	cbz	r3, 8356 <frame_dummy+0xe>
    8354:	4798      	blx	r3
    8356:	bd08      	pop	{r3, pc}
    8358:	00010f14 	.word	0x00010f14
    835c:	00000000 	.word	0x00000000

00008360 <test_no_restrict>:
    8360:	2300      	movs	r3, #0
    8362:	6003      	str	r3, [r0, #0]
    8364:	2301      	movs	r3, #1
    8366:	600b      	str	r3, [r1, #0]
    8368:	6800      	ldr	r0, [r0, #0]
    836a:	4770      	bx	lr

0000836c <test_have_restrict>:
    836c:	2300      	movs	r3, #0
    836e:	2201      	movs	r2, #1
    8370:	6003      	str	r3, [r0, #0]
    8372:	600a      	str	r2, [r1, #0]
    8374:	4618      	mov	r0, r3
    8376:	4770      	bx	lr

00008378 <__libc_csu_init>:
    8378:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
    837c:	4606      	mov	r6, r0
    837e:	4d0d      	ldr	r5, [pc, #52]	; (83b4 <__libc_csu_init+0x3c>)
    8380:	460f      	mov	r7, r1
    8382:	f8df 9034 	ldr.w	r9, [pc, #52]	; 83b8 <__libc_csu_init+0x40>
    8386:	4690      	mov	r8, r2
    8388:	447d      	add	r5, pc
    838a:	f7ff ff87 	bl	829c <_init>
    838e:	44f9      	add	r9, pc
    8390:	ebc5 0909 	rsb	r9, r5, r9
    8394:	ea5f 09a9 	movs.w	r9, r9, asr #2
    8398:	d00a      	beq.n	83b0 <__libc_csu_init+0x38>
    839a:	3d04      	subs	r5, #4
    839c:	2400      	movs	r4, #0
    839e:	f855 3f04 	ldr.w	r3, [r5, #4]!
    83a2:	4630      	mov	r0, r6
    83a4:	4639      	mov	r1, r7
    83a6:	4642      	mov	r2, r8
    83a8:	3401      	adds	r4, #1
    83aa:	4798      	blx	r3
    83ac:	454c      	cmp	r4, r9
    83ae:	d1f6      	bne.n	839e <__libc_csu_init+0x26>
    83b0:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
    83b4:	00008b80 	.word	0x00008b80
    83b8:	00008b7e 	.word	0x00008b7e

000083bc <__libc_csu_fini>:
    83bc:	4770      	bx	lr
    83be:	bf00      	nop

Disassembly of section .fini:

000083c0 <_fini>:
    83c0:	b508      	push	{r3, lr}
    83c2:	bf00      	nop
    83c4:	bd08      	pop	{r3, pc}
