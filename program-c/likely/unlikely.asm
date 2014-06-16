
unlikely:     file format elf32-i386


Disassembly of section .init:

080482b4 <_init>:
 80482b4:	53                   	push   %ebx
 80482b5:	83 ec 08             	sub    $0x8,%esp
 80482b8:	e8 00 00 00 00       	call   80482bd <_init+0x9>
 80482bd:	5b                   	pop    %ebx
 80482be:	81 c3 37 1d 00 00    	add    $0x1d37,%ebx
 80482c4:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
 80482ca:	85 c0                	test   %eax,%eax
 80482cc:	74 05                	je     80482d3 <_init+0x1f>
 80482ce:	e8 3d 00 00 00       	call   8048310 <__gmon_start__@plt>
 80482d3:	e8 78 01 00 00       	call   8048450 <frame_dummy>
 80482d8:	e8 23 02 00 00       	call   8048500 <__do_global_ctors_aux>
 80482dd:	83 c4 08             	add    $0x8,%esp
 80482e0:	5b                   	pop    %ebx
 80482e1:	c3                   	ret    

Disassembly of section .plt:

080482f0 <printf@plt-0x10>:
 80482f0:	ff 35 f8 9f 04 08    	pushl  0x8049ff8
 80482f6:	ff 25 fc 9f 04 08    	jmp    *0x8049ffc
 80482fc:	00 00                	add    %al,(%eax)
	...

08048300 <printf@plt>:
 8048300:	ff 25 00 a0 04 08    	jmp    *0x804a000
 8048306:	68 00 00 00 00       	push   $0x0
 804830b:	e9 e0 ff ff ff       	jmp    80482f0 <_init+0x3c>

08048310 <__gmon_start__@plt>:
 8048310:	ff 25 04 a0 04 08    	jmp    *0x804a004
 8048316:	68 08 00 00 00       	push   $0x8
 804831b:	e9 d0 ff ff ff       	jmp    80482f0 <_init+0x3c>

08048320 <__libc_start_main@plt>:
 8048320:	ff 25 08 a0 04 08    	jmp    *0x804a008
 8048326:	68 10 00 00 00       	push   $0x10
 804832b:	e9 c0 ff ff ff       	jmp    80482f0 <_init+0x3c>

Disassembly of section .text:

08048330 <main>:
 8048330:	55                   	push   %ebp
 8048331:	89 e5                	mov    %esp,%ebp
 8048333:	83 e4 f0             	and    $0xfffffff0,%esp
 8048336:	83 ec 20             	sub    $0x20,%esp
 8048339:	c7 44 24 14 01 00 00 	movl   $0x1,0x14(%esp)
 8048340:	00 
 8048341:	8b 44 24 14          	mov    0x14(%esp),%eax
 8048345:	85 c0                	test   %eax,%eax
 8048347:	75 4a                	jne    8048393 <main+0x63>
 8048349:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 804834d:	83 e8 01             	sub    $0x1,%eax
 8048350:	89 44 24 1c          	mov    %eax,0x1c(%esp)
 8048354:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 8048358:	83 e8 01             	sub    $0x1,%eax
 804835b:	89 44 24 1c          	mov    %eax,0x1c(%esp)
 804835f:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 8048363:	83 e8 01             	sub    $0x1,%eax
 8048366:	89 44 24 1c          	mov    %eax,0x1c(%esp)
 804836a:	8b 44 24 1c          	mov    0x1c(%esp),%eax
 804836e:	83 e8 01             	sub    $0x1,%eax
 8048371:	89 44 24 1c          	mov    %eax,0x1c(%esp)
 8048375:	8b 54 24 1c          	mov    0x1c(%esp),%edx
 8048379:	8b 44 24 18          	mov    0x18(%esp),%eax
 804837d:	c7 04 24 50 85 04 08 	movl   $0x8048550,(%esp)
 8048384:	89 54 24 08          	mov    %edx,0x8(%esp)
 8048388:	89 44 24 04          	mov    %eax,0x4(%esp)
 804838c:	e8 6f ff ff ff       	call   8048300 <printf@plt>
 8048391:	c9                   	leave  
 8048392:	c3                   	ret    
 8048393:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048397:	83 c0 01             	add    $0x1,%eax
 804839a:	89 44 24 18          	mov    %eax,0x18(%esp)
 804839e:	8b 44 24 18          	mov    0x18(%esp),%eax
 80483a2:	83 c0 01             	add    $0x1,%eax
 80483a5:	89 44 24 18          	mov    %eax,0x18(%esp)
 80483a9:	8b 44 24 18          	mov    0x18(%esp),%eax
 80483ad:	83 c0 01             	add    $0x1,%eax
 80483b0:	89 44 24 18          	mov    %eax,0x18(%esp)
 80483b4:	8b 44 24 18          	mov    0x18(%esp),%eax
 80483b8:	83 c0 01             	add    $0x1,%eax
 80483bb:	89 44 24 18          	mov    %eax,0x18(%esp)
 80483bf:	eb b4                	jmp    8048375 <main+0x45>
 80483c1:	90                   	nop
 80483c2:	90                   	nop
 80483c3:	90                   	nop

080483c4 <_start>:
 80483c4:	31 ed                	xor    %ebp,%ebp
 80483c6:	5e                   	pop    %esi
 80483c7:	89 e1                	mov    %esp,%ecx
 80483c9:	83 e4 f0             	and    $0xfffffff0,%esp
 80483cc:	50                   	push   %eax
 80483cd:	54                   	push   %esp
 80483ce:	52                   	push   %edx
 80483cf:	68 f0 84 04 08       	push   $0x80484f0
 80483d4:	68 80 84 04 08       	push   $0x8048480
 80483d9:	51                   	push   %ecx
 80483da:	56                   	push   %esi
 80483db:	68 30 83 04 08       	push   $0x8048330
 80483e0:	e8 3b ff ff ff       	call   8048320 <__libc_start_main@plt>
 80483e5:	f4                   	hlt    
 80483e6:	90                   	nop
 80483e7:	90                   	nop
 80483e8:	90                   	nop
 80483e9:	90                   	nop
 80483ea:	90                   	nop
 80483eb:	90                   	nop
 80483ec:	90                   	nop
 80483ed:	90                   	nop
 80483ee:	90                   	nop
 80483ef:	90                   	nop

080483f0 <__do_global_dtors_aux>:
 80483f0:	55                   	push   %ebp
 80483f1:	89 e5                	mov    %esp,%ebp
 80483f3:	53                   	push   %ebx
 80483f4:	83 ec 04             	sub    $0x4,%esp
 80483f7:	80 3d 14 a0 04 08 00 	cmpb   $0x0,0x804a014
 80483fe:	75 3f                	jne    804843f <__do_global_dtors_aux+0x4f>
 8048400:	a1 18 a0 04 08       	mov    0x804a018,%eax
 8048405:	bb 20 9f 04 08       	mov    $0x8049f20,%ebx
 804840a:	81 eb 1c 9f 04 08    	sub    $0x8049f1c,%ebx
 8048410:	c1 fb 02             	sar    $0x2,%ebx
 8048413:	83 eb 01             	sub    $0x1,%ebx
 8048416:	39 d8                	cmp    %ebx,%eax
 8048418:	73 1e                	jae    8048438 <__do_global_dtors_aux+0x48>
 804841a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 8048420:	83 c0 01             	add    $0x1,%eax
 8048423:	a3 18 a0 04 08       	mov    %eax,0x804a018
 8048428:	ff 14 85 1c 9f 04 08 	call   *0x8049f1c(,%eax,4)
 804842f:	a1 18 a0 04 08       	mov    0x804a018,%eax
 8048434:	39 d8                	cmp    %ebx,%eax
 8048436:	72 e8                	jb     8048420 <__do_global_dtors_aux+0x30>
 8048438:	c6 05 14 a0 04 08 01 	movb   $0x1,0x804a014
 804843f:	83 c4 04             	add    $0x4,%esp
 8048442:	5b                   	pop    %ebx
 8048443:	5d                   	pop    %ebp
 8048444:	c3                   	ret    
 8048445:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 8048449:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi

08048450 <frame_dummy>:
 8048450:	55                   	push   %ebp
 8048451:	89 e5                	mov    %esp,%ebp
 8048453:	83 ec 18             	sub    $0x18,%esp
 8048456:	a1 24 9f 04 08       	mov    0x8049f24,%eax
 804845b:	85 c0                	test   %eax,%eax
 804845d:	74 12                	je     8048471 <frame_dummy+0x21>
 804845f:	b8 00 00 00 00       	mov    $0x0,%eax
 8048464:	85 c0                	test   %eax,%eax
 8048466:	74 09                	je     8048471 <frame_dummy+0x21>
 8048468:	c7 04 24 24 9f 04 08 	movl   $0x8049f24,(%esp)
 804846f:	ff d0                	call   *%eax
 8048471:	c9                   	leave  
 8048472:	c3                   	ret    
 8048473:	90                   	nop
 8048474:	90                   	nop
 8048475:	90                   	nop
 8048476:	90                   	nop
 8048477:	90                   	nop
 8048478:	90                   	nop
 8048479:	90                   	nop
 804847a:	90                   	nop
 804847b:	90                   	nop
 804847c:	90                   	nop
 804847d:	90                   	nop
 804847e:	90                   	nop
 804847f:	90                   	nop

08048480 <__libc_csu_init>:
 8048480:	55                   	push   %ebp
 8048481:	57                   	push   %edi
 8048482:	56                   	push   %esi
 8048483:	53                   	push   %ebx
 8048484:	e8 69 00 00 00       	call   80484f2 <__i686.get_pc_thunk.bx>
 8048489:	81 c3 6b 1b 00 00    	add    $0x1b6b,%ebx
 804848f:	83 ec 1c             	sub    $0x1c,%esp
 8048492:	8b 6c 24 30          	mov    0x30(%esp),%ebp
 8048496:	8d bb 20 ff ff ff    	lea    -0xe0(%ebx),%edi
 804849c:	e8 13 fe ff ff       	call   80482b4 <_init>
 80484a1:	8d 83 20 ff ff ff    	lea    -0xe0(%ebx),%eax
 80484a7:	29 c7                	sub    %eax,%edi
 80484a9:	c1 ff 02             	sar    $0x2,%edi
 80484ac:	85 ff                	test   %edi,%edi
 80484ae:	74 29                	je     80484d9 <__libc_csu_init+0x59>
 80484b0:	31 f6                	xor    %esi,%esi
 80484b2:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 80484b8:	8b 44 24 38          	mov    0x38(%esp),%eax
 80484bc:	89 2c 24             	mov    %ebp,(%esp)
 80484bf:	89 44 24 08          	mov    %eax,0x8(%esp)
 80484c3:	8b 44 24 34          	mov    0x34(%esp),%eax
 80484c7:	89 44 24 04          	mov    %eax,0x4(%esp)
 80484cb:	ff 94 b3 20 ff ff ff 	call   *-0xe0(%ebx,%esi,4)
 80484d2:	83 c6 01             	add    $0x1,%esi
 80484d5:	39 fe                	cmp    %edi,%esi
 80484d7:	75 df                	jne    80484b8 <__libc_csu_init+0x38>
 80484d9:	83 c4 1c             	add    $0x1c,%esp
 80484dc:	5b                   	pop    %ebx
 80484dd:	5e                   	pop    %esi
 80484de:	5f                   	pop    %edi
 80484df:	5d                   	pop    %ebp
 80484e0:	c3                   	ret    
 80484e1:	eb 0d                	jmp    80484f0 <__libc_csu_fini>
 80484e3:	90                   	nop
 80484e4:	90                   	nop
 80484e5:	90                   	nop
 80484e6:	90                   	nop
 80484e7:	90                   	nop
 80484e8:	90                   	nop
 80484e9:	90                   	nop
 80484ea:	90                   	nop
 80484eb:	90                   	nop
 80484ec:	90                   	nop
 80484ed:	90                   	nop
 80484ee:	90                   	nop
 80484ef:	90                   	nop

080484f0 <__libc_csu_fini>:
 80484f0:	f3 c3                	repz ret 

080484f2 <__i686.get_pc_thunk.bx>:
 80484f2:	8b 1c 24             	mov    (%esp),%ebx
 80484f5:	c3                   	ret    
 80484f6:	90                   	nop
 80484f7:	90                   	nop
 80484f8:	90                   	nop
 80484f9:	90                   	nop
 80484fa:	90                   	nop
 80484fb:	90                   	nop
 80484fc:	90                   	nop
 80484fd:	90                   	nop
 80484fe:	90                   	nop
 80484ff:	90                   	nop

08048500 <__do_global_ctors_aux>:
 8048500:	55                   	push   %ebp
 8048501:	89 e5                	mov    %esp,%ebp
 8048503:	53                   	push   %ebx
 8048504:	83 ec 04             	sub    $0x4,%esp
 8048507:	a1 14 9f 04 08       	mov    0x8049f14,%eax
 804850c:	83 f8 ff             	cmp    $0xffffffff,%eax
 804850f:	74 13                	je     8048524 <__do_global_ctors_aux+0x24>
 8048511:	bb 14 9f 04 08       	mov    $0x8049f14,%ebx
 8048516:	66 90                	xchg   %ax,%ax
 8048518:	83 eb 04             	sub    $0x4,%ebx
 804851b:	ff d0                	call   *%eax
 804851d:	8b 03                	mov    (%ebx),%eax
 804851f:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048522:	75 f4                	jne    8048518 <__do_global_ctors_aux+0x18>
 8048524:	83 c4 04             	add    $0x4,%esp
 8048527:	5b                   	pop    %ebx
 8048528:	5d                   	pop    %ebp
 8048529:	c3                   	ret    
 804852a:	90                   	nop
 804852b:	90                   	nop

Disassembly of section .fini:

0804852c <_fini>:
 804852c:	53                   	push   %ebx
 804852d:	83 ec 08             	sub    $0x8,%esp
 8048530:	e8 00 00 00 00       	call   8048535 <_fini+0x9>
 8048535:	5b                   	pop    %ebx
 8048536:	81 c3 bf 1a 00 00    	add    $0x1abf,%ebx
 804853c:	e8 af fe ff ff       	call   80483f0 <__do_global_dtors_aux>
 8048541:	83 c4 08             	add    $0x8,%esp
 8048544:	5b                   	pop    %ebx
 8048545:	c3                   	ret    
